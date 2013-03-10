/***************************************************************************
 * NAMS - Not Another MUD Server                                           *
 * Copyright (C) 2012 Matthew Goff (matt@goff.cc) <http://www.ackmud.net/> *
 *                                                                         *
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ***************************************************************************/
/**
 * @file server.cpp
 * @brief All non-template member functions of the Server class.
 *
 * A Server object is created from main() when NAMS is first started. The Server
 * object handles all further interfacing with the host OS and is the central
 * core of NAMS.
 *
 * During its boot sequence, the Server will spawn a SocketServer to listen for
 * incoming SocketClient connections, compile and load any Plugin objects,
 * and then indefinitely loop and process game logic until it receives a shutdown
 * notification.
 */
#include "h/includes.h"
#include "h/class.h"

#include "h/server.h"
#include "h/account.h"
#include "h/command.h"
#include "h/event.h"
#include "h/list.h"

/* Core */
/**
 * @brief Serialize runtime configuration settings and write them to #CFG_DAT_FILE_SETTINGS.
 * @retval false Returned if there was an error serializing settings.
 * @retval true Returned if settings were serialized successfully.
 */
const bool Server::Config::Serialize() const
{
    UFLAGS_DE( flags );
    ofstream ofs;
    string value;
    stringstream line;
    CITER( forward_list, string, li );

    LOGSTR( 0, CFG_STR_FILE_SETTINGS_WRITE );
    ofs.open( CFG_DAT_FILE_SETTINGS CFG_DAT_FILE_EXT_TMP );

    if ( !ofs.good() )
    {
        LOGFMT( flags, "Server::Config::Serialize()-> failed to open settings file: %s", CFG_DAT_FILE_SETTINGS );
        return false;
    }

    KEY( ofs, "account_prohibited_names" );
    {
        for ( li = m_account_prohibited_names.begin(); li != m_account_prohibited_names.end(); li++ )
            line << *li << " ";

        value = line.str();
        value.erase( value.end() - 1 );
        ofs << value << endl;
    }

    ofs.close();
    LOGSTR( 0, CFG_STR_FILE_DONE );

    return true;
}

/**
 * @brief Unserialize runtime configuration settings from #CFG_DAT_FILE_SETTINGS.
 * @retval false Returned if there was an error unserializing settings.
 * @retval true Returned if settings were unserialized successfully.
 */
const bool Server::Config::Unserialize()
{
    UFLAGS_DE( flags );
    ifstream ifs;
    string key, value, line;
    vector<string> token;
    ITER( vector, string, ti );

    LOGSTR( 0, CFG_STR_FILE_SETTINGS_READ );
    ifs.open( CFG_DAT_FILE_SETTINGS );

    if ( !ifs.good() )
    {
        LOGFMT( flags, "Server::Config::Unserialize()-> failed to open settings file: %s", CFG_DAT_FILE_SETTINGS );
        return false;
    }

    while ( getline( ifs, line ) )
    {
        if ( !Utils::KeyValue( key, value, line) )
        {
            LOGFMT( flags, "Server::Config::Unserialize()-> error reading line: %s", CSTR( line ) );
            continue;
        }

        if ( key.compare( "account_prohibited_names" ) == 0 )
        {
            token = Utils::StrTokens( value );
            for ( ti = token.begin(); ti != token.end(); ti++ )
                m_account_prohibited_names.push_front( *ti );
            m_account_prohibited_names.reverse();
        }
    }

    ifs.close();
    LOGSTR( 0, CFG_STR_FILE_DONE );

    return true;
}

/**
 * @brief Sends a message to all clients connected to the Server.
 * @param[in] msg The message to be sent.
 * @retval void
 */
const void Server::Broadcast( const string& msg )
{
    SocketClient *client = NULL;
    ITER( list, SocketClient*, si );

    for ( si = socket_client_list.begin(); si != socket_client_list.end(); si = m_socket_client_next )
    {
        client = *si;
        m_socket_client_next = ++si;

        client->Send( msg );
        client->Send();
    }

    return;
}

/**
 * @brief Compile a Plugin file.
 * @param[in] file The file to be compiled. The file extension should end in #CFG_PLG_BUILD_EXT_IN.
 * @param[in] force Force a rebuild even if the output file already exists.
 * @retval false Returned if a fault is experienced trying to build the plugin.
 * @retval true Returned if the Plugin builds successfully.
 */
const bool Server::BuildPlugin( const string& file, const bool& force )
{
    UFLAGS_DE( flags );
    FILE* popen_fil = NULL;
    string build_cmd, build_res;
    char buf[CFG_STR_MAX_BUFLEN] = {'\0'};

    // No need to run if the output file already exists and a rebuild wasn't forced
    if ( Utils::iReadable( Utils::DirPath( CFG_DAT_DIR_OBJ, file, CFG_PLG_BUILD_EXT_OUT ) ) && !force )
        return true;

    build_cmd = CFG_PLG_BUILD_CMD " -o ";
    build_cmd.append( Utils::DirPath( CFG_DAT_DIR_OBJ, file, CFG_PLG_BUILD_EXT_OUT ) );
    build_cmd.append( " " );
    build_cmd.append( Utils::DirPath( CFG_DAT_DIR_COMMAND, file ) );
    build_cmd.append( " " CFG_PLG_BUILD_OPT );

    // Pipe the build_cmd to the host for processing
    if ( ( popen_fil = popen( CSTR( build_cmd ), "r" ) ) != NULL )
    {
        while( fgets( buf, CFG_STR_MAX_BUFLEN, popen_fil ) != NULL )
            build_res.append( buf );

        pclose( popen_fil );
    }

    // Something went wrong
    if ( !build_res.empty() )
    {
        LOGFMT( flags, "Server::BuildPlugin()->returned error: %s", CSTR( build_res ) );
        return false;
    }
    else
        LOGFMT( 0, "Plugin built successfully: %s", CSTR( file ) );

    return true;
}

Command* Server::FindCommand( const string& name ) const
{
    Command* cmd = NULL;
    bool found = false;
    pair<multimap<const char,Command*>::iterator,multimap<const char,Command*>::iterator> cmd_list;
    MITER( multimap, const char,Command*, mi );

    if ( CFG_GAM_CMD_IGNORE_CASE )
        cmd_list = command_list.equal_range( Utils::Lower( name )[0] );
    else
        cmd_list = command_list.equal_range( name[0] );

    if ( cmd_list.first == cmd_list.second )
        cmd = NULL;
    else
    {
        for ( mi = cmd_list.first; mi != cmd_list.second; mi++ )
        {
            found = false;
            cmd = mi->second;

            if ( CFG_GAM_CMD_IGNORE_CASE )
            {
                if ( Utils::Lower( cmd->gName() ).find( Utils::Lower( name ) ) == 0 )
                    found = true;
            }
            else
            {
                if ( cmd->gName().find( name ) == 0 )
                    found = true;
            }

            if ( found )
                break;
        }

        if ( !found )
            cmd = NULL;
    }

    return cmd;
}

/**
 * @brief Search all subfolders of #CFG_DAT_DIR_COMMAND and call Command::New() to load each file found to memory.
 * @retval false Returned if a fault is experienced trying to obtain a directory listing to process.
 * @retval true Returned if 0 or more Command objects are loaded from disk.
 */
const bool Server::LoadCommands()
{
    UFLAGS_DE( flags );
    timeval start, finish;
    Command* cmd = NULL;
    multimap<bool,string> files;
    MITER( multimap, bool,string, mi );

    start = Utils::CurrentTime();
    LOGSTR( 0, CFG_STR_FILE_COMMAND_READ );

    // Populate the multimap with a recursive listing of the commands folder
    Utils::ListDirectory( CFG_DAT_DIR_COMMAND, true, files, m_dir_close, m_dir_open );

    if ( files.empty() )
    {
        LOGSTR( flags, "Server::LoadCommands()->Utils::ListDirectory()-> CFG_DAT_DIR_COMMAND returned NULL" );
        return false;
    }

    for ( mi = files.begin(); mi != files.end(); mi++ )
    {
        if ( mi->first == UTILS_IS_FILE && ( mi->second.substr( mi->second.find_last_of( "." ) + 1 ).compare( CFG_PLG_BUILD_EXT_IN ) == 0 ) )
        {
            if ( !BuildPlugin( mi->second ) )
            {
                LOGFMT( flags, "Server::LoadCommand()->Server::BuildPlugin()-> file %s returned false", CSTR( mi->second ) );
                continue;
            }

            cmd = new Command();
            if ( !cmd->New( mi->second ) )
            {
                LOGFMT( flags, "Server::LoadCommands()->Command::New()-> command %s returned false", CSTR( mi->second ) );
                delete cmd;
            }
        }
    }

    finish = Utils::CurrentTime();
    LOGFMT( 0, "Loaded %lu commands in %lums.", command_list.size(), Utils::DiffTime( start, finish, UTILS_TIME_MS ) );

    return true;
}

/**
 * @brief Poll all SocketClient objects that are communicating with the associated SocketServer object.
 * @retval false Returned if the file descriptor of the SocketServer is invalid and no further processing can take place.
 * @retval true Returned if the file descriptor of the SocketServer is valid and all SocketClient objects were polled successfully.
 */
const bool Server::PollSockets()
{
    UFLAGS_DE( flags );
    static timespec static_time;
    fd_set exc_set;
    fd_set in_set;
    fd_set out_set;
    ITER( list, SocketClient*, si );
    SocketClient* socket_client;
    sint_t client_desc = 0, max_desc = 0, server_desc = 0;

    FD_ZERO( &exc_set );
    FD_ZERO( &in_set );
    FD_ZERO( &out_set );

    if ( ( server_desc = m_socket->gDescriptor() ) < 1 )
    {
        LOGFMT( flags, "Server::PollSockets()->SocketServer::gDescriptor()-> returned invalid descriptor: %ld", server_desc );
        return false;
    }

    FD_SET( server_desc, &in_set );
    max_desc = server_desc;

    // Build three file descriptor lists to be polled
    for ( si = socket_client_list.begin(); si != socket_client_list.end(); si = m_socket_client_next )
    {
        socket_client = *si;
        m_socket_client_next = ++si;

        if ( ( client_desc = socket_client->gDescriptor() ) < 1 )
        {
            LOGFMT( flags, "Server::PollSockets()->SocketClient::gDescriptor()-> returned invalid descriptor: %ld", client_desc );
            socket_client->Delete();
            continue;
        }

        if ( socket_client->Quitting() )
        {
            socket_client->Delete();
            continue;
        }

        max_desc = max( server_desc, client_desc );

        // Populate lists of: exceptions, pending input, pending output
        FD_SET( client_desc, &exc_set );
        FD_SET( client_desc, &in_set );
        FD_SET( client_desc, &out_set );
    }

    // Ensure the file descriptor lists can be watched for updates
    if ( ::pselect( max_desc + 1, &in_set, &out_set, &exc_set, &static_time, 0 ) < 0 )
    {
        LOGERRNO( flags, "Server::PollSockets()->pselect()->" );
        return false;
    }

    // Process new connections
    if ( FD_ISSET( server_desc, &in_set ) )
        m_socket->Accept();

    // Process faulted connections
    for ( si = socket_client_list.begin(); si != socket_client_list.end(); si = m_socket_client_next )
    {
        socket_client = *si;
        m_socket_client_next = ++si;

        if ( ( client_desc = socket_client->gDescriptor() ) < 1 )
        {
            LOGFMT( flags, "Server::PollSockets()->SocketClient::gDescriptor()-> returned invalid descriptor: %ld", client_desc );
            socket_client->Delete();
            continue;
        }

        // Found a faulted socket in the exceptions list
        if ( FD_ISSET( client_desc, &exc_set ) )
        {
            // Don't try to save characters on faulty clients, just boot them
            LOGFMT( flags, "Server::PollSockets()-> disconnecting faulted descriptor: %ld", client_desc );
            socket_client->Delete();
            continue;
        }
    }

    // Process input from active connections
    for ( si = socket_client_list.begin(); si != socket_client_list.end(); si = m_socket_client_next )
    {
        socket_client = *si;
        m_socket_client_next = ++si;

        if ( ( client_desc = socket_client->gDescriptor() ) < 1 )
        {
            LOGFMT( flags, "Server::PollSockets()->SocketClient::gDescriptor()-> returned invalid descriptor: %ld", client_desc );
            // todo: save character
            socket_client->Delete();
            continue;
        }

        // Found an active socket in the input list
        if ( FD_ISSET( client_desc, &in_set ) )
        {
            // Pending input; clear the idle timeout
            if ( !socket_client->sIdle( 0 ) )
            {
                LOGFMT( flags, "Server::PollSockets()->SocketClient::sIdle()-> descriptor %ld returned false setting idle: 0", client_desc );
                // todo: save character
                socket_client->Delete();
                continue;
            }

            // Read input, save game character and disconnect socket if unable to
            if ( !socket_client->Recv() )
            {
                LOGFMT( flags, "Server::PollSockets()->SocketClient::Recv()-> descriptor %ld returned false", client_desc );
                // todo: save character
                socket_client->Delete();
                continue;
            }
        }
        else
        {
            if ( !socket_client->sIdle( socket_client->gIdle() + 1 ) )
            {
                LOGFMT( flags, "Server::PollSockets()->SocketClient::sIdle()-> descriptor %ld returned false setting idle: %lu", client_desc, socket_client->gIdle() + 1 );
                // todo: save character
                socket_client->Delete();
                continue;
            }
        }
    }

    // Process any pending output
    for ( si = socket_client_list.begin(); si != socket_client_list.end(); si = m_socket_client_next )
    {
        socket_client = *si;
        m_socket_client_next = ++si;

        if ( ( client_desc = socket_client->gDescriptor() ) < 1 )
        {
            LOGFMT( flags, "Server::PollSockets()->SocketClient::gDescriptor()-> returned invalid descriptor: %ld", client_desc );
            // todo: save character
            socket_client->Delete();
            continue;
        }

        // Disconnect sockets that have been idle for too long
        if ( socket_client->gIdle() >= CFG_SOC_MAX_IDLE )
        {
            LOGFMT( flags, "Server::PollSockets()->SocketClient::gIdle()-> disconnecting idle descriptor: %ld", client_desc );
            socket_client->Send( CFG_STR_IDLE );
            socket_client->Delete();
            // todo: save character
            continue;
        }

        // Found an active socket in the output list
        if ( FD_ISSET( client_desc, &out_set ) && socket_client->PendingOutput() )
        {
            // Send output, save game character and disconnect socket if unable to
            if ( !socket_client->Send() )
            {
                LOGFMT( flags, "Server::PollSockets()->SocketClient::PendingOutput()->SocketClient::Send()-> descriptor %ld returned false", client_desc );
                socket_client->Delete();
                // todo: save character
                continue;
            }
        }
    }

    return true;
}

/**
 * @brief Process any scheduled events tied to the Server.
 * @retval void
 */
const void Server::ProcessEvents()
{
    ITER( forward_list, Event*, ei );
    ITER( forward_list, Event*, ei_next );
    Event* event;

    for ( ei = event_list.begin(); ei != event_list.end(); ei = ei_next )
    {
        event = *ei;
        ei_next = ++ei;

        if ( !event->Update() )
            event->Run();
    }

    return;
}

/**
 * @brief Processes input from all active SocketClient objects.
 * @retval void
 */
const void Server::ProcessInput()
{
    UFLAGS_DE( flags );
    ITER( list, SocketClient*, si );
    SocketClient* socket_client;
    sint_t client_desc = 0;

    for ( si = socket_client_list.begin(); si != socket_client_list.end(); si = m_socket_client_next )
    {
        socket_client = *si;
        m_socket_client_next = ++si;

        if ( ( client_desc = socket_client->gDescriptor() ) < 1 )
        {
            LOGFMT( flags, "Server::ProcessInput()->SocketClient::gDescriptor()-> returned invalid descriptor: %ld", client_desc );
            // todo: save character
            socket_client->Delete();
            continue;
        }

        if ( !socket_client->ProcessInput() )
        {
            LOGFMT( flags, "Server::ProcessInput()->SocketClient::ProcessInput()-> descriptor %ld returned false", client_desc );
            // todo: save character
            socket_client->Delete();
            continue;
        }

        if ( socket_client->PendingCommand() )
        {
            if ( !socket_client->ProcessCommand() )
            {
                LOGFMT( flags, "Server::ProcessInput()->SocketClient::PendingCommand()->SocketClient::ProcessCommand()-> descriptor %ld returned false", client_desc );
                // todo: save character
                socket_client->Delete();
                continue;
            }
        }
    }

    return;
}

/**
 * @brief Handle SocketClient objects who have not fully logged into the game yet.
 * @param[in] client The SocketClient to process a login request for.
 * @param[in] cmd The command sent by the SocketClient.
 * @param[in] args Any arguments to the command.
 * @retval void
 */
const void Server::ProcessLogin( SocketClient* client, const string& cmd, const string& args )
{
    UFLAGS_DE( flags );
    Account* account = NULL;
    Command* command = NULL;

    switch ( client->gState() )
    {
        case SOC_STATE_LOGIN_SCREEN:
            if ( ( command = FindCommand( cmd ) ) != NULL && command->Authorized( client->gSecurity() ) )
                command->Run( client, cmd, args );
            else
            {
                account = new Account();
                if ( !account->New( client, cmd ) )
                    account->Delete();
            }
        break;

        default:
        break;
    }

    return;
}

/**
 * @brief Recovers the server state and re-connects client sockets after a reboot.
 * @param[in] reboot True if the server was started via a reboot. Must be true for this to run.
 * @retval void
 * @todo Clean this up.
 */
const void Server::RebootRecovery( const bool& reboot )
{
    ifstream recovery;
    string key, value, line;
    SocketClient *client = NULL;

    if ( reboot )
    {
        recovery.open( CFG_DAT_FILE_REBOOT, ifstream::in );

        while ( recovery.is_open() && recovery.good() && getline( recovery, line ) )
        {
            if ( !Utils::KeyValue( key, value, line) )
            {
                cout << "Error reading line " << line << endl;
                continue;
            }
            cout << "key={" << key << "} && value={" << value << "}" << endl;
            if ( key.compare( "desc" ) == 0 )
                client = new SocketClient( this, atoi( CSTR( value ) ) );
            if ( key.compare( "port" ) == 0 )
                client->sPort( atoi( CSTR( value ) ) );
            if ( key.compare( "host" ) == 0 )
                client->sHostname( value );
            if ( key.compare( "recv" ) == 0 )
                client->aBytesRecvd( atoi( CSTR( value ) ) );
            if ( key.compare( "sent" ) == 0 )
                client->aBytesSent( atoi( CSTR( value ) ) );
            if ( key.compare( "idle" ) == 0 )
                client->sIdle( atoi( CSTR( value ) ) );
            if ( key.compare( "secu" ) == 0 )
                client->sSecurity( atoi( CSTR( value ) ) );
            if ( key.compare( "stat" ) == 0 )
                client->sState( atoi( CSTR( value ) ) );
        }

        recovery.close();
        ::unlink( CFG_DAT_FILE_REBOOT );
    }

    return;
}

/**
 * @brief Reloads a Command Plugin.
 * @param[in] name The name of the Command to be reloaded.
 * @retval false Returned if the command is unable to be reloaded.
 * @retval true Returned if the command was successfully reloaded.
 */
const bool Server::ReloadCommand( const string& name )
{
    UFLAGS_DE( flags );
    Command* command = NULL;
    string file;

    command = FindCommand( name );

    if ( command == NULL )
    {
        LOGFMT( flags, "Server::ReloadCommand()->Server::FindCommand()-> command %s returned NULL", CSTR( name ) );
        return false;
    }

    file = command->gFile();
    command->Delete();
    command = new Command();

    if ( !command->New( file ) )
    {
        LOGFMT( flags, "Server::ReloadCommand()->Command::New()-> command %s returned false", CSTR( file ) );
        delete command;
        return false;
    }

    return true;
}

/**
 * @brief Returns if the Server is shutdown or not.
 * @retval false Returned if the server is inactive and is shutdown.
 * @retval true Returned if the server is active and not shutdown.
 */
const bool Server::Running() const
{
    return !m_shutdown;
}

/**
 * @brief Perform a clean shutdown of the NAMS Server providing a chance to complete disk writes and free all memory to verify there are no leaks.
 * @param[in] status The shutdown code to pass to exit() when NAMS exits. Either EXIT_FAILURE or EXIT_SUCCESS.
 * @retval void
 */
const void Server::Shutdown( const sint_t& status )
{
    bool was_running = !m_shutdown;

    Broadcast( CFG_STR_SHUTDOWN );
    m_shutdown = true;

    // Write runtime settings
    m_config->Serialize();

    // Cleanup commands
    while ( !command_list.empty() )
        command_list.begin()->second->Delete();
    // Cleanup socket clients
    while ( !socket_client_list.empty() )
        socket_client_list.front()->Delete();
    // Cleanup socket servers
    while ( !socket_server_list.empty() )
        socket_server_list.front()->Delete();

    // Only output if the server actually booted; otherwise it probably faulted while getting a port from main()
    if ( was_running )
    {
        if ( status == EXIT_SUCCESS )
            LOGSTR( 0, CFG_STR_EXIT_SUCCESS );
        else
            LOGSTR( 0, CFG_STR_EXIT_FAILURE );
    }

    ::exit( status );
}

/**
 * @brief Start the NAMS Server. Responsible for calling all critical boot-time functions such as SocketServer initialization, Command loading, etc.
 * @retval void
 */
const void Server::Startup( const sint_t& desc )
{
    UFLAGS_DE( flags );
    SocketServer* socket_server = NULL;
    sint_t descriptor = 0;
    bool reboot = false;
    m_shutdown = false;

    LOGFMT( 0, "%s started.", CFG_STR_VERSION );
    m_time_boot = Utils::CurrentTime();

    // Fresh boot, otherwise it would already be assigned during a reboot
    if ( desc == 0 )
    {
        if ( ( descriptor = ::socket( AF_INET6, SOCK_STREAM, 0 ) ) < 0 )
        {
            LOGERRNO( flags, "Server::Startup()->socket()->" );
            Shutdown( EXIT_FAILURE );
        }
    }
    else
    {
        descriptor = desc;
        reboot = true;
    }

    socket_server = new SocketServer( this, descriptor );
    m_socket = socket_server;

    if ( !socket_server->New( reboot ) )
    {
        LOGSTR( flags, "Server::Startup()->SocketServer::New()-> returned false" );
        Shutdown( EXIT_FAILURE );
    }

    // Bump ourselves to the root folder for file paths
    if ( !reboot && ::chdir( ".." ) < 0 )
    {
        LOGERRNO( flags, "Server::Startup()->chdir()->" );
        Shutdown( EXIT_FAILURE );
    }

    if ( !m_config->Unserialize() )
    {
        LOGSTR( flags, "Server::Config::Unserialize()-> returned false" );
        Shutdown( EXIT_FAILURE );
    }

    if ( !LoadCommands() )
    {
        LOGSTR( flags, "Server::Startup()->Server::LoadCommands()-> returned false" );
        Shutdown( EXIT_FAILURE );
    }

    RebootRecovery( reboot );

    LOGFMT( 0, "%s is ready on port %lu.", CFG_STR_VERSION, m_port );
    LOGSTR( 0, "Last compiled on " __DATE__ " at " __TIME__ "." );

    return;
}

/**
 * @brief The core update loop of NAMS. This loop spawns all other subsystem update routines and then sleeps for ( #USLEEP_MAX / #CFG_GAM_PULSE_RATE ) each cycle.
 * @retval void
 */
const void Server::Update()
{
    UFLAGS_DE( flags );

    m_time_current = Utils::CurrentTime();

    // Poll all sockets for changes
    if ( !PollSockets() )
    {
        LOGSTR( flags, "Server::Update()->Server::PollSockets()-> returned false" );
        Shutdown( EXIT_FAILURE );
    }

    // Process any input received
    ProcessInput();

    // Process any scheduled events
    ProcessEvents();

    // Sleep to control game pacing
    ::usleep( USLEEP_MAX / CFG_GAM_PULSE_RATE );

    return;
}

/* Query */
/**
 * @brief Gets the hostname of the machine that NAMS is running on.
 * @retval string A string is returned containing either "(unknown)" or the machine hostname.
 * @todo Move this to Server::sHostname() and redo this func as a simple getter. Don't need to query the host server multiple times.
 */
const string Server::gHostname() const
{
    UFLAGS_DE( flags );
    string output;
    char hostname[CFG_STR_MAX_BUFLEN] = {'\0'};

    if ( ::gethostname( hostname, CFG_STR_MAX_BUFLEN - 1 ) < 0 )
    {
        LOGERRNO( flags, "Server::gHostname()->gethostname()->" );
        output = "(unknown)";

        return output;
    }

    output = hostname;

    return output;
}

/**
 * @brief Returns the port that any SocketServer should bind to. Defaults to #CFG_SOC_PORTNUM.
 * @retval uint_t The port for a SocketServer to bind to.
 */
const uint_t Server::gPort() const
{
    return m_port;
}

/**
 * @brief Returns the SocketServer associated with the current Server instance.
 * @retval SocketServer A pointer to the #SocketServer object spanwed by this object.
 */
SocketServer* Server::gSocket() const
{
    return m_socket;
}

/**
 * @brief Returns the Server's globally referenced next iterator for SocketClient objects.
 * @retval SocketClient* The Server's globally referenced next iterator for SocketClient objects.
 */
list<SocketClient*>::iterator Server::gSocketClientNext() const
{
    return m_socket_client_next;
}

/**
 * @brief Returns the combined number of SocketClient and SocketServer objects that have been destroyed.
 * @retval uint_t The total number of closed sockets that were tied to this object.
 */
const uint_t Server::gSocketClose() const
{
    return m_socket_close;
}

/**
 * @brief Returns the combined number of SocketClient and SocketServer objects that have been created.
 * @retval uint_t The total number of opened sockets that are tied to this object.
 */
const uint_t Server::gSocketOpen() const
{
    return m_socket_open;
}

/**
 * @brief Display miscellaneous data about the NAMS Server, such as total data transfered, objects in memory, etc.
 * @retval string A string is returned containing a pre-formatted data display of all Server information.
 * @todo Write this entire function.
 */
const string Server::gStatus() const
{
    string output;

    return output;
}

/* Manipulate */
/**
 * @brief Set the port of a NAMS Server object.
 * @param[in] port The port number to have a SocketServer instance listen for new connections on.
 * @retval false Returned if the port was <= #CFG_SOC_MIN_PORTNUM or >= #CFG_SOC_MAX_PORTNUM.
 * @retval true Returned if the port was > #CFG_SOC_MIN_PORTNUM and < #CFG_SOC_MAX_PORTNUM.
 */
const bool Server::sPort( const uint_t& port )
{
    // No logger output; this should only be called pre-boot
    if ( port <= CFG_SOC_MIN_PORTNUM || port >= CFG_SOC_MAX_PORTNUM )
        return false;

    m_port = port;

    return true;
}

/**
 * @brief Sets the Server's globally referenced next iterator for SocketClient objects.
 * @param[in] next The next SocketClient in socket_client_list after removing an element.
 * @retval void
 */
const void Server::sSocketClientNext( list<SocketClient*>::iterator next )
{
    m_socket_client_next = next;

    return;
}

/**
 * @brief Set the amount of subordinate SocketClient and SocketServer objects that have been closed on a NAMS Server object.
 * @param[in] amount The amount that Server::m_socket_close should be set to.
 * @retval false Returned if amount is outside the boundaries of a uint_t variable.
 * @retval true Returned if amount is within the boundaries of a uint_t variable.
 */
const bool Server::sSocketClose( const uint_t& amount )
{
    UFLAGS_DE( flags );

    if ( amount < uintmin_t || amount >= uintmax_t )
    {
        LOGFMT( flags, "Server::sSocketClose()-> called with m_socket_close overflow: %lu + %lu", m_socket_close, amount );
        return false;
    }

    m_socket_close = amount;

    return true;
}

/**
 * @brief Set the amount of subordinate SocketClient and SocketServer objects that have been opened on a NAMS Server object.
 * @param[in] amount The amount that Server::m_socket_open should be set to.
 * @retval false Returned if amount is outside the boundaries of a uint_t variable.
 * @retval true Returned if amount is within the boundaries of a uint_t variable.
 */
const bool Server::sSocketOpen( const uint_t& amount )
{
    UFLAGS_DE( flags );

    if ( amount < uintmin_t || amount >= uintmax_t )
    {
        LOGFMT( flags, "Server::sSocketOpen()-> called with m_socket_open overflow: %lu + %lu", m_socket_open, amount );
        return false;
    }

    m_socket_open = amount;

    return true;
}

/* Internal */
/**
 * @brief Constructor for the Server::Config class.
 */
Server::Config::Config()
{
    m_account_prohibited_names.clear();

    return;
}

/**
 * @brief Destructor for the Server::Config class.
 */
Server::Config::~Config()
{
    return;
}

/**
 * @brief Constructor for the Server class.
 */
Server::Server()
{
    m_config = new Server::Config();
    m_dir_close = 0;
    m_dir_open = 0;
    m_port = 0;
    m_shutdown = true;
    m_socket = 0;
    m_socket_client_next = socket_client_list.begin();
    m_socket_close = 0;
    m_socket_open = 0;
    m_time_boot = timeval();
    m_time_current = timeval();

    return;
}

/**
 * @brief Destructor for the Server class.
 */
Server::~Server()
{
    delete m_config;

    return;
}

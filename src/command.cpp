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
 * @file command.cpp
 * @brief All non-template member functions of the Command class.
 *
 * Command objects are built as independently loaded Plugin classes. The
 * Server will compile any file within #CFG_DAT_DIR_COMMAND and ending in
 * #CFG_PLG_BUILD_EXT_IN to a shared object file stored in #CFG_DAT_DIR_OBJ.
 *
 * Each object file is a unique class derived from Plugin. The Command class
 * then acts as an abstraction layer for interacting with thse objects in a
 * more specialized manner, as the Plugin class is broadly generic to support
 * other implementation uses.
 */
#include "h/includes.h"
#include "h/class.h"

#include "h/command.h"
#include "h/list.h"

/* Core */
/**
 * @brief Unload a command from memory that was previously loaded via Command::New().
 * @retval void
 */
const void Command::Delete()
{
    UFLAGS_DE( flags );
    MITER( multimap, const char,Command*, mi );

    m_plg_delete( m_plg );
    ::dlerror();

    if ( ::dlclose( m_plg_handle ) )
        LOGFMT( flags, "Command::Delete()->dlclose() returned error: %s", ::dlerror() );

    for ( mi = command_list.begin(); mi != command_list.end(); )
    {
        if ( mi->second == this )
        {
            command_list.erase( mi++ );
            break;
        }
        else
            ++mi;
    }
    delete this;

   return;
}

/**
 * @brief Load a plugin command from #CFG_DAT_DIR_OBJ.
 * @param[in] file The filename to load without any path prepended to it.
 * @retval false Returned if the command in file was not found or unable to be loaded.
 * @retval true Returned if the command in file was successfully loaded.
 */
const bool Command::New( const string& file )
{
    UFLAGS_DE( flags );
    string path( Utils::DirPath( CFG_DAT_DIR_OBJ, file, CFG_PLG_BUILD_EXT_OUT ) );

    // Ensure there is a valid file to open
    if ( !Utils::iFile( path ) )
    {
        LOGFMT( flags, "Command::New()->Utils::iFile()-> returned false for path: %s", CSTR( path ) );
        return false;
    }

    if ( ( m_plg_handle = ::dlopen( CSTR( path ), RTLD_LAZY | RTLD_GLOBAL ) ) == NULL )
    {
        LOGFMT( flags, "Command::New()->dlopen returned error: %s", ::dlerror() );
        return false;
    }
    else
    {
        m_plg_delete = (PluginDelete*) ::dlsym( m_plg_handle, "Delete" );
        m_plg_file = file;
        m_plg_new = (PluginNew*) ::dlsym( m_plg_handle, "New" );
        m_plg = m_plg_new();
    }

    if ( CFG_GAM_CMD_IGNORE_CASE )
        command_list.insert( pair<const char,Command*>( Utils::Lower( gName() )[0], this ) );
    else
        command_list.insert( pair<const char,Command*>( gName()[0], this ) );

    return true;
}

/**
 * @brief Execute a Plugin object's primary function.
 * @param[in] client If called from a SocketClient, the caller is passed through to the Plugin for reference.
 * @param[in] cmd If called from a SocketClient, the command from the client is passed through.
 * @param[in] arg If called from a SocketClient, the arguments from the client are passed through.
 * @retval void
 */
const void Command::Run( SocketClient* client, const string& cmd, const string& arg ) const
{
    m_plg->Run( client, cmd, arg );

    return;
}

/* Query */
/**
 * @brief Return the name of the associated Plugin object.
 * @retval string A string containing the name of the associated Plugin object.
 */
const string Command::gName() const
{
    return m_plg->gName();
}

/* Manipulate */

/* Internal */
/**
 * @brief Constructor for the Command class.
 */
Command::Command()
{
    m_plg = NULL;
    m_plg_delete = NULL;
    m_plg_file.clear();
    m_plg_handle = NULL;
    m_plg_new = NULL;
    m_preempt = false;
    m_security = SOC_SECURITY_NONE;

    return;
}

/**
 * @brief Destructor for the Command class.
 */
Command::~Command()
{
    return;
}
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
 * @file handler.h
 * @brief The Handler namespace.
 *
 *  This file contains the Handler namespace and template functions.
 */
#ifndef DEC_HANDLER_H
#define DEC_HANDLER_H

using namespace std;

/**
 * @brief The Handler namespace contains process control functions.
 */
namespace Handler
{
    /** @name Core */ /**@{*/
    Character* FindCharacter( const string& name, const uint_t& type, const vector<Character*>& clist );
    Command* FindCommand( const string& name );
    Exit* FindExit( const string& name, Location* location );
    Location* FindLocation( const string& name, const uint_t& type );
    Object* FindObject( const string& name, const uint_t& type, const vector<Object*>& olist );
    Thing* FindThing( const string& name, const uint_t& type, const uint_t& scope, Thing* caller, const bool& self = false );
    const void LoginHandler( SocketClient* client, const string& cmd = "", const string& args = "" );
    /**@}*/

    /** @name Query */ /**@{*/
    const bool CheckCreating( SocketClient* client, const string& name );
    const bool CheckPlaying( const string& name );
    const bool CheckProhibited( SocketClient* client, const string& name, const uint_t& type );
    /**@}*/

    /** @name Manipulate */ /**@{*/
    const void AttachAccount( SocketClient* client, const string& cmd, const string& args );
    const void AttachCharacter( SocketClient* client, const string& cmd, const string& args );
    const void Reconnect( SocketClient* client, Character* character );
    /**@}*/

    /** @name Internal */ /**@{*/
    const void AccountMenuMain( SocketClient* client, const string& cmd, const string& args );
    const void CharacterCreateMenuMain( SocketClient* client, const string& cmd, const string& args );
    const void CharacterCreateName( SocketClient* client, const string& cmd, const string& args );
    const void CharacterCreateSex( SocketClient* client, const string& cmd, const string& args );
    const void CharacterDeleteConfirm( SocketClient* client, const string& cmd, const string& args );
    const void CharacterDeleteMenuMain( SocketClient* client, const string& cmd, const string& args );
    const void CharacterLoadMenuMain( SocketClient* client, const string& cmd, const string& args );
    const void EnterGame( SocketClient* client, const string& cmd, const string& args );
    const void GetNewAccount( SocketClient* client, const string& cmd, const string& args );
    const void GetNewPassword( SocketClient* client, const string& cmd, const string& args );
    const void GetOldPassword( SocketClient* client, const string& cmd, const string& args );
    const void LoadCharacter( SocketClient* client, const string& cmd, const string& args );
    const void LoginScreen( SocketClient* client, const string& cmd, const string& args );
    /**@}*/
};

#endif

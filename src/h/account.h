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
 * @file account.h
 * @brief The Account class.
 *
 *  This file contains the Account class and template functions.
 */
#ifndef DEC_ACCOUNT_H
#define DEC_ACCOUNT_H

#include "character.h"
#include "socketclient.h"

using namespace std;

/**
 * @brief Security structure to maintain characters, roles, and permissions.
 */
class Account {
    public:
        /** @name Core */ /**@{*/
        const void ClearCharacter();
        const void Delete();
        const bool New( SocketClient* client, const bool& exists );
        const bool Serialize() const;
        const bool Unserialize();
        /**@}*/

        /** @name Query */ /**@{*/
        Character* gCharacter() const;
        const list<string> gCharacters() const;
        SocketClient* gClient() const;
        const list<pair<string,string>> gLogins( const uint_t& type ) const;
        const string gName() const;
        const uint_t gSecurity() const;
        /**@}*/

        /** @name Manipulate */ /**@{*/
        const bool aCharacter( const string& name );
        const bool sCharacter( Character* character );
        const bool aLogin( const string& date, const string& name, const uint_t& type );
        const bool sSecurity( const uint_t& security );
        /**@}*/

        /** @name Internal */ /**@{*/
        Account();
        ~Account();
        /**@}*/

    private:
        Character* m_character; /**< The active Character, if any. */
        list<string> m_characters; /**< The names of all characters associated with the account. */
        SocketClient* m_client; /**< The client attached to the account. */
        list<pair<string,string>> m_logins[MAX_ACT_LOGIN]; /**< Date and hostname of previous login failures/successes. */
        string m_name; /**< The name of the account. */
        string m_password; /**< The encrypted password of the account. */
        uint_t m_security; /**< Security level for commands and restricted access. */
};

#endif

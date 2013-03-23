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
 * @file thing.h
 * @brief The Thing class.
 *
 *  This file contains the Thing class and template functions.
 */
#ifndef DEC_THING_H
#define DEC_THING_H

#include "server.h"

using namespace std;

/**
 * @brief A generic "thing": creature, character, room, object, etc.
 */
class Thing {
    public:
        /** @name Core */ /**@{*/
        virtual const void Delete() = 0;
        virtual const bool New( Server* server ) = 0;
        virtual const bool Serialize() const = 0;
        virtual const bool Unserialize() = 0;
        /**@}*/

        /** @name Query */ /**@{*/
        const string gId() const;
        const string gName() const;
        Server* gServer() const;
        /**@}*/

        /** @name Manipulate */ /**@{*/
        const bool sId( const string& id );
        const bool sName( const string& name, const bool& system = false );
        const bool sServer( Server* server );
        /**@}*/

        /** @name Internal */ /**@{*/
        Thing();
        ~Thing();
        /**@}*/

    private:
        string m_id; /**< An identifier to denote ownership. For characters, id = account.name */
        string m_name; /**< The name of the thing. */
        Server* m_server; /**< The Server this Thing exists within. */
};

#endif

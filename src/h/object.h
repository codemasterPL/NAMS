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
 * @file object.h
 * @brief The Object class.
 *
 *  This file contains the Object class and template functions.
 */
#ifndef DEC_OBJECT_H
#define DEC_OBJECT_H

#include "thing.h"

using namespace std;

/**
 * @brief A physical object within the game world.
 */
class Object : public Thing
{
    public:
        /** @name Core */ /**@{*/
        const bool Clone( const string& name, const uint_t& type );
        const void Delete();
        const void Interpret( const uint_t& security, const string& cmd, const string& args );
        const bool New( const string& file );
        const bool Serialize() const;
        const bool Unserialize();
        /**@}*/

        /** @name Query */ /**@{*/
        /**@}*/

        /** @name Manipulate */ /**@{*/
        /**@}*/

        /** @name Internal */ /**@{*/
        Object();
        ~Object();
        /**@}*/

    private:
        string m_file; /**< Path to the file on disk. */
};

#endif

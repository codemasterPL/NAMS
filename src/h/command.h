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
 * @file command.h
 * @brief The Command class.
 *
 *  This file contains the Command class, templates, and trivial member functions.
 */
#ifndef DEC_COMMAND_H
#define DEC_COMMAND_H

#include <cstdio>
#include <dlfcn.h>
#include <fstream>

#include "plugin.h"

class Command {
    public:
        /** @name Core */ /**@{*/
        const void Delete();
        const bool New( const string& file );
        const void Run( SocketClient* client = NULL ) const { m_plg->Run( client ); return; }
        /**@}*/

        /** @name Query */ /**@{*/
        const string gName() const { return m_plg->gName(); }
        /**@}*/

        /** @name Manipulate */ /**@{*/
        /**@}*/

        /** @name Internal */ /**@{*/
        Command();
        ~Command();
        /**@}*/

    private:
        uint_t m_level;
        Plugin* m_plg;
        DeletePlugin* m_plg_delete;
        void* m_plg_handle;
        NewPlugin* m_plg_new;
        bool m_preempt;
};

#endif

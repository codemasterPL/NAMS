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

#include "pincludes.h"

class Put : public Plugin {
    public:
        virtual const void Run( Character* character = NULL, const string& cmd = "", const string& arg = "" ) const;
        virtual const void Run( SocketClient* client = NULL, const string& cmd = "", const string& arg = "" ) const;

        Put( const string& name, const uint_t& type );
        ~Put();
};

const void Put::Run( Character* character, const string& cmd, const string& arg ) const
{
    vector<Thing*> objects;
    vector<Thing*> targets;
    Thing* object = NULL;
    Thing* target = NULL;
    CITER( vector, Thing*, oi );
    CITER( vector, Thing*, ti );
    string args, sobj, star;
    bool found = false;

    if ( character )
    {
        args = arg;
        sobj = Utils::Argument( args );
        star = Utils::Argument( args );

        if ( sobj.empty() || star.empty() )
        {
            character->Send( "Put what in what?" CRLF );
            return;
        }

        objects = character->gContents();
        for ( oi = objects.begin(); oi != objects.end(); oi++ )
        {
            object = *oi;

            if ( Utils::iName( sobj, object->gName() ) )
            {
                found = true;
                break;
            }
        }

        if ( !found )
        {
            character->Send( "You don't have that item." CRLF );
            return;
        }

        found = false;
        targets = character->gContainer()->gContents();
        for ( ti = targets.begin(); ti != targets.end(); ti++ )
        {
            target = *ti;

            // Don't want to 'put' an Object in a Character
            if ( target->gType() != THING_TYPE_OBJECT )
                continue;

            if ( Utils::iName( star, target->gName() ) )
            {
                found = true;
                break;
            }
        }

        // If no target is found in the room, search the inventory
        if ( !found )
        {
            targets = character->gContents();
            for ( ti = targets.begin(); ti != targets.end(); ti++ )
            {
                target = *ti;

                // Don't put an Object inside of itself
                if ( object == target )
                    continue;

                // Don't want to 'put' an Object in a Character
                if ( target->gType() != THING_TYPE_OBJECT )
                    continue;

                if ( Utils::iName( star, target->gName() ) )
                {
                    found = true;
                    break;
                }
            }
        }

        // Final check
        if ( !found )
        {
            character->Send( "There is no " + star + " here." CRLF );
            return;
        }

        character->Send( "You put " + object->gDescription( THING_DESCRIPTION_SHORT ) + " in " + target->gDescription( THING_DESCRIPTION_SHORT ) + "." CRLF );
        character->gContainer()->Send(  character->gName() + " puts " + object->gDescription( THING_DESCRIPTION_SHORT ) + " in " + target->gDescription( THING_DESCRIPTION_SHORT )+ "." CRLF, character );
        object->Move( character, target );
    }

    return;
}

const void Put::Run( SocketClient* client, const string& cmd, const string& arg ) const
{
    return;
}

Put::Put( const string& name = "put", const uint_t& type = PLG_TYPE_COMMAND ) : Plugin( name, type )
{
    Plugin::sBool( PLG_TYPE_COMMAND_BOOL_PREEMPT, false );
    Plugin::sUint( PLG_TYPE_COMMAND_UINT_SECURITY, ACT_SECURITY_AUTH_USER );

    return;
}

Put::~Put()
{
}

extern "C" {
    Plugin* New() { return new Put(); }
    void Delete( Plugin* p ) { delete p; }
}

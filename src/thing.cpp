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
 * @file thing.cpp
 * @brief All non-template member functions of the Thing class.
 *
 * A generic "thing": creature, character, room, object, etc.
 */
#include "h/includes.h"
#include "h/class.h"

#include "h/thing.h"

/* Core */
/**
 * @brief Unload a thing from memory that was previously loaded via Thing::New().
 * @retval void
 */
const void Thing::Delete()
{
    delete this;

    return;
}

/**
 * @brief Create a new thing.
 * @param[in] server The Server the thing will exist within.
 * @retval false Returned if a new Thing was successfully created or loaded.
 * @retval true Returned if a new Thing was unable to be created.
 */
const bool Thing::New( Server* server )
{
    return true;
}

/* Query */
/**
 * @brief Returns the id associated with this Thing.
 * @retval string A string containing the id associated with this Thing.
 */
const string Thing::gId() const
{
    return m_id;
}

/**
 * @brief Returns the name associated with this Thing.
 * @retval string A string containing the name associated with this Thing.
 */
const string Thing::gName() const
{
    return m_name;
}

/* Manipulate */
/**
 * @brief Sets the id of this Thing.
 * @param[in] id A string containing the id this Thing should be set to.
 * @retval false Returned if there was an error setting the id.
 * @retval true Returned if the id was set successfully.
 */
const bool Thing::sId( const string& id )
{
    UFLAGS_DE( flags );

    if ( id.length() < CFG_THG_ID_MIN_LEN || id.length() > CFG_THG_ID_MAX_LEN )
    {
        LOGFMT( flags, "Thing::sId()-> called with invalid id length %lu", id.length() );
        return false;
    }

    m_id = id;

    return true;
}

/**
 * @brief Sets the name of this Thing.
 * @param[in] id A string containing the name this Thing should be set to.
 * @param[in] system If true, denotes a system-level call (NPCs, etc) and will bypass length limits.
 * @retval false Returned if there was an error setting the name.
 * @retval true Returned if the name was set successfully.
 */
const bool Thing::sName( const string& name, const bool& system )
{
    UFLAGS_DE( flags );

    if ( ( name.length() < CFG_THG_NAME_MIN_LEN || name.length() > CFG_THG_NAME_MAX_LEN ) && !system )
    {
        LOGFMT( flags, "Thing::sName()-> called with invalid name length %lu", name.length() );
        return false;
    }

    m_name = name;

    return true;
}

/* Internal */
/**
 * @brief Constructor for the Thing class.
 */
Thing::Thing()
{
    m_id.clear();
    m_name.clear();

    return;
}

/**
 * @brief Destructor for the Thing class.
 */
Thing::~Thing()
{
    return;
}

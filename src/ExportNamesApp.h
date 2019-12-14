/*
 * ExportNamesApp.h
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file "COPYING" for details); if 
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave, 
 * Cambridge, MA 02139, USA.
 */

#pragma once

#include "MyString.h"

class ExportNamesApp {
public:
                        ExportNamesApp();

    void                setPrefix(const char* prefix);
    const char         *getPrefix(void) { return m_prefix; }

    const char         *getCPrefix(void);

    void                updatePrefix(void);

    void                buildCSceneGraphName(void);
    const char         *getCSceneGraphName(void) { return m_cSceneGraphName; }

    void                buildCProcessEventsName(void);
    const char         *getCProcessEventsName(void) 
                           { return m_cProcessEventsName; }

    void                buildCNodeName(void);
    const char         *getCNodeName(void) { return m_cNodeName; }
    const char         *getCNodeNamePtr(void) { return m_cNodeNamePtr; }
    const char         *getCNodeNameArray(void) { return m_cNodeNameArray; }

protected:
    const char         *m_prefix;
    MyString            m_cPrefilx;
    MyString            m_cNodeName;
    MyString            m_cNodeNamePtr;
    MyString            m_cNodeNameArray;
    MyString            m_cSceneGraphName;
    MyString            m_cProcessEventsName;
};

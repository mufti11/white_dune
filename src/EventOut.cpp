/*
 * EventOut.cpp
 *
 * Copyright (C) 1999 Stephen F. White
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

#include <stdio.h>
#include "stdafx.h"

#include "EventOut.h"
#include "Scene.h"
#include "FieldValue.h"
#include "DuneApp.h"

EventOut::EventOut(int type, const MyString &name, int flags, 
                   ExposedField *exposedField, const MyString &x3dName)
{
    m_type = type;
    m_name = name;
    m_x3dName = x3dName;
    m_flags = flags;
    m_exposedField = exposedField;
}

EventOut::EventOut(EventOut *ptr) : Element(ptr)
{
    m_type = ptr->m_type;
    m_name = ptr->m_name;
    m_x3dName = ptr->m_x3dName;
    m_flags = ptr->m_flags;
    m_exposedField = ptr->m_exposedField;
}

EventOut::~EventOut()
{
}

const char *EventOut::getElementName(bool x3d) const
{
    if (x3d) 
        return "outputOnly";
    else
        return "eventOut";
}

int EventOut::write(int f, int indent, int flags) const
{
    if (m_exposedField) return(0);

    RET_ONERROR( writeElementPart(f, indent, flags) )
    
    if (isX3dXml(flags))
        RET_ONERROR( mywritestr(f, "></field>\n") )    
    else
        RET_ONERROR( mywritestr(f, "\n") )    
    TheApp->incSelectionLinenumber();
    return 0;
}

/*
 * NodeExport.cpp
 *
 * Fake node to store Exports in VRML files
 *
 * Copyright (C) 1999 Stephen F. White, 2004 J. "MUFTI" Scheurich
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

#include "NodeExport.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "Scene.h"

ProtoExport::ProtoExport(Scene *scene)
  : Proto(scene, "EXPORT")
{
    localDEF.set(
          addField(SFSTRING, "localDEF", new SFString()));
}

Node *
ProtoExport::create(Scene *scene)
{ 
    return new NodeExport(scene, this); 
}

NodeExport::NodeExport(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

int NodeExport::write(int f, int indent)
{
    TheApp->checkSelectionLinenumberCounting(m_scene, this);
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "EXPORT ") )
    RET_ONERROR( mywritestr(f, localDEF()->getValue()) )
    if (hasName()) {
        RET_ONERROR( mywritestr(f, " AS ") )
        RET_ONERROR( mywritestr(f, (const char *)getName()) )
    }
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeRoutes(f, indent) )
    return(0);
}

int NodeExport::writeXml(int f, int indent, int containerField)
{
    TheApp->checkSelectionLinenumberCounting(m_scene, this);
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "<EXPORT localDEF='") )
    RET_ONERROR( mywritestr(f, localDEF()->getValue()) )
    if (hasName()) {
        RET_ONERROR( mywritestr(f, "' AS='") )
        RET_ONERROR( mywritestr(f, (const char *)getName()) )
    }
    RET_ONERROR( mywritestr(f, "'>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeRoutes(f, indent) )
    return(0);
}


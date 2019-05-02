/*
 * NodeImport.cpp
 *
 * Fake node to store Imports in VRML files
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

#include "NodeImport.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "Scene.h"

ProtoImport::ProtoImport(Scene *scene)
  : Proto(scene, "IMPORT")
{
    importedDEF.set(
          addField(SFSTRING, "importedDEF", new SFString()));
    inlineDEF.set(
          addField(SFSTRING, "inlineDEF", new SFString()));
}

Node *
ProtoImport::create(Scene *scene)
{ 
    return new NodeImport(scene, this); 
}

NodeImport::NodeImport(Scene *scene, Proto *def)
  : DynamicFieldsNode(scene, def)
{
}

int NodeImport::write(int f, int indent)
{
    TheApp->checkSelectionLinenumberCounting(m_scene, this);
    RET_ONERROR( mywritestr(f, "IMPORT ") )
    RET_ONERROR( mywritestr(f, inlineDEF()->getValue()) )
    RET_ONERROR( mywritestr(f, ".") )
    RET_ONERROR( mywritestr(f, importedDEF()->getValue()) )
    if (hasName()) {
        RET_ONERROR( mywritestr(f, " AS ") )
        RET_ONERROR( mywritestr(f, (const char *)getName()) )
    }
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeRoutes(f, indent) )
    return(0);
}

int NodeImport::writeXml(int f, int indent, int containerField)
{
    TheApp->checkSelectionLinenumberCounting(m_scene, this);
    RET_ONERROR( mywritestr(f, "<IMPORT inlineDEF='") )
    RET_ONERROR( mywritestr(f, inlineDEF()->getValue()) )
    RET_ONERROR( mywritestr(f, "' importedDEF='") )
    RET_ONERROR( mywritestr(f, importedDEF()->getValue()) )
    if (hasName()) {
        RET_ONERROR( mywritestr(f, "' AS='") )
        RET_ONERROR( mywritestr(f, (const char *)getName()) )
    }
    RET_ONERROR( mywritestr(f, "'>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeRoutes(f, indent) )
    return(0);
}

void
NodeImport::update()
{
    updateDynamicFields();
}


/*
 * NodeComment.cpp
 *
 * Fake node to store comments in VRML files
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

#include "NodeComment.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "Scene.h"

ProtoComment::ProtoComment(Scene *scene)
  : Proto(scene, "#")
{
    comment.set(
          addField(MFSTRING, "comment", new MFString()));
}

Node *
ProtoComment::create(Scene *scene)
{ 
    return new NodeComment(scene, this); 
}

NodeComment::NodeComment(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

int NodeComment::write(int f, int indent)
{
    MFString *value = comment();
    TheApp->checkSelectionLinenumberCounting(m_scene, this);
    for (int i = 0; i < value->getSize(); i++) {
        RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, "#") )
        RET_ONERROR( mywritestr(f, (const char *) value->getValue(i)) )
        RET_ONERROR( mywritestr(f, "\n") )
        TheApp->incSelectionLinenumber();
    }
    return(0);
}


/*
 * NodeIntegerTrigger.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#include "swt.h"
#include "DuneApp.h"
#include "resource.h"

#include "NodeIntegerTrigger.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFImage.h"
#include "SFBool.h"
#include "Texture.h"

ProtoIntegerTrigger::ProtoIntegerTrigger(Scene *scene)
  : Proto(scene, "IntegerTrigger")
{
    addEventIn(SFBOOL, "set_boolean");
    integerKey.set(
          addExposedField(SFINT32, "integerKey", new SFInt32(-1)));
    addEventOut(SFINT32, "triggerValue");

    addURLs(URL_X3D);
}

Node *
ProtoIntegerTrigger::create(Scene *scene)
{ 
    return new NodeIntegerTrigger(scene, this); 
}

NodeIntegerTrigger::NodeIntegerTrigger(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

NodeIntegerTrigger::NodeIntegerTrigger(const NodeIntegerTrigger &node)
  : Node(node)
{
}

NodeIntegerTrigger::~NodeIntegerTrigger()
{
}

int
NodeIntegerTrigger::getComponentLevel(void) const
{
    return 1;
}

const char* 
NodeIntegerTrigger::getComponentName(void) const
{
    static const char* name = "EventUtilities";
    return name;
}



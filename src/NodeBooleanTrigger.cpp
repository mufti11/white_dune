/*
 * NodeBooleanTrigger.cpp
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

#include "NodeBooleanTrigger.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFImage.h"
#include "SFBool.h"
#include "Texture.h"

ProtoBooleanTrigger::ProtoBooleanTrigger(Scene *scene)
  : Proto(scene, "BooleanTrigger")
{
    set_triggerTime.set(addEventIn(SFTIME, "set_triggerTime"));

    triggerTrue.set(addEventOut(SFBOOL, "triggerTrue"));

    addURLs(URL_X3D);
}

Node *
ProtoBooleanTrigger::create(Scene *scene)
{ 
    return new NodeBooleanTrigger(scene, this); 
}

NodeBooleanTrigger::NodeBooleanTrigger(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

NodeBooleanTrigger::NodeBooleanTrigger(const NodeBooleanTrigger &node)
  : Node(node)
{
}

NodeBooleanTrigger::~NodeBooleanTrigger()
{
}

int
NodeBooleanTrigger::getComponentLevel(void) const
{
    return 1;
}

const char* 
NodeBooleanTrigger::getComponentName(void) const
{
    static const char* name = "EventUtilities";
    return name;
}



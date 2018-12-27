/*
 * NodeBooleanToggle.cpp
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

#include "NodeBooleanToggle.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFImage.h"
#include "SFBool.h"
#include "Texture.h"

ProtoBooleanToggle::ProtoBooleanToggle(Scene *scene)
  : Proto(scene, "BooleanToggle")
{
    set_boolean.set(addEventIn(SFBOOL, "set_boolean"));

    toggle.set(
          addExposedField(SFBOOL, "toggle", new SFBool(false)));

    addURLs(URL_X3D);
}

Node *
ProtoBooleanToggle::create(Scene *scene)
{ 
    return new NodeBooleanToggle(scene, this); 
}

NodeBooleanToggle::NodeBooleanToggle(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

NodeBooleanToggle::NodeBooleanToggle(const NodeBooleanToggle &node)
  : Node(node)
{
}

NodeBooleanToggle::~NodeBooleanToggle()
{
}

int
NodeBooleanToggle::getComponentLevel(void) const
{
    return 1;
}

const char* 
NodeBooleanToggle::getComponentName(void) const
{
    static const char* name = "EventUtilities";
    return name;
}



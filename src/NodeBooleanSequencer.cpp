/*
 * NodeBooleanSequencer.cpp
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

#include "NodeBooleanSequencer.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFImage.h"
#include "SFBool.h"
#include "Texture.h"

ProtoBooleanSequencer::ProtoBooleanSequencer(Scene *scene)
  : Proto(scene, "BooleanSequencer")
{
    addEventIn(SFBOOL, "next");
    addEventIn(SFBOOL, "previous");
    addEventIn(SFFLOAT, "set_fraction", EIF_RECOMMENDED);

    key.set(addExposedField(MFFLOAT, "key", new MFFloat()));

    keyValue.set(addExposedField(MFBOOL, "keyValue", new MFBool()));

    addEventOut(SFBOOL, "value_changed", EOF_RECOMMENDED);

    addURLs(URL_X3D);
}

Node *
ProtoBooleanSequencer::create(Scene *scene)
{ 
    return new NodeBooleanSequencer(scene, this); 
}

NodeBooleanSequencer::NodeBooleanSequencer(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

NodeBooleanSequencer::NodeBooleanSequencer(const NodeBooleanSequencer &node)
  : Node(node)
{
}

NodeBooleanSequencer::~NodeBooleanSequencer()
{
}

int
NodeBooleanSequencer::getComponentLevel(void) const
{
    return 1;
}

const char* 
NodeBooleanSequencer::getComponentName(void) const
{
    static const char* name = "EventUtilities";
    return name;
}



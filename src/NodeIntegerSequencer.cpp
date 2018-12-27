/*
 * NodeIntegerSequencer.cpp
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

#include "NodeIntegerSequencer.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFImage.h"
#include "SFBool.h"
#include "Texture.h"

ProtoIntegerSequencer::ProtoIntegerSequencer(Scene *scene)
  : Proto(scene, "IntegerSequencer")
{
    addEventIn(SFBOOL, "next");
    addEventIn(SFBOOL, "previous");
    addEventIn(SFFLOAT, "set_fraction", EIF_RECOMMENDED);
    key.set(addExposedField(MFFLOAT, "key", new MFFloat()));
    keyValue.set(addExposedField(MFINT32, "keyValue", new MFInt32()));
    addEventOut(SFINT32, "value_changed", EOF_RECOMMENDED);

    addURLs(URL_X3D);
}

Node *
ProtoIntegerSequencer::create(Scene *scene)
{ 
    return new NodeIntegerSequencer(scene, this); 
}

NodeIntegerSequencer::NodeIntegerSequencer(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

NodeIntegerSequencer::NodeIntegerSequencer(const NodeIntegerSequencer &node)
  : Node(node)
{
}

NodeIntegerSequencer::~NodeIntegerSequencer()
{
}

int
NodeIntegerSequencer::getComponentLevel(void) const
{
    return 1;
}

const char* 
NodeIntegerSequencer::getComponentName(void) const
{
    static const char* name = "EventUtilities";
    return name;
}



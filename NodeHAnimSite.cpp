/*
 * NodeHAnimSite.cpp
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

#include "NodeHAnimSite.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFString.h"
#include "SFInt32.h"
#include "MFInt32.h"
#include "SFFloat.h"
#include "MFFloat.h"
#include "SFVec3f.h"

ProtoHAnimSite::ProtoHAnimSite(Scene *scene)
  : ProtoTransform(scene, "HAnimSite")
{
    name.set(
          addExposedField(SFSTRING, "name", new SFString()));
}

Node *
ProtoHAnimSite::create(Scene *scene)
{ 
    return new NodeHAnimSite(scene, this); 
}

NodeHAnimSite::NodeHAnimSite(Scene *scene, Proto *def)
  : NodeTransform(scene, def)
{
}

int
NodeHAnimSite::getComponentLevel(void) const
{
    return 1;
}

const char* 
NodeHAnimSite::getComponentName(void) const
{
    static const char* name = "H-Anim";
    return name;
}


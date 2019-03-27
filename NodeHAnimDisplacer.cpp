/*
 * NodeHAnimDisplacer.cpp
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

#include "NodeHAnimDisplacer.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFImage.h"
#include "SFBool.h"
#include "Texture.h"

ProtoHAnimDisplacer::ProtoHAnimDisplacer(Scene *scene)
  : Proto(scene, "HAnimDisplacer")
{
    coordIndex.set(
          addExposedField(MFINT32, "coordIndex", new MFInt32()));
    displacements.set(
          addExposedField(MFVEC3F, "displacements", new MFVec3f()));
    name.set(
          addExposedField(SFSTRING, "name", new SFString()));
    weight.set(
          addExposedField(SFFLOAT, "weight", new SFFloat(0.0f)));
}
 
Node *
ProtoHAnimDisplacer::create(Scene *scene)
{ 
    return new NodeHAnimDisplacer(scene, this); 
}

NodeHAnimDisplacer::NodeHAnimDisplacer(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

NodeHAnimDisplacer::NodeHAnimDisplacer(const NodeHAnimDisplacer &node)
  : Node(node)
{
}

NodeHAnimDisplacer::~NodeHAnimDisplacer()
{
}

int
NodeHAnimDisplacer::getComponentLevel(void) const
{
    return 1;
}

const char* 
NodeHAnimDisplacer::getComponentName(void) const
{
    static const char* name = "H-Anim";
    return name;
}



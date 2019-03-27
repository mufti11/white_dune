/*
 * NodeHAnimSegment.cpp
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
#ifndef FLT_MAX
# include <float.h>
#endif
#include "stdafx.h"

#include "NodeHAnimSegment.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "DuneApp.h"
#include "NodeNurbsSurface.h"
#include "NodeShape.h"
#include "Scene.h"
#include "NodeNurbsSet.h"

ProtoHAnimSegment::ProtoHAnimSegment(Scene *scene)
  : ProtoGroup(scene, "HAnimSegment")
{
    centerOfMass.set( 
           addExposedField(SFVEC3F, "centerOfMass", new SFVec3f()));
    coord.set(
           addExposedField(SFNODE, "coord", new SFNode(), COORDINATE_NODE));
    displacers.set(
           addExposedField(MFNODE, "displacers", new MFNode(), 
                           X3D_HANIM_DISPLACER));
    mass.set(
          addExposedField(SFFLOAT, "mass", new SFFloat(1.0f), 
                          new SFFloat(0.0f), new SFFloat(FLT_MAX)));
    momentsOfInertia.set( 
           addExposedField(MFFLOAT, "momentsOfInertia", new MFFloat(9)));
    name.set(
          addExposedField(SFSTRING, "name", new SFString()));
}

Node *
ProtoHAnimSegment::create(Scene *scene)
{ 
    return new NodeHAnimSegment(scene, this); 
}

NodeHAnimSegment::NodeHAnimSegment(Scene *scene, Proto *def)
  : NodeGroup(scene, def)
{
}

int
NodeHAnimSegment::getComponentLevel(void) const
{
    return 1;
}

const char* 
NodeHAnimSegment::getComponentName(void) const
{
    static const char* name = "H-Anim";
    return name;
}


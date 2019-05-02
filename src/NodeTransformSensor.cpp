/*
 * NodeTransformSensor.cpp
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#include "NodeTransformSensor.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFBool.h"
#include "SFVec3f.h"
#include "SFNode.h"
#include "DuneApp.h"

ProtoTransformSensor::ProtoTransformSensor(Scene *scene)
  : Proto(scene, "TransformSensor")
{
    center.set(
        addExposedField(SFVEC3F, "center", new SFVec3f(0, 0, 0)));
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    size.set(
        addExposedField(SFVEC3F, "size", new SFVec3f(0, 0, 0)));
    targetObject.set(
        addExposedField(SFNODE, "targetObject", new SFNode()));
}

Node *
ProtoTransformSensor::create(Scene *scene)
{ 
    return new NodeTransformSensor(scene, this); 
}

NodeTransformSensor::NodeTransformSensor(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

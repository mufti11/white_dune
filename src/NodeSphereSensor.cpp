/*
 * NodeSphereSensor.cpp
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

#include "NodeSphereSensor.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFRotation.h"
#include "Field.h"


ProtoSphereSensor::ProtoSphereSensor(Scene *scene)
  : WonderlandExportProto(scene, "SphereSensor")
{
    autoOffset.set(
          addExposedField(SFBOOL, "autoOffset", new SFBool(true)));
    description.set(
          addExposedField(SFSTRING, "description", new SFString("")));
    setFieldFlags(description, FF_X3D_ONLY);
    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    offset.set(
          addExposedField(SFROTATION, "offset",
                          new SFRotation(0.0f, 1.0f, 0.0f, 0.0f)));
    addEventOut(SFBOOL, "isActive");
    addEventOut(SFBOOL, "isOver", FF_X3D_ONLY);
    addEventOut(SFROTATION, "rotation_changed", EOF_RECOMMENDED);
    addEventOut(SFVEC3F, "trackPoint_changed");
}

Node *
ProtoSphereSensor::create(Scene *scene)
{ 
    return new NodeSphereSensor(scene, this); 
}

NodeSphereSensor::NodeSphereSensor(Scene *scene, Proto *def)
  : Node(scene, def)
{
}


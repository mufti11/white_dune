/*
 * NodeVisibilitySensor.cpp
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

#include "NodeVisibilitySensor.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "SFBool.h"
#include "Field.h"


ProtoVisibilitySensor::ProtoVisibilitySensor(Scene *scene)
  : Proto(scene, "VisibilitySensor")
{
    center.set(
          addExposedField(SFVEC3F, "center", new SFVec3f(0.0f, 0.0f, 0.0f)));
    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    size.set(
          addExposedField(SFVEC3F, "size", new SFVec3f(0.0f, 0.0f, 0.0f), 
                          new SFFloat(0.0f)));
    addEventOut(SFTIME, "enterTime", EOF_RECOMMENDED);
    addEventOut(SFTIME, "exitTime");
    addEventOut(SFBOOL, "isActive");
}

Node *
ProtoVisibilitySensor::create(Scene *scene)
{ 
    return new NodeVisibilitySensor(scene, this);
}

NodeVisibilitySensor::NodeVisibilitySensor(Scene *scene, Proto *def)
  : Node(scene, def)
{
}


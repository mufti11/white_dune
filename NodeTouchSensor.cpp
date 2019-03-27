/*
 * NodeTouchSensor.cpp
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

#include "NodeTouchSensor.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "Field.h"


ProtoTouchSensor::ProtoTouchSensor(Scene *scene)
  : WonderlandExportProto(scene, "TouchSensor")
{
    description.set(
          addExposedField(SFSTRING, "description", new SFString("")));
    setFieldFlags(description, FF_X3D_ONLY);
    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    setFieldFlags(enabled, EIF_WONDERLAND_SUPPORTED);

    addEventOut(SFVEC3F, "hitNormal_changed");
    addEventOut(SFVEC3F, "hitPoint_changed", EOF_RECOMMENDED);
    addEventOut(SFVEC2F, "hitTexCoord_changed");
    addEventOut(SFBOOL, "isActive", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFBOOL, "isOver");
    addEventOut(SFTIME, "touchTime", EOF_RECOMMENDED | 
                                     EOF_WONDERLAND_SUPPORTED);
}

Node *
ProtoTouchSensor::create(Scene *scene)
{ 
    return new NodeTouchSensor(scene, this); 
}

NodeTouchSensor::NodeTouchSensor(Scene *scene, Proto *def)
  : Node(scene, def)
{
}


/*
 * NodePlaneSensor.cpp
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

#include "NodePlaneSensor.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFVec2f.h"
#include "SFVec3f.h"
#include "Field.h"


ProtoPlaneSensor::ProtoPlaneSensor(Scene *scene)
  : WonderlandExportProto(scene, "PlaneSensor")
{
    autoOffset.set(
          addExposedField(SFBOOL, "autoOffset", new SFBool(true)));

    axisRotation.set(
          addExposedField(SFROTATION, "axisRotation", 
                          new SFRotation(0, 0, 1, 0)));
    setFieldFlags(axisRotation, FF_X3D_ONLY);

    description.set(
          addExposedField(SFSTRING, "description", new SFString("")));
    setFieldFlags(description, FF_X3D_ONLY);

    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    setFieldFlags(enabled, EIF_WONDERLAND_SUPPORTED);

    maxPosition.set(
          addExposedField(SFVEC2F, "maxPosition", new SFVec2f(-1.0f, -1.0f)));
    setFieldFlags(maxPosition, EIF_WONDERLAND_SUPPORTED);
    
    minPosition.set(
          addExposedField(SFVEC2F, "minPosition", new SFVec2f(0.0f, 0.0f)));
    setFieldFlags(minPosition, EIF_WONDERLAND_SUPPORTED);

    offset.set(
          addExposedField(SFVEC3F, "offset", new SFVec3f(0.0f, 0.0f, 0.0f)));

    addEventOut(SFBOOL, "isOver", FF_X3D_ONLY);
    addEventOut(SFBOOL, "isActive", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFVEC3F, "trackPoint_changed", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFVEC3F, "translation_changed", EOF_RECOMMENDED |
                                                EOF_WONDERLAND_SUPPORTED);
}

Node *
ProtoPlaneSensor::create(Scene *scene)
{ 
   return new NodePlaneSensor(scene, this); 
}

NodePlaneSensor::NodePlaneSensor(Scene *scene, Proto *def)
  : Node(scene, def)
{
}


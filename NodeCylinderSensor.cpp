/*
 * NodeCylinderSensor.cpp
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

#include "NodeCylinderSensor.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "Field.h"
#include "Scene.h"

ProtoCylinderSensor::ProtoCylinderSensor(Scene *scene)
  : WonderlandExportProto(scene, "CylinderSensor")
{
    double unitAngle = scene->getUnitAngle();

    autoOffset.set(
          addExposedField(SFBOOL, "autoOffset", new SFBool(true)));

    axisRotation.set(
          addExposedField(SFROTATION, "axisRotation", 
                          new SFRotation(0, 1, 0, 0)));
    setFieldFlags(axisRotation, FF_X3D_ONLY);

    description.set(
          addExposedField(SFSTRING, "description", new SFString("")));
    setFieldFlags(description, FF_X3D_ONLY);

    diskAngle.set(
          addExposedField(SFFLOAT, "diskAngle", 
                          new SFFloat(M_PI / 12.0f / unitAngle),
                          new SFFloat(0.0f), 
                          new SFFloat(M_PI / 2.0f / unitAngle)));

    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    setFieldFlags(enabled, EIF_WONDERLAND_SUPPORTED);

    maxAngle.set( 
          addExposedField(SFFLOAT, "maxAngle", new SFFloat(-1.0f / unitAngle),
                          new SFFloat(-2.0f * M_PI / unitAngle), 
                          new SFFloat( 2.0f * M_PI / unitAngle)));
    setFieldFlags(maxAngle, EIF_WONDERLAND_SUPPORTED);

    minAngle.set( 
          addExposedField(SFFLOAT, "minAngle", new SFFloat(0.0f),
                          new SFFloat(-2.0f * M_PI / unitAngle),
                          new SFFloat( 2.0f * M_PI / unitAngle)));
    setFieldFlags(minAngle, EIF_WONDERLAND_SUPPORTED);

    offset.set( 
          addExposedField(SFFLOAT, "offset", new SFFloat(0.0f)));

    addEventOut(SFBOOL, "isActive", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFBOOL, "isOver", FF_X3D_ONLY);
    addEventOut(SFROTATION, "rotation_changed", EOF_RECOMMENDED | 
                                                EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFVEC3F, "trackPoint_changed", EOF_WONDERLAND_SUPPORTED);
}

Node *
ProtoCylinderSensor::create(Scene *scene)
{ 
    return new NodeCylinderSensor(scene, this); 
}

NodeCylinderSensor::NodeCylinderSensor(Scene *scene, Proto *def)
  : Node(scene, def)
{
}


/*
 * NodeGeoTouchSensor.cpp
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

#include "NodeGeoTouchSensor.h"
#include "Proto.h"
#include "MFVec3f.h"
#include "ExposedField.h"
#include "Field.h"
#include "RenderState.h"
#include "DuneApp.h"
#include "Util.h"
#include "Vec3f.h"
#include "Scene.h"
#include "NodeIndexedFaceSet.h"
#include "NodeIndexedLineSet.h"
#include "NodePointSet.h"

ProtoGeoTouchSensor::ProtoGeoTouchSensor(Scene *scene)
  : GeoProto(scene, "GeoTouchSensor")
{
    description.set(
          addExposedField(SFSTRING, "description", new SFString("")));
    setFieldFlags(description, FF_X3D_ONLY);
    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    addEventOut(SFVEC3F, "hitNormal_changed");
    addEventOut(SFVEC3F, "hitPoint_changed", EOF_RECOMMENDED);
    addEventOut(SFVEC2F, "hitTexCoord_changed");
    addEventOut(SFSTRING, "hitGeoCoord_changed", FF_VRML_ONLY);
    addEventOut(SFVEC3D, "hitGeoCoord_changed", FF_X3D_ONLY);
    addEventOut(SFBOOL, "isActive");
    addEventOut(SFBOOL, "isOver");
    addEventOut(SFTIME, "touchTime", EOF_RECOMMENDED);
}

Node *
ProtoGeoTouchSensor::create(Scene *scene)
{ 
    return new NodeGeoTouchSensor(scene, this); 
}

NodeGeoTouchSensor::NodeGeoTouchSensor(Scene *scene, Proto *def)
  : GeoNode(scene, def)
{
}

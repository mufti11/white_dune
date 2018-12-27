/*
 * NodeARSensor.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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

#include "DuneApp.h"
#include "NodeARSensor.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFString.h"
#include "SFVec3f.h"
#include "Field.h"
#include "ExposedField.h"

ProtoARSensor::ProtoARSensor(Scene *scene)
  : Proto(scene, "ARSensor")
{
    trackObjects.set(
         addExposedField(SFBOOL, "trackObjects", new SFBool(false)));

    freeze.set(
         addExposedField(SFBOOL, "freeze", new SFBool(false)));

    enabled.set(
         addExposedField(SFBOOL, "enabled", new SFBool(true)));

    currentCamera.set(
         addExposedField(SFBOOL, "currentCamera", new SFBool(true)));

    headingOnly.set(
         addExposedField(SFBOOL, "headingOnly", new SFBool(false)));

    maxPosition.set(
         addExposedField(SFVEC3F, "maxPosition", new SFVec3f(-1, -1, -1)));

    minPosition.set(
         addExposedField(SFVEC3F, "minPosition", new SFVec3f(-1, -1, -1)));

    orientationThreshold.set(
          addExposedField(SFFLOAT, "orientationThreshold", 
                          new SFFloat(15.0f))); 

    positionThreshold.set(
          addExposedField(SFFLOAT, "positionThreshold", 
                          new SFFloat(0.3f))); 

    invisiblePosition.set(
         addExposedField(SFVEC3F, "invisiblePosition", 
                         new SFVec3f(100000, 100000, 100000)));

    cameraPosition.set(
         addExposedField(SFVEC3F, "cameraPosition", new SFVec3f(0, 0, 24)));

    cameraOrientation.set(
         addExposedField(SFVEC3F, "cameraOrientation", new SFVec3f(90, 90, 0)));

    markerName.set(
         addExposedField(SFSTRING, "markerName", new SFString("")));

    addEventOut(SFBOOL, "isVisible");
    addEventOut(SFVEC3F, "translation_changed", EOF_RECOMMENDED);
    addEventOut(SFROTATION, "rotation_changed", EOF_RECOMMENDED);
    addEventOut(SFVEC3F, "scale_changed", EOF_RECOMMENDED);

    addURLs(URL_COVER);
}

Node *
ProtoARSensor::create(Scene *scene)
{ 
    return new NodeARSensor(scene, this); 
}

NodeARSensor::NodeARSensor(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
}

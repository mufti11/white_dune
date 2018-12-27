/*
 * NodeSpaceSensor.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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

#include "NodeSpaceSensor.h"
#include "DuneApp.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFVec2f.h"
#include "SFVec3f.h"
#include "Field.h"


ProtoSpaceSensor::ProtoSpaceSensor(Scene *scene)
  : Proto(scene, "SpaceSensor")
{
    autoOffset.set(
          addExposedField(SFBOOL, "autoOffset", new SFBool(true)));
    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    rotationEnabled.set(
          addExposedField(SFBOOL, "rotationEnabled", new SFBool(false)));
    maxPosition.set(
          addExposedField(SFVEC3F, "maxPosition", 
                          new SFVec3f(-1.0f, -1.0f, -1.0f)));
    minPosition.set(
          addExposedField(SFVEC3F, "minPosition", 
                          new SFVec3f(-1.0f, -1.0f, -1.0f)));
//  similar to the PlaneSensor this should be instead"
//  minPosition.set(
//        addExposedField(SFVEC3F, "minPosition", 
//                        new SFVec3f(0.0f, 0.0f, 0.0f)));
    offset.set(
          addExposedField(SFVEC3F, "offset", new SFVec3f(0.0f, 0.0f, 0.0f)));
    addEventOut(SFBOOL, "isActive");
    addEventOut(SFVEC3F, "trackPoint_changed");
    addEventOut(SFVEC3F, "translation_changed", EOF_RECOMMENDED);
    addEventOut(SFROTATION, "rotation_changed", EOF_RECOMMENDED);

    addURLs(URL_COVER);
}

Node *
ProtoSpaceSensor::create(Scene *scene)
{ 
   return new NodeSpaceSensor(scene, this); 
}

NodeSpaceSensor::NodeSpaceSensor(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
}

/*
 * NodeOrthoViewpoint.cpp
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

#include "NodeOrthoViewpoint.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFString.h"
#include "MFFloat.h"
#include "SFBool.h"
#include "SFRotation.h"
#include "SFVec3f.h"
#include "SFBool.h"
#include "DuneApp.h"

ProtoOrthoViewpoint::ProtoOrthoViewpoint(Scene *scene)
  : Proto(scene, "OrthoViewpoint")
{
    centerOfRotation.set(
        addExposedField(SFVEC3F, "centerOfRotation", new SFVec3f(0, 0, 0)));
    description.set(
        addExposedField(SFSTRING, "description", new SFString("")));
    float values[] = { -1.0f, -1.0f, 1.0f, 1.0f };
    float *v = new float[4];  
    memcpy(v, values, 4 * sizeof(float));
    fieldOfView.set(
        addExposedField(MFFLOAT, "fieldOfView", new MFFloat(v, 4)));
    jump.set(
        addExposedField(SFBOOL, "jump", new SFBool(true)));
    orientation.set(
        addExposedField(SFROTATION, "orientation", new SFRotation(0, 0, 1, 0)));
    position.set(
        addExposedField(SFVEC3F, "position", new SFVec3f(0, 0, 10)));
    retainUserOffsets.set(
        addExposedField(SFBOOL, "retainUserOffsets", new SFBool(false)));

    bind.set(
          addExposedField(SFBOOL, "bind", new SFBool(false)));
    setFieldFlags(bind, FF_X3DOM_ONLY);

    isActive.set(
          addExposedField(SFBOOL, "isActive", new SFBool(false)));
    setFieldFlags(isActive, FF_X3DOM_ONLY);

    zFar.set(
          addExposedField(SFFLOAT, "zFar", new SFFloat(10000)));
    setFieldFlags(zFar, FF_X3DOM_ONLY);

    zNear.set(
          addExposedField(SFFLOAT, "zNear", new SFFloat(1)));
    setFieldFlags(zNear, FF_X3DOM_ONLY);
}

Node *
ProtoOrthoViewpoint::create(Scene *scene)
{ 
    return new NodeOrthoViewpoint(scene, this); 
}

NodeOrthoViewpoint::NodeOrthoViewpoint(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

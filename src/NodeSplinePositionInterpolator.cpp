/*
 * NodeSplinePositionInterpolator.cpp
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

#include "NodeSplinePositionInterpolator.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFBool.h"
#include "MFFloat.h"
#include "MFVec3f.h"
#include "MFVec3f.h"
#include "SFBool.h"
#include "DuneApp.h"

ProtoSplinePositionInterpolator::ProtoSplinePositionInterpolator(Scene *scene)
  : Proto(scene, "SplinePositionInterpolator")
{
    addEventIn(SFFLOAT, "set_fraction");
    closed.set(
        addExposedField(SFBOOL, "closed", new SFBool(false)));
    key.set(
        addExposedField(MFFLOAT, "key", new MFFloat()));
    keyValue.set(
        addExposedField(MFVEC3F, "keyValue", new MFVec3f()));
    keyVelocity.set(
        addExposedField(MFVEC3F, "keyVelocity", new MFVec3f()));
    normalizeVelocity.set(
        addExposedField(SFBOOL, "normalizeVelocity", new SFBool(false)));
}

Node *
ProtoSplinePositionInterpolator::create(Scene *scene)
{ 
    return new NodeSplinePositionInterpolator(scene, this); 
}

NodeSplinePositionInterpolator::NodeSplinePositionInterpolator(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

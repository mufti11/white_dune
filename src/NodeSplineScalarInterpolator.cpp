/*
 * NodeSplineScalarInterpolator.cpp
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

#include "NodeSplineScalarInterpolator.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFBool.h"
#include "MFFloat.h"
#include "MFFloat.h"
#include "MFFloat.h"
#include "SFBool.h"
#include "DuneApp.h"

ProtoSplineScalarInterpolator::ProtoSplineScalarInterpolator(Scene *scene)
  : Proto(scene, "SplineScalarInterpolator")
{
    addEventIn(SFFLOAT, "set_fraction");
    closed.set(
        addExposedField(SFBOOL, "closed", new SFBool(false)));
    key.set(
        addExposedField(MFFLOAT, "key", new MFFloat()));
    keyValue.set(
        addExposedField(MFFLOAT, "keyValue", new MFFloat()));
    keyVelocity.set(
        addExposedField(MFFLOAT, "keyVelocity", new MFFloat()));
    normalizeVelocity.set(
        addExposedField(SFBOOL, "normalizeVelocity", new SFBool(false)));
}

Node *
ProtoSplineScalarInterpolator::create(Scene *scene)
{ 
    return new NodeSplineScalarInterpolator(scene, this); 
}

NodeSplineScalarInterpolator::NodeSplineScalarInterpolator(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

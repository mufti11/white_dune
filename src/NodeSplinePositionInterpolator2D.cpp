/*
 * NodeSplinePositionInterpolator2D.cpp
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

#include "NodeSplinePositionInterpolator2D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFBool.h"
#include "MFFloat.h"
#include "MFVec2f.h"
#include "MFVec2f.h"
#include "SFBool.h"
#include "DuneApp.h"

ProtoSplinePositionInterpolator2D::ProtoSplinePositionInterpolator2D(Scene *scene)
  : Proto(scene, "SplinePositionInterpolator2D")
{
    addEventIn(SFFLOAT, "set_fraction");
    closed.set(
        addExposedField(SFBOOL, "closed", new SFBool(false)));
    key.set(
        addExposedField(MFFLOAT, "key", new MFFloat()));
    keyValue.set(
        addExposedField(MFVEC2F, "keyValue", new MFVec2f()));
    keyVelocity.set(
        addExposedField(MFVEC2F, "keyVelocity", new MFVec2f()));
    normalizeVelocity.set(
        addExposedField(SFBOOL, "normalizeVelocity", new SFBool(false)));
}

Node *
ProtoSplinePositionInterpolator2D::create(Scene *scene)
{ 
    return new NodeSplinePositionInterpolator2D(scene, this); 
}

NodeSplinePositionInterpolator2D::NodeSplinePositionInterpolator2D(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

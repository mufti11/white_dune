/*
 * NodeCubicBezierPositionInterpolator.cpp
 *
 * Copyright (C) 2011 J. "MUFTI" Scheurich
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

#include "NodeCubicBezierPositionInterpolator.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "DuneApp.h"

ProtoCubicBezierPositionInterpolator::ProtoCubicBezierPositionInterpolator(Scene *scene)
  : Proto(scene, "CubicBezierPositionInterpolator")
{
    addEventIn(SFFLOAT, "set_fraction");
    key.set(
        addExposedField(MFFLOAT, "key", new MFFloat()));
    keyValue.set(
        addExposedField(MFVEC3F, "keyValue", new MFVec3f()));
    controlPoints.set(
        addExposedField(MFVEC4F, "controlPoints", new MFVec4f()));
    addEventOut(SFVEC3F, "value_changed", EOF_RECOMMENDED);
}

Node *
ProtoCubicBezierPositionInterpolator::create(Scene *scene)
{ 
    return new NodeCubicBezierPositionInterpolator(scene, this); 
}

NodeCubicBezierPositionInterpolator::NodeCubicBezierPositionInterpolator(Scene *scene, Proto *def)
  : Node(scene, def)
{
}
    

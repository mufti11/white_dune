/*
 * NodeNurbsOrientationInterpolator.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#include "NodeNurbsOrientationInterpolator.h"
#include "Proto.h"
#include "DuneApp.h"
#include "Scene.h"
#include "FieldValue.h"
#include "SFDouble.h"
#include "MFDouble.h"
#include "SFInt32.h"
#include "SFBool.h"
#include "Vec2f.h"
#include "NodeCoordinate.h"

ProtoNurbsOrientationInterpolator::ProtoNurbsOrientationInterpolator(Scene *scene)
  : Proto(scene, "NurbsOrientationInterpolator")
{
    addEventIn(SFFLOAT, "set_fraction", EIF_RECOMMENDED);
    controlPoints.set(
          addExposedField(SFNODE, "controlPoints", new SFNode(), 
                          COORDINATE_NODE));
    knot.set(
          addField(MFDOUBLE, "knot", new MFDouble()));
    order.set(
          addField(SFINT32, "order", new SFInt32(3), new SFInt32(2)));
    weight.set(
          addExposedField(MFDOUBLE, "weight", new MFDouble(), 
                          new SFDouble(0.0f)));
    addEventOut(SFROTATION, "value_changed", EOF_RECOMMENDED);
}

Node *
ProtoNurbsOrientationInterpolator::create(Scene *scene)
{
    return new NodeNurbsOrientationInterpolator(scene, this); 
}

NodeNurbsOrientationInterpolator::NodeNurbsOrientationInterpolator(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

NodeNurbsOrientationInterpolator::~NodeNurbsOrientationInterpolator()
{
}

void
NodeNurbsOrientationInterpolator::flip(int index)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)controlPoints()->getValue();
    if (ncoord)
        if (ncoord->getType() == VRML_COORDINATE) 
            ncoord->flip(index);
}

void
NodeNurbsOrientationInterpolator::swap(int fromTo)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)controlPoints()->getValue();
    if (ncoord)
        if (ncoord->getType() == VRML_COORDINATE) 
            ncoord->swap(fromTo);
}


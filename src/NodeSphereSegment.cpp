/*
 * NodeSphereSegment.cpp
 *
 * Copyright (C) 1999 Stephen F. White
 *               2003 Th. Rothermel
 *               2004 Wu Qingwei
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

#include "NodeSphereSegment.h"
#include "Proto.h"

ProtoSphereSegment::ProtoSphereSegment(Scene *scene)
  : Proto(scene, "SphereSegment")
{
    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    latitude.set(
          addExposedField(MFFLOAT, "latitude", new MFFloat()));
    setFieldFlags(latitude, FF_X3DOM_ONLY);

    longitude.set(
          addExposedField(MFFLOAT, "longitude", new MFFloat()));
    setFieldFlags(longitude, FF_X3DOM_ONLY);

    radius.set(
          addExposedField(SFFLOAT, "radius", new SFFloat(1.0F), new SFFloat(0.0f)));
    setFieldFlags(radius, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);

    stepSize.set(
          addExposedField(SFVEC2F, "stepSize", new SFVec2f(1, 1)));
    setFieldFlags(stepSize, FF_X3DOM_ONLY);
}

Node *
ProtoSphereSegment::create(Scene *scene)
{ 
    return new NodeSphereSegment(scene, this); 
}

NodeSphereSegment::NodeSphereSegment(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

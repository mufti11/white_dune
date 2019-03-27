/*
 * NodeTorus.cpp
 *
 * Copyright (C) 1999 Stephen F. White
 *               2018 J. "MUFTI" Scheurich
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

#include "NodeTorus.h"

ProtoTorus::ProtoTorus(Scene *scene)
  : Proto(scene, "Torus")
{
    x3domGeometryCommonFields()

    angle.set(
          addExposedField(SFFLOAT, "angle", new SFFloat(M_PI), 
                   new SFFloat(0.0f), new SFFloat(M_PI * 2.0f)));
    setFieldFlags(angle, FF_X3DOM_ONLY);

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    caps.set(
          addExposedField(SFBOOL, "caps", new SFBool(true)));
    setFieldFlags(caps, FF_X3DOM_ONLY);

    height.set(
          addExposedField(SFFLOAT, "height", new SFFloat(1.0f), new SFFloat(0.0f)));
    setFieldFlags(height, FF_X3DOM_ONLY);

    innerRadius.set(
          addExposedField(SFFLOAT, "innerRadius", new SFFloat(5.0f), 
                   new SFFloat(0.0f)));
    setFieldFlags(innerRadius, FF_X3DOM_ONLY);

    insideOutsideRadius.set(
          addExposedField(SFBOOL, "insideOutsideRadius", new SFBool(false)));
    setFieldFlags(insideOutsideRadius, FF_X3DOM_ONLY);

    outerRadius.set(
          addExposedField(SFFLOAT, "outerRadius", new SFFloat(0.0f), 
                   new SFFloat(0.0f)));
    setFieldFlags(outerRadius, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);

    subdivision.set(
          addExposedField(SFVEC2F, "subdivision", new SFVec2f(24.0f, 24.0f)));
    setFieldFlags(subdivision, FF_X3DOM_ONLY);
}

Node *ProtoTorus::create(Scene *scene)
{ 
    return new NodeTorus(scene, this); 
}

NodeTorus::NodeTorus(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

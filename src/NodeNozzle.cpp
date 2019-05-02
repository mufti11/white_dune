/*
 * NodeNozzle.cpp
 *
 * Copyright (C) 1999 Stephen F. White
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

#include "NodeNozzle.h"
#include "Scene.h"
#include "Proto.h"

ProtoNozzle::ProtoNozzle(Scene *scene)
  : Proto(scene, "Nozzle")
{
    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);
 
    height.set(
          addExposedField(SFFLOAT, "height", new SFFloat(0), new SFFloat(0)));
    setFieldFlags(height, FF_X3DOM_ONLY);

    innerRadius.set(
          addExposedField(SFFLOAT, "innerRadius", new SFFloat(4), new SFFloat(0)));
    setFieldFlags(innerRadius, FF_X3DOM_ONLY);

    nozzleHeight.set(
          addExposedField(SFFLOAT, "nozzleHeight", new SFFloat(1), new SFFloat(0)));
    setFieldFlags(nozzleHeight, FF_X3DOM_ONLY);

    nozzleRadius.set(
          addExposedField(SFFLOAT, "nozzleRadius", new SFFloat(6), new SFFloat(0)));
    setFieldFlags(nozzleRadius, FF_X3DOM_ONLY);

    outerRadius.set(
          addExposedField(SFFLOAT, "outerRadius", new SFFloat(5), new SFFloat(0)));
    setFieldFlags(outerRadius, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);
    
    subdivision.set(
          addExposedField(SFFLOAT, "subdivision", new SFFloat(32), new SFFloat(2)));
    setFieldFlags(subdivision, FF_X3DOM_ONLY);

    x3domGeometryCommonFields()
}

Node *
ProtoNozzle::create(Scene *scene)
{ 
    return new NodeNozzle(scene, this); 
}

NodeNozzle::NodeNozzle(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

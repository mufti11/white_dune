/*
 * NodeSlopedCylinder.cpp
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

#include "NodeSlopedCylinder.h"
#include "Proto.h"

ProtoSlopedCylinder::ProtoSlopedCylinder(Scene *scene)
  : Proto(scene, "SlopedCylinder")
{
    bottom.set(
           addExposedField(SFBOOL, "bottom", new SFBool(true)));
    setFieldFlags(bottom, FF_X3DOM_ONLY);

    height.set(
           addExposedField(SFFLOAT, "height", new SFFloat(2.0f), new SFFloat(0.0f)));
    setFieldFlags(height, FF_X3DOM_ONLY);

    radius.set(
           addExposedField(SFFLOAT, "radius", new SFFloat(1.0f), new SFFloat(0.0f)));
    setFieldFlags(radius, FF_X3DOM_ONLY);

    side.set(
           addExposedField(SFBOOL, "side", new SFBool(true)));
    setFieldFlags(side, FF_X3DOM_ONLY);

    top.set(
           addExposedField(SFBOOL, "top", new SFBool(true)));
    setFieldFlags(top, FF_X3DOM_ONLY);

    solid.set(
           addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);

    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    subdivision.set(
          addExposedField(SFFLOAT, "subdivision", new SFFloat(32)));
    setFieldFlags(subdivision, FF_X3DOM_ONLY);

    xbshear.set(
           addExposedField(SFFLOAT, "xbshear", new SFFloat(26179.0f)));
    setFieldFlags(xbshear, FF_X3DOM_ONLY);

    xtshear.set(
           addExposedField(SFFLOAT, "xtshear", new SFFloat(26179.0f)));
    setFieldFlags(xtshear, FF_X3DOM_ONLY);

    ybshear.set(
           addExposedField(SFFLOAT, "ybshear", new SFFloat(0.0f)));
    setFieldFlags(ybshear, FF_X3DOM_ONLY);
 
    ytshear.set(
           addExposedField(SFFLOAT, "ytshear", new SFFloat(0.0f)));
    setFieldFlags(ytshear, FF_X3DOM_ONLY); 
}

Node *
ProtoSlopedCylinder::create(Scene *scene)
{ 
    return new NodeSlopedCylinder(scene, this); 
}

NodeSlopedCylinder::NodeSlopedCylinder(Scene *scene, Proto *def)
  : Node(scene, def)
{
}


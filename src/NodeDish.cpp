/*
 * NodeDish.cpp
 *
 * Copyright (C) 1999 Stephen F. White
 *               2003 Th. Rothermel
 *               2004 Wu Qingwei
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

#include "NodeDish.h"
#include "Proto.h"
#include "FieldValue.h"
#include "Node.h"
#include "Scene.h"

ProtoDish::ProtoDish(Scene *scene)
  : Proto(scene, "Dish")
{
    bottom.set(
           addExposedField(SFBOOL, "bottom", new SFBool(true)));
    setFieldFlags(bottom, FF_X3DOM_ONLY);

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    diameter.set(
          addExposedField(SFFLOAT, "diameter", new SFFloat(2.0f), new SFFloat(0.0f)));
    setFieldFlags(diameter, FF_X3DOM_ONLY);

    height.set(
           addExposedField(SFFLOAT, "height", new SFFloat(2.0f), new SFFloat(0.0f)));
    setFieldFlags(height, FF_X3DOM_ONLY);

    radius.set(
           addExposedField(SFFLOAT, "radius", new SFFloat(1.0f), new SFFloat(0.0f)));
    setFieldFlags(radius, FF_X3DOM_ONLY);

    side.set(
           addExposedField(SFBOOL, "side", new SFBool(true)));
    setFieldFlags(side, FF_X3DOM_ONLY);

    solid.set(
           addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);

    subdivision.set(
          addExposedField(SFVEC2F, "subdivision", new SFVec2f(24, 24)));
    setFieldFlags(subdivision, FF_X3DOM_ONLY);

    x3domGeometryCommonFields()
}

Node *
ProtoDish::create(Scene *scene)
{ 
    return new NodeDish(scene, this); 
}

NodeDish::NodeDish(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

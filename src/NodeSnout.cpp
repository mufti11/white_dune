/*
 * NodeSnout.cpp
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

#include "NodeSnout.h"
#include "Proto.h"

ProtoSnout::ProtoSnout(Scene *scene)
  : Proto(scene, "Snout")
{
    x3domGeometryCommonFields()

    bottom.set(
          addExposedField(SFBOOL, "bottom", new SFBool(true)));
    setFieldFlags(bottom, FF_X3DOM_ONLY);

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    dbottom.set(
          addExposedField(SFFLOAT, "dbottom", new SFFloat(0.0f), new SFFloat(0.0f)));
    setFieldFlags(dbottom, FF_X3DOM_ONLY);
                   
    dtop.set(
          addExposedField(SFFLOAT, "dtop", new SFFloat(0.0f), new SFFloat(0.0f)));
    setFieldFlags(dtop, FF_X3DOM_ONLY);
                   
    height.set(
          addExposedField(SFFLOAT, "height", new SFFloat(0.0f), new SFFloat(0.0f)));
    setFieldFlags(height, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3D_ONLY);

    subdivision.set(
          addExposedField(SFFLOAT, "subdivision", new SFFloat(32)));
    setFieldFlags(subdivision, FF_X3DOM_ONLY);

    top.set(
          addExposedField(SFBOOL, "top", new SFBool(true)));
    setFieldFlags(top, FF_X3DOM_ONLY);

    xoff.set(
          addExposedField(SFFLOAT, "xoff", new SFFloat(25), new SFFloat(0)));
    setFieldFlags(xoff, FF_X3DOM_ONLY);

    yoff.set(
          addExposedField(SFFLOAT, "yoff", new SFFloat(25), new SFFloat(0)));
    setFieldFlags(yoff, FF_X3DOM_ONLY);
}

Node *ProtoSnout::create(Scene *scene)
{ 
    return new NodeSnout(scene, this); 
}

NodeSnout::NodeSnout(Scene *scene, Proto *def)
  : Node(scene, def)
{
}


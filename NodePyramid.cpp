/*
 * NodePyramid.cpp
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

#include "NodePyramid.h"

ProtoPyramid::ProtoPyramid(Scene *scene)
  : Proto(scene, "Pyramid")
{
    x3domGeometryCommonFields()

    ccw.set(
          addField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3D_ONLY);

    height.set(
          addField(SFFLOAT, "height", new SFFloat(1.0f), new SFFloat(0.0f)));
    setFieldFlags(height, FF_X3D_ONLY);

    solid.set(
          addField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3D_ONLY);

    xbottom.set(
          addField(SFFLOAT, "xbottom", new SFFloat(1.0f), new SFFloat(0.0f)));
    setFieldFlags(xbottom, FF_X3D_ONLY);

    xoff.set(
          addField(SFFLOAT, "xoff", new SFFloat(25.0f), new SFFloat(0.0f)));
    setFieldFlags(xoff, FF_X3D_ONLY);

    xtop.set(
          addField(SFFLOAT, "xtop", new SFFloat(5.0f), new SFFloat(0.0f)));
    setFieldFlags(xtop, FF_X3D_ONLY);

    ybottom.set(
          addField(SFFLOAT, "ybottom", new SFFloat(1.0f), new SFFloat(0.0f)));
    setFieldFlags(ybottom, FF_X3D_ONLY);

    yoff.set(
          addField(SFFLOAT, "yoff", new SFFloat(25.0f), new SFFloat(0.0f)));
    setFieldFlags(yoff, FF_X3D_ONLY);

    ytop.set(
          addField(SFFLOAT, "ytop", new SFFloat(5.0f), new SFFloat(0.0f)));
    setFieldFlags(ytop, FF_X3D_ONLY);
}

Node *ProtoPyramid::create(Scene *scene)
{ 
    return new NodePyramid(scene, this); 
}

NodePyramid::NodePyramid(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

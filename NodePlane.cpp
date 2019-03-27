/*
 * NodePlane.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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

#include "NodePlane.h"
#include "Scene.h"

ProtoPlane::ProtoPlane(Scene *scene)
  : Proto(scene, "Plane")
{
    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    center.set(
          addExposedField(SFVEC3F, "center", new SFVec3f(0, 0, 0)));
    setFieldFlags(center, FF_X3DOM_ONLY);

    primType.set(
          addExposedField(SFSTRING, "primType", new SFString("triangle")));
    setFieldFlags(primType, FF_X3DOM_ONLY);


    size.set(
          addExposedField(SFVEC2F, "size", new SFVec2f(2, 2)));
    setFieldFlags(size, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);
}

Node *
ProtoPlane::create(Scene *scene)
{ 
    return new NodePlane(scene, this); 
}

NodePlane::NodePlane(Scene *scene, Proto *def)
  : GeometryNode(scene, def)
{
}

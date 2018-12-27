/*
 * NodeParticleSet.cpp
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

#include "NodeParticleSet.h"
#include "Scene.h"

ProtoParticleSet::ProtoParticleSet(Scene *scene)
  : Proto(scene, "ParticleSet")
{
    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    color.set(
          addExposedField(SFNODE, "color", new SFNode(NULL), COLOR_NODE));
    setFieldFlags(color, FF_X3DOM_ONLY);

    coord.set(
          addExposedField(SFNODE, "coord", new SFNode(NULL), COORDINATE_NODE));
    setFieldFlags(coord, FF_X3DOM_ONLY);

    drawOrder.set(
          addExposedField(SFSTRING, "drawOrder", new SFString("Any")));
    setFieldFlags(drawOrder, FF_X3DOM_ONLY);

    index.set(
          addExposedField(MFINT32, "index", new MFInt32()));
    setFieldFlags(index, FF_X3DOM_ONLY);

    mode.set(
          addExposedField(SFSTRING, "mode", new SFString("ViewDirQuads")));
    setFieldFlags(mode, FF_X3DOM_ONLY);

    normal.set(
          addExposedField(SFNODE, "normal", new SFNode(NULL)));
    setFieldFlags(normal, FF_X3DOM_ONLY);

    size.set(
          addExposedField(MFVEC3F, "size", new MFVec3f()));
    setFieldFlags(size, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);

    textureZ.set(
          addExposedField(MFFLOAT, "textureZ", new MFFloat()));
    setFieldFlags(textureZ, FF_X3DOM_ONLY);
}

Node *
ProtoParticleSet::create(Scene *scene)
{ 
    return new NodeParticleSet(scene, this); 
}

NodeParticleSet::NodeParticleSet(Scene *scene, Proto *def)
  : GeometryNode(scene, def)
{
}


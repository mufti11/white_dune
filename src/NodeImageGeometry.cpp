/*
 * NodeImageGeometry.cpp
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

#include "NodeImageGeometry.h"
#include "Scene.h"

ProtoImageGeometry::ProtoImageGeometry(Scene *scene)
  : Proto(scene, "ImageGeometry")
{
    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    color.set(
          addExposedField(SFNODE, "color", new SFNode(NULL), TEXTURE_NODE));
    setFieldFlags(color, FF_X3DOM_ONLY);

    coord.set(
          addExposedField(MFNODE, "coord", new MFNode(), TEXTURE_NODE));
    setFieldFlags(coord, FF_X3DOM_ONLY);

    implicitMeshSize.set(
          addExposedField(SFVEC2F, "implicitMeshSize", new SFVec2f(256, 256)));
    setFieldFlags(implicitMeshSize, FF_X3DOM_ONLY);

    index.set(
          addExposedField(SFNODE, "index", new SFNode(NULL), TEXTURE_NODE));
    setFieldFlags(index, FF_X3DOM_ONLY);

    normal.set(
          addExposedField(SFSTRING, "normal", new SFNode(NULL), TEXTURE_NODE));
    setFieldFlags(normal, FF_X3DOM_ONLY);

    numColorComponents.set(
          addExposedField(SFINT32, "numColorComponents", new SFInt32(2), 
                   new SFInt32(3)));
    setFieldFlags(numColorComponents, FF_X3DOM_ONLY);

    numTexCoordComponents.set(
          addExposedField(SFINT32, "numTexCoordComponents", new SFInt32(2), 
                   new SFInt32(2)));
    setFieldFlags(numTexCoordComponents, FF_X3DOM_ONLY);

    position.set(
          addExposedField(SFVEC3F, "position", new SFVec3f(0, 0, 0)));
    setFieldFlags(position, FF_X3DOM_ONLY);

    primType.set(
          addExposedField(MFSTRING, "primType", new MFString("TRIANGLES")));
    setFieldFlags(primType, FF_X3DOM_ONLY);

    size.set(
          addExposedField(SFVEC3F, "size", new SFVec3f(1, 1, 1)));
    setFieldFlags(size, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);

    texCoord.set(
          addExposedField(SFSTRING, "texCoord", new SFNode(NULL), TEXTURE_NODE));
    setFieldFlags(texCoord, FF_X3DOM_ONLY);

    vertexCount.set(
          addExposedField(MFINT32, "vertexCount", new MFInt32(0)));
    setFieldFlags(vertexCount, FF_X3DOM_ONLY);
}

Node *
ProtoImageGeometry::create(Scene *scene)
{ 
    return new NodeImageGeometry(scene, this); 
}

NodeImageGeometry::NodeImageGeometry(Scene *scene, Proto *def)
  : GeometryNode(scene, def)
{
}

/*
 * NodeRectangle2D.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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

#include "NodeRectangle2D.h"
#include "MyMesh.h"
#include "Proto.h"
#include "Scene.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "RenderState.h"
#include "Util.h"
#include "stdafx.h"

ProtoRectangle2D::ProtoRectangle2D(Scene *scene)
  : Proto(scene, "Rectangle2D")
{
    size.set(
          addField(SFVEC2F, "size", new SFVec2f(2.0f, 2.0f)));
    solid.set(
          addField(SFBOOL, "solid", new SFBool(false)));

    addURLs(URL_X3D);

    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);
}

Node *
ProtoRectangle2D::create(Scene *scene)
{ 
    return new NodeRectangle2D(scene, this); 
}

NodeRectangle2D::NodeRectangle2D(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
}

void
NodeRectangle2D::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    int numPoints = 4;
    float *fpoint = new float[numPoints * 3];

    const float *fsize = size()->getValue();

    fpoint[0 * 3 + 0] = -fsize[0] / 2.0f;
    fpoint[0 * 3 + 1] = -fsize[1] / 2.0f;
    fpoint[0 * 3 + 2] = 0;

    fpoint[1 * 3 + 0] = -fsize[0] / 2.0f;
    fpoint[1 * 3 + 1] =  fsize[1] / 2.0f;
    fpoint[1 * 3 + 2] = 0;

    fpoint[2 * 3 + 0] =  fsize[0] / 2.0f;
    fpoint[2 * 3 + 1] =  fsize[1] / 2.0f;
    fpoint[2 * 3 + 2] = 0;

    fpoint[3 * 3 + 0] =  fsize[0] / 2.0f;
    fpoint[3 * 3 + 1] = -fsize[1] / 2.0f;
    fpoint[3 * 3 + 2] = 0;

    
    Array<int> icoordIndex;
    icoordIndex.append(0);
    icoordIndex.append(1);
    icoordIndex.append(2);
    icoordIndex.append(3);
    icoordIndex.append(-1);

    MFVec3f *coords = new MFVec3f(fpoint, numPoints * 3);

    MFInt32 *coordIndex = (MFInt32 *)(new MFInt32((int *)icoordIndex.getData(),
                                                  icoordIndex.size()))->copy();

    int meshFlags = MESH_NORMAL_PER_VERTEX;
    if (solid()->getValue())
        meshFlags |= MESH_SOLID;
    meshFlags |= MESH_CCW;

    float transparency = 0;
    if (hasParent())
        transparency = getParent()->getTransparency();
    Array<MFVec2f *>texCoords;

    m_mesh = new MyMesh(this, coords, coordIndex, NULL, NULL, NULL, NULL,
                        texCoords, NULL, 0, meshFlags, transparency);
}



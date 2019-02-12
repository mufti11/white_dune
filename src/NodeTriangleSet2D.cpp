/*
 * NodeTriangleSet2D.cpp
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
#include <string.h>
#include "stdafx.h"

#include "NodeTriangleSet2D.h"
#include "Proto.h"
#include "Scene.h"
#include "FieldValue.h"
#include "Node.h"
#include "MyMesh.h"
#include "FaceData.h"
#include "Vec3f.h"
#include "NodeIndexedLineSet.h"
#include "Util.h"

ProtoTriangleSet2D::ProtoTriangleSet2D(Scene *scene)
  : GeometryProto(scene, "TriangleSet2D")
{
    vertices.set(addExposedField(MFVEC2F, "vertices", new MFVec2f()));
    solid.set(
          addField(SFBOOL, "solid", new SFBool(false)));

    addURLs(URL_X3D);

    x3domGeometryCommonFields()
}

Node *
ProtoTriangleSet2D::create(Scene *scene)
{ 
      return new NodeTriangleSet2D(scene, this);
}

NodeTriangleSet2D::NodeTriangleSet2D(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
    m_coordIndex = NULL;
}

NodeTriangleSet2D::~NodeTriangleSet2D()
{
    delete m_coordIndex;
}

void
NodeTriangleSet2D::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
    m_mesh = NULL;
    Node::setField(index, value, cf);
    update();
}

void
NodeTriangleSet2D::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    MFVec3f *coords = new MFVec3f(vertices());
    MFVec3f *normals = NULL;
    MFFloat *colors = NULL;
    
    if (coords->getSFSize() > 0) {
        int numFaces = coords->getSFSize() / 3;
        int *indices = new int[numFaces * 4 + 1];
        for (int i = 0; i < numFaces; i++) {
            indices[i * 4    ] = i * 3; 
            indices[i * 4 + 1] = i * 3 + 1; 
            indices[i * 4 + 2] = i * 3 + 2;
            indices[i * 4 + 3] = -1;            
        }
        delete m_coordIndex;
        m_coordIndex = new MFInt32(indices, numFaces * 4);    
    }

    int meshFlags = 0;
    float transparency = 0;
    if (hasParent())
        transparency = getParent()->getTransparency();
    if (solid()->getValue())
        meshFlags |= MESH_SOLID;
    MyArray<MFVec2f *>texCoords;
    m_mesh = new MyMesh(this, coords, m_coordIndex, normals, NULL, colors, 
                        NULL, texCoords, NULL, 0, meshFlags, transparency);
}

int NodeTriangleSet2D::getProfile(void) const
{ 
    return PROFILE_IMMERSIVE;
}


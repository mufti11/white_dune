/*
 * NodeIndexedTriangleSet.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#include "NodeIndexedTriangleSet.h"
#include "Proto.h"
#include "Scene.h"
#include "FieldValue.h"
#include "Node.h"
#include "MyMesh.h"
#include "FaceData.h"
#include "Vec3f.h"
#include "NodeColor.h"
#include "NodeColorRGBA.h"
#include "NodeCoordinate.h"
#include "NodeGeoCoordinate.h"
#include "NodeNormal.h"
#include "NodeTextureCoordinate.h"
#include "NodeIndexedLineSet.h"
#include "NodeFogCoordinate.h"
#include "Util.h"

ProtoIndexedTriangleSet::ProtoIndexedTriangleSet(Scene *scene)
  : IndexedTriangleSetProto(scene, "IndexedTriangleSet")
{
    ComposedGeometryElements()
    x3domGeometryCommonFields()
    normalUpdateMode.set(
      addExposedField(SFSTRING, "normalUpdateMode", new SFString("fast")));
    setFieldFlags(normalUpdateMode, FF_X3DOM_ONLY);
}

Node *
ProtoIndexedTriangleSet::create(Scene *scene)
{ 
      Node *node = new NodeIndexedTriangleSet(scene, this);
      return node;
}

NodeIndexedTriangleSet::NodeIndexedTriangleSet(Scene *scene, Proto *def)
  : IndexedTriangleSetNode(scene, def)
{
    m_colorPerVertexWarning = false;
}

NodeIndexedTriangleSet::~NodeIndexedTriangleSet()
{
}

void
NodeIndexedTriangleSet::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    Node *coord = ((SFNode *) getField(coord_Field()))->getValue();
    bool bcolorPerVertex = colorPerVertex()->getValue();
   
    if (!coord)
        return;

    MFVec3f *coords = NULL;
    MFVec3d *coordsDouble = NULL;
    if (coord->getType() == VRML_COORDINATE)
        coords = ((NodeCoordinate *)coord)->point();
    else
        coordsDouble = ((NodeGeoCoordinate *)coord)->pointX3D();

    MyArray<MFVec2f *> texCoords;
    Util::getTexCoords(texCoords, texCoord()->getValue());    

    MFVec3f *normals = NULL;
    if (normal()->getValue())
        if (normal()->getValue()->getType() == VRML_NORMAL)
            normals = ((NodeNormal *)(normal()->getValue()))->vector();
    
    int meshFlags = 0;
    MFFloat *colors = NULL;
    if (color()->getValue()) {
        if (color()->getValue()->getType() == VRML_COLOR) 
            colors = ((NodeColor *)(color()->getValue()))->color();
        else if (color()->getValue()->getType() == X3D_COLOR_RGBA) {
            colors = ((NodeColorRGBA *)(color()->getValue()))->color();
            meshFlags |= MESH_COLOR_RGBA;
        }
    }    

    MFInt32 *indices = index();

    if ((coords && (coords->getSFSize() > 0)) ||
        (coordsDouble && (coordsDouble->getSFSize() > 0))) {
        int numFaces = indices->getSFSize() / 3;
        int *meshIndices = (numFaces == 0) ? NULL : new int[numFaces * 4];
        for (int i = 0; i < numFaces; i++) {
            meshIndices[i * 4    ] = indices->getValue(i * 3); 
            meshIndices[i * 4 + 1] = indices->getValue(i * 3 + 1); 
            meshIndices[i * 4 + 2] = indices->getValue(i * 3 + 2);
            meshIndices[i * 4 + 3] = -1;            
        }
        if (m_coordIndex != NULL)
            m_coordIndex->unref();
        m_coordIndex = new MFInt32(meshIndices, numFaces * 4);    
        m_coordIndex->ref();
    }

    MFFloat *fogCoords = NULL;
    if (fogCoord()->getValue())
        if (fogCoord()->getValue()->getType() == X3D_FOG_COORDINATE) 
            fogCoords = ((NodeFogCoordinate *)
                         (fogCoord()->getValue()))->depth();

    float transparency = 0;
    if (hasParent())
        transparency = getParent()->getTransparency();
    if (ccw()->getValue())
        meshFlags |= MESH_CCW;
    if (solid()->getValue())
        meshFlags |= MESH_SOLID;    
    // according to 
    // http://www.web3d.org/x3d/specifications/ISO-IEC-19775-X3DAbstractSpecification/Part01/components/rendering.html#TriangleSet
    // The value of the colorPerVertex field is ignored and always treated as 
    // TRUE. 
    if (!bcolorPerVertex && !m_colorPerVertexWarning) {
        m_scene->warning("Warning: According to chapter 11.4.13 TriangleSet of ISO/IEC 19775-1:2008, the value of the colorPerVertex field is ignored and always treated as TRUE");
        m_colorPerVertexWarning = true;
    }
    meshFlags |= MESH_COLOR_PER_VERTEX;
    float creaseAngle = M_PI / 2.0f;

    if (coords)
        m_mesh = new MyMesh(this, coords, m_coordIndex, normals, NULL, colors, 
                            NULL, texCoords, NULL, creaseAngle, meshFlags, 
                            transparency, fogCoords);
   else
        m_meshDouble = new MyMeshDouble(this, coordsDouble, m_coordIndex, 
                                        normals, NULL, colors, NULL, texCoords, 
                                        NULL, creaseAngle, meshFlags, 
                                        transparency, fogCoords, 
                                        texCoord()->getValue());
}

void
NodeIndexedTriangleSet::setNormalFromMesh(Node *nnormal)
{
    if (nnormal->getType() != VRML_NORMAL)
        return;

    if (meshDirty() || (m_mesh == NULL)) {
        createMesh(this);
        m_meshDirty = false;
        if (m_mesh == NULL)
            return;
    }
    MFVec3f *normals = m_mesh->getNormals();
    MFVec3f *vertices = m_mesh->getVertices();
    if (normals != NULL) {
        // rebuild normals similar to the "index" field
        MFInt32 *ni = m_mesh->getNormalIndex();
        MFVec3f *newNormals = new MFVec3f();
        for (int i = 0; i < vertices->getSFSize(); i++)
            newNormals->appendSFValue(0.0f, 0.0f, 0.0f);
        int nextVertex = 0;
        for (int i = 0; i < ni->getSFSize(); i++) {
             int niIndex = ni->getValue(i);
             if (niIndex > -1) {
                 Vec3f v1 = normals->getVec(niIndex);
                 int nextVertexIndex = index()->getValue(nextVertex);
                 Vec3f v2 = newNormals->getVec(nextVertexIndex);
                 Vec3f v = v1 + v2;
                 newNormals->setSFValue(nextVertexIndex, v.x, v.y, v.z);
                 nextVertex++;
             }
        }
        for (int i = 0; i < newNormals->getSFSize(); i++) {
            Vec3f v = newNormals->getVec(i);
            v.normalize();
            newNormals->setSFValue(i, v.x, v.y, v.z);
        }
        ((NodeNormal *)nnormal)->vector(newNormals);
        normalPerVertex(new SFBool(true));
    }
}

void
NodeIndexedTriangleSet::setTexCoordFromMesh(Node *ntexCoord)
{
    if (ntexCoord->getType() != VRML_TEXTURE_COORDINATE)
        return;
    if (m_mesh == NULL)
        return;
    MFVec2f *v = m_mesh->generateTextureCoordinates();
    if (v != NULL) {
        ((NodeTextureCoordinate *)ntexCoord)->point(v);
    }
}



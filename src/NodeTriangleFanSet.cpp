/*
 * NodeTriangleFanSet.cpp
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

#include "NodeTriangleFanSet.h"
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
#include "Util.h"

ProtoTriangleFanSet::ProtoTriangleFanSet(Scene *scene)
  : GeometryProto(scene, "TriangleFanSet")
{
    color.set(
          addExposedField(SFNODE, "color", new SFNode(NULL), COLOR_NODE));
    coord.set(
          addExposedField(SFNODE, "coord", new SFNode(NULL), COORDINATE_NODE));
    fanCount.set(
          addExposedField(MFINT32, "fanCount", new MFInt32(), new SFInt32(3)));
    normal.set(
          addExposedField(SFNODE, "normal", new SFNode(NULL), VRML_NORMAL));
    texCoord.set(
          addExposedField(SFNODE, "texCoord", new SFNode(NULL), 
                          TEXTURE_COORDINATE_NODE));
    ccw.set(
          addField(SFBOOL, "ccw", new SFBool(true)));
    colorPerVertex.set(
          addField(SFBOOL, "colorPerVertex", new SFBool(true)));
    normalPerVertex.set(
          addField(SFBOOL, "normalPerVertex", new SFBool(true)));
    solid.set(
          addField(SFBOOL, "solid", new SFBool(true)));
    ComposedGeometryElements()
}

Node *
ProtoTriangleFanSet::create(Scene *scene)
{ 
    return new NodeTriangleFanSet(scene, this); 
}

NodeTriangleFanSet::NodeTriangleFanSet(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
    m_coordIndex = NULL;
    m_colorPerVertexWarning = false;
}

NodeTriangleFanSet::~NodeTriangleFanSet()
{
    if (m_coordIndex != NULL)
        m_coordIndex->unref();
}

void 
NodeTriangleFanSet::draw()
{
    Node *ncoord = coord()->getValue();
    if (ncoord != NULL) {
        glPushName(coord_Field());       // field coord
        glPushName(0);                   // index 0
        if (ncoord->getType() == VRML_COORDINATE)
            ((NodeCoordinate *)ncoord)->draw(this);
        else if (ncoord->getType() == VRML_GEO_COORDINATE) {
            setDoubleMesh(true);
            ((NodeGeoCoordinate *)ncoord)->draw(this);
        }
        glPopName();
        glPopName();
    }
}

void
NodeTriangleFanSet::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
    Node::setField(index, value, cf);
    update();
}

MFVec3f *
NodeTriangleFanSet::getCoordinates() 
{
    Node *ncoord = coord()->getValue();
    if (ncoord == NULL)
        return NULL;
    else
        return ((NodeCoordinate *)ncoord)->point();
}

MFVec2f *
NodeTriangleFanSet::getTextureCoordinates()
{
    Node *ntexCoord = texCoord()->getValue();
    if (ntexCoord == NULL)
        return NULL;
    else
        return ((NodeTextureCoordinate *)ntexCoord)->point();
}

void
NodeTriangleFanSet::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    Node *coord = ((SFNode *) getField(coord_Field()))->getValue();
    bool bcolorPerVertex = colorPerVertex()->getValue();
    bool bnormalPerVertex = normalPerVertex()->getValue();
   
    if (!coord || ((NodeCoordinate *) coord)->point()->getType() != MFVEC3F)
        return;

    MFVec3f *coords = ((NodeCoordinate *)coord)->point();
    MFVec3f *normals = NULL;
    MFFloat *colors = NULL;

    if (normal()->getValue())
        if (normal()->getValue()->getType() == VRML_NORMAL)
            normals = ((NodeNormal *)(normal()->getValue()))->vector();
    
    int meshFlags = 0;
    if (color()->getValue()) {
        if (color()->getValue()->getType() == VRML_COLOR) 
            colors = ((NodeColor *)(color()->getValue()))->color();
        else if (color()->getValue()->getType() == X3D_COLOR_RGBA) {
            colors = ((NodeColorRGBA *)(color()->getValue()))->color();
            meshFlags |= MESH_COLOR_RGBA;
        }
    }    

    if (coords->getSFSize() > 0) {
        MFInt32 *mffanCount = fanCount();
        int numFaces = 0;
        for (int i = 0; i < mffanCount->getSize(); i++)
            numFaces += mffanCount->getValue(i) - 2;
        int *indices = (numFaces == 0) ? NULL : new int[numFaces * 4];
        int vertexCount = 0;
        int indexCount = 0;
        for (int i = 0; i < mffanCount->getSize(); i++) {
            for (int j = 0; j < (mffanCount->getValue(i) - 2); j++) {
                indices[(indexCount + j) * 4    ] = vertexCount; 
                indices[(indexCount + j) * 4 + 1] = vertexCount + j + 1; 
                indices[(indexCount + j) * 4 + 2] = vertexCount + j + 2;
                indices[(indexCount + j) * 4 + 3] = -1;
            }
            indexCount += mffanCount->getValue(i) - 2;
            vertexCount += mffanCount->getValue(i);
        }             
        if (m_coordIndex != NULL)
            m_coordIndex->unref();        
        m_coordIndex = new MFInt32(indices, numFaces * 4);    
        m_coordIndex->ref();
    }

    MyArray<MFVec2f *> texCoords;
    Util::getTexCoords(texCoords, texCoord()->getValue());    
    
    //if (!texCoord()->getValue())
    //    texCoords = generateTextureCoordinates(coords, texCoordIndex);
    float transparency = 0;
    if (hasParent())
        transparency = getParent()->getTransparency();
    if (ccw()->getValue())
        meshFlags |= MESH_CCW;
    if (solid()->getValue())
        meshFlags |= MESH_SOLID;
    // according to 
    // http://www.web3d.org/x3d/specifications/ISO-IEC-19775-X3DAbstractSpecification/Part01/components/rendering.html#TriangleFanSet
    // The value of the colorPerVertex field is ignored and always treated as 
    // TRUE. 
    if (!bcolorPerVertex && !m_colorPerVertexWarning) {
        m_scene->warning("Warning: According to chapter 11.4.13 TriangleSet of ISO/IEC 19775-1:2008, the value of the colorPerVertex field is ignored and always treated as TRUE");
        m_colorPerVertexWarning = true;
    }
    meshFlags |= MESH_COLOR_PER_VERTEX;

    // If the normal field is not supplied, the normal shall be generated as 
    // perpendicular to the face for either version of normalPerVertex.
    // if (bnormalPerVertex)
    meshFlags |= MESH_NORMAL_PER_VERTEX;
    float creaseAngle = 0;
    if (bnormalPerVertex) {
        creaseAngle = 1.57f;
        meshFlags |= MESH_NORMAL_PER_VERTEX;
    }

    m_mesh = new MyMesh(this, coords, m_coordIndex, normals, NULL, colors, 
                        NULL, texCoords, NULL,creaseAngle, meshFlags, 
                        transparency);
}

Node * 
NodeTriangleFanSet::toIndexedLineSet(void)
{
    if (m_mesh == NULL)
        return NULL;
    NodeCoordinate *ncoord = (NodeCoordinate *)m_scene->createNode("Coordinate");
    ncoord->point(new MFVec3f(*(m_mesh->getVertices())));
    NodeIndexedLineSet *node = (NodeIndexedLineSet *)
                               m_scene->createNode("IndexedLineSet");
    node->coord(new SFNode(ncoord));
    node->coordIndex(new MFInt32(*(m_mesh->getCoordIndex())));
    return node;
}

void
NodeTriangleFanSet::flip(int index)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord)
        if (ncoord->getType() == VRML_COORDINATE)
            ncoord->flip(index);
    NodeNormal *nnormal = (NodeNormal *)normal()->getValue();
    if (nnormal)
        if (nnormal->getType() == VRML_NORMAL)
            nnormal->flip(index);
    SFBool *bccw = new SFBool(!(ccw()->getValue()));
    ccw(bccw);
    m_meshDirty = true;
}

void
NodeTriangleFanSet::swap(int fromTo)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord)
        if (ncoord->getType() == VRML_COORDINATE) 
             ncoord->swap(fromTo);
    NodeNormal *nnormal = (NodeNormal *)normal()->getValue();
    if (nnormal)
        if (nnormal->getType() == VRML_NORMAL)
            nnormal->swap(fromTo);
    SFBool *bccw = new SFBool(!(ccw()->getValue()));
    ccw(bccw);
    m_meshDirty = true;
}

void
NodeTriangleFanSet::setNormalFromMesh(Node *nnormal)
{
    if (nnormal->getType() != VRML_NORMAL)
        return;

    if (meshDirty() || (m_mesh == NULL)) {
        createMesh();
        m_meshDirty = false;
    }

    MFVec3f *v = getSmoothNormals();
    if (v != NULL) {
        ((NodeNormal *)nnormal)->vector(v);
        normalPerVertex(new SFBool(true));
    }
}

void
NodeTriangleFanSet::setTexCoordFromMesh(Node *ntexCoord)
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


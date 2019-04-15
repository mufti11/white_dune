/*
 * NodeIndexedQuadSet.cpp
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#include "NodeIndexedQuadSet.h"
#include "Proto.h"
#include "FieldValue.h"
#include "Scene.h"
#include "MyMesh.h"
#include "Vec3f.h"
#include "MFInt32.h"
#include "MFNode.h"
#include "SFNode.h"
#include "SFBool.h"
#include "MFInt32.h"
#include "DuneApp.h"
#include "NodeColor.h"
#include "NodeColorRGBA.h"
#include "NodeCoordinate.h"
#include "NodeGeoCoordinate.h"
#include "NodeNormal.h"
#include "NodeTextureCoordinate.h"
#include "NodeIndexedLineSet.h"
#include "Util.h"

ProtoIndexedQuadSet::ProtoIndexedQuadSet(Scene *scene)
  : GeometryProto(scene, "IndexedQuadSet")
{
    addEventIn(MFINT32, "set_index");
    color.set(
        addExposedField(SFNODE, "color", new SFNode(), COLOR_NODE));
    coord.set(
        addExposedField(SFNODE, "coord", new SFNode(), COORDINATE_NODE));
    normal.set(
        addExposedField(SFNODE, "normal", new SFNode(), VRML_NORMAL));
    texCoord.set(
        addExposedField(SFNODE, "texCoord", new SFNode(),
                        TEXTURE_COORDINATE_NODE));
    ccw.set(
        addField(SFBOOL, "ccw", new SFBool(true)));
    colorPerVertex.set(
        addField(SFBOOL, "colorPerVertex", new SFBool(true)));
    normalPerVertex.set(
        addField(SFBOOL, "normalPerVertex", new SFBool(true)));
    solid.set(
        addField(SFBOOL, "solid", new SFBool(true)));
    index.set(
        addField(MFINT32, "index", new MFInt32()));
    ComposedGeometryElements()
    x3domGeometryCommonFields()
    normalUpdateMode.set(
      addExposedField(SFSTRING, "normalUpdateMode", new SFString("fast")));
    setFieldFlags(normalUpdateMode, FF_X3DOM_ONLY);
}

Node *
ProtoIndexedQuadSet::create(Scene *scene)
{ 
    return new NodeIndexedQuadSet(scene, this); 
}

NodeIndexedQuadSet::NodeIndexedQuadSet(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
    m_coordIndex = NULL;
    m_colorPerVertexWarning = false;
}

NodeIndexedQuadSet::~NodeIndexedQuadSet()
{
    if (m_coordIndex != NULL)
        m_coordIndex->unref();
}

void 
NodeIndexedQuadSet::draw()
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
NodeIndexedQuadSet::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
    Node::setField(index, value, cf);
    update();
}

MFVec3f *
NodeIndexedQuadSet::getCoordinates() 
{
    Node *ncoord = coord()->getValue();
    if (ncoord == NULL)
        return NULL;
    else
        return ((NodeCoordinate *)ncoord)->point();
}

MFVec2f *
NodeIndexedQuadSet::getTextureCoordinates()
{
    Node *ntexCoord = texCoord()->getValue();
    if (ntexCoord == NULL)
        return NULL;
    else
        return ((NodeTextureCoordinate *)ntexCoord)->point();
}

void
NodeIndexedQuadSet::createMesh(bool cleanDoubleVertices, bool triangulate)
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

    MFInt32 *indices = index();

    if (coords->getSFSize() > 0) {
        int numFaces = indices->getSFSize() / 4;
        int *meshIndices = (numFaces == 0) ? NULL : new int[numFaces * 5];
        for (int i = 0; i < numFaces; i++) {
            meshIndices[i * 5    ] = indices->getValue(i * 4); 
            meshIndices[i * 5 + 1] = indices->getValue(i * 4 + 1); 
            meshIndices[i * 5 + 2] = indices->getValue(i * 4 + 2);
            meshIndices[i * 5 + 3] = indices->getValue(i * 4 + 3);
            meshIndices[i * 5 + 4] = -1;            
        }
        if (m_coordIndex != NULL)
            m_coordIndex->unref();
        m_coordIndex = new MFInt32(meshIndices, numFaces * 5);    
        m_coordIndex->ref();
    }

    MyArray<MFVec2f *> texCoords;
    Util::getTexCoords(texCoords, texCoord()->getValue());
        
    float transparency = 0;
    if (hasParent())
        transparency = getParent()->getTransparency();
    if (ccw()->getValue())
        meshFlags |= MESH_CCW;
    if (solid()->getValue())
        meshFlags |= MESH_SOLID;    
    // according to 
    // http://www.web3d.org/x3d/specifications/ISO-IEC-19775-X3DAbstractSpecification/Part01/components/rendering.html#QuadSet
    // The value of the colorPerVertex field is ignored and always treated as 
    // TRUE. 
    if (!bcolorPerVertex && !m_colorPerVertexWarning) {
        m_scene->warning("Warning: According to chapter 11.4.13 TriangleSet of ISO/IEC 19775-1:2008, the value of the colorPerVertex field is ignored and always treated as TRUE");
        m_colorPerVertexWarning = true;
    }
    meshFlags |= MESH_COLOR_PER_VERTEX;
    float creaseAngle = 0;
    if (bnormalPerVertex) {
        creaseAngle = 1.57f;
        meshFlags |= MESH_NORMAL_PER_VERTEX;
    }

    m_mesh = new MyMesh(this, coords, m_coordIndex, normals, NULL, colors, 
                        NULL, texCoords, NULL, creaseAngle, meshFlags, 
                        transparency);
}

Node * 
NodeIndexedQuadSet::toIndexedLineSet(void)
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
NodeIndexedQuadSet::flip(int index)
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
NodeIndexedQuadSet::swap(int fromTo)
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
NodeIndexedQuadSet::setNormalFromMesh(Node *nnormal)
{
    if (nnormal->getType() != VRML_NORMAL)
        return;

    if (meshDirty() || (m_mesh == NULL)) {
        createMesh(this);
        m_meshDirty = false;
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
NodeIndexedQuadSet::setTexCoordFromMesh(Node *ntexCoord)
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




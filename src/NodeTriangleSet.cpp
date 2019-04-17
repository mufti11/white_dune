/*
 * NodeTriangleSet.cpp
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

#include "NodeTriangleSet.h"
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

ProtoTriangleSet::ProtoTriangleSet(Scene *scene)
  : GeometryProto(scene, "TriangleSet")
{
    color.set(
          addExposedField(SFNODE, "color", new SFNode(NULL), COLOR_NODE));
    coord.set(
          addExposedField(SFNODE, "coord", new SFNode(NULL), COORDINATE_NODE));
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
    x3domGeometryCommonFields()
    normalUpdateMode.set(
      addExposedField(SFSTRING, "normalUpdateMode", new SFString("fast")));
    setFieldFlags(normalUpdateMode, FF_X3DOM_ONLY);
}

Node *
ProtoTriangleSet::create(Scene *scene)
{ 
    return new NodeTriangleSet(scene, this); 
}

NodeTriangleSet::NodeTriangleSet(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
    m_coordIndex = NULL;
    m_colorPerVertexWarning = false;
}

NodeTriangleSet::~NodeTriangleSet()
{
    if (m_coordIndex != NULL)
        m_coordIndex->unref();
}

void 
NodeTriangleSet::draw()
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
NodeTriangleSet::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
    Node::setField(index, value, cf);
    update();
}

MFVec3f *
NodeTriangleSet::getCoordinates() 
{
    Node *ncoord = coord()->getValue();
    if (ncoord == NULL)
        return NULL;
    else
        return ((NodeCoordinate *)ncoord)->point();
}

MFVec2f *
NodeTriangleSet::getTextureCoordinates()
{
    Node *ntexCoord = texCoord()->getValue();
    if (ntexCoord == NULL)
        return NULL;
    else
        return ((NodeTextureCoordinate *)ntexCoord)->point();
}

void
NodeTriangleSet::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    Node *coord = ((SFNode *) getField(coord_Field()))->getValue();
    bool bcolorPerVertex = colorPerVertex()->getValue();
   
    MFVec3f *coords = NULL;
    MFVec3d *coordsDouble = NULL;
    if (coord->getType() == VRML_COORDINATE)
        coords = ((NodeCoordinate *)coord)->point();
    else
        coordsDouble = ((NodeGeoCoordinate *)coord)->pointX3D();

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
        int numFaces = coords->getSFSize() / 3;
        int *indices = (numFaces == 0) ? NULL : new int[numFaces * 4];
        for (int i = 0; i < numFaces; i++) {
            indices[i * 4    ] = i * 3; 
            indices[i * 4 + 1] = i * 3 + 1; 
            indices[i * 4 + 2] = i * 3 + 2;
            indices[i * 4 + 3] = -1;            
        }
        if (m_coordIndex != NULL)
            m_coordIndex->unref();
        m_coordIndex = new MFInt32(indices, numFaces * 4);    
        m_coordIndex->ref();
    }

    MFFloat *fogCoords = NULL;
    if (fogCoord()->getValue())
        if (fogCoord()->getValue()->getType() == X3D_FOG_COORDINATE)
            fogCoords = ((NodeFogCoordinate *) 
                         (fogCoord()->getValue()))->depth();

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
    // http://www.web3d.org/x3d/specifications/ISO-IEC-19775-X3DAbstractSpecification/Part01/components/rendering.html#TriangleSet
    // The value of the colorPerVertex field is ignored and always treated as 
    // TRUE. 
    if (!bcolorPerVertex && !m_colorPerVertexWarning) {
        m_scene->warning("Warning: According to chapter 11.4.13 TriangleSet of ISO/IEC 19775-1:2008, the value of the colorPerVertex field is ignored and always treated as TRUE");
        m_colorPerVertexWarning = true;
    }
    meshFlags |= MESH_COLOR_PER_VERTEX;
    // according to
    // http://www.web3d.org/x3d/specifications/ISO-IEC-19775-1.2-X3D-AbstractSpecification/Part01/components/rendering.html#TriangleSet
    // If the normal field is not supplied, the normal shall be generated as 
    // perpendicular to the face for either version of normalPerVertex.
    meshFlags |= MESH_NORMAL_PER_VERTEX;
    float creaseAngle = M_PI / 2.0f;

    if (coords)
        m_mesh = new MyMesh(this, coords, m_coordIndex, normals, NULL, colors, 
                            NULL, texCoords, NULL, creaseAngle, meshFlags, 
                            transparency, fogCoords);
   else
        m_meshDouble = new MyMeshDouble(this, coordsDouble, m_coordIndex, 
                                        normals, NULL, colors, NULL, texCoords, 
                                        NULL, creaseAngle, meshFlags, 
                                        transparency, fogCoords);
}

Node * 
NodeTriangleSet::toIndexedLineSet(void)
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
NodeTriangleSet::flip(int index)
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
NodeTriangleSet::swap(int fromTo)
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
NodeTriangleSet::setNormalFromMesh(Node *nnormal)
{
    if (nnormal->getType() != VRML_NORMAL)
        return;

    if (meshDirty() || (m_mesh == NULL)) {
        createMesh();
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
                 int nextVertexIndex = nextVertex;
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
NodeTriangleSet::setTexCoordFromMesh(Node *ntexCoord)
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


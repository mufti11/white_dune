/*
 * MeshBasedNode.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2004 J. "MUFTI" Scheurich
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

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
# include <fcntl.h>
# include <sys/wait.h>
#endif

#include "swt.h"
#include "resource.h"
#include "stdafx.h"
#include "MeshBasedNode.h"
#include "Scene.h"
#include "FieldValue.h"
#include "SFInt32.h"
#include "MyMesh.h"
#include "FaceData.h"
#include "MFFloat.h"
#include "MFInt32.h"
#include "MFVec2f.h"
#include "MFVec3f.h"
#include "SFNode.h"
#include "SFBool.h"
#include "SFVec3f.h"
#include "Vec2f.h"
#include "Vec3f.h"
#include "RenderState.h"
#include "DuneApp.h"
#include "MeshSubdivide.h"
#include "NodeShape.h"
#include "NodeCoordinate.h"
#include "NodeNormal.h"
#include "NodeTextureCoordinate.h"
#include "NodeColor.h"
#include "NodeColorRGBA.h"
#include "NodeIndexedFaceSet.h"
#include "NodeIndexedTriangleSet.h"
#include "NodeTriangleSet.h"
#include "NodeElevationGrid.h"
#include "NodeGeoElevationGrid.h"
#include "NodeCone.h"
#include "NodeMultiTexture.h"
#include "NodeViewpoint.h"
#include "LdrawDefines.h"

MeshBasedNode::MeshBasedNode(Scene *scene, Proto *proto)
  : GeometryNode(scene, proto)
{
    assert(proto->isMesh());
    m_mesh = NULL;
    m_meshDouble = NULL;
    m_isDoubleMesh = false;
    m_meshDirty = true;
}

MeshBasedNode::~MeshBasedNode()
{
    delete m_mesh;
    m_mesh = NULL;
    delete m_meshDouble;
    m_meshDouble = NULL;
}

void 
MeshBased::updateMesh(void)
{
    if (meshDirty()) {
        if (m_isDoubleMesh) {
            delete m_meshDouble;
            m_meshDouble = NULL;
            createMesh();
        } else {
            delete m_mesh;
            m_mesh = NULL;
            createMesh();
        }
        m_meshDirty = false;
    }
}

void
MeshBased::meshDraw()
{
    updateMesh();

    if (m_isDoubleMesh) {    
        if (!m_meshDouble) return;

        m_meshDouble->sort();
        m_meshDouble->draw(-1);
    } else {
        if (!m_mesh) return;

        m_mesh->sort();
        m_mesh->draw(-1);
    }
}

void
MeshBased::drawNormals()
{
    updateMesh();

    if (m_isDoubleMesh) {
       if (!m_meshDouble) return;

       m_meshDouble->drawNormals();
    } else {
       if (!m_mesh) return;

       m_mesh->drawNormals();
    }
}

MyMesh *
MeshBasedNode::getMesh(void) {
    updateMesh(); 
    return m_mesh;
}

MyMeshDouble *
MeshBasedNode::getMeshDouble(void) {
    updateMesh(); 
    return m_meshDouble;
}

bool    
MeshBasedNode::hasColor(void)
{
    updateMesh(); 

    if (m_isDoubleMesh) {
        if (!m_meshDouble) return false;

        return m_meshDouble->hasColor();
    } else {
        if (!m_mesh) return false;

        return m_mesh->hasColor();
    }
}

bool    
MeshBasedNode::hasColorRGBA(void)
{
    updateMesh(); 

    if (m_isDoubleMesh) {
        if (!m_meshDouble) return false;

        return m_meshDouble->hasColorRGBA();
    } else {
        if (!m_mesh) return false;

        return m_mesh->hasColorRGBA();
    }
}

bool    
MeshBasedNode::hasColorPerVertex(void)
{
    updateMesh(); 

    if (m_isDoubleMesh) {
         if (!m_meshDouble) return false;

         return m_meshDouble->colorPerVertex();
    } else {
         if (!m_mesh) return false;

         return m_mesh->colorPerVertex();
    }
}

Node * 
MeshBasedNode::toIndexedFaceSet(int meshFlags, bool cleanVertices,
                                bool triangulate)
{
    bool wantSimpleQuadTriangulate = (meshFlags & MESH_SIMPLE_TRIANGULATE);
    updateMesh(); 

    if (m_isDoubleMesh) {
       if (!m_meshDouble) return NULL;

       if (wantSimpleQuadTriangulate)
           m_meshDouble->simpleQuadTriangulate();

       m_meshDirty = true;
    
       return m_meshDouble->toIndexedFaceSet(meshFlags, m_scene);
    }
    if (!m_mesh) return NULL;

    if (wantSimpleQuadTriangulate)
        m_mesh->simpleQuadTriangulate();

    m_meshDirty = true;
    
    return m_mesh->toIndexedFaceSet(meshFlags, m_scene);
}

#define createNormalFromMesh(node) \
{ \
    NodeNormal *nnormal = (NodeNormal *)m_scene->createNode("Normal"); \
    node->setNormalFromMesh(nnormal); \
    node->normal(new SFNode(nnormal)); \
}

#define createTexCoordFromMesh(node) \
{ \
    NodeTextureCoordinate *ntexCoord = (NodeTextureCoordinate *) \
         m_scene->createNode("TextureCoordinate"); \
    node->setTexCoordFromMesh(ntexCoord); \
    node->texCoord(new SFNode(ntexCoord)); \
}

Node * 
MeshBasedNode::toTriangleSet(int meshFlags)
{
    bool targetHasCcw = (meshFlags & MESH_TARGET_HAS_CCW);
    bool wantNormal = (meshFlags & MESH_WANT_NORMAL);
    bool wantTexCoord = (meshFlags & MESH_WANT_TEX_COORD);

    updateMesh(); 

    if (m_isDoubleMesh) {
        if (!m_meshDouble) return NULL;
    } else {
        if (!m_mesh) return NULL;
    }
    MyMesh *mesh = triangulateMesh();

    MFVec3f *vertices = mesh->getVertices();

    NodeCoordinate *ncoord = (NodeCoordinate *)m_scene->createNode("Coordinate");
    MFVec3f *points = new MFVec3f();

    MFInt32 *coordIndices = mesh->getCoordIndex();

    MFVec3f *normals = mesh->getNormals();
    bool hasNormals = normals && normals->getValues();
    MFVec3f *vectors = new MFVec3f();

    bool hasColors = mesh->getColors() && mesh->getColors()->getValues();
    bool hasColorsRGBA = hasColors && mesh->hasColorRGBA();
    hasColors = hasColors && !mesh->hasColorRGBA();
    MFFloat *mfColors = mesh->getColors();
    MFColor *colors = hasColors ? new MFColor() : NULL;
    MFColorRGBA *colorsRGBA = hasColorsRGBA ? new MFColorRGBA() : NULL;

    MFVec2f *texCoords = mesh->getTexCoords();
    bool hasTexCoords = texCoords && texCoords->getValues();
    MFVec2f *coords = new MFVec2f();

    bool solid = mesh->solid();
    bool ccw = mesh->ccw();
    bool drawFront = !solid || !ccw;
    bool drawBack = !solid || ccw;
    if (targetHasCcw) {
        drawFront = true;
        drawBack = false;
    }
    int numVertices = 0;
    if (drawFront)
        for (int i = 0; i < coordIndices->getSize(); i++) {
            int index = coordIndices->getValue(i);
            if (index >= 0) {
                points->setSFValue(numVertices, vertices->getValue(index)); 
                if (hasNormals)
                    vectors->setSFValue(numVertices, 
                                        normals->getValue(index)); 
                if (hasColors)
                    colors->setSFValue(numVertices, 
                                       mfColors->getValues() + index * 3); 
                if (hasColorsRGBA)
                    colorsRGBA->setSFValue(numVertices, 
                                           mfColors->getValues() + index * 4); 
                if (hasTexCoords)
                    coords->setSFValue(numVertices, 
                                       texCoords->getValue(index)); 

                numVertices++;
            }
        }
    if (drawBack)
        for (int i = coordIndices->getSize() - 1; i > -1; i--) {
            int index = coordIndices->getValue(i);
            if (index >= 0) {
                points->setSFValue(numVertices, vertices->getValue(index)); 
                if (hasNormals)
                    vectors->setSFValue(numVertices, 
                                        normals->getValue(index)); 
                if (hasColors)
                    colors->setSFValue(numVertices, 
                                       mfColors->getValues() + index * 3); 
                if (hasColorsRGBA)
                    colorsRGBA->setSFValue(numVertices, 
                                           mfColors->getValues() + index * 4); 
                if (hasTexCoords)
                    coords->setSFValue(numVertices, 
                                       texCoords->getValue(index)); 

                numVertices++;
            }
        }
    ncoord->point(points);
    NodeTriangleSet *node = (NodeTriangleSet *)
                            m_scene->createNode("TriangleSet");
    node->coord(new SFNode(ncoord));
    node->solid(new SFBool(mesh->solid()));
    node->ccw(new SFBool(mesh->ccw()));

    if (hasNormals || wantNormal) {
        NodeNormal *nnormal = (NodeNormal *)m_scene->createNode("Normal");
        if (hasNormals) {
            nnormal->vector(new MFVec3f(vectors));
        }
        node->normal(new SFNode(nnormal));
    }

    if (hasColors) {
        NodeColor *ncolor = (NodeColor *) m_scene->createNode("Color");
        ncolor->color(new MFColor(colors));
        node->color(new SFNode(ncolor));
    }
    if (hasColorsRGBA) {
        NodeColorRGBA *ncolor = (NodeColorRGBA *) 
                                m_scene->createNode("ColorRGBA");
        ncolor->color(new MFColorRGBA(colorsRGBA));
        node->color(new SFNode(ncolor));
    }

    if (hasTexCoords) {
        NodeTextureCoordinate *ntexCoord = (NodeTextureCoordinate *)
                                           m_scene->createNode("TextureCoordinate");
        ntexCoord->point(new MFVec2f(coords));
        node->texCoord(new SFNode(ntexCoord));
    } else if (wantTexCoord)
        createTexCoordFromMesh(node);
    
    m_meshDirty = true;
    return node;
}

Node * 
MeshBasedNode::toIndexedTriangleSet(int meshFlags)
{
    bool targetHasCcw = (meshFlags & MESH_TARGET_HAS_CCW);
    bool wantNormal = (meshFlags & MESH_WANT_NORMAL);
    bool wantTexCoord = false; // (meshFlags & MESH_WANT_TEX_COORD);

    updateMesh(); 

    bool solid;
    bool ccw;
    if (m_isDoubleMesh) {
        if (!m_meshDouble) return NULL;

        solid = m_meshDouble->solid();
        ccw = m_meshDouble->ccw();
    } else {
        if (!m_mesh) return NULL;

        solid = m_mesh->solid();
        ccw = m_mesh->ccw();
   }

    NodeTriangleSet *triangles = (NodeTriangleSet *)toTriangleSet(meshFlags);
    if (triangles == NULL)
        return NULL;
    MFVec3f *mfVertices = NULL;
    MFInt32 *mfCoordIndex = NULL;
    if (optimizeOnConvertionToIndexedTriangleSet())
        triangles->optimizeMesh(&mfVertices, &mfCoordIndex);
    bool drawFront = !solid || !ccw;
    bool drawBack = !solid || ccw;
    if (targetHasCcw) {
        drawFront = true;
        drawBack = false;
    }
    int indexCount = 0;
    int size = mfCoordIndex->getSize();
    if (drawFront)
        for (int i = 0; i < size; i++) {
            if (mfCoordIndex->getValue(i) > -1)
                mfCoordIndex->setSFValue(indexCount++, 
                                         mfCoordIndex->getValue(i));
        }
    if (drawBack) {
        for (int i = size - 1; i > -1; i--) {
            if (mfCoordIndex->getValue(i) > -1)
                mfCoordIndex->setSFValue(indexCount++, 
                                         mfCoordIndex->getValue(i));
        }
    }
    int indexSize = mfCoordIndex->getSize();
    for (int i = indexCount; i < indexSize; i++)
        mfCoordIndex->removeSFValue(i);
    NodeIndexedTriangleSet *node = (NodeIndexedTriangleSet *)
                                   m_scene->createNode("IndexedTriangleSet");
    NodeCoordinate *ncoord = (NodeCoordinate *)
                             m_scene->createNode("Coordinate");
    if (ncoord && mfVertices && mfCoordIndex) {
        ncoord->point(new MFVec3f(mfVertices));
        node->coord(new SFNode(ncoord));
        node->index(new MFInt32(mfCoordIndex));
        node->ccw(new SFBool(triangles->ccw()->getValue()));
        node->solid(new SFBool(triangles->solid()->getValue()));
    }

    if (wantNormal)
        createNormalFromMesh(node);

    if (wantTexCoord)
        createTexCoordFromMesh(node);

    m_meshDirty = true;

    return node;
}

Node * 
MeshBasedNode::alreadyTriangulatedToIndexedTriangleSet(bool hasCcw)
{
    updateMesh(); 

    bool solid;
    bool ccw;
    MFInt32 *coordIndices;
    if (m_isDoubleMesh) {
        if (!m_meshDouble) return NULL;

        coordIndices = m_mesh->getCoordIndex();
        solid = m_meshDouble->solid();
        ccw = m_meshDouble->ccw();
    } else {
        if (!m_mesh) return NULL;

        coordIndices = m_mesh->getCoordIndex();
        solid = m_mesh->solid();
        ccw = m_mesh->ccw();
    }

    bool drawFront = !solid || !ccw;
    bool drawBack = !solid || ccw;
    MFInt32 *indices = new MFInt32();
    if (hasCcw) {
        drawFront = true;
        drawBack = false;
    }
    int numIndices = 0;
    if (drawFront)
        for (int i = 0; i < coordIndices->getSize(); i++) {
            int index = coordIndices->getValue(i);
            if (index >= 0)
                indices->setSFValue(numIndices++, index); 
        }
    if (drawBack)
        for (int i = coordIndices->getSize() - 1; i > -1; i++) {
            int index = coordIndices->getValue(i);
            if (index >= 0)
                indices->setSFValue(numIndices++, index); 
        }

    NodeCoordinate *ncoord = (NodeCoordinate *)m_scene->createNode("Coordinate");
    ncoord->point(m_mesh->getVertices());
    NodeIndexedTriangleSet *node = (NodeIndexedTriangleSet *)
                               m_scene->createNode("IndexedTriangleSet");
    node->coord(new SFNode(ncoord));
    NodeNormal *nnormal = NULL;
    if (m_isDoubleMesh) {
        if (m_meshDouble->getNormals()) {
            nnormal = (NodeNormal *)m_scene->createNode("Normal");
            nnormal->vector(m_meshDouble->getNormals());
        }
    } else {
        if (m_mesh->getNormals()) {
            nnormal = (NodeNormal *)m_scene->createNode("Normal");
            nnormal->vector(m_mesh->getNormals());
        }
    }
    if (nnormal)
        node->normal(new SFNode(nnormal));
    node->normalPerVertex(new SFBool(m_mesh->normalPerVertex()));
    node->index(new MFInt32(indices));
    node->solid(new SFBool(solid));
    node->ccw(new SFBool(ccw));
    NodeTextureCoordinate *ntexCoord = NULL;
    if (m_isDoubleMesh) {
        if (m_meshDouble->getTexCoords()) {
            ntexCoord = (NodeTextureCoordinate *)
                        m_scene->createNode("TextureCoordinate");
           ntexCoord->point(m_meshDouble->getTexCoords());
        }
    } else {
        if (m_mesh->getTexCoords()) {
            ntexCoord = (NodeTextureCoordinate *)
                        m_scene->createNode("TextureCoordinate");
           ntexCoord->point(m_mesh->getTexCoords());
        }
    }
    if (ntexCoord)
        node->texCoord(new SFNode(ntexCoord));

    m_meshDirty = true;
    return node;
}

Vec3f
MeshBasedNode::getMinBoundingBox(void) 
{ 
    updateMesh(); 

    Vec3f ret(0, 0, 0);
    if (m_isDoubleMesh) {
        if (m_mesh && m_meshDouble->getVertices())
           return m_meshDouble->getVertices()->getMFVec3f()->
                      getMinBoundingBox();
    } else {
        if (m_mesh && m_mesh->getVertices())
           return m_mesh->getVertices()->getMinBoundingBox();
    }
    return ret;
}

Vec3f
MeshBasedNode::getMaxBoundingBox(void) 
{ 
    updateMesh(); 

    Vec3f ret(0, 0, 0);
    if (m_isDoubleMesh) {
        if (m_mesh && m_meshDouble->getVertices())
           return m_meshDouble->getVertices()->getMFVec3f()->
                      getMaxBoundingBox();
    } else {
        if (m_mesh && m_mesh->getVertices())
           return m_mesh->getVertices()->getMaxBoundingBox();
    }
    return ret;
}

MFVec3f *
MeshBased::getVertices(void)
{
    updateMesh(); 

    if (m_isDoubleMesh) {
        if (m_meshDouble == NULL)
            return NULL;
        return m_meshDouble->getVertices()->getMFVec3f();
    }
    if (m_mesh == NULL)
        return NULL;
    return m_mesh->getVertices();
}

MFVec2f *
MeshBased::getTextureCoordinates()
{
    updateMesh(); 

    if (m_isDoubleMesh) {
        if (m_meshDouble == NULL)
            return NULL;
        return m_meshDouble->generateTextureCoordinates();
    }
    if (m_mesh == NULL)
        return NULL;
    return m_mesh->generateTextureCoordinates();
}

MFInt32 *
MeshBased::getTexCoordIndex()
{
    updateMesh(); 

    if (m_isDoubleMesh) {
        if (m_meshDouble == NULL)
            return NULL;
        return m_meshDouble->getCoordIndex();
    }
    if (m_mesh == NULL)
        return NULL;
    return m_mesh->getCoordIndex();
}

int
MeshBasedNode::optimizeNormals(int *coordIndex, Vec3f *vertices, 
                               Vec3f *normals, int length, bool ccw,
                               bool cleanDoubleVertices)
{
    if (length < 2) 
        return length;
    int *newCoordIndex = new int[length];
    int newIndex = 1;
    newCoordIndex[0] = coordIndex[0];
    for (int i = 1; i < length; i++)
        if (coordIndex[i-1] < 0)
            newCoordIndex[newIndex++] = coordIndex[i];
        else if (coordIndex[i] < 0)
            newCoordIndex[newIndex++] = coordIndex[i];
        else {
            if ((vertices[coordIndex[i-1]] - vertices[coordIndex[i]]).length() 
                > FEPSILON)
                newCoordIndex[newIndex++] = coordIndex[i];
            else {
                // reaccount normal for remaining vertex i-1
                int index1 = i-2;
                int index2 = i;
                int index3 = i+1;
                if ((index1 < 0) ||
                    (index3 > length) || 
                    (coordIndex[index1] < 0) ||
                    (coordIndex[index2] < 0) ||   
                    (coordIndex[index3] < 0)) {    
                    index1 = i-3;
                    index2 = i-2;
                    index3 = i;
                }
                if ((index1 < 0) ||
                    (index3 > length) || 
                    (coordIndex[index1] < 0) ||
                    (coordIndex[index2] < 0) ||   
                    (coordIndex[index3] < 0)) {    
                    index1 = i;
                    index2 = i+1;
                    index3 = i+2;
                }
                if ((index1 < 0) ||
                    (index3 > length) || 
                    (coordIndex[index1] < 0) ||
                    (coordIndex[index2] < 0) ||   
                    (coordIndex[index3] < 0)) {    
                    continue;
                }
                if (normals != NULL) {
                    Vec3f edge = (vertices[coordIndex[index2]] - 
                                  vertices[coordIndex[index1]]);
                    normals[coordIndex[i]] = edge.cross(
                                             vertices[coordIndex[index3]] - 
                                             vertices[coordIndex[index2]]);
                    if (!ccw) 
                        normals[coordIndex[i]] = -normals[coordIndex[i]];
                    normals[coordIndex[i]].normalize();
                    normals[coordIndex[i-1]] = normals[coordIndex[i]];
                }
            }
        }
      if (cleanDoubleVertices) {
          for (int i = 0; i < newIndex; i++)
              coordIndex[i] = newCoordIndex[i];
          delete [] newCoordIndex;
      } else
          newIndex = length;
      return newIndex;
}

int
MeshBasedNode::optimizeNormals(int *coordIndex, MFVec3f *vertices, 
                               MFVec3f *normals, int length, bool ccw,
                               bool cleanDoubleVertices)
{
    if (length < 2) 
        return length;
    int *newCoordIndex = new int[length];
    int newIndex = 1;
    newCoordIndex[0] = coordIndex[0];
    for (int i = 1; i < length; i++)
        if (coordIndex[i-1] < 0)
            newCoordIndex[newIndex++] = coordIndex[i];
        else if (coordIndex[i] < 0)
            newCoordIndex[newIndex++] = coordIndex[i];
        else {
            if ((vertices->getVec(coordIndex[i-1]) - 
                 vertices->getVec(coordIndex[i])).length() > FEPSILON)
                newCoordIndex[newIndex++] = coordIndex[i];
            else {
                // reaccount normal for remaining vertex i-1
                int index1 = i-2;
                int index2 = i;
                int index3 = i+1;
                if ((index1 < 0) ||
                    (index3 > length) || 
                    (coordIndex[index1] < 0) ||
                    (coordIndex[index2] < 0) ||   
                    (coordIndex[index3] < 0)) {    
                    index1 = i-3;
                    index2 = i-2;
                    index3 = i;
                }
                if ((index1 < 0) ||
                    (index3 > length) || 
                    (coordIndex[index1] < 0) ||
                    (coordIndex[index2] < 0) ||   
                    (coordIndex[index3] < 0)) {    
                    index1 = i;
                    index2 = i+1;
                    index3 = i+2;
                }
                if ((index1 < 0) ||
                    (index3 > length) || 
                    (coordIndex[index1] < 0) ||
                    (coordIndex[index2] < 0) ||   
                    (coordIndex[index3] < 0)) {    
                    continue;
                }
                if (normals != NULL) {
                    Vec3f edge1 = vertices->getVec(coordIndex[index2]) - 
                                  vertices->getVec(coordIndex[index1]);
                    Vec3f edge2 = vertices->getVec(coordIndex[index3]) - 
                                  vertices->getVec(coordIndex[index2]);
                    Vec3f normal = edge1.cross(edge2);
                    if (!ccw) 
                        normal = -normal;
                    normal.normalize();
                    normals->setVec(coordIndex[i-1], normal);
                    normals->setVec(coordIndex[i], normal);
                }
            }
        }
      if (cleanDoubleVertices) {
          for (int i = 0; i < newIndex; i++)
              coordIndex[i] = newCoordIndex[i];
          delete [] newCoordIndex;
      } else
          newIndex = length;
      return newIndex;
}

int 
MeshBasedNode::countPolygons(void)
{
    updateMesh(); 

    if (m_isDoubleMesh) {
        if (!m_meshDouble) return 0;
        return m_meshDouble->countPolygons();
    }
    if (!m_mesh) return 0;
    return m_mesh->countPolygons();
}

int 
MeshBasedNode::countPolygons2Sided(void)
{
    return isDoubleSided() ? 2 * countPolygons() : 0;
}

int 
MeshBasedNode::countPolygons1Sided(void)
{
    return isDoubleSided() ? 0 : countPolygons();
}

MFVec3f *
MeshBased::getSmoothNormals(void)
{
    if (m_isDoubleMesh) {
        if (m_meshDouble == NULL)
            return NULL;
        return m_meshDouble->getSmoothNormals();
    }
    if (m_mesh == NULL)
        return NULL;
    return m_mesh->getSmoothNormals();
}

MFInt32 *
MeshBased::getSmoothNormalIndex(void)
{
    MFInt32 *i;
    if (m_isDoubleMesh) {
        if (m_meshDouble == NULL)
            return NULL;
        i = m_meshDouble->getNormalIndex();
    } else {
        if (m_mesh == NULL)
            return NULL;
        i = m_mesh->getNormalIndex();
    }    

    if (i && (i->getValues() != NULL)) {
        i =  new MFInt32((int *)((MFInt32 *)i->copy())->getValues(), 
                         i->getSize());
    }
    return i;
}

void
MeshBasedNode::setTexCoordFromMesh(Node *ntexCoord)
{
    if (ntexCoord->getType() != VRML_TEXTURE_COORDINATE)
        return;
    MFVec2f *v;
    if (m_isDoubleMesh) {
        if (m_meshDouble == NULL)
            return;
        v = m_meshDouble->getTexCoords();
    } else {
        if (m_mesh == NULL)
            return;
        v = m_mesh->getTexCoords();
    }
    if (v != NULL) {
        int field = ((NodeTextureCoordinate *)ntexCoord)->point_Field();
        m_scene->setField(ntexCoord, field, v);
    }
}

int             
MeshBasedNode::write(int f, int indent) 
{
    int flags = m_scene->getWriteFlags();
    if (flags & TRIANGULATE) {
        bool shouldBeIndexedTriangleSet = shouldConvertToIndexedTriangleSet();
        bool shouldBeTriangleSet = shouldConvertToTriangleSet();

        if (shouldBeTriangleSet) {
            Node *node = (NodeTriangleSet *)toTriangleSet();
            node->setVariableName(strdup(getVariableName()));
            int ret = node->write(f, indent);
            node->unref();
            return ret;
        } else if (shouldBeIndexedTriangleSet) {
            Node *node = (NodeIndexedTriangleSet *)toIndexedTriangleSet();
            node->setVariableName(strdup(getVariableName()));
            int ret = node->write(f, indent);
            node->unref();
            return ret;
        }
    } else if ((flags & (PURE_VRML97 | PURE_X3DV | X3DOM)) &&
               shouldConvertToIndexedFaceSet()) {
        NodeIndexedFaceSet *node = (NodeIndexedFaceSet *)toIndexedFaceSet();
        node->setVariableName(strdup(getVariableName()));
        int ret = node->write(f, indent);
        node->unref();
        return ret;
    }
    return Node::write(f, indent);
}

int             
MeshBasedNode::writeXml(int f, int indent, int containerField) 
{
    int flags = m_scene->getWriteFlags();
    if (flags & X3D_4_WONDERLAND) {
        updateMesh(); 

        MFFloat *mfcolors;
        if (m_isDoubleMesh) {
           if (!m_meshDouble) return -1;
           mfcolors = m_meshDouble->getColors();
        } else {
           if (!m_mesh) return -1;
           mfcolors = m_mesh->getColors();
        }
        MFColor *colors = NULL;
        if (mfcolors != NULL) {
            float *fcolors = (float *)mfcolors->getValues();
            colors = new MFColor(fcolors, mfcolors->getSize());
        }
        bool hasColors = (colors != NULL); 
        Node *node = toIndexedFaceSet(false, false);
        NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet *)node;
        float color[3] = { 0.8, 0.8, 0.8 };
        NodeColor *ncolor = (NodeColor *)m_scene->createNode("Color");
        if ((hasColors && colors->getSFSize() == 1)) {
            const float *c = colors->getValue(0);
            for (int i = 0; i < 3; i++)
                color[i] = c[i];
        } else {
            if (hasParent()) {
                Node *parent = getParent();
                if ((parent->getType() == VRML_NURBS_GROUP) ||
                    (parent->getType() == X3D_NURBS_SET))
                    parent = getParent();
                if (parent->getType() == VRML_SHAPE) {
                    NodeShape *shape = (NodeShape *)parent;
                    NodeAppearance *nappearance = (NodeAppearance *)
                          shape->appearance()->getValue();
                    if (nappearance &&
                        (nappearance->getType() == VRML_APPEARANCE)) {
                        NodeMaterial *nmaterial = (NodeMaterial *) 
                              nappearance->material()->getValue();
                        if (nmaterial && 
                            (nmaterial->getType() == VRML_MATERIAL)) {
                            const float *c = 
                                   nmaterial->diffuseColor()->getValue();
                            for (int i = 0; i < 3; i++)
                                color[i] = c[i];
                        }
                    } 
                }
            }
        }    
        int numVertices;
        if (m_isDoubleMesh)
            numVertices = m_meshDouble->getVertices()->getSFSize();
        else 
            numVertices = m_mesh->getVertices()->getSFSize();
        int errorID = -1;
        bool validColors = false;
        if (m_isDoubleMesh ? m_meshDouble->colorPerVertex() :
                             m_mesh->colorPerVertex()) {
            if (hasColors) {
                if (numVertices == colors->getSFSize())
                    validColors = true;
                else
                    errorID = IDS_WRONG_COLOR_SIZE_IN_WONDERLAND;
            }
        } else if (hasColors)
            if (colors->getSFSize() > 1)
                errorID = IDS_NEED_COLOR_PER_VERTEX_IN_WONDERLAND;
        if (errorID != -1) {
            char string[256];
            swLoadString(errorID, string, 255);
            const char *name = NULL;
            if (hasName())
                name = getName();
            else
                name = getProto()->getName(true);
            swDebugf(string, name);
        }
        if (validColors)
            ncolor->color(colors);
        else {
            MFColor *uniqColours = new MFColor();
            for (int i = 0; i < numVertices; i++)
                uniqColours->insertSFValue(i, color);
            ncolor->color(uniqColours);
        }                    
        faceSet->color(new SFNode(ncolor));
        RET_ONERROR( node->Node::writeXml(f, indent) )
        node->unref();
        m_meshDirty = true;
    } else
        RET_ONERROR( Node::writeXml(f, indent) )
    return 0;
}

void
MeshBasedNode::addToConvertedNodes(int flags)
{ 
    if (m_convertedNodes.size() > 0)
        return;

    if (flags & TRIANGULATE) {
        bool shouldBeIndexedTriangleSet = shouldConvertToIndexedTriangleSet();
        bool shouldBeTriangleSet = shouldConvertToTriangleSet();

        int meshFlags = MESH_WANT_NORMAL | MESH_WANT_TEX_COORD;

        if (shouldBeTriangleSet) {
            Node *node = (NodeTriangleSet *)toTriangleSet(meshFlags);
            if (node != NULL) {
                node->setVariableName(strdup(getVariableName()));
                node->addParent(getParent(), getParentField());
                m_convertedNodes.append(node);
            }
            return;
        } else if (shouldBeIndexedTriangleSet) {
            Node *node = (NodeIndexedTriangleSet *)
                         toIndexedTriangleSet(meshFlags);
            if (node != NULL) {
                node->setVariableName(strdup(getVariableName()));
                node->addParent(getParent(), getParentField());
                m_convertedNodes.append(node);
            }
            return;
        }
        if (getType() == X3D_TRIANGLE_SET) {
            NodeTriangleSet *node = (NodeTriangleSet *)this;
            if (node->normal()->getValue() == NULL)
                createNormalFromMesh(node);
            if (node->texCoord()->getValue() == NULL)
                createTexCoordFromMesh(node);
        } else if (getType() == X3D_INDEXED_TRIANGLE_SET) {
            NodeIndexedTriangleSet *node = (NodeIndexedTriangleSet *)this;
            if (node->normal()->getValue() == NULL)
                createNormalFromMesh(node);
            if (node->texCoord()->getValue() == NULL)
                createTexCoordFromMesh(node);
        }   
    } else if (flags & (C_SOURCE | CC_SOURCE | JAVA_SOURCE)) {
        bool shouldBeIndexedFaceSet = (getType() != VRML_INDEXED_FACE_SET);

        int meshFlags = MESH_WANT_NORMAL | MESH_WANT_TEX_COORD;

        if (shouldBeIndexedFaceSet) {
            Node *node = (NodeIndexedFaceSet *)toIndexedFaceSet(meshFlags);
            if (node != NULL) {
                node->setVariableName(strdup(getVariableName()));
                node->addParent(getParent(), getParentField());
                m_convertedNodes.append(node);
            }
            return;
        }

        if (getType() == VRML_INDEXED_FACE_SET) {
            NodeIndexedFaceSet *node = (NodeIndexedFaceSet *)this;
            if (node->normal()->getValue() == NULL)
                createNormalFromMesh(node);
            if (node->texCoord()->getValue() == NULL)
                createTexCoordFromMesh(node);
        }
    }
    return; 
}       

#define writeAc3dRef(index) {                                            \
    int offset = face->getOffset() + index;                              \
    int ref = m_mesh->getCoordIndex()->getValue(offset);                 \
    float texCoordX = 0;                                                 \
    float texCoordY = 0;                                                 \
    int texRef = ref;                                                    \
    if (m_mesh->getTexCoordIndex() != NULL)                              \
        texRef =  m_mesh->getTexCoordIndex()->getValue(offset);          \
    if (texRef < texCoords->getSFSize()) {                               \
        texCoordX = texCoords->getValue(texRef)[0];                      \
        texCoordY = texCoords->getValue(texRef)[1];                      \
    }                                                                    \
    RET_ONERROR( mywritef(f, "%d %f %f\n", ref, texCoordX, texCoordY) )  \
}

int
MeshBasedNode::writeAc3d(int f, int indent)
{
    updateMesh(); 

    if (m_isDoubleMesh) {
        if (m_meshDouble == NULL)
            return 0;

        m_meshDouble->optimize();
    } else {
        if (m_mesh == NULL)
            return 0;

        m_mesh->optimize();
    }
    NodeColor *color = getColorNode();

    NodeMaterial *nmaterial = getMaterialNode();
    NodeImageTexture *nimageTexture = getImageTextureNode();
    NodeTextureTransform *ntextureTransfrom = getTextureTransformNode();

    RET_ONERROR( mywritestr(f, "OBJECT poly\n") )

    if (hasName())
        RET_ONERROR( mywritef(f, "name \"%s\"\n", (const char *)getName()) )
        
    if (nimageTexture) {
        const char *texture = nimageTexture->url()->getValue(0);
        if (texture) {
            URL url;
            url.FromPath(texture);
            MyString filename = rewriteURL(url,url.GetDir(),m_scene->getURL());
            char *file = strdup(filename);
            char *gifFile = (char *)malloc(strlen(file) + strlen(".gif") + 1);
            strcpy(gifFile, file);
#ifdef HAVE_IMAGE_CONVERTER
            bool convert = TheApp->GetAc3dExportConvert2Gif();
            if (convert) {                
                char *dotptr = strrchr(gifFile, '.');
                if (dotptr != NULL) {
                    // shorten file, if the dot looks like belonging to a extension
                    if (dotptr >= (gifFile - strlen(".jpeg")))   
                        *dotptr = (char)0;
                }
                strcat(gifFile, ".gif");
                int pid = fork();
                if (pid == 0) {
                    execl(HAVE_IMAGE_CONVERTER, HAVE_IMAGE_CONVERTER, 
                          file, gifFile, (char *)NULL);
                } else {
                   int status;
                   waitpid(-1, &status, 0);
                }
            }
#endif                    
            RET_ONERROR( mywritef(f, "texture \"%s\"\n", gifFile) )
            free(file);
            free(gifFile);
            if (ntextureTransfrom) {
                RET_ONERROR( mywritef(f, "texrep %f %f\n", 
                                      ntextureTransfrom->scale()->getValue(0),
                                      ntextureTransfrom->scale()->getValue(1)) )
                RET_ONERROR( mywritef(f, "texoff %f %f\n", 
                      ntextureTransfrom->translation()->getValue(0),
                      ntextureTransfrom->translation()->getValue(1)) )
                /* is there a secrect keyword for texture rotation ? 
                RET_ONERROR( mywritef(f, "???texrot??? %f\n", 
                                      ntextureTransfrom->rotation()->getValue)) )
                */
            }
        }
    }
    MFFloat *colors;
    MFInt32 *colorIndex;
    bool colorPerVertexProblem;
    colors = m_mesh->getColors();
    if (m_isDoubleMesh) {
        colorIndex = m_meshDouble->getColorIndex();
        colorPerVertexProblem = m_meshDouble->colorPerVertex();
    } else {
        colorIndex = m_mesh->getColorIndex();
        colorPerVertexProblem = m_mesh->colorPerVertex();
    }
    if (!colors || (colors && (colors->getSize() == 0)))
        if (colorPerVertexProblem)
            colorPerVertexProblem = false;
    if (colorPerVertexProblem) {
        const char *name = "";
        if (hasName())
            name = getName();
        else
            name = getProto()->getName(m_scene->isX3d());
        swDebugf("Warning: color node of %s node can not ", name);
        swDebugf("be exported to AC3D cause of colorPerVertex TRUE\n");
    }
    if (m_isDoubleMesh)
        RET_ONERROR( mywritef(f, "crease %f\n", 
                              RAD2DEG(m_meshDouble->creaseAngle())) )
    else
        RET_ONERROR( mywritef(f, "crease %f\n", RAD2DEG(m_mesh->creaseAngle()))
                   )
    MFVec3f *vertices = getVertices();
    RET_ONERROR( mywritef(f, "numvert %d\n", vertices->getSFSize()) )
    Matrix matrix;
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) matrix);
    for (int i = 0; i < vertices->getSFSize(); i++) {
        Vec3f v(vertices->getValue(i)[0],
                vertices->getValue(i)[1],
                vertices->getValue(i)[2]);
        v = matrix * v;
        RET_ONERROR( mywritef(f, "%f %f %f\n", -v.z, v.y, -v.x) )
    }
    if (m_isDoubleMesh)
        RET_ONERROR( mywritef(f, "numsurf %d\n", m_meshDouble->getNumFaces()) )
    else
        RET_ONERROR( mywritef(f, "numsurf %d\n", m_mesh->getNumFaces()) )
    int surfaceFlags = 0x00; // polygon
    surfaceFlags += (m_isDoubleMesh ? m_meshDouble->solid() : m_mesh->solid()) ?
                    1 << 4 : 1 << 5; // strange flat/smooth shading rules for the "virtual accoustics" program
    MFVec2f* texCoords = NULL;
    if (m_isDoubleMesh) {
        if (m_mesh->getTexCoords() == NULL)
            texCoords = m_meshDouble->generateTextureCoordinates();
        else
            texCoords = m_meshDouble->getTexCoords();
    } else {
        if (m_mesh->getTexCoords() == NULL)
            texCoords = m_mesh->generateTextureCoordinates();
        else
            texCoords = m_mesh->getTexCoords();
    }
    for (int i = 0; i < (m_isDoubleMesh ? m_meshDouble->getNumFaces() : 
                                          m_mesh->getNumFaces()); i++) {
        RET_ONERROR( mywritef(f, "SURF 0x%x\n", surfaceFlags) )
        int index = -1; 
        bool materialToWrite = true;
        if (!colorPerVertexProblem) {
            if (colorIndex && (colorIndex->getSize() > 0)) {
                if (i < colorIndex->getSize())
                    index = colorIndex->getValue(i);
            } else
                index = i;
            int materialIndex = 0;
            if ((index >= 0) && color)
                materialIndex = color->getAc3dMaterialIndex(index);
            if ((index >= 0) && colors && (index < colors->getSize()) && 
                (materialIndex > -1)) { 
                RET_ONERROR( mywritef(f, "mat %d\n", materialIndex) )
                materialToWrite = false;
            }
        }
        if (materialToWrite) {
            if (nmaterial)
                RET_ONERROR( mywritef(f, "mat %d\n", 
                                      nmaterial->getAc3dMaterialIndex()) )
            else
                RET_ONERROR( mywritef(f, "mat %d\n", 
                                      m_scene->getAc3dEmptyMaterial()) )
        }
        FaceData *face = m_mesh->getFace(i);
        RET_ONERROR( mywritef(f, "refs %d\n", face->getNumVertices()) )
        if (m_mesh->ccw())
            for (int j = 0; j < face->getNumVertices(); j++)
                writeAc3dRef(j)
        else
            for (int j = face->getNumVertices() - 1; j >= 0; j--)
                writeAc3dRef(j)
    }
    RET_ONERROR( mywritestr(f, "kids 0\n") )
    m_meshDirty = true;
    return 0;
}

#define COORD_INDEX 1
#define NORMAL_INDEX 2
#define COLOR_INDEX 3
#define TEX_COORD_INDEX 4

int
MeshBasedNode::writeRib(int f, int indent)
{
    updateMesh(); 

    if (m_isDoubleMesh) {
        if (m_meshDouble == NULL)
            return 0;
        if (!m_meshDouble->validMesh())
            return 0;
        if (m_meshDouble->getNumFaces() == 0)
            return 0;
    } else {
        if (m_mesh == NULL)
            return 0;
        if (!m_mesh->validMesh())
            return 0;
        if (m_mesh->getNumFaces() == 0)
            return 0;
    }

    NodeMaterial *nmaterial = getMaterialNode();
    NodeImageTexture *nimageTexture = getImageTextureNode();
    NodeTextureTransform *ntextureTransfrom = getTextureTransformNode();

    RET_ONERROR( mywritestr(f, "AttributeBegin\n") )

    float specDefault[3] = { 0, 0, 0 };
    float *specularColor = specDefault;
    if (nimageTexture)
        RET_ONERROR( mywritestr(f, "Color [1 1 1]\n") )
    else if (nmaterial) {
        const float *c = nmaterial->diffuseColor()->getValue();
        RET_ONERROR( mywritef(f, "Color [%f %f %f]\n", c[0], c[1], c[2]) )
        specularColor = (float *)nmaterial->specularColor()->getValue();
    } else 
        RET_ONERROR( mywritestr(f, "Color [0.8 0.8 0.8]\n") )

    if (hasName())
        RET_ONERROR( mywritef(f, "# name \"%s\"\n", (const char *)getName()) )
        
    if (nimageTexture) {
        const char *texture = nimageTexture->url()->getValue(0);
        if (texture) {
            URL url(TheApp->getImportURL(), texture);
            MyString filename = "";
            filename += url.ToPath();
            
            URL writeURL(TheApp->getWriteUrl());

            bool validTexturer = false;
            MyString texfile = "";
            MyString storedTxFile;
            storedTxFile += m_scene->getRibTexureFile(url.GetPath());
            if (storedTxFile.length() > 0) {
                texfile += storedTxFile;
                validTexturer = true;
            } else {
                if (writeURL.GetDir().length() > 0) {
                    texfile += writeURL.GetDir();
                    texfile += "/";
                }
                texfile += url.GetFileNameWithoutExtension();
                texfile += ".tx";

            MyString tiffile = "";
#ifdef _WIN32
            tiffile += "c:\\windows\\temp\\";
#else
            if (writeURL.GetDir().length() > 0) {
                tiffile += writeURL.GetDir();
                tiffile += "/";
            }
#endif
            tiffile += url.GetFileNameWithoutExtension();
            tiffile += ".tif";

            MyString converterCommand;
            converterCommand += TheApp->GetImageConverter();
            bool validConverter = converterCommand.length() > 0;
            converterCommand += " ";
            converterCommand += filename;
            converterCommand += " ";
            converterCommand += tiffile;
            if (strchr(converterCommand, '`') != NULL)
                validConverter = false;
            if (validConverter) {
                if (system(converterCommand) != 0)
                    validConverter = false;
            }

            if (validConverter) {
                MyString texturerCommand = "";;
                texturerCommand += TheApp->GetRibTexturer();
                if (texturerCommand.length() == 0) {
#ifdef HAVE_RIB_TEXTURER
                    texturerCommand += HAVE_RIB_TEXTURER;
#endif
                }
                validTexturer = (texturerCommand.length() > 0);
                texturerCommand += " ";
                texturerCommand += tiffile;
                texturerCommand += " ";
                texturerCommand += texfile;
                if (strchr(texturerCommand, '>') != NULL)
                    validTexturer = false;
#ifndef _WIN32
                texturerCommand += " > /dev/null";
#endif
                if (strchr(texturerCommand, '`') != NULL)
                    validTexturer = false;
                if (validTexturer) {
                    if (system(texturerCommand) != 0)
                        validTexturer = false;
                    swRemoveFile(tiffile);
                }
             }

             }
             if (validTexturer) {
                 m_scene->addRibTexureFile(url.GetPath(), texfile);

                 RET_ONERROR( mywritestr(f, "Surface \"paintedplastic\" ") )
                 RET_ONERROR( mywritestr(f, "\"Kd\" [ 1 ] \"Ks\" [ 1 ]") )
                 RET_ONERROR( mywritef(f, " \"specularcolor\" [%f %f %f] ",
                                       specularColor[0],
                                       specularColor[1],
                                       specularColor[2]) )
                 RET_ONERROR( mywritef(f, "\"texturename\" [\"%s\"]\n", 
                                       (const char *)texfile) ) 
             } else
                 swDebugf("RIB texturer not found (or texture url invalid)\n");
/*
            if (ntextureTransfrom) {
                RET_ONERROR( mywritef(f, "texrep %f %f\n", 
                                      ntextureTransfrom->scale()->getValue(0),
                                      ntextureTransfrom->scale()->getValue(1)) )
                RET_ONERROR( mywritef(f, "texoff %f %f\n", 
                      ntextureTransfrom->translation()->getValue(0),
                      ntextureTransfrom->translation()->getValue(1)) )
              }
*/
            if (ntextureTransfrom)
                RET_ONERROR( mywritestr(f, "# warning: TextureTransfrom unsupported\n") )
        }
    }
    MFVec3f *vertices = getVertices();
    if (vertices == NULL) // bug in mesh hidden in internal node ? 
        return 0;
    MFInt32 *coordIndex = getCoordIndexFromMesh();
    const int *ci = coordIndex->getValues();
    const float *colors = NULL;
    int numColors = 0;
    MFInt32 *colorIndex;
    bool colorPerVertex;
    MFInt32 *texCoordIndex;
    if (m_isDoubleMesh) {
        if (m_mesh->getColors())
            colors = m_meshDouble->getColors()->getValues();
        numColors = m_meshDouble->getColors()->getSFSize();
        colorIndex = m_meshDouble->getColorIndex();
        colorPerVertex = m_meshDouble->colorPerVertex();
        texCoordIndex = m_meshDouble->getTexCoordIndex();
    } else {
        if (m_mesh->getColors())
            colors = m_mesh->getColors()->getValues();
        numColors = m_mesh->getColors()->getSFSize();
        colorIndex = m_mesh->getColorIndex();
        colorPerVertex = m_mesh->colorPerVertex();
        texCoordIndex = m_mesh->getTexCoordIndex();
    }
    if ((colorIndex == NULL) || (colorIndex->getSize() == 0))
        colorIndex = coordIndex;
    if (texCoordIndex == NULL)
        texCoordIndex = coordIndex;

    MyArray<int> indexArray;
    // search longest *Index
    int numIndex = COORD_INDEX;
    int numCoordIndices = 0;
    MFInt32 *normalIndex = m_mesh->getNormalIndex();
    if ((normalIndex == NULL) || (normalIndex->getSize() == 0))
        normalIndex = coordIndex;
    bool normalPerVertex;
    if (m_isDoubleMesh)
        normalPerVertex = m_meshDouble->normalPerVertex();
    else
        normalPerVertex = m_mesh->normalPerVertex();
    int numNormalIndices = 0;
    for (int i = 0; i < normalIndex->getSize(); i++)
        if (normalIndex->getValue(i) > -1)
            numNormalIndices++;
    if (numNormalIndices > numCoordIndices)
        numIndex = NORMAL_INDEX;
    int numColorIndices = 0;
    for (int i = 0; i < colorIndex->getSize(); i++)
        if (colorIndex->getValue(i) > -1)
            numColorIndices++;
    if ((numColorIndices > numCoordIndices) &&
        (numColorIndices > numNormalIndices))
        numIndex = COLOR_INDEX;
    int numTexCoordIndices = 0;
    for (int i = 0; i < texCoordIndex->getSize(); i++)
        if (texCoordIndex->getValue(i) > -1)
            numTexCoordIndices++;
    if ((numTexCoordIndices > numCoordIndices) &&
        (numTexCoordIndices > numNormalIndices) &&
        (numTexCoordIndices > numColorIndices))
        numIndex = TEX_COORD_INDEX;
    switch (numIndex) {
      case COLOR_INDEX:
        for (int i = 0; i < colorIndex->getSize(); i++)
            indexArray.append(colorIndex->getValue(i));
        break;
      case NORMAL_INDEX:
        for (int i = 0; i < normalIndex->getSize(); i++)
            indexArray.append(normalIndex->getValue(i));
        break;
      case TEX_COORD_INDEX:
        for (int i = 0; i < texCoordIndex->getSize(); i++)
            indexArray.append(texCoordIndex->getValue(i));
        break;
      default:
        for (int i = 0; i < coordIndex->getSize(); i++)
            indexArray.append(coordIndex->getValue(i));
    }

    RET_ONERROR( mywritef(f, "PointsPolygons [\n") )
    int numVertices = 0;
    for (size_t i = 0; i < indexArray.size(); i++) {
        if (indexArray[i] > -1)
            numVertices++;
        else {
            RET_ONERROR( mywritef(f, "%d\n", numVertices) )
            numVertices = 0;
        } 
    }    
    RET_ONERROR( mywritef(f, "]\n") )
    RET_ONERROR( mywritef(f, "[\n") )
    int numCi = 0;
    int numFaces;
    if (m_isDoubleMesh)
        numFaces = m_meshDouble->getNumFaces();
    else
        numFaces = m_mesh->getNumFaces();
    for (int i = 0; i < numFaces; i++) {
        FaceData *face;
        if (m_isDoubleMesh)
            face = m_meshDouble->getFace(i);
        else
            face = m_mesh->getFace(i);
        int numVertices = face->getNumVertices();
        int offset = face->getOffset();
        for (int j = offset; j < offset + numVertices; j++) {
            int k = ci[j];
            if (k > -1) {
                RET_ONERROR( mywritef(f, "%d\n", numCi++) )
            }
        }
    }
    RET_ONERROR( mywritef(f, "]\n") )

    Matrix transformMatrix;
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) transformMatrix);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    Matrix matrix;
    Matrix viewpointMatrix;
    Node *viewpoint = m_scene->getCamera();
    viewpoint->apply();
    viewpoint->getMatrix(viewpointMatrix);
    matrix = viewpointMatrix * transformMatrix;
    glPopMatrix();

    if (m_isDoubleMesh)
        numFaces = m_meshDouble->getNumFaces();
    else
        numFaces = m_mesh->getNumFaces();
    RET_ONERROR( mywritef(f, "\"P\" [\n") )
    for (int i = 0; i < numFaces; i++) {
        FaceData *face;
        if (m_isDoubleMesh)
            face = m_meshDouble->getFace(i);
        else
            face = m_mesh->getFace(i);
        int numVertices = face->getNumVertices();
        int offset = face->getOffset();
        for (int j = offset; j < offset + numVertices; j++) {
            int k = ci[j];
            if (k > -1) {
                Vec3f v;
                v.x = vertices->getValue(k)[0];
                v.y = vertices->getValue(k)[1];
                v.z = vertices->getValue(k)[2];
                v = matrix * v;
                RET_ONERROR( mywritef(f, "%f %f %f\n", v.x, v.y, -v.z) )
            }
        }
    }
    RET_ONERROR( mywritef(f, "]\n") )

    RET_ONERROR( mywritef(f, "\"N\" [\n") )
    if (!normalPerVertex) {
        int numFaces = 0;
        int numFace;
        if (m_isDoubleMesh)
            numFace = m_meshDouble->getNumFaces();
        else
           numFace = m_mesh->getNumFaces();
        for (int i = 0; i < numFace; i++) {
            FaceData *face;
            if (m_isDoubleMesh)
                face = m_meshDouble->getFace(i);
            else
                face = m_mesh->getFace(i);
            int numVertices = face->getNumVertices();
            int offset = face->getOffset();
            for (int j = offset; j < offset + numVertices; j++) {
                int k = numFaces * 3;
                if (m_isDoubleMesh)
                    RET_ONERROR( mywritef(f, "%f %f %f\n", 
                                    m_meshDouble->getNormals()->getValue(k)[0], 
                                    m_meshDouble->getNormals()->getValue(k)[1], 
                                    m_meshDouble->getNormals()->getValue(k)[2])
                               )
                else
                    RET_ONERROR( mywritef(f, "%f %f %f\n", 
                                          m_mesh->getNormals()->getValue(k)[0], 
                                          m_mesh->getNormals()->getValue(k)[1], 
                                          m_mesh->getNormals()->getValue(k)[2])
                                ) 
            }
            numFaces++;
        }
    } else
        for (int i = 0; i < normalIndex->getSize(); i++) {
            int j = normalIndex->getValue(i);
            if (j > -1)  {
                Vec3f norm;
                if (m_isDoubleMesh) {
                    norm.x = m_meshDouble->getNormals()->getValue(j)[0];   
                    norm.y = m_meshDouble->getNormals()->getValue(j)[1];   
                    norm.z = m_meshDouble->getNormals()->getValue(j)[2];   
                } else {
                    norm.x = m_mesh->getNormals()->getValue(j)[0];   
                    norm.y = m_mesh->getNormals()->getValue(j)[1];   
                    norm.z = m_mesh->getNormals()->getValue(j)[2];   
                }
                RET_ONERROR( mywritef(f, "%f %f %f\n", norm.x, norm.y, norm.z) )
            }
        }
    RET_ONERROR( mywritef(f, "]\n") )

    if (numColors > 0) {
        RET_ONERROR( mywritef(f, "\"Cs\" [\n") )
        int coffset = 3;
        if (m_mesh->hasColorRGBA())
            coffset = 4;
        if (!colorPerVertex) {
            int numFaces = 0;
            int numFace;
            if (m_isDoubleMesh)
                numFace = m_meshDouble->getNumFaces();
            else
                numFace = m_mesh->getNumFaces();

            for (int i = 0; i < numFace; i++) {
                FaceData *face;
                if (m_isDoubleMesh)
                    face = m_meshDouble->getFace(i);
                else
                    face = m_mesh->getFace(i);
                int numVertices = face->getNumVertices();
                int offset = face->getOffset();
                for (int j = offset; j < offset + numVertices; j++) {
                    int k = numFaces * coffset;
                    RET_ONERROR( mywritef(f, "%f %f %f\n", 
                                          colors[k], 
                                          colors[k + 1], 
                                          colors[k + 2]) )
                }
                numFaces++;
            }
        } else
            for (int i = 0; i < colorIndex->getSize(); i++) {
                int j = colorIndex->getValue(i) * coffset;
                if (j > -1) {
                    RET_ONERROR( mywritef(f, "%f %f %f\n", 
                                          colors[j], 
                                          colors[j + 1], 
                                          colors[j + 2]) )
            }
        }
        RET_ONERROR( mywritef(f, "]\n") )
      
        if (m_mesh->hasColorRGBA()) {
            RET_ONERROR( mywritef(f, "\"Os\" [\n") )
            if (!colorPerVertex) {
                int numFaces = 0;
                int numFace;
                if (m_isDoubleMesh)
                    numFace = m_meshDouble->getNumFaces();
                else
                    numFace = m_mesh->getNumFaces();
                for (int i = 0; i < numFace; i++) {
                    FaceData *face;
                    if (m_isDoubleMesh)
                        face = m_meshDouble->getFace(i);
                    else
                        face = m_mesh->getFace(i);
                    int numVertices = face->getNumVertices();
                    int offset = face->getOffset();
                    for (int j = offset; j < offset + numVertices; j++) {
                        int k = numFaces * coffset + 3;
                        RET_ONERROR( mywritef(f, "%f %f %f\n", 
                                              colors[k], 
                                              colors[k], 
                                              colors[k]) )
                    }
                    numFaces++;
                }
            } else
                for (int i = 0; i < colorIndex->getSize(); i++) {
                    int j = colorIndex->getValue(i);
                    if (j > -1) {
                        j = j * coffset + 3;
                        RET_ONERROR( mywritef(f, "%f %f %f\n", 
                                              colors[j], colors[j], colors[j]) )
                    }
                }
            RET_ONERROR( mywritef(f, "]\n") )
        }
    }
    MFVec2f* texCoords;
    if (m_isDoubleMesh)
        texCoords = m_meshDouble->getTexCoords();
    else
        texCoords = m_mesh->getTexCoords();
    RET_ONERROR( mywritef(f, "\"st\" [\n") )
    for (int i = 0; i < texCoordIndex->getSFSize(); i++) {
        int j = texCoordIndex->getValue(i);
        if (j > -1) {
            float texCoordX = texCoords->getValue(j)[0];
            float texCoordY = 1 - texCoords->getValue(j)[1];
            RET_ONERROR( mywritef(f, "%f %f\n", texCoordX, texCoordY) )
        }
    }
    RET_ONERROR( mywritef(f, "]\n") )

    RET_ONERROR( mywritestr(f, "AttributeEnd\n") )

    return 0;
}

#define UNKNOWN_MATERIAL "TOTREF"

int
MeshBasedNode::writeCattGeo(int filedes, int indent)
{ 
    const char *path = TheApp->GetCattExportPath();
    int len = strlen(path)+256;
    char *filename = new char[len];
    mysnprintf(filename, len - 1, "%sShape`%d.geo", path,
               m_scene->getAndIncCattGeoFileCounter());
    int f = open(filename, O_WRONLY | O_CREAT, 00666);
    if (f == -1)
        return -1;
    delete [] filename;

    updateMesh(); 

    if (m_isDoubleMesh) {
        if (m_meshDouble == NULL) 
            return 0;

        m_meshDouble->optimize();
    } else {
        if (m_mesh == NULL) 
            return 0;

        m_mesh->optimize();
    }
    NodeMaterial *nmaterial = getMaterialNode();
   
    RET_ONERROR( mywritestr(f, "OBJECT") )
    RET_ONERROR( mywritestr(f, swGetLinefeedString()) )

    if (hasName())
        RET_ONERROR( mywritef(f, "; Shape with vrml97 DEF name %s: ", 
                              (const char *)getName()) )
    RET_ONERROR( mywritestr(f, swGetLinefeedString()) )

    RET_ONERROR( mywritestr(f, "CORNERS") )
    RET_ONERROR( mywritestr(f, swGetLinefeedString()) )
    MFVec3f *vertices = getVertices();
    Matrix matrix;
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) matrix);
    int cornerCounter = m_scene->getCattGeoCornerCounter();
    for (int i = 0; i < vertices->getSFSize(); i++) {
        Vec3f v(vertices->getValue(i)[0], 
                vertices->getValue(i)[1], 
                vertices->getValue(i)[2]);
        v = matrix * v;        
        RET_ONERROR( mywritef(f, "%d %f %f %f%s", 
                              m_scene->getAndIncCattGeoCornerCounter(),
                              v.x, v.z, v.y, swGetLinefeedString()) )
    }

    RET_ONERROR( mywritestr(f, "PLANES") )
    RET_ONERROR( mywritestr(f, swGetLinefeedString()) )
    bool meshCcw;
    if (m_isDoubleMesh)
        meshCcw = m_meshDouble->ccw();
    else
        meshCcw = m_mesh->ccw();
    // write both sides, if solid is false
    int sides;
    if (m_isDoubleMesh)
        sides = m_meshDouble->solid() ? 1 : 2;
    else
        sides = m_mesh->solid() ? 1 : 2;
    for (int side = 1; side < (sides + 1); side++) {
        if (side == 2)
            meshCcw = !meshCcw;
        int numFaces;
        if (m_isDoubleMesh)
            numFaces = m_meshDouble->getNumFaces();
        else
            numFaces = m_mesh->getNumFaces();

        for (int i = 0; i < numFaces; i++) {
            FaceData *face;
            if (m_isDoubleMesh)
                face = m_meshDouble->getFace(i);
            else
                face = m_mesh->getFace(i);
            RET_ONERROR( mywritef(f, "[%d ", 
                                  m_scene->getAndIncCattGeoPlaneCounter()) )
            if (nmaterial)
                RET_ONERROR( mywritestr(f, nmaterial->getExportMaterialName(
                      UNKNOWN_MATERIAL)) )
            else
                RET_ONERROR( mywritestr(f, UNKNOWN_MATERIAL) )
            RET_ONERROR( mywritestr(f, " / ") )
            MFInt32 *mfcoordIndex;
            if (m_isDoubleMesh)
                mfcoordIndex = m_meshDouble->getCoordIndex();
            else
                mfcoordIndex = m_mesh->getCoordIndex();
            if (meshCcw)
                for (int j = face->getNumVertices() - 1; j >= 0; j--) {
                    int ref = mfcoordIndex->getValue(face->getOffset() + j);
                    RET_ONERROR( mywritef(f, "%d ", ref + cornerCounter) )
                }
            else 
                for (int j = 0; j < face->getNumVertices(); j++) {
                    int ref = mfcoordIndex->getValue(face->getOffset() + j);
                    RET_ONERROR( mywritef(f, "%d ", ref + cornerCounter) )
                }
            RET_ONERROR( mywritestr(f, "/ ") )
            if (nmaterial)
                RET_ONERROR( mywritestr(f, nmaterial->getExportMaterialName(
                      UNKNOWN_MATERIAL)) )
            else
                RET_ONERROR( mywritestr(f, UNKNOWN_MATERIAL) )
            RET_ONERROR( mywritestr(f, " ]") )
            RET_ONERROR( mywritestr(f, swGetLinefeedString()) )
        }            
    }

    m_meshDirty = true;
    return swTruncateClose(f);
}

int
MeshBasedNode::writeLdrawDat(int f, int indent)
{
    updateMesh(); 

    if (m_isDoubleMesh) {
        if (m_meshDouble == NULL)
           return 0;

        if (m_meshDouble->getMaxNumberEdgesPerFace() > 4)
            m_meshDouble->triangulate(NULL);
    } else {
        if (m_mesh == NULL)
           return 0;

        if (m_mesh->getMaxNumberEdgesPerFace() > 4)
            m_mesh->triangulate(NULL);
    }
    MFVec3f *vertices = getVertices();
    MFInt32 *mfcoordIndex;
    if (m_isDoubleMesh)
        mfcoordIndex = m_meshDouble->getCoordIndex();
    else
        mfcoordIndex = m_mesh->getCoordIndex();
    Matrix matrix;
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) matrix);
    RET_ONERROR( mywritestr(f, "0 //") )
    if (hasName())
        RET_ONERROR( mywritef(f, " %s", (const char *)getName()))
    RET_ONERROR( mywritef(f, " %s\n", (const char *)getProto()->getName(false)))
    int numFaces;
    if (m_isDoubleMesh)
        numFaces = m_meshDouble->getNumFaces();
    else
        numFaces = m_mesh->getNumFaces();
    for (int i = 0; i < numFaces; i++) {
        FaceData *face;
        if (m_isDoubleMesh)
            face = m_meshDouble->getFace(i);
        else
            face = m_mesh->getFace(i);
        if (face->getNumVertices() == 4) 
            RET_ONERROR( mywritestr(f, "4 ") )
        else if (face->getNumVertices() == 3)
            RET_ONERROR( mywritestr(f, "3 ") )
        else 
            continue;
        int color = m_scene->getCurrentLdrawColor();
        if (color == -1)
            color = LDRAW_CURRENT_COLOR;
        else {
            Node *node = NULL;
            if (getType() == VRML_ELEVATION_GRID)
                node = ((NodeElevationGrid *)this)->color()->getValue();
            else if (getType() == VRML_GEO_ELEVATION_GRID)
                node = ((NodeGeoElevationGrid *)this)->color()->getValue();
            else if (getType() == VRML_INDEXED_FACE_SET)
                node = ((NodeIndexedFaceSet *)this)->color()->getValue();
            NodeColor *colorNode = (NodeColor *)node;

            MFFloat *colors;
            if (m_isDoubleMesh)
                colors =  m_meshDouble->getColors();
            else
                colors = m_mesh->getColors();
            MFInt32 *colorIndex;
            if (m_isDoubleMesh)
                colorIndex = m_meshDouble->getColorIndex();
            else
                colorIndex = m_mesh->getColorIndex();
            bool colorPerVertexProblem;
            if (m_isDoubleMesh)
                colorPerVertexProblem = m_meshDouble->colorPerVertex();
            else
                colorPerVertexProblem = m_mesh->colorPerVertex();
            if (!colors || (colors && (colors->getSize() == 0)))
                if (colorPerVertexProblem)
                    colorPerVertexProblem = false;
            int index = -1; 
            if (colorPerVertexProblem) {
                const char *name = "";
                if (hasName())
                    name = getName();
                else
                    name = getProto()->getName(m_scene->isX3d());

                swDebugf("Warning: color node of %s node can not", name);
                swDebugf("be exported to Ldraw cause of colorPerVertex");
                swDebugf(" TRUE\n");
            }
            else if (colorIndex && (colorIndex->getSize() > 0)) {
                if (i < colorIndex->getSize())
                    index = colorIndex->getValue(i);
                else
                    index = i;
                if ((index >= 0) && colorNode)
                    color = colorNode->getLdrawColor(index);
                if (color < 0)
                    color = LDRAW_CURRENT_COLOR;
            }
        }
        RET_ONERROR( mywritef(f, "%d ", color) )
        for (int j = 0; j < face->getNumVertices(); j++) {
             int ref = mfcoordIndex->getValue(face->getOffset() + j);
             Vec3f v(vertices->getValue(ref)[0],
                     vertices->getValue(ref)[1],
                     vertices->getValue(ref)[2]);
             v = matrix * v;
             RET_ONERROR( mywritef(f, " %f %f %f",  v.z * LDRAW_SCALE, 
                                                   -v.y * LDRAW_SCALE, 
                                                    v.x * LDRAW_SCALE) )
        }
        RET_ONERROR( mywritestr(f, "\n") )
    }
    m_meshDirty = true;
    return 0;
}

void
MeshBased::smoothNormals(void)
{
    updateMesh(); 

    if (m_isDoubleMesh) {
        if (m_meshDouble == NULL)
           return;
        m_meshDouble->smoothNormals();
    } else {
        if (m_mesh == NULL)
           return;
        m_mesh->smoothNormals();
   }
}

void
MeshBasedNode::flip(int index)
{
    switch (getType()) {
      case VRML_BOX:
      case VRML_CYLINDER:
      case VRML_SPHERE:
        return;
      default:
        {
        Node *faceSet = m_scene->convertToIndexedFaceSet(this);
        if (faceSet != NULL)
            faceSet->flip(index);
        }
     }
}

void
MeshBasedNode::swap(int fromTo)
{
    Node *faceSet = m_scene->convertToIndexedFaceSet(this);
    if (faceSet != NULL)
        faceSet->swap(fromTo);
}

void            
MeshBased::setCreaseAngle(float angle)
{ 
    if (m_mesh)
        m_mesh->setCreaseAngle(angle); 
}

void
MeshBasedNode::optimizeMesh(MFVec3f **newCoord, MFInt32 **newCoordIndex,
                            float epsilon)
{
    updateMesh(); 

    if (m_mesh == NULL)
        return;
    m_mesh->optimize(epsilon);
    *newCoord = m_mesh->getVertices();
    *newCoordIndex = m_mesh->getCoordIndex();
}

MyMesh *           
MeshBasedNode::triangulateMesh(void)
{
    updateMesh(); 

    if (m_mesh == NULL)
        return NULL;

#ifdef HAVE_GLUNEWTESS
    return (MyMesh *)m_mesh->triangulate(this);
#else
    if (m_mesh->getMaxNumberEdgesPerFace() <= 4) {
        m_mesh->simpleQuadTriangulate();
        return m_mesh;
    }
    return NULL;
#endif
}

MyMesh *           
MeshBasedNode::simpleQuadTriangulateMesh(void)
{
    updateMesh(); 

    if (m_mesh == NULL)
        return NULL;

    if (m_mesh->getMaxNumberEdgesPerFace() <= 4) {
        m_mesh->simpleQuadTriangulate();
        return m_mesh;
    }
    return m_mesh;
}

MFVec3f *
MeshBased::getCoordFromMesh()
{ 
    updateMesh(); 

    if (m_mesh == NULL)
        return NULL;
    return m_mesh->getVertices(); 
}

MFInt32 *
MeshBased::getCoordIndexFromMesh()
{ 
    updateMesh(); 

    if (m_mesh == NULL)
        return NULL;
    return m_mesh->getCoordIndex(); 
}

bool
MeshBased::isFlat(void)
{ 
    updateMesh(); 

    if (m_mesh == NULL)
        return false;
    return (m_mesh->creaseAngle() == 0.0f);
}

bool       
MeshBasedNode::canSimpleTriangulate(void)
{
    updateMesh(); 

    if (m_mesh == NULL)
        return false;
    // is simpleQuadTriangulate() possible ?
    if (m_mesh->getMaxNumberEdgesPerFace() <= 4)
        return true;
    return false; 
}

bool    
MeshBasedNode::canConvertToTriangleSet(void) 
#ifdef HAVE_GLUNEWTESS
{ 
    return hasParent(); 
}
#else
{ 
    return hasParent() && canSimpleTriangulate();
}
#endif

int             
MeshBasedNode::getTextureUnits(void)
{
    Node *node = getParent();
    if (node && node->getType() == VRML_SHAPE) {
        NodeShape *nshape = (NodeShape *) node;
        node = nshape->appearance()->getValue();
        if (node && node->getType() == VRML_APPEARANCE) {
            NodeAppearance *nappearance = (NodeAppearance *) node;
            Node *ntexture = nappearance->texture() ?
                             nappearance->texture()->getValue() :
                             NULL;
            if (ntexture && ntexture->getType() == X3D_MULTI_TEXTURE) {
                NodeMultiTexture *multiTex = (NodeMultiTexture *)ntexture;
                return multiTex->texture()->getSize();
            } else
               return 0;
        }
    }
    return 0;
}

NodeIndexedFaceSet *
MeshBasedNode::subdivide(void)
{
    NodeIndexedFaceSet *faceset = NULL;
    if (getType() == VRML_INDEXED_FACE_SET)
        faceset = (NodeIndexedFaceSet *)this;
    else {
        faceset = (NodeIndexedFaceSet *) m_scene->createNode("IndexedFaceSet");
        NodeCoordinate *coordinate = (NodeCoordinate *)
                                     m_scene->createNode("Coordinate");
        faceset->coord(new SFNode(coordinate));
    }
    if (meshSubdivide(faceset, this->getMesh()))
        return faceset;
    return NULL;
}

void            
MeshBasedNode::reInit(void)
{
    m_mesh = NULL; 
    m_meshDirty = true; 
    Node::reInit();
}


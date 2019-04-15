/*
 * MeshBasedNode.h
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

#ifndef _MESH_BASED_NODE_H
#define _MESH_BASED_NODE_H

#ifndef GEOMETRY_NODE_H
#include "GeometryNode.h"
#endif
#ifndef _VEC3F
# include "Vec3f.h"
#endif

#include "MeshFlags.h"

class Scene;
class MFVec3f;
class MFVec2f;
class MFInt32;
class MyMesh;
class MyMeshDouble;
class FaceData;
class NodeIndexedFaceSet;

class MeshBased {
public:
    void            updateMesh(void);
    
    virtual void    meshDraw();
    virtual void    drawNormals();

    void            setCreaseAngle(float angle);

    MFVec3f        *getVertices(void);
    MFVec2f        *getTextureCoordinates();
    MFInt32        *getTexCoordIndex(); 

    MFInt32        *getCoordIndexFromMesh(void);
    MFVec3f        *getCoordFromMesh(void);

    virtual void    createMesh(bool cleanDoubleVertices = true,
                               bool triangulateMesh = true) = 0;

    void            smoothNormals(void);
    virtual bool    isFlat(void);

    void            optimizeMesh(MFVec3f **newCoord, MFInt32 **newCoordIndex,
                                 float epsilon = 0.0f);

    MFVec3f        *getSmoothNormals(void);
    MFInt32        *getSmoothNormalIndex(void);

    void            setDoubleMesh(bool flag) { m_isDoubleMesh = flag; }
    bool            getDoubleMesh(void) { return m_isDoubleMesh; }

    bool            meshDirty(void) 
                        {
                        if (m_isDoubleMesh)
                            return (m_meshDouble == NULL) || m_meshDirty; 
                        return (m_mesh == NULL) || m_meshDirty; 
                        }

protected:
    MyMesh         *m_mesh;
    MyMeshDouble   *m_meshDouble;
    bool            m_isDoubleMesh;  
    bool            m_meshDirty;
};

class MeshBasedNode : public GeometryNode , public MeshBased {
public:
                    MeshBasedNode(Scene *scene, Proto *proto);
    virtual        ~MeshBasedNode();

    virtual bool    isMeshBasedNode(void) { return true; }

    int             optimizeNormals(int *coordIndex, Vec3f *vertices,
                                    Vec3f *normals, int index, bool ccw, 
                                    bool cleanDoubleVertices = true);
    int             optimizeNormals(int *coordIndex, MFVec3f *vertices,
                                    MFVec3f *normals, int index, bool ccw, 
                                    bool cleanDoubleVertices = true);
    virtual Node   *toIndexedFaceSet(int meshFlags = MESH_WANT_NORMAL,
                                     bool cleanVertices = true,
                                     bool triangulate = true);
    virtual bool    canConvertToIndexedFaceSet(void) { return hasParent(); }

    virtual bool    canSimpleTriangulate(void);

    virtual Node   *toIndexedTriangleSet(int meshFlags = MESH_TARGET_HAS_CCW);

    virtual bool    canConvertToIndexedTriangleSet(void) { return hasParent(); }
    // currently, the optimization of the convert to IndexedTriangleSet 
    // would break normals, color and TextureCoordinates
    virtual bool    shouldConvertToIndexedTriangleSet(void) { return false; }
    virtual bool    optimizeOnConvertionToIndexedTriangleSet(void) 
                       { return true; }

    virtual Node   *alreadyTriangulatedToIndexedTriangleSet(bool hasCcw = true);

    virtual bool    hasColor(void);
    virtual bool    hasColorRGBA(void);
    virtual bool    hasColorPerVertex(void);

    virtual Node   *toTriangleSet(int meshFlags = MESH_TARGET_HAS_CCW);
    virtual bool    canConvertToTriangleSet(void);

    virtual bool    isInvalidChildNode(void) { return true; }
    virtual bool    hasBoundingBox(void) { return true; }
    virtual Vec3f   getMinBoundingBox(void);
    virtual Vec3f   getMaxBoundingBox(void);

    virtual void    setTexCoordFromMesh(Node *ntexCoord);

    void            update(void) { m_meshDirty = true; }
    void            reInit(void);

    virtual void    flip(int index);
    virtual void    swap(int fromTo);

    virtual int     countPolygons(void);
    virtual int     countPolygons1Sided(void);
    virtual int     countPolygons2Sided(void);

    virtual int     write(int filedes, int indent);
    virtual int     writeXml(int filedes, int indent, int containerField = -1);

    virtual void    addToConvertedNodes(int writeFlags);

    virtual bool    canWriteAc3d() { return true; }
    virtual int     writeAc3d(int filedes, int indent);

    virtual int     writeRib(int filedes, int indent);

    virtual bool    canWriteCattGeo() { return true; }
    virtual int     writeCattGeo(int filedes, int indent);

    virtual bool    canWriteLdrawDat() { return true; }
    virtual int     writeLdrawDat(int filedes, int indent);

    void            optimizeMesh(MFVec3f **newCoord, MFInt32 **newCoordIndex,
                                 float epsilon = 0.0f);
    MyMesh         *triangulateMesh();
    MyMesh         *simpleQuadTriangulateMesh();

    int             getTextureUnits(void);    

    virtual void    setMeshDirty(void) { m_meshDirty = true; }

    NodeIndexedFaceSet *subdivide(void);

    virtual MyMesh *getMesh(void);
    virtual MyMeshDouble *getMeshDouble(void);
};

#endif // _MESH_BASED_NODE_H


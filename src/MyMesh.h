/*
 * MyMesh.h
 *
 * Copyright (C) 1999 Stephen F. White
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

#ifndef _MY_MESH_H
#define _MY_MESH_H

#ifndef _ARRAY_H
#include "Array.h"
#endif

#ifndef _LIST_H
#include "List.h"
#endif

#ifndef _VEC4F_H
#include "Vec4f.h"
#endif

#ifndef _VEC3F_H
#include "Vec3f.h"
#endif

#ifndef _VEC2F_H
#include "Vec2f.h"
#endif

#include "MeshBasedNode.h"

#ifndef FLT_MIN
# include <float.h>
#endif

#define floatEpsilon() (FLT_MIN * 5)

enum {
    MESH_CCW                = 1<<1,
    MESH_COLOR_PER_VERTEX   = 1<<2, 
    MESH_CONVEX             = 1<<3, 
    MESH_NORMAL_PER_VERTEX  = 1<<4, 
    MESH_SOLID              = 1<<5,
    MESH_COLOR_RGBA         = 1<<6, 
    MESH_SIMPLE_TRIANGULATE = 1<<7,
    MESH_TRIANGULATED       = 1<<8
};

class FaceData;
class MFFloat;
class MFVec3f;
class MFVec2f;
class MFInt32;

#ifdef HAVE_GLUNEWTESS
struct VertexInfo {
   GLdouble vertex[3];
   float normal[3];
   float color[4];
   float texCoord[2];
};
#endif

class MyLine  {
public: 
    int begin;
    int end;
};

class MyMesh {
public:
                        MyMesh(MeshBasedNode *that,
                               MFVec3f *vertices, MFInt32 *coordIndex,
                               MFVec3f *normals, MFInt32 *normalIndex,
                               MFFloat *colors, MFInt32 *colorIndex,
                               Array<MFVec2f *>texCoords, 
                               MFInt32 *texCoordIndex,
                               float creaseAngle, int meshFlags, 
                               float transparency = 0);
    virtual            ~MyMesh();
                
    MyMesh             *copy(void);

    void                draw() { draw(-1); }
    void                draw(int meshFlags);
    void                drawNormals(void);
    void                sort();

    bool                validMesh(void) { return m_vertices != NULL; }  

    MFVec3f            *getVertices(void)      { return m_vertices; }
    MFInt32            *getCoordIndex(void)    { return m_coordIndex; }
    MFFloat            *getColors(void)        { return m_colors; }
    MFInt32            *getColorIndex(void)    { return m_colorIndex; }
    MFVec3f            *getNormals(void)       { return m_normals; }
    MFInt32            *getNormalIndex(void)   { return m_normalIndex; }
    MFVec2f            *getTexCoords(void)     { return m_texCoords[0]; }
    MFInt32            *getTexCoordIndex(void) { return m_texCoordIndex; }
    int                 getTexCoordsSize(void) { return m_texCoords.size(); }
    MFVec2f            *getTexCoords(int i)    { return m_texCoords[i]; }
    float               creaseAngle()          { return m_creaseAngle; }
    bool                solid()                { return m_solid; }
    bool                ccw()                  { return m_ccw; }
    bool                convex()               { return m_convex; }
    bool                colorPerVertex()       { return m_colorPerVertex; }
    bool                normalPerVertex()      { return m_normalPerVertex; }

    void                addCoords(MFInt32 *coords, float *color, 
                                  MFFloat *colors, bool rgbaColor, 
                                  MFVec3f *normals, MFVec2f *texCoords);
    void                setRGBAColor(MFColorRGBA *color) 
                           { 
                           if (m_colors != NULL)
                               m_colors->unref();
                           m_colorRGBA = true;
                           m_colors = color;
                           m_colors->ref();
                           }
    MFVec2f            *generateTextureCoordinates();
    void                generateFaceNormals();
    void                smoothNormals();

    bool                onlyPlanarFaces(void);
    int                 getNumFaces(void) { return m_numFaces; }
    FaceData           *getFace(int i) { return m_faces[i]; }
    int                 getNumLines(void) { return m_lines.size(); }
    MyLine              getLine(int index);

    void                setCreaseAngle(float angle) { m_creaseAngle = angle; }
    void                setCcw(bool ccw) { m_ccw = ccw; }
    void                setNormals(MFVec3f *normals);

    Vec3f               getSmoothNormal(int i)
                            { return m_smoothNormalsArray[i]; }
    int                 getNumSmoothNormal(void)
                            { return m_smoothNormalsArray.size(); }

    void                optimize(float epsilon = 0.0f);
    MFInt32            *optimizeCoordIndex(void);
    void                optimizeVertices(MFVec3f **newCoord, 
                                         MFInt32 **newCoordIndex,
                                         float epsilon = 0.0f);
    int                 getMaxNumberEdgesPerFace(void);
    virtual bool        hasColor(void) { return (m_colors != NULL); }
    virtual bool        hasColorRGBA(void) { return m_colorRGBA; }
    void                appendIndices(MFInt32 *indices, MFInt32 *index, 
                                      int offset, int appendList[8]);
    void                simpleQuadTriangulate(void);
    MyMesh             *triangulate(MeshBasedNode *that);
    int                 getMeshFlags(void);
    void                setMeshFlags(int MeshFlags);
    void                setVerticesZero(void);
    virtual int         countPolygons(void);
    virtual int         countPolygons1Sided(void);
    virtual int         countPolygons2Sided(void);
    Node               *toIndexedFaceSet(int meshFlags, Scene* scene);
    MFVec3f            *getSmoothNormals(void);
    MFInt32            *getSmoothNormalIndex(void);
#ifdef HAVE_GLUNEWTESS
    void                addNewVertex(VertexInfo * vertexInfo);
    void                endNewPolygon(void);
    void                startTessellation(GLenum type);
    void                collectMemoryToFree(VertexInfo * vertexInfo);
#endif

protected:
    void                buildFaces(void);
#ifdef HAVE_GLUNEWTESS
    void                initialiseTriangulation(GLUtesselator *tess);
    int                 searchInOriginalVertices(GLdouble* vertex);
    int                 searchInNewVertices(GLdouble* vertex);
    MyMesh             *buildNewTriangulatedMesh(MeshBasedNode *that); 
#endif 

private:
    MeshBasedNode      *m_node;

    MFVec3f            *m_vertices;
    MFVec3f            *m_normals;
    MFFloat            *m_colors;
    Array<MFVec2f*>     m_texCoords;

    MFInt32            *m_coordIndex;
    MFInt32            *m_colorIndex;
    MFInt32            *m_normalIndex;
    MFInt32            *m_texCoordIndex;

    int                 m_numFaces;
    FaceData          **m_faces;
    Array<MyLine>       m_lines;

    float               m_creaseAngle;
    bool                m_ccw;
    bool                m_convex;
    bool                m_solid;
    bool                m_normalPerVertex;
    bool                m_colorPerVertex;
    bool                m_colorRGBA;
    float               m_transparency;

    bool                m_isTriangulated;

    int                 m_tessellationStart;
    GLenum              m_tessellationType;
    bool                m_evenTriangleStrip;
    int                 m_drawCounter;
    unsigned int        m_listIndex;
    int                 m_currentFace;
    bool                m_texcoords_generated;
    Array<Vec3f>        m_smoothNormalsArray;

#ifdef HAVE_GLUNEWTESS
    Array<VertexInfo>   m_verticesInfo;

    Array<float>        m_triangulatedVertices;
    Array<int>          m_triangulatedIndices;

    Array<float>        m_triangulatedNormals;
    bool                m_triangulatedHasNormals;

    Array<float>        m_triangulatedColors;
    bool                m_triangulatedHasColors;
    bool                m_triangulatedHasColorsRGBA;

    Array<float>        m_triangulatedTexCoords;
    bool                m_triangulatedHasTexCoords;

    List<VertexInfo *>  m_newMemory4VerticesInfo;
#endif 
};

MFVec2f *generateTextureCoordinates(MFVec3f *points, MFInt32* cindex);

#endif // _MY_MESH_H



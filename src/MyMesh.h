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


// MyMeshX::generateTextureCoordinates() based on Polyrep.c of FreeWRL
/*******************************************************************
 Copyright (C) 1998 Tuomas J. Lukka
 Copyright (C) 2002 John Stewart, CRC Canada.
 DISTRIBUTED WITH NO WARRANTY, EXPRESS OR IMPLIED.
 See the GNU Library General Public License (file COPYING in the distribution)
 for conditions of use and redistribution.
*********************************************************************/

#pragma once

#include "Array.h"
#include "List.h"
#include "Vec4f.h"
#include "Vec3f.h"
#include "Vec2f.h"
#include "Vec3d.h"

#include <stdlib.h>
#ifndef FLT_MAX 
# include <float.h>
#endif
#ifdef _WIN32
# include <search.h>
# include <stdlib.h>
#endif

#include "stdafx.h"
#include "swt.h"

#include "NodeIndexedFaceSet.h"

#include "FaceData.h"
#include "List.h"
#include "MFColor.h"
#include "MFColorRGBA.h"
#include "MFInt32.h"
#include "MFVec2f.h"
#include "MFVec3f.h"
#include "SFInt32.h"
#include "Matrix.h"
#include "DuneApp.h"
#include "NodeCoordinate.h"
#include "NodeNormal.h"
#include "NodeTextureCoordinate.h"
#include "NodeTextureCoordinateGenerator.h"
#include "NodeMultiTextureCoordinate.h"
#include "NodeColor.h"
#include "NodeColorRGBA.h"
#include "Scene.h"
#include "Util.h"
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

enum {
   TEX_GEN_SPHERE,
   TEX_GEN_CAMERA_SPACE_NORMAL,
   TEX_GEN_SPHERE_REFLECT
};

class FaceData;
class MFFloat;
class MFVec3f;
class MFVec2f;
class MFVec3d;
class MFInt32;

extern void beginTriangleCallback(GLenum type, void *data);

extern void newVertexCallback(void *vertex_data, void *data);

extern void endTriangleCallback(void *data);

extern void 
combineVertices(GLdouble coords[3], GLdouble *vertex_data[4],
                GLfloat weight[4], GLdouble **dataOut, void *data); 


#ifdef _WIN32
extern void tesselationError(GLenum error); 
#endif

#ifdef HAVE_GLUNEWTESS
struct VertexInfo {
   GLdouble vertex[3] = { 0, 0, 0 };
   float normal[3] = { 0, 0, 0 };
   float color[4] = { 0, 0, 0, 1 };
   float texCoord[2] = { 0, 0 };
};
#endif

class MyLine  {
public: 
    int begin;
    int end;
};

template<class X,class MFX, class VECX> 
class MyMeshX {
public:
                        MyMeshX(MeshBasedNode *that,
                                MFX *vertices, MFInt32 *coordIndex,
                                MFVec3f *normals, MFInt32 *normalIndex,
                                MFFloat *colors, MFInt32 *colorIndex,
                                MyArray<MFVec2f *>texCoords, 
                                MFInt32 *texCoordIndex,
                                float creaseAngle, int meshFlags, 
                                float transparency, MFFloat *fogCoords = NULL,
                                Node *texCoordGen = NULL);
    virtual            ~MyMeshX();
                
    MyMeshX            *copy(void);

    void                draw(int pass, void (drawVert)(X *));
    void                drawNormals(void);
    void                sort();

    bool                validMesh(void) { return m_vertices != NULL; }  

    MFX                *getVertices(void)      { return m_vertices; }
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
    MFFloat            *getFogCoords()         { return m_fogCoords; }
    Node               *getTexCoordGen()       { return m_texCoordGen; }

    void                addCoords(MFInt32 *coords, float *color, 
                                  MFFloat *colors, bool rgbaColor, 
                                  MFVec3f *normals, MFVec2f *texCoords);
    void                setColorRGBA()
                           { 
                           m_colorRGBA = true;
                           }
 
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
    void                optimizeVertices(MFX **newCoord, 
                                         MFInt32 **newCoordIndex,
                                         float epsilon = 0.0f);
    int                 getMaxNumberEdgesPerFace(void);
    virtual bool        hasColor(void) { return (m_colors != NULL); }
    virtual bool        hasColorRGBA(void) { return m_colorRGBA; }
    void                appendIndices(MFInt32 *indices, MFInt32 *index, 
                                      int offset, int appendList[8]);
    void                simpleQuadTriangulate(void);
    MyMeshX            *triangulate(MeshBasedNode *that);
    int                 getMeshFlags(void);
    void                setMeshFlags(int MeshFlags);
    void                setVerticesZero(void);
    void                setValidVertex(int i);  
    virtual int         countPolygons(void);
    virtual int         countPolygons1Sided(void);
    virtual int         countPolygons2Sided(void);
    Node               *toIndexedFaceSet(int meshFlags, Scene* scene);
    MFVec3f            *getSmoothNormals(void);
    MFInt32            *getSmoothNormalIndex(void);
    int                 getTexCoordParameter(long i)
                             {
                             if (i < m_texCoordParameter.size())
                                 return m_texCoordParameter[i];
                             return -1;
                             }
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
    MyMeshX            *buildNewTriangulatedMesh(MeshBasedNode *that); 
#endif 

    MeshBasedNode      *m_node;

    MFX                *m_vertices;
    MFVec3f            *m_normals;
    MFFloat            *m_colors;
    MyArray<MFVec2f*>   m_texCoords;

    MFInt32            *m_coordIndex;
    MFInt32            *m_colorIndex;
    MFInt32            *m_normalIndex;
    MFInt32            *m_texCoordIndex;

    int                 m_numFaces;
    FaceData          **m_faces;
    MyArray<MyLine>     m_lines;

    float               m_creaseAngle;
    bool                m_ccw;
    bool                m_convex;
    bool                m_solid;
    bool                m_normalPerVertex;
    bool                m_colorPerVertex;
    bool                m_colorRGBA;
    float               m_transparency;
    MFFloat            *m_fogCoords;    
    Node               *m_texCoordGen;    

    bool                m_isTriangulated;

    long              m_tessellationStart;
    GLenum              m_tessellationType;
    bool                m_evenTriangleStrip;
    int                 m_drawCounter;
    unsigned int        m_listIndex;
    int                 m_currentFace;
    bool                m_texcoords_generated;
    MyArray<Vec3f>      m_smoothNormalsArray;
    MyArray<bool>       m_validVertices;

    MyArray<int>        m_texCoordParameter;
#ifdef HAVE_GLUNEWTESS
    MyArray<VertexInfo> m_verticesInfo;

    MyArray<float>      m_triangulatedVertices;
    MyArray<int>        m_triangulatedIndices;

    MyArray<float>      m_triangulatedNormals;
    bool                m_triangulatedHasNormals;

    MyArray<float>      m_triangulatedColors;
    bool                m_triangulatedHasColors;
    bool                m_triangulatedHasColorsRGBA;

    MyArray<float>      m_triangulatedTexCoords;
    bool                m_triangulatedHasTexCoords;

    List<VertexInfo *>  m_newMemory4VerticesInfo;
#endif 
};


template class MyMeshX<float, MFVec3f, Vec3f>;

class MyMesh : public MyMeshX<float, MFVec3f, Vec3f> {
public:
    MyMesh(MeshBasedNode *that, MFVec3f *vertices, MFInt32 *coordIndex,
           MFVec3f *normals, MFInt32 *normalIndex, MFFloat *colors, 
           MFInt32 *colorIndex, MyArray<MFVec2f *>texCoords, 
           MFInt32 *texCoordIndex, float creaseAngle, int meshFlags, 
           float transparency, MFFloat *fogCoords = NULL, 
           Node *texCoordGen = NULL) :
        MyMeshX(that, vertices, coordIndex, normals, normalIndex, colors, 
                colorIndex, texCoords, texCoordIndex, creaseAngle, meshFlags,
                transparency, fogCoords, texCoordGen) {}
    void static drawVertex(float *v); 
    void draw(int pass) { MyMeshX::draw(pass, &drawVertex); }
};

template class MyMeshX<double, MFVec3d, Vec3d>;

class MyMeshDouble : public MyMeshX<double, MFVec3d, Vec3d> {
public:
    MyMeshDouble(MeshBasedNode *that, MFVec3d *vertices, MFInt32 *coordIndex,
                 MFVec3f *normals, MFInt32 *normalIndex, MFFloat *colors, 
                 MFInt32 *colorIndex, MyArray<MFVec2f *>texCoords, 
                 MFInt32 *texCoordIndex, float creaseAngle, int meshFlags, 
                 float transparency, MFFloat *fogCoords = NULL,
                 Node *texCoordGen = NULL) :
        MyMeshX(that, vertices, coordIndex, normals, normalIndex, colors, 
                colorIndex, texCoords, texCoordIndex, creaseAngle, meshFlags,
                transparency, fogCoords, texCoordGen) {}
    void static drawVertex(double *v); 
    void draw(int pass) { MyMeshX::draw(pass, &drawVertex); }
};

template <typename X, typename MFX>
MFVec2f *generateTextureCoordinates(MFX *points, MFInt32* cindex, X dummy);

template <class X,class MFX, class VEC3X>
MyMeshX<X, MFX,VEC3X>::MyMeshX(
     MeshBasedNode *node, MFX *vertices, MFInt32 *coordIndex, MFVec3f *normals,
     MFInt32 *normalIndex, MFFloat *colors, MFInt32 *colorIndex,
     MyArray<MFVec2f *>texCoords, MFInt32 *texCoordIndex, float creaseAngle, 
     int meshFlags, float transparency, MFFloat *fogCoords, Node *texCoordGen)
{
    m_node = node;

    setMeshFlags(meshFlags);

    m_creaseAngle = creaseAngle;
    m_transparency = transparency;

    m_vertices = vertices;
    if (m_vertices)
        m_vertices->ref();

    m_normals = normals;
    if (m_normals) {
        if (m_normals->getSize() == 0)
            m_normals = NULL;
        else
            m_normals->ref();
    }

    m_colors = colors;
    if (m_colors) {
        if (m_colors->getSize() == 0)
            m_colors = NULL;
        else
            m_colors->ref();
    }

    m_coordIndex = coordIndex;
    m_colorIndex = colorIndex;
    m_normalIndex = normalIndex;
    m_texCoordIndex = texCoordIndex;

    if (m_coordIndex == NULL)
        m_coordIndex = new MFInt32();
    m_coordIndex->ref();

    m_texCoords.resize(0);
    for (long i = 0; i < texCoords.size(); i++) {
        m_texCoords[i] = texCoords[i];
        m_texCoords[i]->ref();
    }

    if (m_texCoords.size() == 0) {
        m_texCoords.append(::generateTextureCoordinates(m_vertices, 
                                                        m_coordIndex,
                                                        (X)0.0f));
    }
    
    if (m_normalPerVertex && (normalIndex == NULL))
        m_normalIndex = m_coordIndex;
    if (m_normalIndex)
        m_normalIndex->ref();

    if (m_colorPerVertex && (colorIndex == NULL))
        m_colorIndex = m_coordIndex;
    if (m_colorIndex)
        m_colorIndex->ref();

    if (m_texCoordIndex == NULL)
        m_texCoordIndex = m_coordIndex;
    if (m_texCoordIndex)
        m_texCoordIndex->ref();
    m_fogCoords = fogCoords;
    if (m_fogCoords)
        m_fogCoords->ref();
    m_texCoordGen = texCoordGen;
    m_texCoordParameter.resize(0);
    if (m_texCoordGen &&
        m_texCoordGen->getType() == X3D_TEXTURE_COORDINATE_GENERATOR) {
        NodeTextureCoordinateGenerator *texGen = 
            (NodeTextureCoordinateGenerator *)m_texCoordGen;
        if (strcmp(texGen->mode()->getValue(), "SPHERE") == 0)
            m_texCoordParameter.append(TEX_GEN_SPHERE);
        else if (strcmp(texGen->mode()->getValue(), "CAMERASPACENORMAL") == 0)
            m_texCoordParameter.append(TEX_GEN_CAMERA_SPACE_NORMAL);
        else if (strcmp(texGen->mode()->getValue(), "SPHERE-REFLECT") == 0)
            m_texCoordParameter.append(TEX_GEN_SPHERE_REFLECT);
    } else if (m_texCoordGen &&
               m_texCoordGen->getType() == X3D_MULTI_TEXTURE_COORDINATE) {
        NodeMultiTextureCoordinate *multiTexCoord = 
            (NodeMultiTextureCoordinate *)m_texCoordGen;
        for (int i = 0; i < multiTexCoord->texCoord()->getSize(); i++) {
            if (multiTexCoord->texCoord()->getValue(i) && 
               multiTexCoord->texCoord()->getValue(i)->getType() == 
                   X3D_TEXTURE_COORDINATE_GENERATOR) {
               NodeTextureCoordinateGenerator *texGen = 
                   (NodeTextureCoordinateGenerator *)
                   multiTexCoord->texCoord()->getValue(i);
                if (strcmp(texGen->mode()->getValue(), "SPHERE") == 0)
                    m_texCoordParameter.append(TEX_GEN_SPHERE);
                else if (strcmp(texGen->mode()->getValue(), 
                                "CAMERASPACENORMAL") == 0)
                    m_texCoordParameter.append(TEX_GEN_CAMERA_SPACE_NORMAL);
                else if (strcmp(texGen->mode()->getValue(), "SPHERE-REFLECT") 
                         == 0)
                    m_texCoordParameter.append(TEX_GEN_SPHERE_REFLECT);
                else
                    m_texCoordParameter.append(-1);
            } else
                m_texCoordParameter.append(-1);
        }
    }

    m_faces = NULL;
    m_numFaces = 0;
    buildFaces();
    generateFaceNormals();
    if (!m_normals)
        smoothNormals();
    m_listIndex = 0;
    m_drawCounter = 0;
    if (TheApp->disabledGlList())
        m_drawCounter = -1;
    m_currentFace = -1;
    m_texcoords_generated = false;
    m_validVertices.resize(0);
}

template <class X,class MFX,class VEC3X>
MyMeshX<X, MFX, VEC3X>::~MyMeshX()
{
    if (m_vertices) {
        bool last = m_vertices->getRefs() == 1;
        m_vertices->unref();
        if (last)
            m_vertices = NULL;
    }

    if (m_normals) {
        bool last = m_normals->getRefs() == 1;
        m_normals->unref();
        if (last)
            m_normals = NULL;
    }
    if (m_colors) {
        bool last = m_colors->getRefs() == 1;
        m_colors->unref();
        if (last)
            m_colors = NULL;
    }
    if (m_texCoords.size() > 0)
        if (m_texCoords[0] != NULL) {
            if (m_texcoords_generated)
                delete m_texCoords[0];
            else
                m_texCoords[0]->unref();
        }
    for (int i = 0; 1 < m_texCoords.size(); i++) 
         if (m_texCoords[i] != NULL)
             m_texCoords[i]->unref();
    m_texCoords.resize(0);

    if (m_coordIndex) {
        bool last = m_coordIndex->getRefs() == 1;
        m_coordIndex->unref();
        if (last)
            m_coordIndex = NULL;
    }

    if (m_normalIndex) {
        bool last = m_normalIndex->getRefs() == 1;
        m_normalIndex->unref();
        if (last)
            m_normalIndex = NULL;
    }
    if (m_colorIndex) {
        bool last = m_colorIndex->getRefs() == 1;
        m_colorIndex->unref();
        if (last)
            m_colorIndex = NULL;
    }
    if (m_texCoordIndex) {
        bool last = m_texCoordIndex->getRefs() == 1;
        m_texCoordIndex->unref();
        if (last)
            m_texCoordIndex = NULL;
    }

    for (int i = 0; i < m_numFaces; i++) {
        delete m_faces[i];
    }
    delete [] m_faces;
    if (m_listIndex != 0)
        glDeleteLists(m_listIndex, 1);
}

template <class X,class MFX, class VEC3X>
MyMeshX<X, MFX, VEC3X> *
MyMeshX<X, MFX, VEC3X>::copy(void)
{
    MyArray<MFVec2f*> texCoords;
    for (long i = 0; i < m_texCoords.size(); i++)
        if (m_texCoords[i] != NULL) {
            if (m_texCoords[i]->getSize() > 0)
                texCoords.append((MFVec2f *)m_texCoords[i]->copy());
        }

    MyMeshX *mesh = new MyMeshX(m_node, 
                               (MFX *)m_vertices->copy(), 
                               (MFInt32 *)m_coordIndex->copy(), 
                               getNormals() ? 
                               (MFVec3f *)getNormals()->copy() : NULL, 
                               getNormalIndex() ? (MFInt32 *)
                                                  getNormalIndex()->copy() : 
                                                  NULL, 
                               getColors() ? (MFFloat *) getColors()->copy() : 
                                            NULL, 
                               getColorIndex() ? (MFInt32 *)
                                                 getColorIndex()->copy() : NULL,
                               texCoords, 
                               getTexCoordIndex() ? (MFInt32 *)
                                                    getTexCoordIndex()->copy() :
                                                    NULL,
                               m_creaseAngle, getMeshFlags(), m_transparency,
                               getFogCoords(), getTexCoordGen());
    return mesh;
}

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::buildFaces(void)
{
    int start = 0;
    long nci = m_coordIndex->getSize();
    const int *c = m_coordIndex->getValues();
    long numFaces = 0;

    long i;
    long lines = 0;
    m_lines.resize(0);
    long maxci = 0;
    for (i = 0; i < nci; i++)
         if (c[i] > maxci)
             maxci = c[i];
    for (i = 0; i < nci; i++) {
        if (c[i] < -1) {
            swDebugf("silently ignoring coordIndex < -1, assuming -1\n");
            m_coordIndex->setSFValue(i,-1);
            c = m_coordIndex->getValues();
        }
        if (c[i] <= -1) {
            if (i - start > 0) {
                numFaces++;
            }
            start = i + 1;
        } else {
            bool validLine = false;
            MyLine line;
            line.begin = i;
            if (c[i + 1] > -1) {
                line.end = i + 1;
                validLine = true;
            }
            if (validLine && 
                line.begin != line.end && 
                line.begin <= nci && 
                line.begin > -1 &&
                line.end <= nci && 
                line.end > -1
               ) {
                m_lines[lines] = line;
                lines++;
            }
        }
    }
    if ((i != 0) && (i != 1))
        if ((c[i-1] != -1) && (c[i-2] != -1))
            numFaces++;
    for (int i = 0; i < m_numFaces; i++) {
        delete m_faces[i];
    }
    delete [] m_faces;
    if (numFaces != 0)
       m_faces = new FaceData *[numFaces];
    else
       m_faces = new FaceData *[1];
    m_numFaces = numFaces;
    int newNumFaces = 0;
    start = 0;
    for (long i = 0; i < nci; i++) {
        if (c[i] == -1) {
            if (i - start > 0) {
                m_faces[newNumFaces] = new FaceData(i - start, start);
                newNumFaces++;
            }
            start = i + 1;
        } 
    }
    // handle last face if coordIndex array do not end with -1
    if (newNumFaces < m_numFaces) {
        m_faces[newNumFaces] = new FaceData(nci - start, start);
       newNumFaces++;
    }
    m_numFaces = newNumFaces;
}

#define NUMBER_OF_DRAWS_BEFORE_TRYING_GL_LIST 5

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::draw(int pass, void (*drawVert)(X *v))
{
    if ((pass == 1) && m_colorRGBA)
        return;
    if ((pass == 2) && !m_colorRGBA)
        return;
    if (m_vertices == NULL)
        return;
    if (m_vertices->getSize() == 0)
        return;       
    if (m_coordIndex == NULL)
        return;
    if (m_coordIndex->getSize() == 0)
        return;
   
    if (m_listIndex > 0) {
        glCallList(m_listIndex);
        return;
    } else {
        if (m_drawCounter >= 0) {
            if (!TheApp->trackAnimation()) {
                if (m_drawCounter == NUMBER_OF_DRAWS_BEFORE_TRYING_GL_LIST) {
                    m_listIndex = glGenLists(1);
                    if (m_listIndex == 0) {
                       // disable further usage of display lists
                       m_drawCounter = -1; 
                    }
                } else
                    m_drawCounter++;
            }
        }
    }

    if (m_listIndex > 0)
        glNewList(m_listIndex, GL_COMPILE_AND_EXECUTE);

    const X *vertices = m_vertices->getValues();
    const int *coordIndex = m_coordIndex->getValues();
    const float *normals = m_normals ? m_normals->getValues() : NULL;
    const float *texCoords = (m_texCoords.size() > 0) ? 
                              m_texCoords[0] ? m_texCoords[0]->getValues() 
                              : NULL
                             : NULL;
    const int *texCoordIndex = m_texCoordIndex ? m_texCoordIndex->getValues() 
                                                 : NULL;
    const float *colors = m_colors ? m_colors->getValues() : NULL;
    const int *colorIndex = m_colorIndex ? m_colorIndex->getValues() : NULL;

    const float *fogDepth = m_fogCoords ? m_fogCoords->getValues() : NULL;
    int numFogDepth = m_fogCoords ? m_fogCoords->getSize() : 0;

    bool ccw = m_ccw;
    bool solid = m_solid;
    int meshFlags = getMeshFlags();
    if (meshFlags != -1) {
        ccw = meshFlags & MESH_CCW;
        solid = meshFlags & MESH_SOLID;
    }

    if (ccw) 
        glFrontFace(GL_CCW);
    else 
        glFrontFace(GL_CW);
    if (solid) {
        glEnable(GL_CULL_FACE);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    } else {
        glDisable(GL_CULL_FACE);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    }
    if (m_colors) {
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    }
    int colorStride = m_colorRGBA ? 4 : 3; 
    for (int i = m_numFaces - 1; i >= 0; i--) {
        int offset = m_faces[i]->getOffset();
        int numVertices = m_faces[i]->getNumVertices();
        if (numVertices < 3) 
            continue;
        glBegin(GL_POLYGON);
        for (int j = offset; j < offset + numVertices; j++) {
            if ((coordIndex[j] < 0) || (j >= m_coordIndex->getSize()) ||
                (coordIndex[j] > m_vertices->getSFSize()))
                continue;
            if ((m_validVertices.size() > 0) && !m_validVertices[coordIndex[j]])
                continue;
            if (texCoords && texCoordIndex &&
                (j < m_texCoordIndex->getSFSize()) &&
                (texCoordIndex[j] < m_texCoords[0]->getSFSize()) &&
                (texCoordIndex[j] >= 0)) {
                glTexCoord2fv(texCoords + texCoordIndex[j] * 2);
#ifdef HAVE_MULTI_TEXTURE
                long textureUnits = 0;
                if (m_node)
                    textureUnits = m_node->getTextureUnits();
                for (long k = 0; k < textureUnits; k++)
                    if (k < m_texCoords.size())
                        glMultiTexCoord2fv(GL_TEXTURE0 + k, 
                                           m_texCoords[k]->getValues() + 
                                           texCoordIndex[j] * 2);
                    else
                        glMultiTexCoord2fv(GL_TEXTURE0 + k, 
                            m_texCoords[((long)m_texCoords.size() - 1) > -1 ? 
                                m_texCoords.size() -1 : 0]->getValues() +
                                           texCoordIndex[j] * 2);
#endif
            }
            if (colors) { 
                int index = -1; 
                if (m_colorPerVertex) {
                    if (j < m_colorIndex->getSize())
                        index = colorIndex[j] * colorStride;
                } else {
                    if (m_colorIndex && (m_colorIndex->getSize() > 0)) {
                        if (i < m_colorIndex->getSize())
                               index = colorIndex[i] * colorStride;
                    } else
                        index = i * colorStride;
                }
                if ((index >= 0) && (index < m_colors->getSize())) {
                    if (m_colorRGBA) {
                        Util::myGlColor4f(colors[index],
                                          colors[index + 1],
                                          colors[index + 2],
                                          colors[index + 3]);
                    } else {
                        Util::myGlColor4f(colors[index],
                                          colors[index + 1],
                                          colors[index + 2],
                                          1 - m_transparency);
                   }
                }
            }
            if (normals) {
                int index = -1; 
                if (m_normalPerVertex) {
                    if (j < m_normalIndex->getSize())
                        index = m_normalIndex->getValue(j) * 3;
                } else {
                    if (m_normalIndex && (m_normalIndex->getSize() > 0)) {
                        if (i < m_normalIndex->getSize())
                            index = m_normalIndex->getValue(i) * 3;
                    } else
                        index = i * 3;
                } 
                if ((index >= 0) && (index < m_normals->getSize()))
                    glNormal3fv(normals + index);
            }
#ifdef HAVE_GLFOGCOORDF
            if (fogDepth) {
                int fogIndex = numFogDepth - 1;
                    if (coordIndex[j] < numFogDepth)
                        fogIndex = coordIndex[j];
                    if (fogIndex > -1)
                        glFogCoordf(fogDepth[fogIndex]);
               }
#endif

            drawVert((X *)(vertices + coordIndex[j] * 3));            
        }
        glEnd();
    }

    if (m_colors) {
        glDisable(GL_COLOR_MATERIAL);
    }

    if (solid) {
        glDisable(GL_CULL_FACE);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    } else {
        glEnable(GL_CULL_FACE);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    }
    if (!ccw) glFrontFace(GL_CCW);

    if (m_listIndex > 0) {
        glEndList();
        unsigned int error = TheApp->printRenderErrors(false);
        if (error == GL_OUT_OF_MEMORY) {
            glDeleteLists(m_listIndex, 1);
            m_listIndex = 0;
            m_drawCounter = -1; // disables further usage of display list
        } else if (error != GL_NO_ERROR)
            TheApp->printRenderErrors(error);
    }
}

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::drawNormals(void) 
{
    if (m_vertices == NULL)
        return;
    if (m_vertices->getSize() == 0)
        return;       
    if (m_coordIndex == NULL)
        return;
    if (m_coordIndex->getSize() == 0)
        return;

    const X *vertices = m_vertices->getValues();
    const int *coordIndex = m_coordIndex->getValues();
    const float *normals = m_normals ? m_normals->getValues() : NULL;
    const int *normalIndex = m_normalIndex ? m_normalIndex->getValues() : 
                                            coordIndex;

    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    for (int i = 0; i < m_numFaces; i++) {
        int offset = m_faces[i]->getOffset();
        int numVertices = m_faces[i]->getNumVertices();
        for (int j = offset; j < offset + numVertices; j++) {
            VEC3X v1 = vertices + coordIndex[j] * 3;
            VEC3X v2 = v1;
            Vec3f norm = Vec3f(normals + normalIndex[j] * 3) * 0.5f;
            v2.x += norm.x;
            v2.y += norm.y;
            v2.z += norm.z;
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);
}


class MyEdge {
public:
                MyEdge(int p1, int p2, int f, bool s = false)
                   { 
                   pos1 = p1;
                   pos2 = p2; 
                   face = f; 
                   smooth = s; 
                   }
    int         pos1;
    int         pos2;
    int         face;
    bool        smooth;
};

typedef List<MyEdge *> EdgeList;

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::generateFaceNormals()
{
    int n = m_coordIndex->getSize();
    const int *coordIndex = m_coordIndex->getValues();
    const X *vertices = m_vertices->getValues();
    int numVertices = m_vertices->getSFSize();
    FaceData *const *face = m_faces;
    int indFaces;

    indFaces = 0;
    if (m_vertices->getSize() != 0) {
        for (int i = 0; i < n; face++) {
            indFaces++;
            if (indFaces > m_numFaces) 
                break;

            if ((coordIndex[i] >= numVertices) || (coordIndex[i] < 0)) {
                while ((i < n) && (coordIndex[i] < 0)) 
                    i++;
                continue;
            }
            VEC3X c1(vertices + coordIndex[i++] * 3);

            if ((coordIndex[i] >= numVertices) || (coordIndex[i] < 0)) {
                while ((i < n) && (coordIndex[i] < 0)) 
                    i++;
                continue;
            }
            VEC3X c2(vertices + coordIndex[i++] * 3);

            if ((coordIndex[i] >= numVertices) || (coordIndex[i] < 0)) {
                while ((i < n) && (coordIndex[i] < 0)) 
                    i++;
                continue;
            }
            VEC3X c3(vertices + coordIndex[i++] * 3);

            VEC3X v1 = c1 - c2;
            VEC3X v2 = c3 - c2;
            VEC3X normal = m_ccw ? v2.cross(v1) : v1.cross(v2);
            if (normal.isZero())
                normal = c2;

            normal.normalize();
            Vec3f norm(normal.x, normal.y, normal.z);
            if (face)
                if (*face) 
                    (*face)->setNormal(norm);
            while (i < n && coordIndex[i] != -1) i++;
            while (i < n && coordIndex[i] == -1) i++;
        }
    }
}

static MyEdge *
findEdge(const int *coordIndex, EdgeList::Iterator *i, int vertex)
{
    for(; i != NULL; i = i->next()) {
        MyEdge *e = i->item();
        if (e == NULL)
            return NULL;
        if (coordIndex[e->pos2] == vertex) 
            return e;
    }
    return NULL;
}

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::smoothNormals()
{
    if ((m_normalIndex != NULL) && (m_normalIndex->getSize() > 0))
        m_normalIndex->unref();

    if (m_vertices->getSize()==0)
       return;       

    EdgeList::Iterator *j;
    long nVerts = m_vertices->getSFSize();
    MyArray<Vec3f> normals;
    // the created normals are normalPerVertex, so any vertex need a normal
    m_normalPerVertex = true;
    float cosAngle = (float) cos(m_creaseAngle);
    int numFaces = 0;
    EdgeList *edgeLists = new EdgeList[nVerts];
    const int *coordIndex = m_coordIndex->getValues();
    long nCoords = m_coordIndex->getSize();
    int start = 0;
    MyArray<int> normalIndex;

    for (long i = 0; i < nCoords; i++) {
        long v = coordIndex[i];
        if ((v == -1) || (v >= (long)nVerts)) {
            if (i - start > 0) {
                numFaces++;
            }
            start = i + 1;
        } else {
            int pos2;
            if (i == nCoords - 1 || coordIndex[i + 1] == -1) {
                pos2 = start;
            } else {
                pos2 = i + 1;
            }
            edgeLists[v].append(new MyEdge(i, pos2, numFaces));
        }
        normalIndex[i] = -1;
    }
    // vertices without normals get the face normal
    for (long n = 0; n < nCoords; n++) {
        long i = coordIndex[n];
        if ((i == -1) || (i >= (long)nVerts))
            continue;
        for (j = edgeLists[i].first(); j != NULL; j = j->next()) {
            MyEdge *e = j->item();
            if (normalIndex[e->pos1] == -1) {
                int index = normals.size();
                if ((e->face < m_numFaces) && m_faces[e->face]) {
                    normals[index] = m_faces[e->face]->getNormal();
                    normalIndex[e->pos1] = index;
                }
             } else {
                 int index = normalIndex[e->pos1];
                 if (index < (int)normalIndex.size() &&
                     (normals[index].x < floatEpsilon()) &&
                     (normals[index].y < floatEpsilon()) &&
                     (normals[index].z < floatEpsilon()))
                     normals[index] = m_faces[e->face]->getNormal();
            }
        }
    }
    for (long n = 0; n < nCoords; n++) {
        long i = coordIndex[n];
        if ((i == -1) || (i >= (long)nVerts))
            continue;
        for (j = edgeLists[i].first(); j != NULL; j = j->next()) {
            MyEdge *e = j->item();
            if (e->face >= m_numFaces)
                continue;
            const Vec3f *refNormal = NULL;
            bool validRefNormal = false;
            if (m_faces[e->face]) {
                refNormal = &m_faces[e->face]->getNormal();
                validRefNormal = true;
            }
            if (e->pos2 >= (int)nCoords)
                continue;
            long v2 = coordIndex[e->pos2];
            if (v2 >= (long)nVerts)
                continue;
            MyEdge *f = findEdge(coordIndex, edgeLists[v2].first(), i);
            if (f && (f->face < m_numFaces) && m_faces[f->face] && 
                validRefNormal) {
                const Vec3f &otherNormal = m_faces[f->face]->getNormal();
                float dotNormals = refNormal->dot(otherNormal);
                if (dotNormals > cosAngle) {
                    // this edge is smooth
                    int i1 = normalIndex[e->pos1];
                    int i2 = normalIndex[f->pos2];
                    if (i1 == -1 && i2 == -1) {
                        // create a new normal
                        normals[i1] = (*refNormal + otherNormal);
                        normals[i2] = (*refNormal + otherNormal);
                    } else if (i1 == -1 && i2 != -1) {
                        // use v2's normal
                        normals[i2] += *refNormal;
                    } else if (i1 != -1 && i2 == -1) {
                        // use v1's normal
                        normals[i1] += otherNormal;
                    } else {
                        // they're both specified, so combine them in place
                        normals[i1] += normals[i2];
                        normals[i2] = normals[i1];
                        if (dotNormals != 1.0f) {
                            e->smooth = true;
                            f->smooth = true;
                        }
                    }
                }
            }
        }
    }
    // join all smoothed normals of a vertex
    MyArray<IntArray *> smoothNormalIndices;
    for (long i = 0; (i < normalIndex.size()) || (i < nVerts); i++)
        smoothNormalIndices.append(new IntArray());
    for (long n = 0; n < nCoords; n++) {
        long i = coordIndex[n];
        if ((i == -1) || (i >= (long)nVerts))
            continue;
        // find all normals for a vertex
        for (j = edgeLists[i].first(); j != NULL; j = j->next()) {
            MyEdge *e = j->item();
            if (e->face >= m_numFaces)
                continue;
            if (e->smooth)
                smoothNormalIndices[i]->append(e->pos1);
        }
    }
    for (long n = 0; n < nCoords; n++) {
        long i = coordIndex[n];
        if ((i == -1) || (i >= (long)nVerts))
            continue;
        Vec3f averageNormal(0, 0, 0);
        bool hasAverageNormal = false;
        for (long j = 0; j < smoothNormalIndices[i]->size(); j++) {
            hasAverageNormal = true;
            int indexNormal = smoothNormalIndices[i]->get(j);
            averageNormal += normals[normalIndex[indexNormal]];
        }
        if (hasAverageNormal) {
            averageNormal.normalize();
            for (long j = 0; j < smoothNormalIndices[i]->size(); j++) {
                int indexNormal = smoothNormalIndices[i]->get(j);
                normals[normalIndex[indexNormal]] = averageNormal;
            }
        }
    }
    // vertices with normal 0 0 0 get the face normal
    for (long n = 0; n < nCoords; n++) {
         long i = coordIndex[n];
         if ((i == -1) || (i >= (long)nVerts))
             continue;
        for (j = edgeLists[i].first(); j != NULL; j = j->next()) {
            MyEdge *e = j->item();
            if (normalIndex[e->pos1] == -1) {
                int index = normals.size();
                if ((e->face < m_numFaces) && m_faces[e->face]) {
                    normals[index] = m_faces[e->face]->getNormal();
                    normalIndex[e->pos1] = index;
                }
             } else {
                 int index = normalIndex[e->pos1];
                 if ((normals[index].x < floatEpsilon()) &&
                     (normals[index].y < floatEpsilon()) &&
                     (normals[index].z < floatEpsilon()))
                     normals[index] = m_faces[e->face]->getNormal();
            }
        }
    }

    int face = 0;
    for (long n = 0; n < normalIndex.size(); n++) {
        long i = normalIndex[n];        
        if ((i < 0) || (i >= (long)normalIndex.size())) {
            if (i < 0)
                face++;
            continue;
        }
        normals[i].normalize();
        if (normals[i].isZero()) {
            if (face < m_numFaces)
                normals[i] = m_faces[face]->getNormal();
        }
    }
    // cleanup
    for (long n = 0; n < nCoords; n++) {
         int i = coordIndex[n];
         if ((i == -1) || (i >= (long)nVerts))
             continue;
        for (j = edgeLists[i].first(); j != NULL; j = j->next()) {
            MyEdge *e = j->item();
            delete e;
        }
        edgeLists[i].removeAll();
    }
    delete [] edgeLists;
    for (long i = 0; i < smoothNormalIndices.size(); i++) {
        delete smoothNormalIndices[i];
        smoothNormalIndices[i] = NULL;        
    }
    long maxNormalIndex = -1;
    for (long k = 0; k < normalIndex.size(); k++)
        if (normalIndex[k] > maxNormalIndex)
            maxNormalIndex = normalIndex[k];
    if (normals.size() > 0)
        for (long k = normals.size() - 1; k > maxNormalIndex; k--)
            normals.remove(k);
    for (long k = 0; k < normals.size(); k++) {
        normals[k].normalize();
    }
    if (m_normals)
        m_normals->unref();
    m_normals = new MFVec3f((float *)normals.extractData(), normals.size() * 3);
    if (normalIndex.size() > 0) {
        int *n = new int[normalIndex.size() + 1];
        for (long i = 0; i < normalIndex.size(); i++)
            n[i] = normalIndex[i];
        int nsize = normalIndex.size();
        if (n[nsize - 1] != -1) {
            nsize++;
            n[nsize - 1] = -1;
        }
        m_normalIndex = new MFInt32(n, nsize);
    }
    m_normals->ref();
    if (m_normalIndex)
        m_normalIndex->ref();

    m_smoothNormalsArray.resize(0);
    for (long n = 0; n < nVerts; n++) {
        Vec3f vec;
        m_smoothNormalsArray[n] = vec;
    }
    for (long n = 0; n < nCoords; n++) {
        long i = coordIndex[n];
        if ((i < 0) || (i >= (long)nVerts))
            continue;
        m_smoothNormalsArray[i] += m_normals->getVec(m_normalIndex->getValue(n)
                                                    );
    }
    for (long i = 0; i < m_smoothNormalsArray.size(); i++)
        m_smoothNormalsArray[i].normalize();
}

static int
compareFaceFunction(const void *f1, const void *f2)
{
    float min1 = (*(FaceData **) f1)->getMinZ();
    float max1 = (*(FaceData **) f1)->getMaxZ();
    float min2 = (*(FaceData **) f2)->getMinZ();
    float max2 = (*(FaceData **) f2)->getMaxZ();

    if (max1 < min2) return -1;
    if (max2 < min1) return 1;
    return 0;
}

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::sort()
{
#ifdef SORT_IN_MESH
    if (TheApp->GetRenderFasterWorse())
#endif
        return;
    const int  *coordIndex = m_coordIndex->getValues();
    const X *vertices = m_vertices->getValues();
    Matrix matrix;

    if (m_vertices->getSize()==0)
       return;       

    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) matrix);

    for (int i = 0; i < m_numFaces; i++) {
        int offset = m_faces[i]->getOffset();
        int numVertices = m_faces[i]->getNumVertices();
        VEC3X v(0.0f, 0.0f, 0.0f);
        float min = FLT_MAX;
        float max = 0.0f;
        for (int j = 0; j < numVertices; j++) {
            if (offset + j >= m_vertices->getSFSize())
                break;
            v = -(matrix * VEC3X(vertices + coordIndex[offset + j] * 3));
            if (v.z > max) max = v.z;
            if (v.z < min) min = v.z;
        }
        m_faces[i]->setMinZ(min);
        m_faces[i]->setMaxZ(max);
    }
    qsort(m_faces, m_numFaces, sizeof(FaceData *), compareFace);
}

template <typename X, typename MFX>
MFVec2f* 
generateTextureCoordinates(MFX *points, MFInt32* cindex, X dummy)
{
    MFVec2f *texcoords = NULL;
    if (!points)
        return texcoords;
    int npoints = points->getSFSize(); 

    if (npoints > 0) {
        float* tcoords = new float[npoints * 2];
        for (int i = 0; i < npoints; i++) {
            tcoords[i * 2] = 0;
            tcoords[i * 2 + 1] = 0;
        }

        /* texture generation points... */

        float minVals[3];
        float maxVals[3];

        // find first valid point to initialise 
        bool initialised = false;
        for (int i = 0; i < cindex->getSize(); i++) {
            int ind = cindex->getValue(i);
            if ((ind >= 0) && (ind < npoints)) {
                const X *point = points->getValue(ind);
                if (point) {
                    for (int j = 0; j < 3; j++) {
                        minVals[j] = point[j];
                        maxVals[j] = point[j];
                    }
                initialised = true;
                break;
                } 
            } 
        }

        if (initialised == false)
            return NULL;

        /* generate default texture mapping */
        for (int i = 0; i < cindex->getSize(); i++) {
            int ind = cindex->getValue(i);
            if ((ind >= 0) && (ind < npoints)) {
                const X *point = points->getValue(ind);
                if (point) {
                    for (int j = 0; j < 3; j++) {
                        if (minVals[j] > point[j]) 
                            minVals[j] = point[j];
                        if (maxVals[j] < point[j]) 
                            maxVals[j] = point[j];
                    }
                } 
            } 
        }

        float Ssize = 0.0;
        float Tsize = 0.0;
        int Sindex = 0;
        int Tindex = 0;

        /* find the S,T mapping. */
        float Xsize = maxVals[0]-minVals[0]; 
        float Ysize = maxVals[1]-minVals[1];
        float Zsize = maxVals[2]-minVals[2];

        if ((Xsize >= Ysize) && (Xsize >= Zsize)) {
            /* X size largest */
            Ssize = Xsize;
            Sindex = 0;
            if (Ysize >= Zsize) {
                Tsize = Ysize;
                Tindex = 1;
            } else {
                Tsize = Zsize;
                Tindex = 2;
            }
        } else if ((Ysize >= Xsize) && (Ysize >= Zsize)) {
            /* Y size largest */
            Ssize = Ysize;
            Sindex = 1;
            if (Xsize >= Zsize) {
                Tsize = Xsize;
                Tindex = 0;
            } else {
                Tsize = Zsize;
                Tindex = 2;
            }
        } else {
            /* Z is the largest */
            Ssize = Zsize;
            Sindex = 2;
            if (Xsize >= Ysize) {
                Tsize = Xsize;
                Tindex = 0;
            } else {
                Tsize = Ysize;
                Tindex = 1;
            }
        }
        if (Ssize == 0) 
            return NULL;

        for(int i = 0; i < cindex->getSize(); i++) {
            int ind = cindex->getValue(i);
            if ((ind >= 0) && (ind < npoints)) {
                const X *point = points->getValue(ind);
                if (point) {
                    /* temporary place for X,Y,Z */
                    X XYZ[3] = {0.0, 0.0, 0.0};

                    XYZ[0] = point[0]; 
                    XYZ[1] = point[1]; 
                    XYZ[2] = point[2];
                       
                    // default textures 
                    // we want the S values to range from 0..1, 
                    //     and the T values to range from 0..S/T
                    tcoords[ind*2]   = (XYZ[Sindex] - minVals[Sindex])/Ssize;
                    tcoords[ind*2+1] = (XYZ[Tindex] - minVals[Tindex])/Tsize;
                }
            }
        }
        texcoords = new MFVec2f(tcoords, 2 * npoints);
    }
    return texcoords;
}

template <class X,class MFX,class VEC3X>
MFVec2f* 
MyMeshX<X, MFX, VEC3X>::generateTextureCoordinates()
{
    m_texcoords_generated = true;
    return ::generateTextureCoordinates(m_vertices, m_coordIndex, (X)0.0f); 
}

#define VEC3F(x,y) VEC3X((*x).getValue(y))

template <class X,class MFX,class VEC3X>
bool 
MyMeshX<X, MFX, VEC3X>::onlyPlanarFaces(void)
{
    int vertexCounter = 0;
    for (int i = 0; i < m_coordIndex->getSize(); i++) {
        if (m_coordIndex->getValue(i) <= -1)
            vertexCounter = 0;
        else {
            VEC3X vec1(0, 0, 0);
            VEC3X vec2(0, 0, 0);
            VEC3X vec3(0, 0, 0);
            VEC3X cross12(0, 0, 0);
            if (vertexCounter == 1)
                vec1 = VEC3F(m_vertices,1) - VEC3F(m_vertices,0);
            else if (vertexCounter == 2) {
                vec2 = VEC3F(m_vertices,2) - VEC3F(m_vertices,0);
                cross12 = vec1.cross(vec2);
            } else if (vertexCounter > 2) {
                vec3 = VEC3F(m_vertices,3) - VEC3F(m_vertices,0);
                if (cross12.cross(vec3).length() > FEPSILON)
                    return false;
            }
            vertexCounter++;
        }
   } 
   return true;
}

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::setNormals(MFVec3f *normals) 
{
    if (m_normals)
        m_normals->unref();
    m_normals = normals;
}

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::optimize(float epsilon)
{
    m_coordIndex = optimizeCoordIndex();
    buildFaces();
    MFX *vertices = new MFX();
    MFInt32 *coordIndex = new MFInt32();
    optimizeVertices(&vertices, &coordIndex, epsilon);
    m_vertices->unref();
    m_vertices = vertices;
    m_coordIndex->unref();
    m_coordIndex = coordIndex;
    buildFaces();
    generateFaceNormals();
    if (!m_normals) {
        smoothNormals();
    }
}

template <class X,class MFX,class VEC3X>
MFInt32 *
MyMeshX<X, MFX, VEC3X>::optimizeCoordIndex(void)
{
    const X *vertices = m_vertices->getValues();
    MFInt32 *coordIndex = new MFInt32();
    for (int i = 0; i < m_coordIndex->getSize(); i++)
        coordIndex->appendSFValue(m_coordIndex->getValue(i));

    // test if all coordIndex values are in range and 
    // delete them, which are not
    for (int i = 0; i < coordIndex->getSize(); i++)
        if (coordIndex->getValue(i) >= m_vertices->getSFSize())
            coordIndex->removeSFValue(i--);        

    // test if there are faces with identical vertices in a row
    // if such unnecessary repeated vertices are found, 
    // the matching coordIndices are deleted
    MyArray<int> coordIndexDeleteArray;
    for (int i = 0; i < m_numFaces; i++) {
        int offset = m_faces[i]->getOffset();
        if (offset >= coordIndex->getSize())
            continue;
        int numVertices = m_faces[i]->getNumVertices();
        if (numVertices < 3)
            continue;
        VEC3X lastVertex(vertices + coordIndex->getValue(offset) * 3);
        for (int j = 1; j < numVertices; j++) {
            VEC3X vertex(vertices + coordIndex->getValue(offset + j) * 3);
            if (vertex == lastVertex)
                coordIndexDeleteArray.append(offset + j);
            lastVertex = vertex;
        }
    }
    for (int i = coordIndexDeleteArray.size() - 1; i >= 0; i--)
        coordIndex->removeSFValue(coordIndexDeleteArray[i]);

    // search identical vertices with different coordIndex
    for (int i = coordIndex->getSize() - 1; i >= 0; i--) {
        if (coordIndex->getValue(i) >= 0) {
            VEC3X vertex1(vertices + coordIndex->getValue(i) * 3);
            for (int j = i + 1; j < coordIndex->getSize(); j++) {
                if (coordIndex->getValue(j) >= 0) {
                    VEC3X vertex2(vertices + coordIndex->getValue(j) * 3);
                    // replace coordIndex with index of first identical vertex 
                    if (vertex1 == vertex2)
                        coordIndex->setSFValue(j, coordIndex->getValue(i));
                }
            }
        }
    }

    // test for repeated faces and delete them
    coordIndexDeleteArray.resize(0);
    int faceOffset1 = 0;
    for (int i = 0; i < coordIndex->getSize(); i++)
        if (coordIndex->getValue(i) < 0) {
            int j = i + 1;
            int faceOffset2 = j;
            bool identicalFace = true;
            while (j < coordIndex->getSize()) {
                if (faceOffset1 >= i)
                    break;
                for (int coordIndex1 = faceOffset1; coordIndex1 < i; 
                     coordIndex1++) {
                    j++;
                    VEC3X vert1(vertices + 
                                coordIndex->getValue(coordIndex1) * 3);
                    int coordIndex2 = faceOffset2 + coordIndex1 - faceOffset1;
                    if (coordIndex2 < coordIndex->getSize()) {
                        // end of second face ?            
                        if (coordIndex->getValue(coordIndex2) < 0) {
                            if (identicalFace)
                                for (int k = faceOffset2; k < coordIndex2; k++)
                                     coordIndexDeleteArray.append(k);
                            faceOffset2 = coordIndex2 + 1;
                            break;                       
                        } else {
                            VEC3X vert2(vertices + 
                                        coordIndex->getValue(coordIndex2) * 3);
                            if (vert1 != vert2)
                                identicalFace = false;
                        }
                    } else {
                       if (identicalFace)
                           for (int k = faceOffset2; k < coordIndex2; k++)
                               coordIndexDeleteArray.append(k);
                        break;
                    }
                }
            }
            faceOffset1 = i + 1;
        }
    for (int i = coordIndexDeleteArray.size() - 1; i >= 0; i--)
        coordIndex->removeSFValue(coordIndexDeleteArray[i]);

    // search for faces with identical vertices and delete them
    int faceBeginPtr = 0;
    for (int i = 0; i < coordIndex->getSize(); i++) {
        if (coordIndex->getValue(i) < 0) {
            for (int j = faceBeginPtr; j < i; j++) {
                for (int n = faceBeginPtr; n < j; n++)
                    if (coordIndex->getValue(j) == coordIndex->getValue(n)) {
                        coordIndex->removeSFValue(j);
                    }
            }
            faceBeginPtr = i + 1;
        }
    }    
    
    // search for faces with less than 3 vertices and delete them
    int verticesPerFace = 0;
    for (int i = 0; i < coordIndex->getSize(); i++)
        if (coordIndex->getValue(i) < 0) {
            if (verticesPerFace < 3) {
                coordIndex->removeSFValue(i--);
                for (int j = i; j >= 0; j--)
                    if (coordIndex->getValue(j) < 0)
                        break;
                    else {
                        coordIndex->removeSFValue(j);
                        i--;
                    }
            }
            verticesPerFace = 0;
        } else
            verticesPerFace++;       
    
    buildFaces();
    return coordIndex;
}

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::optimizeVertices(MFX **newCoord, 
                                         MFInt32 **newCoordIndex, float epsilon)
{
    const X *vertex = m_vertices->getValues();
    MyArray<bool> vertexAlreadyHandled;
    for (int i = 0; i < m_vertices->getSize(); i++)
        vertexAlreadyHandled.append(false);

    const int *coordIndex = m_coordIndex->getValues();
    MyArray<int> tmpCoordIndex;
    for (int i = 0; i < m_coordIndex->getSize(); i++)
        tmpCoordIndex.append(-1);

    int vertexCounter = 0;
    for (int i = 0; i < m_vertices->getSize(); i++) {
        bool increaseVertexCounter = false;
        for (int j = 0; j < m_coordIndex->getSize(); j++) {
            if ((coordIndex[j] == i) && (!vertexAlreadyHandled[i])) {
                tmpCoordIndex[j] = vertexCounter;
                increaseVertexCounter = true;
                if (coordIndex[j] > -1) {
                    for (int k = j + 1; k < m_coordIndex->getSize(); k++) {
                        if (coordIndex[k] < 0)
                            continue;
                        if (i == coordIndex[k])
                            continue;
                        int v1 = i * 3;
                        int v2 = coordIndex[k] * 3;
                        Vec3f vec1(vertex[v1], vertex[v1 + 1], vertex[v1 + 2]);
                        Vec3f vec2(vertex[v2], vertex[v2 + 1], vertex[v2 + 2]);
                        if ((vec1 - vec2).length() <= epsilon) {
                            vertexAlreadyHandled[coordIndex[k]] = true;
                            tmpCoordIndex[k] = vertexCounter;
                        }
                    }
                }
            }
        }
        if (increaseVertexCounter && (!vertexAlreadyHandled[i]))  {
            vertexCounter++;
            int v = i * 3;
            (*newCoord)->appendSFValue(vertex[v], vertex[v + 1], vertex[v + 2]);
        }
    }

    for (int i = 0; i < m_coordIndex->getSize(); i++)
        (*newCoordIndex)->appendSFValue(tmpCoordIndex[i]);
}

template <class X,class MFX,class VEC3X>
int                 
MyMeshX<X, MFX, VEC3X>::getMaxNumberEdgesPerFace(void)
{
    int maxNumberEdges = 0;
    for (int i = 0; i < getNumFaces(); i++)
        if (m_faces[i]->getNumVertices() > maxNumberEdges)
            maxNumberEdges = m_faces[i]->getNumVertices();
    return maxNumberEdges;
}

template <class X,class MFX,class VEC3X>
void 
MyMeshX<X, MFX, VEC3X>::appendIndices(MFInt32 *indices, MFInt32 *index, 
                                      int offset, int appendList[8])
{
    for (int i = 0; i < 8; i++)
        if (appendList[i] > -1)
            indices->appendSFValue(index->getValue(offset + appendList[i]));
        else
            indices->appendSFValue(-1);
}  

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::simpleQuadTriangulate(void)
{
    MFInt32 *coordIndices = new MFInt32();
    MFInt32 *normalIndices = new MFInt32();
    MFInt32 *colorIndices = new MFInt32();
    MFInt32 *texCoordIndices = new MFInt32();
    for (int i = 0; i < getNumFaces(); i++) {
        int offset = m_faces[i]->getOffset();
        if (m_faces[i]->getNumVertices() == 4) {
            const X *vertices = m_vertices->getValues();
            VEC3X v1(vertices + m_coordIndex->getValue(offset + 0) * 3);
            VEC3X v2(vertices + m_coordIndex->getValue(offset + 1) * 3);
            VEC3X v3(vertices + m_coordIndex->getValue(offset + 2) * 3);
            VEC3X v4(vertices + m_coordIndex->getValue(offset + 3) * 3);
            // account length
            float v12 = (v1 - v2).squaredNorm();
            float v13 = (v1 - v3).squaredNorm();
            float v14 = (v1 - v4).squaredNorm();
            float v24 = (v2 - v4).squaredNorm();
            // prefer triangles with less edge length
            float cut13 = (v13 < v24);
            /*
                 WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW v2
                 WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWP""W
                 WWWWWWWWWWWWW     WWWWWWWWWWWWWWP""    dW
                 WWWWWWWWWWWWW v12 WWWWWWWWW""'        dWW
                 WWWWWWWWWWWWW     WWP""'            dWWW
                 WWWWWWWWWWWWWWWWP""'                aWWWW
                 WWWWWWWWWWWP""                     _WWWWW
                 WWWWWWP""                         _WWWWWW 
                 W""'                             _WWWWWWW
              v1 Wa,                             _WWWWWWWW
                 WWWWa,                          W    WWWW
                 WWWWWWWa,         v13          jW v3 WWWW
                 WWWWWWWWWWa,                    W    WWWW
                 WWWWWWWWWWWWWa,                 jWWWWWWWW
                 WWWWWWWWWWWWWWWWa,               4WWWWWWW
                 WWWWWWWWWWWWWWWWWWWa,            "WWWWWWW
                 WWWWWWWWWWWWWW     WWWa,          4WWWWWW
                 WWWWWWWWWWWWWW v14 WWWWWWa,       "WWWWWW
                 WWWWWWWWWWWWWW     WWWWWWWWWa,     4WWWWW
                 WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWa,  "WWWWW
                 WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWa jWWWW
                 WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW 
                                                       v4                 
            */
            if ((v13 < v12) && (v13 < v14))
                 cut13 = true;
            if (cut13) {
                int appendList[8] = { 0, 1, 2, -1, 0, 2, 3, -1 };
                appendIndices(coordIndices,  m_coordIndex,  offset, appendList);
                appendIndices(normalIndices, m_normalIndex, offset, appendList);
                appendIndices(colorIndices,  m_colorIndex,  offset, appendList);
                appendIndices(texCoordIndices, m_texCoordIndex, offset, 
                              appendList);  
            } else {
                int appendList[8] = { 1, 2, 3, -1, 1, 3, 0, -1 };
                appendIndices(coordIndices,  m_coordIndex,  offset, appendList);
                appendIndices(normalIndices, m_normalIndex, offset, appendList);
                appendIndices(colorIndices,  m_colorIndex,  offset, appendList);
                appendIndices(texCoordIndices, m_texCoordIndex, offset, 
                              appendList);  
            }
        } else if (m_faces[i]->getNumVertices() == 3) {
            for (int j = 0; j < m_faces[i]->getNumVertices(); j++) {
                coordIndices->appendSFValue(m_coordIndex->getValue(offset + j));
                normalIndices->appendSFValue(m_normalIndex->getValue(offset + j)
                                            );
                colorIndices->appendSFValue(m_colorIndex->getValue(offset + j));
                texCoordIndices->appendSFValue(m_texCoordIndex->getValue(offset +
                                                                        j));
            }
            coordIndices->appendSFValue(-1);
            normalIndices->appendSFValue(-1);
            colorIndices->appendSFValue(-1);
            texCoordIndices->appendSFValue(-1);
        }
    }
    if (m_coordIndex) m_coordIndex->unref();
    m_coordIndex = coordIndices;
    if (m_normalIndex) m_normalIndex->unref();
    m_normalIndex = normalIndices;
    if (m_colorIndex) m_colorIndex->unref();
    m_colorIndex = colorIndices;
    if (m_texCoordIndex) m_texCoordIndex->unref();
    m_texCoordIndex = texCoordIndices;
    m_isTriangulated = true;
}

template <class X,class MFX,class VEC3X>
void                
MyMeshX<X, MFX, VEC3X>::addCoords(MFInt32 *coords, float *color, 
                                  MFFloat *colors, bool rgbaColor, 
                                  MFVec3f *normals, MFVec2f *texCoords)
{
    if (m_coordIndex == NULL)
        return;
    int colorInc = rgbaColor ? 4 : 3;
    if (coords->getSFSize() > 0) {
        if (m_colors == NULL)
            m_colors = new MFColorRGBA();
        else if (!m_colorRGBA) {
            MFColorRGBA *newColors = new MFColorRGBA();
            for (int i = 0; i < m_colors->getSize(); i += 3) {
                float colors[4] = { m_colors->getValue(i),
                                    m_colors->getValue(i + 1), 
                                    m_colors->getValue(i + 2), 1 };
                newColors->setSFValue(i, colors);
            }
            m_colors->unref();
            m_colors = newColors;
            m_colors->ref();
        } 
        m_colorRGBA = true;
        if (m_coordIndex->getValue(m_coordIndex->getSFSize() - 1) != -1)
            m_coordIndex->appendSFValue(-1);
        int numThings = 0;
        for (int i = 0; i < coords->getSFSize(); i++) {
            m_coordIndex->appendSFValue(coords->getValue(i));
            if (coords->getValue(i) > 0) {
                for (int j = 0; j < 4; j++) {
                    m_colors->setValue(coords->getValue(i) * 4 + j, color[j]);
                }
                if (colors && colors->getSize() > 0) {
                    for (int j = 0; j < 3; j++)
                        m_colors->setValue(coords->getValue(i) * 4 + j, 
                                           colors->getValue(numThings * 
                                                            colorInc + j));
                }
                if (rgbaColor)
                    m_colors->setValue(coords->getValue(i) * 4 + 3, 
                                       colors->getValue(numThings * 
                                                        colorInc + 3));
                else
                    m_colors->setValue(coords->getValue(i) * 4 + 3, 1);
                if (normals && normals->getSize() > 0) {
                    m_normals->setSFValue(coords->getValue(i), 
                                          normals->getSFValue(numThings));
                }
                if (texCoords && texCoords->getSize() > 0) {
                    m_texCoords[0]->setSFValue(coords->getValue(i), 
                                               texCoords->getSFValue(numThings)
                                              );
                }
            }
            numThings++;
        }
    }

    m_normalIndex->unref();
    m_normalIndex = m_coordIndex;
    m_normalIndex->ref();

    m_colorIndex->unref();
    m_colorIndex = m_coordIndex;
    m_colorIndex->ref();

    m_texCoordIndex->unref();
    m_texCoordIndex = m_coordIndex;
    m_texCoordIndex->ref();

    m_faces = NULL;
    m_numFaces = 0;
    buildFaces();
    generateFaceNormals();
    smoothNormals();
    m_listIndex = 0;
    m_drawCounter = 0;
    if (TheApp->disabledGlList())
        m_drawCounter = -1;
    m_currentFace = -1;
}

template <class X,class MFX,class VEC3X>
MyLine
MyMeshX<X, MFX, VEC3X>::getLine(int line)
{
    return m_lines[line];
}

template <class X,class MFX,class VEC3X>
int
MyMeshX<X, MFX, VEC3X>::getMeshFlags(void) {
    int meshFlags = 0;
    meshFlags |= m_isTriangulated ? MESH_TRIANGULATED : 0;
    meshFlags |= m_ccw ? MESH_CCW : 0;
    meshFlags |= m_solid ? MESH_SOLID : 0;
    meshFlags |= m_convex ? MESH_CONVEX : 0;
    meshFlags |= m_normalPerVertex ? MESH_NORMAL_PER_VERTEX : 0;
    meshFlags |= m_colorPerVertex ? MESH_COLOR_PER_VERTEX : 0;
    meshFlags |= m_colorRGBA ? MESH_COLOR_RGBA : 0;
    return meshFlags;
}

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::setMeshFlags(int meshFlags) {
    m_isTriangulated = meshFlags & MESH_TRIANGULATED;
    m_ccw = meshFlags & MESH_CCW;
    m_solid = meshFlags & MESH_SOLID;
    m_convex = meshFlags & MESH_CONVEX;
    m_normalPerVertex = meshFlags & MESH_NORMAL_PER_VERTEX;
    m_colorPerVertex = meshFlags & MESH_COLOR_PER_VERTEX;
    m_colorRGBA = meshFlags & MESH_COLOR_RGBA;
}

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::setVerticesZero(void)
{
     for (int i = 0; i < m_vertices->getSFSize(); i++) {
         m_vertices->setSFValue(i, 0, 0, 0);
         m_validVertices[i] = false;
     }
}

template <class X,class MFX,class VEC3X>
void
MyMeshX<X, MFX, VEC3X>::setValidVertex(int i)
{
     m_validVertices[i] = true;
}

template <class X,class MFX,class VEC3X>
int 
MyMeshX<X, MFX, VEC3X>::countPolygons(void)
{
    int ret = 0;
    MFInt32 *coordIndex = getCoordIndex();
    if (coordIndex != NULL) {     
        int lastCoordIndex = -1;
        for (int i = 0; i < coordIndex->getSize(); i++) {
            if ((coordIndex->getValue(i) == -1) && (lastCoordIndex != -1))
                ret++;
            lastCoordIndex = coordIndex->getValue(i);
        }
        // need to count last polygon ?
        if (coordIndex->getSize() > 2)
            if (coordIndex->getValue(coordIndex->getSize() - 1) != -1)
                ret++;
    }
    return ret;
}

template <class X,class MFX,class VEC3X>
int 
MyMeshX<X, MFX, VEC3X>::countPolygons2Sided(void)
{
    return m_node->isDoubleSided() ? 2 * countPolygons() : 0;
}

template <class X,class MFX,class VEC3X>
int 
MyMeshX<X, MFX, VEC3X>::countPolygons1Sided(void)
{
    return m_node->isDoubleSided() ? 0 : countPolygons();
}

template <class X,class MFX,class VEC3X>
Node * 
MyMeshX<X, MFX, VEC3X>::toIndexedFaceSet(int meshFlags, Scene *scene)
{
    bool wantNormal = (meshFlags & MESH_WANT_NORMAL);

    NodeCoordinate *ncoord = (NodeCoordinate *)scene->createNode("Coordinate");
    MFVec3f *points = new MFVec3f();
    MFX *vertices = getVertices();
    for (int i = 0; i < vertices->getSFSize(); i++) {
        VEC3X vec = vertices->getValue(i);
        Vec3f point(vec.x, vec.y, vec.z); 
        points->setVec(i, point);
    } 
    ncoord->point(points);
    NodeIndexedFaceSet *node = (NodeIndexedFaceSet *)
                               scene->createNode("IndexedFaceSet");
    node->coord(new SFNode(ncoord));
    if (wantNormal) {
        NodeNormal *nnormal = NULL;
        if (getSmoothNormals()) {
            nnormal = (NodeNormal *)scene->createNode("Normal");
            nnormal->vector(getSmoothNormals());
        }
        if (nnormal) {
            node->normal(new SFNode(nnormal));
            MFInt32 *ni = getSmoothNormalIndex();
            if (ni != NULL) {
                node->normalIndex(ni);
                node->normalIndex()->ref();
            }
        } 
    } 
    node->colorPerVertex(new SFBool(colorPerVertex()));
    if (getColorIndex())
        node->colorIndex(new MFInt32(getColorIndex()));
    node->normalPerVertex(new SFBool(normalPerVertex()));
    if (getNormalIndex())
        node->normalIndex(new MFInt32(getNormalIndex()));
    node->creaseAngle(new SFFloat(creaseAngle())); 
    node->coordIndex(new MFInt32(getCoordIndex()));
    node->solid(new SFBool(solid()));
    node->ccw(new SFBool(ccw()));
    node->convex(new SFBool(convex()));
    Node *nColor = NULL;
    if (hasColorRGBA()) {
        if (getColors()) {
            nColor = (NodeColorRGBA *)scene->createNode("ColorRGBA");
            ((NodeColorRGBA *)nColor)->color(
                new MFColorRGBA((float *)getColors()->getValues(), 
                                 getColors()->getSize()));
        }
    } else {
        if (getColors()) {
            nColor = (NodeColor *)scene->createNode("Color");
            ((NodeColor *)nColor)->color(
                new MFColor((float *)getColors()->getValues(), 
                            getColors()->getSize()));
        }
    }
    if (nColor) {
        node->color(new SFNode(nColor));
        node->color()->ref();
    }
    NodeTextureCoordinate *ntexCoord = NULL;
    if (getTexCoords()) {
        ntexCoord = (NodeTextureCoordinate *)
                    scene->createNode("TextureCoordinate");
        ntexCoord->point(getTexCoords());
    }
    if (ntexCoord) {
        node->texCoord(new SFNode(ntexCoord));
        node->texCoordIndex(getTexCoordIndex());
        node->texCoordIndex()->ref();
    }

    node->ref();

    if (hasColorRGBA())
        node->getMesh()->setColorRGBA();

    return node;
}

template <class X,class MFX,class VEC3X>
MFVec3f *
MyMeshX<X, MFX, VEC3X>::getSmoothNormals(void)
{
    MFVec3f *v = getNormals();
    
    if (v && (v->getValues() != NULL)) {
        v = new MFVec3f((float *)((MFVec3f *)v->copy())->getValues(), 
                        v->getSize());
    }
    return v;
}

template <class X,class MFX,class VEC3X>
MFInt32 *
MyMeshX<X, MFX, VEC3X>::getSmoothNormalIndex(void)
{
    MFInt32 *i = getNormalIndex();
    
    if (i && (i->getValues() != NULL)) {
        i =  new MFInt32((int *)((MFInt32 *)i->copy())->getValues(), 
                         i->getSize());
    }
    return i;
}


#ifndef HAVE_GLUNEWTESS
template <class X,class MFX,class VEC3X>
MyMeshX<X, MFX> *
MyMeshX<X, MFX, VEC3X>::triangulate(MeshBasedNode *that)
{
    simpleQuadTriangulate();
    return this;
}
#else
template <class X,class MFX,class VEC3X>
void                
MyMeshX<X, MFX, VEC3X>::startTessellation(GLenum type)
{
    m_tessellationType = type;
    m_tessellationStart = m_triangulatedVertices.size() / 3;
    m_evenTriangleStrip = false;
}

template <class X,class MFX,class VEC3X>
void 
MyMeshX<X, MFX, VEC3X>::addNewVertex(VertexInfo* vertexInfo)
{
    long vertexNumber = m_triangulatedVertices.size() / 3;
    if (m_tessellationType == GL_TRIANGLE_FAN) {
        // repeat indices of first and last vertices of triangle fan
        if (vertexNumber > (m_tessellationStart + 2)) {
            m_triangulatedIndices.append(-1);
            m_triangulatedIndices.append(m_tessellationStart);
            m_triangulatedIndices.append(vertexNumber - 1);
        }
    } else if (m_tessellationType == GL_TRIANGLE_STRIP) {
        // repeat indices of the two last vertices of triangle fan
        if (vertexNumber > (m_tessellationStart + 2)) {
            m_triangulatedIndices.append(-1);
            if (m_evenTriangleStrip) {
                m_triangulatedIndices.append(vertexNumber - 2);
                m_triangulatedIndices.append(vertexNumber - 1);
            } else {
                m_triangulatedIndices.append(vertexNumber - 1);
                m_triangulatedIndices.append(vertexNumber - 2);
            }
            m_evenTriangleStrip = ! m_evenTriangleStrip;
        }
    } else if (m_tessellationType == GL_TRIANGLES) {
        if (vertexNumber > (m_tessellationStart + 2)) {
            m_triangulatedIndices.append(-1);
            m_tessellationStart = vertexNumber;
        }
    }
   
    m_triangulatedIndices.append(vertexNumber);

    m_triangulatedVertices.append(vertexInfo->vertex[0]);
    m_triangulatedVertices.append(vertexInfo->vertex[1]);
    m_triangulatedVertices.append(vertexInfo->vertex[2]);

#ifdef HAVE_NULL_COMPARE
    if (vertexInfo->normal != NULL) {
#endif
        m_triangulatedNormals.append(vertexInfo->normal[0]);
        m_triangulatedNormals.append(vertexInfo->normal[1]);
        m_triangulatedNormals.append(vertexInfo->normal[2]);
#ifdef HAVE_NULL_COMPARE
    }
#endif

#ifdef HAVE_NULL_COMPARE
    if (vertexInfo->color != NULL) {
#endif
        m_triangulatedColors.append(vertexInfo->color[0]);
        m_triangulatedColors.append(vertexInfo->color[1]);
        m_triangulatedColors.append(vertexInfo->color[2]);
        m_triangulatedColors.append(vertexInfo->color[3]);
#ifdef HAVE_NULL_COMPARE
    }
#endif

#ifdef HAVE_NULL_COMPARE
    if (vertexInfo->texCoord != NULL) {
#endif
        m_triangulatedTexCoords.append(vertexInfo->texCoord[0]);
        m_triangulatedTexCoords.append(vertexInfo->texCoord[1]);
#ifdef HAVE_NULL_COMPARE
    }
#endif
}

template <class X,class MFX,class VEC3X>
void 
MyMeshX<X, MFX, VEC3X>::endNewPolygon(void) 
{
    m_triangulatedIndices.append(-1);
}

template <class X,class MFX,class VEC3X>
void MyMeshX<X, MFX, VEC3X>::collectMemoryToFree(VertexInfo * vertexInfo)
{
    m_newMemory4VerticesInfo.append(vertexInfo);
}

template <class X,class MFX,class VEC3X>
void 
MyMeshX<X, MFX, VEC3X>::initialiseTriangulation(GLUtesselator *tess)
{
    gluTessCallback(tess, GLU_TESS_BEGIN_DATA, 
                    (MY_GLU_CALLBACK_PTR)beginTriangleCallback);
    gluTessCallback(tess, GLU_TESS_VERTEX_DATA, 
                    (MY_GLU_CALLBACK_PTR)newVertexCallback);
    gluTessCallback(tess, GLU_TESS_END_DATA, 
                    (MY_GLU_CALLBACK_PTR)endTriangleCallback);
    gluTessCallback(tess, GLU_TESS_COMBINE_DATA, 
                    (MY_GLU_CALLBACK_PTR)combineVertices);
#ifdef _WIN32
    gluTessCallback(tess, GLU_TESS_ERROR, 
                    (MY_GLU_CALLBACK_PTR)tesselationError);
#endif
    m_evenTriangleStrip = false;
}

template <class X,class MFX,class VEC3X>
MyMeshX<X, MFX, VEC3X> *
MyMeshX<X, MFX, VEC3X>::buildNewTriangulatedMesh(MeshBasedNode *that)
{
    MFX *vertices = new MFX(m_triangulatedVertices.size() / 3);
    for (long i = 0; i < m_triangulatedVertices.size(); i++)
        vertices->setValue(i, m_triangulatedVertices[i]); 

    MFInt32 *coordIndex = new MFInt32(m_triangulatedIndices.size());
    for (long i = 0; i < m_triangulatedIndices.size(); i++)
        coordIndex->setSFValue(i, m_triangulatedIndices[i]);

    MFVec3f *normals = NULL;
    if (m_triangulatedHasNormals) {
        normals = new MFVec3f(m_triangulatedNormals.size() / 3);
        for (long i = 0; i < m_triangulatedNormals.size(); i++)
            normals->setValue(i, m_triangulatedNormals[i]); 
    }
    MFInt32 *normalIndex = NULL;

    MFColor *colors = NULL;
    if (m_triangulatedHasColors) {
        colors = new MFColor(m_triangulatedColors.size() / 4);
        for (long i = 0; i < m_triangulatedColors.size(); i++)
            colors->setValue(i, m_triangulatedColors[i]); 
    }
    MFColorRGBA *colorsRGBA = NULL;
    if (m_triangulatedHasColorsRGBA) {
        colorsRGBA = new MFColorRGBA(m_triangulatedColors.size() / 4);
        for (long i = 0; i < m_triangulatedColors.size(); i++)
            colorsRGBA->setValue(i, m_triangulatedColors[i]); 
    }    
    MFInt32 *colorIndex = NULL;

    MFVec2f *texCoord = NULL;
    if (m_triangulatedHasTexCoords) {
        texCoord = new MFVec2f(m_triangulatedTexCoords.size() / 2);
        for (long i = 0; i < m_triangulatedTexCoords.size(); i++)
            texCoord->setValue(i, m_triangulatedTexCoords[i]); 
    }
    if (texCoord == NULL)
        texCoord = ::generateTextureCoordinates(vertices, coordIndex, (X)0.0f);
    MyArray<MFVec2f *> texCoords;
    texCoords.append(texCoord);
    MFInt32 *texCoordIndex = NULL;

    float creaseAngle = m_creaseAngle;
    int meshFlags = 0;
    if (m_ccw)
        meshFlags |= MESH_CCW;
    if (m_solid)
        meshFlags |= MESH_SOLID;
    if (m_convex)
        meshFlags |= MESH_CONVEX;
    if (m_colorPerVertex)
        meshFlags |= MESH_COLOR_PER_VERTEX;
    if (m_normalPerVertex)
        meshFlags |= MESH_NORMAL_PER_VERTEX;
    if (m_triangulatedHasColorsRGBA)
        meshFlags |= MESH_COLOR_RGBA;
    float transparency = m_transparency;

    return new MyMeshX(that,
                       vertices, coordIndex, normals, normalIndex, colors, 
                       colorIndex, texCoords, texCoordIndex,
                       creaseAngle, meshFlags, transparency);
}

template <class X,class MFX,class VEC3X>
MyMeshX<X, MFX, VEC3X> *
MyMeshX<X, MFX, VEC3X>::triangulate(MeshBasedNode *that)
{
    m_triangulatedVertices.resize(0);
    m_triangulatedIndices.resize(0);
    m_triangulatedNormals.resize(0);
    m_triangulatedColors.resize(0);
    m_triangulatedTexCoords.resize(0);

    GLUtesselator *tess = gluNewTess();

    if (tess == NULL)
        return NULL;

    initialiseTriangulation(tess);

    const int  *coordIndex = m_coordIndex->getValues();

    const float *normals = m_normals ? m_normals->getValues() : NULL;
    const int *normalIndex = m_normalIndex ? m_normalIndex->getValues() : NULL;

    const float *colors = m_colors ? m_colors->getValues() : NULL;
    const int *colorIndex = m_colorIndex ? m_colorIndex->getValues() : NULL;

    const float *texCoords = (m_texCoords.size() > 0) ? 
                             m_texCoords[0]->getValues() : NULL;
    const int *texCoordIndex = m_texCoordIndex ? m_texCoordIndex->getValues() 
                                                : NULL;

    m_triangulatedHasNormals = normals ? true : false;

    m_triangulatedHasColors = colors ? true : false;
    m_triangulatedHasColorsRGBA = m_triangulatedHasColors && m_colorRGBA;
    if (m_colorRGBA)
        m_triangulatedHasColors = false;

    m_triangulatedHasTexCoords = (texCoords && texCoordIndex) ? true : false;
  
    for (int i = 0; i < m_numFaces; i++) {
        m_currentFace = i;
        int offset = m_faces[i]->getOffset();
        int numVertices = m_faces[i]->getNumVertices();
        if (numVertices < 3) 
            continue;

        int colorStride = m_colorRGBA ? 4 : 3; 
        m_verticesInfo.resize(0);
        int j;
        for (j = offset; j < offset + numVertices; j++) {
            if ((coordIndex[j] * 3) >= m_vertices->getSize())
                continue;
            VertexInfo vertexInfo;
            const X *vertices = m_vertices->getValue(coordIndex[j]);
            vertexInfo.vertex[0] = vertices[0];
            vertexInfo.vertex[1] = vertices[1];
            vertexInfo.vertex[2] = vertices[2];
            if (normals) {
                int index = -1; 
                if (m_normalPerVertex) {
                    if (j < m_normalIndex->getSize())
                          index = normalIndex[j] * 3;
                } else {
                    if (m_normalIndex && (m_normalIndex->getSize() > 0)) {
                        if (i < m_normalIndex->getSize())
                            index = normalIndex[i] * 3;
                    } else
                       index = i * 3;
                } 
                if ((index >= 0) && (index < m_normals->getSize())) {
                    vertexInfo.normal[0] = normals[index];
                    vertexInfo.normal[1] = normals[index + 1];
                    vertexInfo.normal[2] = normals[index + 2];
                } else
                    m_triangulatedHasNormals = false; // invalid Normal node
            }
            if (colors) {
                int index = -1; 
                if (m_colorPerVertex) {
                    if (j < m_colorIndex->getSize())
                        index = colorIndex[j] * colorStride;
                } else {
                    if (m_colorIndex && (m_colorIndex->getSize() > 0)) {
                        if (i < m_colorIndex->getSize())
                               index = colorIndex[i] * colorStride;
                    } else
                        index = i * colorStride;
                }
                if ((index >= 0) && (index < m_colors->getSize())) {
                    vertexInfo.color[0] = colors[index];
                    vertexInfo.color[1] = colors[index + 1];
                    vertexInfo.color[2] = colors[index + 2];
                    if (m_colorRGBA) 
                        vertexInfo.color[3] = colors[index + 3];
                    else
                        vertexInfo.color[3] = 1 - m_transparency;
                } else {
                    // invalid Color node
                    m_triangulatedHasColors = false;
                    m_triangulatedHasColorsRGBA = false; 
                }
            }

            if (texCoords && texCoordIndex &&
                (j < m_texCoordIndex->getSFSize()) &&
                (texCoordIndex[j] < m_texCoords[0]->getSFSize())) {
                 vertexInfo.texCoord[0] = texCoords[texCoordIndex[j] * 2];
                 vertexInfo.texCoord[1] = texCoords[texCoordIndex[j] * 2 + 1];
            }
            m_verticesInfo.append(vertexInfo);
        }

        gluTessBeginPolygon(tess, this);
        gluTessBeginContour(tess);
        for (j = 0; j < numVertices; j++) {
            gluTessVertex(tess, (GLdouble*)
                                &(m_verticesInfo[j].vertex), 
                                &(m_verticesInfo[j].vertex));
        }
        gluTessEndContour(tess);
        gluTessEndPolygon(tess);
    }

    // destroy the tesselation object
    gluDeleteTess(tess);

    m_verticesInfo.resize(0);
    for (List<VertexInfo *>::Iterator *it = m_newMemory4VerticesInfo.first(); 
         it != NULL; it = it->next())
        delete it->item();

    m_isTriangulated = true;
    return buildNewTriangulatedMesh(that);
}

#endif



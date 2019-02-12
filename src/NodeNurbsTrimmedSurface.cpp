/*
 * NodeNurbsTrimmedSurface.cpp
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
#include "stdafx.h"

#ifdef HAVE_FPCLASSIFY 
# include <math.h>
#endif

#include "NodeNurbsTrimmedSurface.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "Scene.h"
#include "NodeTrimmedSurface.h"
#include "MoveCommand.h"
#include "Util.h"
#include "NodeContour2D.h"
#include "NodeNurbsCurve2D.h"
#include "NodeContourPolyline2D.h"
#include "NodePolyline2D.h"
#include "MyMesh.h"
 
ProtoNurbsTrimmedSurface::ProtoNurbsTrimmedSurface(Scene *scene)
  : ProtoNurbsSurface(scene, "NurbsTrimmedSurface")
{                   
    addEventIn(MFNODE, "addTrimmingContour");
    addEventIn(MFNODE, "removeTrimmingContour");
    trimmingContour.set(
          addExposedField(MFNODE, "trimmingContour", new MFNode(), 
                          VRML_CONTOUR_2D));
    addURLs(URL_NULL);
}

Node *
ProtoNurbsTrimmedSurface::create(Scene *scene)
{ 
    return new NodeNurbsTrimmedSurface(scene, this); 
}

static int tesselationType = 0;
static MyArray<float> vertices;
static MyArray<int> coordIndices;
static float boundingBoxMax[3] = { 0, 0, 0 };
static float boundingBoxMin[3] = { 0, 0, 0 };
static int startIndex;
static int currentIndex;
static int outOfBoundingBox;

static void vertexTess(GLfloat *v)
{
#ifdef HAVE_GLU_NURBS_TESSELLATOR
    if ((v[0] > boundingBoxMax[0]) ||
        (v[1] > boundingBoxMax[1]) ||
        (v[2] > boundingBoxMax[2]) ||
        (v[0] < boundingBoxMin[0]) ||
        (v[1] < boundingBoxMin[1]) ||
        (v[2] < boundingBoxMin[2])) {
        outOfBoundingBox++;
        return;
    }
#ifndef _WIN32
# ifdef HAVE_FPCLASSIFY 
    if ((fpclassify(v[0]) != FP_ZERO) || isnormal(v[0]) ||
        (fpclassify(v[1]) != FP_ZERO) || isnormal(v[1]) ||
        (fpclassify(v[2]) != FP_ZERO) || isnormal(v[2])) {
        outOfBoundingBox++;
        return;
    }
# endif
#endif
       
    vertices.append(v[0]);
    vertices.append(v[1]);
    vertices.append(v[2]);

    switch (tesselationType) {
      case GL_TRIANGLES:
        coordIndices.append(currentIndex);        
        if (currentIndex % 3 == 2)
            coordIndices.append(-1);        
        break;
      case GL_TRIANGLE_FAN:
        if (currentIndex == 2) {
            coordIndices.append(startIndex);
            coordIndices.append(startIndex + 1);
            coordIndices.append(startIndex + 2);
            coordIndices.append(-1);
        } else if ((currentIndex > 2) && (currentIndex % 2 == 0)) {
            coordIndices.append(startIndex);
            coordIndices.append(startIndex + currentIndex);
        } else if (currentIndex > 2) {
            coordIndices.append(startIndex + currentIndex);
            coordIndices.append(-1);
        }
        break;
      case GL_TRIANGLE_STRIP:
        if (currentIndex == 2) {
            coordIndices.append(startIndex + 0);
            coordIndices.append(startIndex + 1);
            coordIndices.append(startIndex + 2);
            coordIndices.append(-1);
        } else if (currentIndex > 2) {
            coordIndices.append(startIndex + currentIndex - 2);
            coordIndices.append(startIndex + currentIndex - 1);
            coordIndices.append(startIndex + currentIndex);
            coordIndices.append(-1);
        }        
        break;
      case GL_QUAD_STRIP:
        if (currentIndex == 3) {
            coordIndices.append(startIndex + 1);
            coordIndices.append(startIndex + 0);
            coordIndices.append(startIndex + 2);
            coordIndices.append(startIndex + 3);
            coordIndices.append(-1);
        } else if ((currentIndex > 3) && ((currentIndex) % 2 == 1)) {
            coordIndices.append(startIndex + currentIndex - 2);
            coordIndices.append(startIndex + currentIndex - 3);
            coordIndices.append(startIndex + currentIndex - 1);
            coordIndices.append(startIndex + currentIndex);
            coordIndices.append(-1);
        }   
        break;
      default:
        assert(0);
    }
    currentIndex++;
#endif
}

static void beginTess(GLenum type)
{
    tesselationType = type;
    startIndex += currentIndex;
    currentIndex = 0;
}


static void endTess(void)
{
#ifndef HAVE_GLU_NURBS_TESSELLATOR
    switch (tesselationType) {
      case GL_TRIANGLES:
        glBegin(GL_TRIANGLES);
        break;
      case GL_TRIANGLE_FAN:
        glBegin(GL_TRIANGLE_FAN);
        break;
      case GL_TRIANGLE_STRIP:
        glBegin(GL_TRIANGLE_STRIP);
        break;
      case GL_QUAD_STRIP:
        glBegin(GL_QUAD_STRIP);
        break;
      default:
        assert(0);
    }
    for (int i = 0; i < vertices.size(); i += 3)
        glVertex3fv((GLfloat *)(vertices.getData() + i));
    glEnd();
#endif
}

static void errorTess(GLenum err)
{
    swDebugf("NurbsTrimmedSurface error: %s\n", gluErrorString(err));
}

NodeNurbsTrimmedSurface::NodeNurbsTrimmedSurface(Scene *scene, Proto *def)
  : NodeNurbsSurface(scene, def)
{
    m_theNurb = gluNewNurbsRenderer(); 
    gluNurbsProperty(m_theNurb, GLU_SAMPLING_TOLERANCE, 50.0); 
    gluNurbsProperty(m_theNurb, GLU_DISPLAY_MODE, GLU_FILL); 
#ifdef HAVE_GLU_NURBS_TESSELLATOR
    gluNurbsProperty(m_theNurb, GLU_NURBS_MODE, GLU_NURBS_TESSELLATOR);
    gluNurbsCallback(m_theNurb, GLU_NURBS_VERTEX, (GLvoid (*)())vertexTess);
    gluNurbsCallback(m_theNurb, GLU_NURBS_BEGIN, (GLvoid (*)())beginTess);
    gluNurbsCallback(m_theNurb, GLU_NURBS_END, (GLvoid (*)())endTess);
    gluNurbsCallback(m_theNurb, GLU_NURBS_ERROR, (GLvoid (*)())errorTess);
    gluNurbsProperty(m_theNurb, GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
#endif
    m_meshDirty = true;
}

Node *
NodeNurbsTrimmedSurface::toTrimmedSurface(void)
{
    NodeTrimmedSurface *ret = (NodeTrimmedSurface *)
                              m_scene->createNode("TrimmedSurface");
    NodeNurbsSurface *nsurface = (NodeNurbsSurface *)
                                 ((NodeNurbsSurface *)this)->copy();

    nsurface->NodeNurbsSurface::convert2Vrml();
    
    m_scene->execute(new MoveCommand(nsurface, NULL, -1, ret,
                                    ret->surface_Field()));


    m_scene->changeRoutes(this, this->controlPoint_Field(), 
                         nsurface, nsurface->controlPoint_Field());        
    return ret;
}

Node *
NodeNurbsTrimmedSurface::convert2Vrml(void)
{
    NodeTrimmedSurface *ret = (NodeTrimmedSurface *)toTrimmedSurface();

    NodeList *trimm = trimmingContour()->getValues();
    for (int i = 0; i < trimm->size(); i++)
        m_scene->execute(new MoveCommand(trimm->get(i), NULL, -1, ret, 
                                        ret->trimmingContour_Field()));
    return ret;    
}

void
NodeNurbsTrimmedSurface::draw(void)
{
#ifdef HAVE_GLU_NURBS_TESSELLATOR
    if (m_meshDirty || (m_mesh == NULL)) {
        createMesh();
    }
    if (!m_meshDirty && (m_mesh != NULL))
        m_mesh->draw();
#else
    glPushMatrix(); 

    if (solid()->getValue()) {
        glEnable(GL_CULL_FACE);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    } else {
        glDisable(GL_CULL_FACE);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    }
    
    glEnable(GL_LIGHTING);

    drawOrCreateMesh();

    glPopMatrix(); 
#endif
}

void 
NodeNurbsTrimmedSurface::createMesh(bool cleanDoubleVertices, 
                                    bool triangulateMesh)
{
#ifdef HAVE_GLU_NURBS_TESSELLATOR
    outOfBoundingBox = 0;

    if (swHasVisual() == 0) {
        swDebugf("Sorry, TrimmedSurface conversion needs OpenGL\n");
        return;
    }

    vertices.resize(0);
    coordIndices.resize(0);

    drawOrCreateMesh();

    MFVec3f *coords = new MFVec3f(vertices.getData(), vertices.size());
    MFInt32 *coordIndex = new MFInt32((int *)coordIndices.getData(), 
                                      coordIndices.size());
    MyArray<MFVec2f *> texCoords;

    int meshFlags = 0;
    float transparency = 0;
    if (hasParent())
        transparency = getParent()->getTransparency();
    if (solid()->getValue())
        meshFlags |= MESH_SOLID;

    m_mesh = new MyMesh(this, (MFVec3f *)coords->copy(), 
                        (MFInt32 *)coordIndex->copy(), NULL, NULL, NULL, NULL, 
                        texCoords, NULL, M_PI / 2.0, transparency, false);

    m_meshDirty = false;

#ifdef DEBUG
    if (outOfBoundingBox > 0)
        swDebugf("sorted out %d vertices\n", outOfBoundingBox);
#endif
#endif
}

#ifdef _WIN32
# define USE_NEW 1
#endif

#define CP_SIZE 4

void
NodeNurbsTrimmedSurface::drawOrCreateMesh(void)
{
    startIndex = 0;
    currentIndex = 0;
    if (solid()->getValue()) {
        gluNurbsProperty(m_theNurb, GLU_CULLING, GL_TRUE); 
    } else {
        gluNurbsProperty(m_theNurb, GLU_CULLING, GL_FALSE); 
    }

    int uk = uKnot()->getSize();
    int u = uk - uOrder()->getValue(); 
    int vk = vKnot()->getSize();
    int v = vk - vOrder()->getValue(); 
    static int cpAndWSFSize;
    cpAndWSFSize = getControlPoints()->getSFSize();
    if (weight()->getSize() < cpAndWSFSize)
        return;
#ifdef USE_NEW
    GLfloat *cpAndW = new GLfloat[u * v * CP_SIZE];
#else
    GLfloat cpAndW[u * v * CP_SIZE];
#endif
    const float *cp = getControlPoints()->getValues();
    const float *w = weight()->getValues();
    if (cpAndWSFSize > 0)
        for (int i = 0; i < 3; i++)  {
            boundingBoxMax[i] = cp[i] + 0.1;        
            boundingBoxMin[i] = cp[i] - 0.1;        
        }
    int count = 0;
    for (int i = 0; i < u; i++) {
        for (int j = 0; j < v; j++) {
            int m = j * u + i; 
            float w0 = (w[m] > 0.0) ? w[m] : 1.0; 
            for (int n = 0; n < 3; n++)
#if CP_SIZE == 4
                cpAndW[count * CP_SIZE + n] = cp[m * 3 + n];
#else
                cpAndW[count * CP_SIZE + n] = cp[m * 3 + n] / w0;
#endif
#if CP_SIZE == 4
            cpAndW[count * CP_SIZE + 3] = w0;
#endif
            count++;
        }
    }
    for (int i = 0; i < cpAndWSFSize; i++) {
        float w0 = (w[i] > 0.0) ? w[i] : 1.0; 
        for (int j = 0; j < 3; j++) {
            float p = cp[i * 3 + j] / w[i];
            if (p > boundingBoxMax[j])
                boundingBoxMax[j] = p + 0.1f;
            if (p < boundingBoxMin[j])
                boundingBoxMin[j] = p - 0.1f;
        }
    }
#ifdef HAVE_GLU_NURBS_TESSELLATOR
    gluNurbsProperty(m_theNurb, GLU_U_STEP, uTessellation()->getValue() == 0 ?
                                            32 : 
                                            uTessellation()->getValue() - 1);
    gluNurbsProperty(m_theNurb, GLU_V_STEP, vTessellation()->getValue() == 0 ?
                                            32 : 
                                            vTessellation()->getValue() - 1);
#endif
    gluBeginSurface(m_theNurb); 
    gluNurbsSurface(m_theNurb, uk, (GLfloat *)uKnot()->getValues(), 
                               vk, (GLfloat *)vKnot()->getValues(), 
                               vDimension()->getValue() * CP_SIZE, CP_SIZE,
                               cpAndW, 
                               uOrder()->getValue(), vOrder()->getValue(), 
#if CP_SIZE == 4
                               GL_MAP2_VERTEX_4); 
#else
                               GL_MAP2_VERTEX_3); 
#endif

#ifndef HAVE_GLU_NURBS_TESSELLATOR
    gluNurbsSurface(m_theNurb, uKnot()->getSize(), 
                               (GLfloat *)uKnot()->getValues(), 
                               vKnot()->getSize(), 
                               (GLfloat *)vKnot()->getValues(), 
                               uDimension()->getValue() * CP_SIZE, CP_SIZE,
                               cpAndW, 
                               uOrder()->getValue(), vOrder()->getValue(), 
                               GL_MAP2_NORMAL); 
#endif

    MyArray<float*> point;

    MFNode *nodes = trimmingContour();
    if (nodes)
        for (int j = 0; j < nodes->getSize(); j++) {
            NodeContour2D *contour = (NodeContour2D *)
                                     trimmingContour()->getValue(j); 
            if ((contour != NULL) && 
                (contour->getType() == VRML_CONTOUR_2D)) {

                if (contour->children()->getSize() > 0)
                    gluBeginTrim(m_theNurb); 

                for (int i = 0; i < contour->children()->getSize(); i++) {
                    Node *node = contour->children()->getValue(i);
                    if (node->getType() == VRML_NURBS_CURVE_2D) {
                        NodeNurbsCurve2D *curve = (NodeNurbsCurve2D *)node;
                        MFVec2d *p = curve->controlPoint();
                        point.append(new float[p->getSFSize() * 3]);
                        int j = point.size() - 1;
                        for (int i = 0; i < p->getSFSize(); i++) {
                             float w = curve->weight()->getValue(i);
                             point[j][i * 3] = p->getValue(i)[0];
                             point[j][i * 3 + 1] = p->getValue(i)[1];
                             point[j][i * 3 + 2] = w;
                        }
                        MFFloat *curveKnot = curve->knot();
        
                        gluNurbsCurve(m_theNurb, curveKnot->getSize(), 
                                      (float *)curveKnot->getValues(), 3, 
                                      point[j], curve->order()->getValue(), 
                                      GLU_MAP1_TRIM_3); 
                    } else if ((node->getType() == X3D_CONTOUR_POLYLINE_2D) || 
                               (node->getType() == VRML_POLYLINE_2D)) {
                        if (node->getType() == X3D_CONTOUR_POLYLINE_2D) {
                            MFVec2d *p = ((NodeContourPolyline2D *)node)->
                                         lineSegments();
                            if (p->getSFSize() < 3)
                                continue;
                            point.append(new float[p->getSFSize() * 2]);
                            int j = point.size() - 1;
                            for (int i = 0; i < p->getSFSize(); i++) {
                                point[j][i * 2] = p->getValue(i)[0];
                                point[j][i * 2 + 1] = p->getValue(i)[1];
                            }
                            gluPwlCurve(m_theNurb, p->getSFSize(), point[j], 
                                        2, GLU_MAP1_TRIM_2); 
                        } else if (node->getType() == VRML_POLYLINE_2D) {
                            MFVec2f *p = ((NodePolyline2D *)node)->point();
                            if (p->getSFSize() < 3)
                                continue;
                            gluPwlCurve(m_theNurb, p->getSFSize(), 
                                        (GLfloat *)p->getValues(), 2,
                                        GLU_MAP1_TRIM_2); 
                        }
                    }
                }

                if (contour->children()->getSize() > 0)
                    gluEndTrim(m_theNurb); 
            }
        }

    gluEndSurface(m_theNurb); 
    m_meshDirty = false;
    for (int i = 0; i < point.size(); i++)
        delete [] point[i];
#ifdef USE_NEW
    delete [] cpAndW;
#endif
}

void    
NodeNurbsTrimmedSurface::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
    NodeNurbsSurface::setField(index, value, cf);
}

/*
 * NodeNurbsCurve2D.cpp
 *
 * Copyright (C) 2003 Th. Rothermel
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "resource.h"

#ifndef _WIN32
# include "stdlib.h"
#endif

#include "stdafx.h"
#include "NodeNurbsCurve2D.h"
#include "Scene.h"
#include "FieldValue.h"
#include "SFInt32.h"
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
#include "NodeNurbsCurve.h"
#include "Util.h"

ProtoNurbsCurve2D::ProtoNurbsCurve2D(Scene *scene)
  : Proto(scene, "NurbsCurve2D")
{
    if (scene->getParsedX3dVersion() != 0)
        controlPoint.set(
             addExposedField(MFVEC2D, "controlPoint", new MFVec2d()));
    else
        controlPoint.set(
             addExposedField(MFVEC2F, "controlPoint", new MFVec2f()));

    tessellation.set(
          addExposedField(SFINT32, "tessellation", new SFInt32(0)));
    weight.set(
          addExposedField(MFFLOAT, "weight", new MFFloat(), new SFFloat(0.0f)));
    closed.set(
          addField(SFBOOL, "closed", new SFBool(false)));
    setFieldFlags(closed, FF_X3D_ONLY);
    knot.set(
          addField(MFFLOAT, "knot", new MFFloat()));
    order.set(
          addField(SFINT32, "order", new SFInt32(3), new SFInt32(2)));

    addURLs(URL_X3D);
}

Node *
ProtoNurbsCurve2D::create(Scene *scene)
{
    return new NodeNurbsCurve2D(scene, this); 
}

NodeNurbsCurve2D::NodeNurbsCurve2D(Scene *scene, Proto *proto)
  : Node(scene, proto)
{
    m_isInternal = false;
    m_chain.resize(0);
    m_chainDirty = true;
    m_handleRendering = HANDLE_RENDERING_XY;
}

void
NodeNurbsCurve2D::setField(int index, FieldValue *value, int cf) 
{
    m_chainDirty = true;
    Node::setField(index, value, cf);
    update();
}

int
NodeNurbsCurve2D::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f, "EXTERNPROTO NurbsCurve2D[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f ," ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:web3d:vrml97:node:NurbsCurve2D\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:inet:blaxxun.com:node:NurbsCurve2D\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:ParaGraph:NurbsCurve2D\",\n") )
    TheApp->incSelectionLinenumber();
#ifdef HAVE_VRML97_AMENDMENT1_PROTO_URL
    RET_ONERROR( mywritestr(f, " \"") )
    RET_ONERROR( mywritestr(f, HAVE_VRML97_AMENDMENT1_PROTO_URL) )
    RET_ONERROR( mywritestr(f, "/NurbsCurve2DPROTO.wrl") )
    RET_ONERROR( mywritestr(f, "\"\n") )
    TheApp->incSelectionLinenumber();
#else
    RET_ONERROR( mywritestr(f ," \"NurbsCurve2DPROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#endif
    RET_ONERROR( mywritestr(f ," \"http://wdune.ourproject.org/docs/vrml97Amendment1/NurbsCurve2DPROTO.wrl\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int             
NodeNurbsCurve2D::write(int filedes, int indent, bool avoidUse)
{
    if (!m_scene->isPureVRML())
        RET_ONERROR( Node::write(filedes, indent, avoidUse) )
    return 0;
}

void
NodeNurbsCurve2D::drawHandles()
{
    int iDimension = controlPoint()->getSize() / 2;
    RenderState state;

    if (weight()->getSize() != iDimension) {
        return;
    }

    glPushName(iDimension + 1);
    glDisable(GL_LIGHTING);
    Util::myGlColor3f(1.0f, 1.0f, 1.0f);
    glLoadName(NO_HANDLE);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < iDimension; i++) {
        const double *v = controlPoint()->getValue(i);
        float w = weight()->getValue(i);
        switch (m_handleRendering) {
          case HANDLE_RENDERING_YZ:
            glVertex3d(0, v[0] / w, v[1] / w);
            break;
          case HANDLE_RENDERING_ZY:
            glVertex3d(0, v[1] / w, v[0] / w);
            break;
          case HANDLE_RENDERING_XZ:
            glVertex3d(v[0] / w, 0, v[1] / w);
            break;
          default:
            glVertex3d(v[0] / w, v[1] / w, 0);
        }
    }
    glEnd();
    
    int ci;

    state.startDrawHandles();
    for (ci = 0; ci < iDimension; ci++) {
        state.setHandleColor(m_scene, ci);
        glLoadName(ci);
        const double *v = controlPoint()->getValue(ci);
        float w = weight()->getValue(ci);
        switch (m_handleRendering) {
          case HANDLE_RENDERING_YZ:
            state.drawHandle(Vec3f(0, (float)v[0] / w, (float)v[1] / w));
            break;
          case HANDLE_RENDERING_ZY:
            state.drawHandle(Vec3f(0, (float)v[1] / w, (float)v[0] / w));
            break;
          case HANDLE_RENDERING_XZ:
            state.drawHandle(Vec3f((float)v[0] / w, 0, (float)v[1] / w));
            break;
          default:
            state.drawHandle(Vec3f((float)v[0] / w, (float)v[1] / w, 0));
        }
    }
    state.endDrawHandles();
    glPopName();
    glEnable(GL_LIGHTING);
}


Vec3f
NodeNurbsCurve2D::getHandle(int handle, int *constraint, int *field)
{
    *field = controlPoint_Field() ;

    if (handle >= 0 && handle < controlPoint()->getSize() / 2) {
        const double *p = controlPoint()->getValue(handle);
        float w = weight()->getValue(handle);
        if (w != 0) {
            switch (m_handleRendering) {
              case HANDLE_RENDERING_YZ:
                {                
                Vec3f ret(0, p[0] / w, p[1] / w);
                TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, 
                                              "controlPoint", handle, ret);
                return ret;
                }
                break;
              case HANDLE_RENDERING_ZY:
                {                
                Vec3f ret(0, p[1] / w, p[0] / w);
                TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, 
                                              "controlPoint", handle, ret);
                return ret;
                }
                break;
              case HANDLE_RENDERING_XZ:
                {                
                Vec3f ret(p[0] / w, 0, p[1] / w);
                TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, 
                                              "controlPoint", handle, ret);
                return ret;
                }
                break;
              default:
                {                
                Vec3f ret(p[0] / w, p[1] / w, 0);
                TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, 
                                              "controlPoint", handle, ret);
                return ret;
                }
            }
        }
    } 
    *field = -1;
    return Vec3f(0.0f, 0.0f, 0.0f);
}

struct VecMapper {
    int index;
    Vec2d vec;
};

void
NodeNurbsCurve2D::setHandle(int handle, const Vec3f &v)
{
    float epsilon = TheApp->GetHandleEpsilon();
    MFVec2d *newValue = new MFVec2d(*controlPoint());

    int numPoints = controlPoint()->getSize() / 2; 
    if (handle >= 0 && handle < numPoints) {
        Vec2d vec =newValue->getVec(handle);
        float w = weight()->getValue(handle);
        if (w != 0) {
           switch (m_handleRendering) {
              case HANDLE_RENDERING_YZ:
                newValue->setValue(handle * 2    , v.y * w);
                newValue->setValue(handle * 2 + 1, v.z * w);
                break;
              case HANDLE_RENDERING_ZY:
                newValue->setValue(handle * 2    , v.z * w);
                newValue->setValue(handle * 2 + 1, v.y * w);
                break;
              case HANDLE_RENDERING_XZ:
                newValue->setValue(handle * 2    , v.x * w);
                newValue->setValue(handle * 2 + 1, v.z * w);
                break;
              default:
                newValue->setValue(handle * 2    , v.x * w);
                newValue->setValue(handle * 2 + 1, v.y * w);
            }
            MyArray <VecMapper> sameChanges;
            MyArray <VecMapper> symChanges;
            for (int i = 0; i < numPoints; i++) {
                VecMapper vecMap;
                if (m_scene->getXSymetricMode() && (i != handle)) {
                    if ((fabs(vec.x + newValue->getVec(i).x) < epsilon) &&
                        (fabs(vec.y - newValue->getVec(i).y) < epsilon)) {
                            Vec2d vecSym;
                            vecSym.x = -(newValue->getVec(handle).x);
                            vecSym.y =   newValue->getVec(handle).y;
                            vecMap.index = i;
                            vecMap.vec = vecSym;
                            symChanges.append(vecMap);
                    }
                }
                // keep a NurbsCurve2d closed in any situation, 
                // otherwise libGLU will crash in NurbsTrimmedSurface
                if (i != handle) {
                    if ((fabs(vec.x - newValue->getVec(i).x) < epsilon) &&
                        (fabs(vec.y - newValue->getVec(i).y) < epsilon)) {
                            vecMap.index = i;
                            vecMap.vec = newValue->getVec(handle);
                            sameChanges.append(vecMap);
                    }                    
                }
            }
            for (long j = 0; j < sameChanges.size(); j++) {
                newValue->setVec(sameChanges[j].index, sameChanges[j].vec);
            }
            for (long j = 0; j < symChanges.size(); j++) {
                bool flag = false;
                for (long k = 0; k < sameChanges.size(); k++)
                    if (sameChanges[k].index == (int)j)
                        flag = true;
                if (flag)
                    continue; 
                newValue->setVec(symChanges[j].index, symChanges[j].vec);
            }
            m_scene->setField(this, controlPoint_Field(), newValue);
        }
    }
    if (m_isInternal && hasParent())
        getParent()->update();
}

void
NodeNurbsCurve2D::update(void) 
{ 
    m_chainDirty = true; 
    if (hasParent())
        getParent()->update();
}

void
NodeNurbsCurve2D::flip(int index)
{
    if (controlPoint())
        controlPoint()->flip(index);
    m_chainDirty = true;
    
}

void
NodeNurbsCurve2D::swap(int fromTo)
{
    if (controlPoint())
        controlPoint()->swap(fromTo);
    m_chainDirty = true;
    
}

void            
NodeNurbsCurve2D::reInit(void) 
{
    m_chain.resize(0);
    m_chainDirty = true; 
    Node::reInit();
}

Node *
NodeNurbsCurve2D::degreeElevate(int newDegree)
{
    NodeNurbsCurve *nurbs = (NodeNurbsCurve *)m_scene->createNode("NurbsCurve");

    float *newControlPoints = new float[controlPoint()->getSFSize() * 3];
    float *newKnots = (float *)knot()->getValues();
    int newOrder = order()->getValue();
    float *newWeights = (float *)weight()->getValues();
    for (int i = 0; i < controlPoint()->getSFSize(); i++) {
        newControlPoints[i * 3] = controlPoint()->getValue(i)[0];
        newControlPoints[i * 3 + 1] = controlPoint()->getValue(i)[1];
        newControlPoints[i * 3 + 2] = 0;
    }

    int newDimension = knot()->getSFSize() - order()->getValue();
    
    nurbs->knot(new MFFloat(newKnots, newDimension + newOrder));
    nurbs->setField(nurbs->order_Field(), new SFInt32(newOrder));
    nurbs->weight(new MFFloat(newWeights, newDimension));
    nurbs->createControlPoints(new MFVec3f(newControlPoints, newDimension * 3));
    nurbs->tessellation(new SFInt32(tessellation()->getValue()));

    nurbs->degreeElevate(newDegree);

    double *newControlPoints2 = new double[nurbs->controlPoint()->getSFSize() *
                                           2];
    newKnots = (float *)nurbs->knot()->getValues();
    newOrder = nurbs->order()->getValue();
    newWeights = (float *)nurbs->weight()->getValues();
    for (int i = 0; i < nurbs->getControlPoints()->getSFSize(); i++) {
        newControlPoints2[i * 2] = nurbs->getControlPoints()->getValue(i)[0];
        newControlPoints2[i * 2 + 1] = nurbs->getControlPoints()->
                                           getValue(i)[1];
    }
    for (int i = 0; i < nurbs->weight()->getSFSize(); i++)
        newWeights[i] = nurbs->weight()->getValue(i);
    
    newDimension = nurbs->knot()->getSFSize() - nurbs->order()->getValue();

    knot(new MFFloat(newKnots, newDimension + newOrder));
    setField(nurbs->order_Field(), new SFInt32(newOrder));
    weight(new MFFloat(newWeights, newDimension));
    controlPoint(new MFVec2d(newControlPoints2, newDimension * 2));
    tessellation(new SFInt32(tessellation()->getValue()));

    delete nurbs;
    delete [] newControlPoints;
    delete [] newControlPoints2;

    return this;
}

Node *
NodeNurbsCurve2D::setWeightsTo1(void)
{
    NodeNurbsCurve2D *ret = (NodeNurbsCurve2D *)copy();
    
    MFFloat *weights = weight();
    MFVec2d *points = controlPoint();
    if (weights->getSize() < points->getSFSize()) {
        swDebugf("not enough weights\n");
        return NULL;
    }
    MFVec2d *newPoints = new MFVec2d();
    MFFloat *newWeights = new MFFloat();
    for (int i = 0; i < points->getSFSize(); i++) {
        Vec2d p = points->getVec(i); 
        p = p / weights->getValue(i);
        newPoints->setVec(i, p);
        newWeights->setValue(i, 1.0f);
    }
    
    m_scene->backupFieldsStart();
    m_scene->backupFieldsAppend(ret, controlPoint_Field());
    m_scene->backupFieldsAppend(ret, weight_Field());
    m_scene->backupFieldsDone();

    ret->weight(newWeights);
    ret->controlPoint(newPoints);

    update();   
   
    return ret;
}

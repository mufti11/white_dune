/*
 * NodeNurbsTextureSurface.cpp
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

#include <stdlib.h>
#include <stdio.h>

#ifndef _WIN32
# include "stdlib.h"
#endif

#include "stdafx.h"
#include "NodeNurbsTextureSurface.h"
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
#include "NodeCoordinate.h"
#include "NodeNormal.h"
#include "NodeTextureCoordinate.h"
#include "NodeIndexedFaceSet.h"
#include "NodeNurbsGroup.h"
#include "NurbsSurfaceDegreeElevate.h"
#include "Util.h"
#include "Field.h"
#include "ExposedField.h"

ProtoNurbsTextureSurface::ProtoNurbsTextureSurface(Scene *scene)
  : Proto(scene, "NurbsTextureSurface")
{
    uDimension.set(
          addField(SFINT32, "uDimension", new SFInt32(0), new SFInt32(0)));
    vDimension.set(
          addField(SFINT32, "vDimension", new SFInt32(0), new SFInt32(0)));
    uKnot.set(
          addField(MFFLOAT, "uKnot", new MFFloat()));
    vKnot.set(
          addField(MFFLOAT, "vKnot", new MFFloat()));
    uOrder.set(
          addField(SFINT32, "uOrder", new SFInt32(3), new SFInt32(2)));
    vOrder.set(
          addField(SFINT32, "vOrder", new SFInt32(3), new SFInt32(2)));

    ExposedField* cpoint = new ExposedField(MFVEC2F, "controlPoint", 
                                            new MFVec2f());
    controlPoint.set(addExposedField(cpoint));

    weight.set(
          addExposedField(MFFLOAT, "weight", new MFFloat(), new SFFloat(0.0f)));

    addURLs(URL_VRML97_AMENDMENT1);
}

Node *
ProtoNurbsTextureSurface::create(Scene *scene)
{
    return new NodeNurbsTextureSurface(scene, this); 
}

NodeNurbsTextureSurface::NodeNurbsTextureSurface(Scene *scene, Proto *proto)
  : Node(scene, proto)
{
}

NodeNurbsTextureSurface::~NodeNurbsTextureSurface()
{
}

/*
void
NodeNurbsTextureSurface::drawHandles()
{
    int iuDimension = uDimension()->getValue();
    int ivDimension = vDimension()->getValue();
    RenderState state;

    if (controlPoint()->getSize() != iuDimension * ivDimension * 3)
        return;

    if (weight()->getSize() != iuDimension * ivDimension) {
        return;
    }

    glPushName(iuDimension * ivDimension + 1);
    glDisable(GL_LIGHTING);
    Util::myGlColor3f(1.0f, 1.0f, 1.0f);
    if (TheApp->GetHandleMeshAlways() ||
        (m_scene->getSelectedHandlesSize() == 0)) {
        for (int i = 0; i < iuDimension; i++) {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < ivDimension; j++) {
                const float *v = controlPoint()->getValue(i + j*iuDimension);
                float        w = weight()->getValue(i + j*iuDimension);
                glVertex3f(v[0] / w, v[1] / w, v[2] / w);
            }
            glEnd();
        }
        for (int j = 0; j < ivDimension; j++) {
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < iuDimension; i++) {
                const float *v = controlPoint()->getValue(i + j*iuDimension);
                float        w = weight()->getValue(i + j*iuDimension);
                glVertex3f(v[0] / w, v[1] / w, v[2] / w);
            }
            glEnd();
        }
    }
    state.startDrawHandles();
    for (int ci = 0; ci < iuDimension * ivDimension; ci++) {
        state.setHandleColor(m_scene, ci);
        glLoadName(ci);
        state.drawHandle(Vec3f(controlPoint()->getValue(ci)) / 
                               weight()->getValue(ci));
    }
    state.endDrawHandles();
    glPopName();
    glEnable(GL_LIGHTING);
}

Vec3f
NodeNurbsTextureSurface::getHandle(int handle, int *constraint,
                                   int *field)
{
    *field = controlPoint_Field() ;

    if (handle >= 0 && handle < controlPoint()->getSize() / 2) {
        Vec3f ret((Vec2f)controlPoint()->getValue(handle)
                         / weight()->getValue(handle));
        return ret;
    } else {
        return Vec3f(0.0f, 0.0f, 0.0f);
    }
}

void
NodeNurbsTextureSurface::setHandle(MFVec3f *value, int handle, float newWeight,
                            const Vec3f &newV, const Vec3f &oldV,
                            bool already_changed)
{
    bool changed = false;
    MFVec3f *newValue = (MFVec3f *) value->copy();

    if (m_scene->getXSymetricMode()) {
        float epsilon = TheApp->GetHandleEpsilon();
        int numPoints = newValue->getSize() / 3; 
        for (int i = 0; i < numPoints; i++) {
            if (i != handle) {
                Vec3f vPoint = controlPoint()->getValue(i);
                float wPoint = weight()->getValue(i);
                float w = wPoint;
                if (wPoint != 0)
                   vPoint = vPoint / wPoint;
                if (newWeight != 0)
                   w = w / newWeight;
                if (   (fabs(vPoint.z - oldV.z) < epsilon) 
                    && (fabs(vPoint.y - oldV.y) < epsilon)) {
                    if (fabs(vPoint.x + oldV.x) < epsilon) {
                        changed = true;
                        if (fabs(oldV.x) < epsilon)
                            newValue->setValue(i * 3,   0);
                        else
                        newValue->setValue(i * 3,   - newV.x * w);
                        newValue->setValue(i * 3+1,   newV.y * w);
                        newValue->setValue(i * 3+2,   newV.z * w);
                    } else if (fabs(vPoint.x - oldV.x) < epsilon) {
                        changed = true;
                        if (fabs(oldV.x) < epsilon)
                            newValue->setValue(i * 3,   0);
                        else
                            newValue->setValue(i * 3,   newV.x * w);
                        newValue->setValue(i * 3+1, newV.y * w);
                        newValue->setValue(i * 3+2, newV.z * w);
                    }         
                }                 
            }
        }
    }
    if (already_changed)
        changed = true;

    if (changed) {
        m_scene->setField(this, controlPoint_Field(), newValue);
    }
}

void
NodeNurbsTextureSurface::setHandle(float newWeight, 
                            const Vec3f &newV, const Vec3f &oldV)
{
    setHandle(controlPoint(), -1, newWeight, newV, oldV);
}

void
NodeNurbsTextureSurface::setHandle(int handle, const Vec3f &v)
{
    MFVec3f *newValue = new MFVec3f(*controlPoint());

    float epsilon = TheApp->GetHandleEpsilon();
    int numPoints = controlPoint()->getSize() / 3; 
    if (handle >= 0 && handle < numPoints) {
        float w = weight()->getValue(handle);
        Vec3f oldV = controlPoint()->getValue(handle);
        if (w != 0)
            oldV = oldV / w;
        Vec3f newV = v * w;
        if (m_scene->getXSymetricMode() && (fabs(oldV.x) < epsilon))
            newValue->setValue(handle * 3, 0);
        else
            newValue->setValue(handle * 3, newV.x);
        newValue->setValue(handle * 3+1, newV.y);
        newValue->setValue(handle * 3+2, newV.z);
        // set other handles for symetric modelling
        // which also snap handles at the same place
        setHandle(newValue, handle, w, newV, oldV, true);
        // search for NurbsGroup nodes and set handles
        if (m_scene->getXSymetricMode()) {
            NodeNurbsGroup *nurbsGroup = findNurbsGroup();
            if (nurbsGroup)
               nurbsGroup->setHandle(this, w, newV, oldV);
        }
    }
}
*/

NodeNurbsGroup *
NodeNurbsTextureSurface::findNurbsGroup()
{
    if (hasParent()) {
        Node* parent = getParent();
        if (parent->getType() == VRML_NURBS_GROUP) {
            return (NodeNurbsGroup *) parent;
        } else if (parent->getType() == VRML_SHAPE)
            if (parent->hasParent())
                if (parent->getParent()->getType() == VRML_NURBS_GROUP) {
                    return (NodeNurbsGroup *) parent->getParent();    
               } 
    }
    return NULL;
}

void
NodeNurbsTextureSurface::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
}

int
NodeNurbsTextureSurface::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f ,"EXTERNPROTO NurbsTextureSurface[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f ," ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:web3d:vrml97:node:NurbsTextureSurface\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:inet:blaxxun.com:node:NurbsTextureSurface\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:ParaGraph:NurbsTextureSurface\",\n") )
    TheApp->incSelectionLinenumber();
#ifdef HAVE_VRML97_AMENDMENT1_PROTO_URL
    RET_ONERROR( mywritestr(f ," \"") )
    RET_ONERROR( mywritestr(f ,HAVE_VRML97_AMENDMENT1_PROTO_URL) )
    RET_ONERROR( mywritestr(f ,"/NurbsTextureSurfacePROTO.wrl") )
    RET_ONERROR( mywritestr(f ,"\"\n") )
    TheApp->incSelectionLinenumber();
#else
    RET_ONERROR( mywritestr(f ," \"NurbsTextureSurfacePROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#endif
    RET_ONERROR( mywritestr(f ," \"http://wdune.ourproject.org/docs/vrml97Amendment1/NurbsTextureSurfacePROTO.wrl\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

   
int             
NodeNurbsTextureSurface::write(int filedes, int indent) 
{
    if (m_scene->isPureVRML()) {
        Node * node = toIndexedFaceSet();
        RET_ONERROR( node->write(filedes, indent) )
        node->unref();
    } else
        RET_ONERROR( Node::write(filedes, indent) )
    return 0;
}

void
NodeNurbsTextureSurface::flip(int index)
{
    if (controlPoint())
        controlPoint()->flip(index);    
}

void
NodeNurbsTextureSurface::swap(int fromTo)
{
    if (controlPoint())
        controlPoint()->swap(fromTo);    
}

Node*
NodeNurbsTextureSurface::degreeElevate(int newUDegree, int newVDegree)
{
  
  //degree elevate a nurbs surface,
  
  if(newUDegree < ((uOrder()->getValue())-1)){
    return NULL;
  }
  if(newVDegree < ((vOrder()->getValue())-1)){
    return NULL;
  }
  
  NodeNurbsTextureSurface *node = (NodeNurbsTextureSurface *)
                         m_scene->createNode("NurbsTextureSurface");

  if((newUDegree >= ((uOrder()->getValue())-1)) && 
     (newVDegree >= ((vOrder()->getValue())-1))){
    
    //load old values
    int tuDimension = uDimension()->getValue();
    int tvDimension = vDimension()->getValue();
    Vec3f *tPoints = new Vec3f[tuDimension * tvDimension];
    float *tWeights = new float[tuDimension * tvDimension];
    int tuOrder = uOrder()->getValue();
    int tvOrder = vOrder()->getValue();
    int uKnotSize = uKnot()->getSize();
    int vKnotSize = vKnot()->getSize();
    MyArray<float> tuKnots(uKnotSize);
    MyArray<float> tvKnots(vKnotSize);
    int tuDegree = tuOrder - 1;
    int tvDegree = tvOrder - 1;
    int tuUpDegree = newUDegree - tuDegree;
    int tvUpDegree = newVDegree - tvDegree;
    
    for (int i=0; i<(tuDimension*tvDimension); i++){
      tPoints[i] = controlPoint()->getValue(i);
      tWeights[i] =weight()->getValue(i);
    }

    for (int i=0; i<uKnotSize; i++){
      tuKnots[i] = uKnot()->getValue(i);
    }
    for (int i=0; i<vKnotSize; i++){
      tvKnots[i] = vKnot()->getValue(i);
    }

    //elevate surface
    NurbsSurfaceDegreeElevate elevatedSurface(tPoints, tWeights, 
          tuKnots, tvKnots, tuDimension, tvDimension, tuDegree, tvDegree, 
          tuUpDegree, tvUpDegree);

    //load new node
    
    int newUDimension = elevatedSurface.getUDimension();
    int newVDimension = elevatedSurface.getVDimension();
    float *newControlPoints = new float[newUDimension * newVDimension * 2];
    float *newWeights = new float[newUDimension * newVDimension];
    float *newUKnots = new float[elevatedSurface.getUKnotSize()];
    float *newVKnots = new float[elevatedSurface.getVKnotSize()];
    int newUOrder = newUDegree + 1;
    int newVOrder = newVDegree + 1;
    
    for(int i=0; i<(newUDimension * newVDimension); i++){
      newControlPoints[(i*2)] = elevatedSurface.getControlPoints(i).x;
      newControlPoints[(i*2)+1] = elevatedSurface.getControlPoints(i).y;
      newWeights[i] = elevatedSurface.getWeights(i);
    }
    for(int i=0; i<(elevatedSurface.getUKnotSize()); i++){
      newUKnots[i] = elevatedSurface.getUKnots(i);
    }
    for(int i=0; i<(elevatedSurface.getVKnotSize()); i++){
      newVKnots[i] = elevatedSurface.getVKnots(i);
    }

   
    node->setField(node->uDimension_Field(), new SFInt32(newUDimension));
    node->setField(node->vDimension_Field(), new SFInt32(newVDimension));
    node->uKnot(new MFFloat(newUKnots, newUDimension + newUOrder));
    node->vKnot(new MFFloat(newVKnots, newVDimension + newVOrder));
    node->setField(node->uOrder_Field(), new SFInt32(newUOrder));
    node->setField(node->vOrder_Field(), new SFInt32(newVOrder));
    node->controlPoint(new MFVec2f(newControlPoints, newUDimension * newVDimension * 2));
    node->weight(new MFFloat(newWeights, newUDimension * newVDimension));
    
    return node;
  }
  return NULL;
}

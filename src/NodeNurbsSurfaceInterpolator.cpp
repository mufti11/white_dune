/*
 * NodeNurbsSurfaceInterpolator.cpp
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

#include "stdafx.h"
#include "NodeNurbsSurfaceInterpolator.h"
#include "Scene.h"
#include "FieldValue.h"
#include "SFInt32.h"
#include "MyMesh.h"
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
#include "MoveCommand.h"

ProtoNurbsSurfaceInterpolator::ProtoNurbsSurfaceInterpolator(Scene *scene)
  : Proto(scene, "NurbsSurfaceInterpolator")
{
    addEventIn(SFVEC2F, "set_fraction", EIF_RECOMMENDED);
    uDimension.set(
          addField(SFINT32, "uDimension", new SFInt32(0), new SFInt32(0)));
    vDimension.set(
          addField(SFINT32, "vDimension", new SFInt32(0), new SFInt32(0)));
    uKnot.set(
          addField(MFDOUBLE, "uKnot", new MFDouble()));
    vKnot.set(
          addField(MFDOUBLE, "vKnot", new MFDouble()));
    uOrder.set(
          addField(SFINT32, "uOrder", new SFInt32(3), new SFInt32(2)));
    vOrder.set(
          addField(SFINT32, "vOrder", new SFInt32(3), new SFInt32(2)));
    controlPoint.set(
          addExposedField(SFNODE, "controlPoint", new SFNode(NULL), 
                          COORDINATE_NODE));
    weight.set(
          addExposedField(MFDOUBLE, "weight", new MFDouble(), 
                          new SFDouble(0.0f)));
    addEventOut(SFVEC3F, "position_changed", EOF_RECOMMENDED);
    addEventOut(SFVEC3F, "normal_changed");
}

Node *
ProtoNurbsSurfaceInterpolator::create(Scene *scene)
{
    return new NodeNurbsSurfaceInterpolator(scene, this); 
}

NodeNurbsSurfaceInterpolator::NodeNurbsSurfaceInterpolator(Scene *scene, Proto *proto)
  : Node(scene, proto)
{
}

NodeNurbsSurfaceInterpolator::~NodeNurbsSurfaceInterpolator()
{
}

MFVec3f *
NodeNurbsSurfaceInterpolator::getControlPoints(void)
{
    Node *pointNode = controlPoint()->getValue();
    if (pointNode)
        return ((NodeCoordinate *)pointNode)->point();
    return NULL;
}

void
NodeNurbsSurfaceInterpolator::setControlPoints(MFVec3f *points)
{
    NodeCoordinate *coord = (NodeCoordinate *)controlPoint()->getValue();
    if (coord != NULL)
        m_scene->setField(coord, coord->point_Field(), new MFVec3f(points));
}

void
NodeNurbsSurfaceInterpolator::createControlPoints(MFVec3f *points)
{
    controlPoint(new SFNode(m_scene->createNode("Coordinate"))); 
    setControlPoints(points);
}

void
NodeNurbsSurfaceInterpolator::repairNormal(MFVec3f *normals, MFVec3f *tess, 
                               int c1, int c2, int c3)
{
    Vec3f tessC1 = tess->getVec(c1); 
    Vec3f tessC2 = tess->getVec(c2); 
    Vec3f tessC3 = tess->getVec(c3); 
    Vec3f v1 = tessC3 - tessC1;
    Vec3f v2 = tessC2 - tessC1;
    Vec3f normal = v2.cross(v1);
    normal.normalize();
    normals->setVec(c1, normal);   
}

void
NodeNurbsSurfaceInterpolator::drawHandles()
{
    int iuDimension = uDimension()->getValue();
    int ivDimension = vDimension()->getValue();
    RenderState state;

    if (getControlPoints() == NULL)
        return;

    if (getControlPoints()->getSize() != iuDimension * ivDimension * 3)
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
                const float *v = getControlPoints()->getValue(i + j*iuDimension);
                float        w = weight()->getValue(i + j*iuDimension);
                glVertex3f(v[0] / w, v[1] / w, v[2] / w);
            }
            glEnd();
        }
        for (int j = 0; j < ivDimension; j++) {
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < iuDimension; i++) {
                const float *v = getControlPoints()->getValue(i + j*iuDimension);
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
        state.drawHandle(Vec3f(getControlPoints()->getValue(ci)) / 
                               weight()->getValue(ci));
    }
    state.endDrawHandles();
    glPopName();
    glEnable(GL_LIGHTING);
}

Vec3f
NodeNurbsSurfaceInterpolator::getHandle(int handle, int *constraint,
                            int *field)
{
    *field = controlPoint_Field();

    if (handle >= 0 && handle < getControlPoints()->getSize() / 3) {
        Vec3f ret((Vec3f)getControlPoints()->getValue(handle)
                         / weight()->getValue(handle));
        return ret;
    } else {
        return Vec3f(0.0f, 0.0f, 0.0f);
    }
}

void
NodeNurbsSurfaceInterpolator::setHandle(MFVec3f *value, int handle, float newWeight,
                            const Vec3f &newV, const Vec3f &oldV,
                            bool already_changed)
{
    bool        changed = false;
    MFVec3f *newValue = (MFVec3f *) value->copy();

    if (m_scene->getXSymetricMode()) {
        float epsilon = TheApp->GetHandleEpsilon();
        int numPoints = newValue->getSize() / 3; 
        for (int i = 0; i < numPoints; i++) {
            if (i != handle) {
                Vec3f vPoint = getControlPoints()->getValue(i);
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
                            newValue->setValue(i * 3, 0);
                        else
                            newValue->setValue(i * 3, - newV.x * w);
                        newValue->setValue(i * 3+1, newV.y * w);
                        newValue->setValue(i * 3+2, newV.z * w);
                    } else if (fabs(vPoint.x - oldV.x) < epsilon) {
                        changed = true;
                        if (fabs(oldV.x) < epsilon)
                            newValue->setValue(i * 3,   0);
                        else
                            newValue->setValue(i * 3, newV.x * w);
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
        NodeCoordinate *pointNode = (NodeCoordinate *)
                                    controlPoint()->getValue();
        if (pointNode)
            m_scene->setField(pointNode, pointNode->point_Field(), newValue);
    }
}

void
NodeNurbsSurfaceInterpolator::setHandle(float newWeight, 
                            const Vec3f &newV, const Vec3f &oldV)
{
    setHandle(getControlPoints(), -1, newWeight, newV, oldV);
}

NodeNurbsGroup *
NodeNurbsSurfaceInterpolator::findNurbsGroup()
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
NodeNurbsSurfaceInterpolator::setHandle(int handle, const Vec3f &v)
{
    MFVec3f    *newValue = new MFVec3f(*getControlPoints());

    float epsilon = TheApp->GetHandleEpsilon();
    int numPoints = getControlPoints()->getSize() / 3; 
    if (handle >= 0 && handle < numPoints) {
        float w = weight()->getValue(handle);
        Vec3f oldV = getControlPoints()->getValue(handle);
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

void
NodeNurbsSurfaceInterpolator::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
}

int
NodeNurbsSurfaceInterpolator::findSpan(int dimension, int order, float u,
                           const float knots[])
{
    int low, mid, high;
    int n = dimension + order - 1;

    if (u >= knots[n]) {
        return n - order;
    }
    low = order - 1;
    high = n - order + 1;

    mid = (low + high) / 2;

    int oldLow = low;
    int oldHigh = high;
    int oldMid = mid;
    while (u < knots[mid] || u >= knots[mid+1]) {
        if (u < knots[mid]) {
            high = mid;
        } else {
            low = mid;
        }
        mid = (low+high)/2;

        // emergency abort of loop, otherwise a endless loop can occure
        if ((low == oldLow) && (high == oldHigh) && (mid == oldMid))
            break;

        oldLow = low;
        oldHigh = high;
        oldMid = mid;
    }
    return mid;
}

void
NodeNurbsSurfaceInterpolator::basisFuns(int span, float u, int order,
                            const float knots[], float basis[],
                            float deriv[])
{
    float *left = (float *) malloc(order * sizeof(float));
    float *right = (float *) malloc(order * sizeof(float));

    if ((left==NULL) || (right==NULL))
       return;
    basis[0] = 1.0f;
    for (int j = 1; j < order; j++) {
        left[j] = u - knots[span+1-j];
        right[j] = knots[span+j]-u;
        float saved = 0.0f, dsaved = 0.0f;
        for (int r = 0; r < j; r++) {
            if ((right[r+1] + left[j-r]) == 0)
                return;
            float temp = basis[r] / (right[r+1] + left[j-r]);
            basis[r] = saved + right[r+1] * temp;
            deriv[r] = dsaved - j * temp;
            saved = left[j-r] * temp;
            dsaved = j * temp;
        }
        basis[j] = saved;
        deriv[j] = dsaved;
    }
    free(left);
    free(right);
}

Vec3f
NodeNurbsSurfaceInterpolator::SurfaceInterpolatorPoint(int uDimension, int uOrder, const float uKnots[],
                               int vDimension, int vOrder, const float vKnots[],
                               const Vec3f controlPoints[],
                               const float weight[], float u, float v,
                               Vec3f &normal)
{
    float *uBasis = (float *) malloc(uOrder * sizeof(float));
    float *vBasis = (float *) malloc(vOrder * sizeof(float));
    float *uDeriv = (float *) malloc(uOrder * sizeof(float));
    float *vDeriv = (float *) malloc(vOrder * sizeof(float));

    int uSpan = findSpan(uDimension, uOrder, u, uKnots);
    int vSpan = findSpan(vDimension, vOrder, v, vKnots);

    basisFuns(uSpan, u, uOrder, uKnots, uBasis, uDeriv);
    basisFuns(vSpan, v, vOrder, vKnots, vBasis, vDeriv);

    int uBase = uSpan-uOrder+1;
    int vBase = vSpan-vOrder+1;

    int index = vBase*uDimension + uBase;
    Vec3f S(0.0f, 0.0f, 0.0f), du(0.0f, 0.0f, 0.0f), dv(0.0f, 0.0f, 0.0f);
    float w = 0.0f, duw = 0.0f, dvw = 0.0f;
    for (int j = 0; j < vOrder; j++) {
        for (int i = 0; i < uOrder; i++) {
            float gain = uBasis[i] * vBasis[j];
            float dugain = uDeriv[i] * vBasis[j];
            float dvgain = uBasis[i] * vDeriv[j];
            S += controlPoints[index] * gain;
            w += weight[index] * gain;
            du += controlPoints[index] * dugain;
            dv += controlPoints[index] * dvgain;
            duw += weight[index] * dugain;
            dvw += weight[index] * dvgain;
            index++;
        }
        index += uDimension - uOrder;
    }
    S = S / w;
    Vec3f un = (du - S * duw) / w;
    Vec3f vn = (dv - S * dvw) / w;
    normal = un.cross(vn);
    normal.normalize();
    free(uBasis);
    free(vBasis);
    free(uDeriv);
    free(vDeriv);
    return S;
}

void
NodeNurbsSurfaceInterpolator::rotate(SFRotation rot)
{
    int         numPoints = getControlPoints()->getSize() / 3;

    for (int i = 0; i < numPoints; i++) {
        Vec3f vPoint = getControlPoints()->getValue(i);
        vPoint = rot.getQuat() * vPoint;
        getControlPoints()->setSFValue(i, new SFVec3f(vPoint));
    }        
}

void
NodeNurbsSurfaceInterpolator::flip(int index)
{
    if (getControlPoints())
        getControlPoints()->flip(index);
}

void
NodeNurbsSurfaceInterpolator::swap(int fromTo)
{
    if (getControlPoints())
        getControlPoints()->swap(fromTo);
}

void 
NodeNurbsSurfaceInterpolator::linearUknot(void)
{
    int iuOrder = uOrder()->getValue();
    int iuDimension = uDimension()->getValue();
    float *uknots = new float[iuOrder + iuDimension]; 
    for(int i = 0; i < iuOrder; i++) {
        uknots[i] = 0.0f;
        uknots[i + iuDimension] = (float) (iuDimension - iuOrder + 1);
    }
    for(int i = 0; i < (iuDimension - iuOrder); i++) {
        uknots[i + iuOrder] = (float)(i + 1);  
    } 
    for (int i = 0; i < (iuOrder + iuDimension); i++)
        uknots[i] /= (iuDimension - iuOrder + 1);
    m_scene->setField(this, uKnot_Field(), 
                     new MFFloat(uknots, iuOrder + iuDimension));
}

void 
NodeNurbsSurfaceInterpolator::linearVknot(void)
{
    int ivOrder = vOrder()->getValue();
    int ivDimension = vDimension()->getValue();
    float *vknots = new float[ivOrder + ivDimension]; 
    for(int i = 0; i < ivOrder; i++) {
        vknots[i] = 0.0f;
        vknots[i + ivDimension] = (float) (ivDimension - ivOrder + 1);
    }
    for(int i = 0; i < (ivDimension - ivOrder); i++) {
        vknots[i + ivOrder] = (float)(i + 1);  
    } 
    for (int i = 0; i < (ivOrder + ivDimension); i++)
        vknots[i] /= (ivDimension - ivOrder + 1);
    m_scene->setField(this, vKnot_Field(), 
                     new MFFloat(vknots, ivOrder + ivDimension));
}

Node*
NodeNurbsSurfaceInterpolator::degreeElevate(int newUDegree, int newVDegree)
{
  
  //degree elevate a nurbs SurfaceInterpolator,
  
  if(newUDegree < ((uOrder()->getValue())-1)){
    return NULL;
  }
  if(newVDegree < ((vOrder()->getValue())-1)){
    return NULL;
  }
  
  NodeNurbsSurfaceInterpolator *node = (NodeNurbsSurfaceInterpolator *)
                         m_scene->createNode("NurbsSurfaceInterpolator");

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
      tPoints[i] = getControlPoints()->getValue(i);
      tWeights[i] =weight()->getValue(i);
    }

    for (int i=0; i<uKnotSize; i++){
      tuKnots[i] = uKnot()->getValue(i);
    }
    for (int i=0; i<vKnotSize; i++){
      tvKnots[i] = vKnot()->getValue(i);
    }

    //elevate SurfaceInterpolator
    NurbsSurfaceDegreeElevate elevatedSurfaceInterpolator(tPoints, tWeights, 
          tuKnots, tvKnots, tuDimension, tvDimension, tuDegree, tvDegree, 
          tuUpDegree, tvUpDegree);

    //load new node
    
    int newUDimension = elevatedSurfaceInterpolator.getUDimension();
    int newVDimension = elevatedSurfaceInterpolator.getVDimension();
    float *newControlPoints = new float[newUDimension * newVDimension *3];
    float *newWeights = new float[newUDimension * newVDimension];
    float *newUKnots = new float[elevatedSurfaceInterpolator.getUKnotSize()];
    float *newVKnots = new float[elevatedSurfaceInterpolator.getVKnotSize()];
    int newUOrder = newUDegree + 1;
    int newVOrder = newVDegree + 1;
    
    for(int i=0; i<(newUDimension * newVDimension); i++){
      newControlPoints[(i*3)] = elevatedSurfaceInterpolator.getControlPoints(i).x;
      newControlPoints[(i*3)+1] = elevatedSurfaceInterpolator.getControlPoints(i).y;
      newControlPoints[(i*3)+2] = elevatedSurfaceInterpolator.getControlPoints(i).z;
      newWeights[i] = elevatedSurfaceInterpolator.getWeights(i);
    }
    for(int i=0; i<(elevatedSurfaceInterpolator.getUKnotSize()); i++){
      newUKnots[i] = elevatedSurfaceInterpolator.getUKnots(i);
    }
    for(int i=0; i<(elevatedSurfaceInterpolator.getVKnotSize()); i++){
      newVKnots[i] = elevatedSurfaceInterpolator.getVKnots(i);
    }

   
    node->createControlPoints(new MFVec3f(newControlPoints, 
                                          newUDimension * newVDimension * 3));
    node->weight(new MFDouble(newWeights, newUDimension * newVDimension));
    node->setField(node->uDimension_Field(), new SFInt32(newUDimension));
    node->setField(node->vDimension_Field(), new SFInt32(newVDimension));
    node->uKnot(new MFDouble(newUKnots, newUDimension + newUOrder));
    node->vKnot(new MFDouble(newVKnots, newVDimension + newVOrder));
    node->setField(node->uOrder_Field(), new SFInt32(newUOrder));
    node->setField(node->vOrder_Field(), new SFInt32(newVOrder));
    
    return node;
  }
  return NULL;
}

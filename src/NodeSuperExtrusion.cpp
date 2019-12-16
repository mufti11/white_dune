/*
 * NodeSuperExtrusion.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "NodeSuperExtrusion.h"
#include "Proto.h"
#include "DuneApp.h"
#include "Scene.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFInt32.h"
#include "SFBool.h"
#include "Vec2f.h"
#include "NodeNurbsSurface.h"
#include "MyMesh.h"
#include "Util.h"
#include "RenderState.h"
#include "resource.h"

ProtoSuperExtrusion::ProtoSuperExtrusion(Scene *scene)
  : Proto(scene, "SuperExtrusion")
{
    a.set( 
          addExposedField(SFFLOAT, "a", new SFFloat(1.0f)));
    setFieldFlags(a, FF_4KIDS);
    b.set( 
          addExposedField(SFFLOAT, "b", new SFFloat(1.0f)));
    setFieldFlags(b, FF_4KIDS);
    m.set( 
          addExposedField(SFFLOAT, "m", new SFFloat(0.0f)));
    setFieldFlags(m, FF_4KIDS);
    n1.set( 
          addExposedField(SFFLOAT, "n1", new SFFloat(1.0f)));
    setFieldFlags(n1, FF_4KIDS);
    n2.set( 
          addExposedField(SFFLOAT, "n2", new SFFloat(1.0f)));
    setFieldFlags(n2, FF_4KIDS);
    n3.set( 
          addExposedField(SFFLOAT, "n3", new SFFloat(1.0f)));
    setFieldFlags(n3, FF_4KIDS);

    border.set( 
          addExposedField(SFFLOAT, "border", new SFFloat(M_PI),
                          new SFFloat(-M_PI), new SFFloat(M_PI)));
    setFieldFlags(border, FF_4KIDS);
    bottomBorder.set( 
          addExposedField(SFFLOAT, "bottomBorder", new SFFloat(-M_PI),
                          new SFFloat(-M_PI), new SFFloat(M_PI)));
    setFieldFlags(bottomBorder, FF_4KIDS);

    superTessellation.set(
          addExposedField(SFINT32, "superTessellation", new SFInt32(0)));
    setFieldFlags(superTessellation, FF_4KIDS);
    spineTessellation.set(
          addExposedField(SFINT32, "spineTessellation", new SFInt32(0)));
    setFieldFlags(spineTessellation, FF_4KIDS);

    controlPoint.set(
          addExposedField(MFVEC3F, "controlPoint", new MFVec3f()));
    weight.set(
          addExposedField(MFFLOAT, "weight", new MFFloat(), new SFFloat(0.0f)));
    knot.set(
          addField(MFFLOAT, "knot", new MFFloat()));
    order.set(
          addField(SFINT32, "order", new SFInt32(3), new SFInt32(2)));

    creaseAngle.set( 
          addField(SFFLOAT, "creaseAngle", new SFFloat(0.7854f), 
                   new SFFloat(0.0f)));
    setFieldFlags(creaseAngle, FF_4KIDS);

    beginCap.set(
          addField(SFBOOL, "beginCap", new SFBool(true)));
    setFieldFlags(beginCap, FF_4KIDS | FF_UPDATE);
    endCap.set(
          addField(SFBOOL, "endCap", new SFBool(true)));
    setFieldFlags(endCap, FF_4KIDS | FF_UPDATE);

    solid.set(
          addField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_4KIDS);

    float values3[] = { 1.0, 1.0 };
    float *v = new float[2];  
    memcpy(v, values3, 2 * sizeof(float));
    scale.set(
          addField(MFVEC2F, "scale", new MFVec2f(v, 2), new SFFloat(0.0f)));
    setFieldFlags(scale, FF_4KIDS);

    addURLs(URL_SCRIPTED_NODES);
}

Node *
ProtoSuperExtrusion::create(Scene *scene)
{
    return new NodeSuperExtrusion(scene, this); 
}

NodeSuperExtrusion::NodeSuperExtrusion(Scene *scene, Proto *def)
  : MeshMorphingNode(scene, def)
{
    m_extrusionDirty = true;
    m_extrusion = (NodeExtrusion *) scene->createNode("Extrusion");
    m_nurbsCurve = (NodeNurbsCurve *) scene->createNode("NurbsCurve");
    m_nurbsCurve->setInternal(true);
}

NodeSuperExtrusion::~NodeSuperExtrusion()
{
    delete m_extrusion;
    delete m_nurbsCurve;
}

static float superFormula(float angle, float a, float b, float m, 
                          float n1, float n2, float n3)
    {
    float f = m * angle / 4.0;
    float c = cos(f);
    float s = sin(f);
    return pow(pow(fabs(c / a), n2) + pow(fabs(s / b), n3), -1.0f / n1);
    }

void
NodeSuperExtrusion::createExtrusion()
{
    if (m_extrusion == NULL)
        m_extrusion = (NodeExtrusion *) m_scene->createNode("Extrusion");

    if (m_nurbsCurve == NULL)
        m_nurbsCurve = (NodeNurbsCurve *) m_scene->createNode("NurbsCurve");

    int superTess = superTessellation()->getValue();
    int spineTess = spineTessellation()->getValue();

    if (superTess <= 0) superTess = TheApp->getTessellation();
    if (spineTess <= 0) spineTess = TheApp->getTessellation();

    superTess++;

    if (superTess < 3) return;
    if (spineTess < 3) return;

    int size = superTess;
    float *vert = new float[size * 2];

    float low = bottomBorder()->getValue();
    float high = border()->getValue();
    
    float inc1 = (high - low) / (superTess-1);
    if (inc1 < FEPSILON) return;
    int a1;
    float fa = a()->getValue();
    float fb = b()->getValue();
    float fm = m()->getValue();
    float fn1 = n1()->getValue();
    float fn2 = n2()->getValue();
    float fn3 = n3()->getValue();
    MyArray<float> r1(superTess);
    MyArray<float> c1(superTess);
    MyArray<float> s1(superTess);
    for (a1 = 0; a1 < superTess; a1++) {
        float angle1 = low + a1 * inc1;
        r1[a1] = superFormula(angle1, fa, fb, fm, fn1, fn2, fn3);
        c1[a1] = cos(angle1 + M_PI / 2.0);
        s1[a1] = sin(angle1 + M_PI / 2.0);
    }
    int index = 0;
    for (a1 = superTess - 1; a1 >= 0; a1--) {
        vert[index] = r1[a1]*c1[a1];
        vert[index+1] = r1[a1]*s1[a1];
        index = index + 2;
    }

    m_extrusion->crossSection(new MFVec2f(vert, size * 2));
    
    m_extrusion->solid(new SFBool(solid()->getValue()));
    m_extrusion->convex(new SFBool(false));
    m_extrusion->creaseAngle(new SFFloat(creaseAngle()->getValue()));
    m_extrusion->beginCap(new SFBool(beginCap()->getValue()));
    m_extrusion->endCap(new SFBool(endCap()->getValue()));
    m_nurbsCurve->tessellation(new SFInt32(spineTessellation()->getValue()));
    int len = controlPoint()->getSize();
    float *points = new float[len];
    for (int i = 0; i < len; i++)
         points[i] = controlPoint()->getValues()[i];
    m_nurbsCurve->setControlPoints(new MFVec3f(points, len));
    float *weights = new float[weight()->getSize()];
    for (int i = 0; i < weight()->getSize(); i++)
         weights[i] = weight()->getValues()[i];
    m_nurbsCurve->weight(new MFFloat(weights, weight()->getSize()));
    float *knots = new float[knot()->getSize()];
    for (int i = 0; i < knot()->getSize(); i++)
         knots[i] = knot()->getValues()[i];
    m_nurbsCurve->knot(new MFFloat(knots, knot()->getSize()));
    m_nurbsCurve->order(new SFInt32(order()->getValue()));
    const Vec3f *chain = m_nurbsCurve->getChain();
    int chainLength = m_nurbsCurve->getChainLength();
    float *fchain = new float[chainLength * 3];
    for (int i = 0; i < chainLength; i++) {
         fchain[i * 3    ] = chain[i].x;
         fchain[i * 3 + 1] = chain[i].y;
         fchain[i * 3 + 2] = chain[i].z;
    }   
    m_extrusion->spine(new MFVec3f(fchain, chainLength * 3));
    if (scale()->getSFSize() > 0) {
        float *scales;
        int scaleLen = scale()->getSFSize();
        if (scaleLen == 1)
            scales = new float[2];
        else
           scales = new float[2 * (spineTess + 1)];
        scales[0] = scale()->getValues()[0];
        scales[1] = scale()->getValues()[1];
        if (scaleLen > 1) {
            float tess_inc = 1.0 / (spineTess);
            float scale_inc = 1.0 / (scaleLen - 1);
            float scale_left = 0.0;
            int scale_next_index = 1;
            for (int i = 1; i <= spineTess; i++) {
                float left_scale_x; 
                left_scale_x = scale()->getValues()[scale_next_index * 2 - 2];
                float scale_diff_x;
                scale_diff_x = scale()->getValues()[scale_next_index * 2 + 0] - 
                               left_scale_x;
                scales[i * 2 + 0] = left_scale_x + scale_diff_x * 
                                    ((i * tess_inc - scale_left) / scale_inc);

                float left_scale_y; 
                left_scale_y = scale()->getValues()[scale_next_index * 2 - 1];
                float scale_diff_y;
                scale_diff_y = scale()->getValues()[scale_next_index * 2 + 1] - 
                               left_scale_y;
                scales[i * 2 + 1] = left_scale_y + scale_diff_y * 
                                    ((i * tess_inc - scale_left) / scale_inc);

                if ((i * tess_inc) >= (scale_next_index * scale_inc)) {
                    scale_next_index++;
                    scale_left += scale_inc;
                }              
            }
        }
        m_extrusion->scale(new MFVec2f(scales, scaleLen == 1 ? 2 : 
                                              (spineTess + 1) * 2));
    }
    m_extrusionDirty = false;
}

int
NodeSuperExtrusion::writeProto(int f)
{
    return ((Node *)this)->writeProto(f, "", "scripted_Nodes"
#ifdef HAVE_SCRIPTED_NODES_PROTO_URL
                                            , HAVE_SCRIPTED_NODES_PROTO_URL
#endif
                                            );
}

void
NodeSuperExtrusion::setField(int index, FieldValue *value, int cf)
{
    m_extrusionDirty = true;
    Node::setField(index, value, cf);
    if (m_scene->getXSymetricMode()) {
        if (index == border_Field())
            Node::setField(bottomBorder_Field(),
                           new SFFloat(-border()->getValue()), cf);
        if (index == bottomBorder_Field())
            Node::setField(border_Field(),
                           new SFFloat(-bottomBorder()->getValue()), cf);
    }
    update();
}

void
NodeSuperExtrusion::draw()
{
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }

    if (!m_extrusion) return;

    m_extrusion->draw();
}

void
NodeSuperExtrusion::drawAHandles()
{
    float fa = a()->getValue();
    RenderState state;

    int spineLen = m_extrusion->spine()->getSFSize();
    if (spineLen <= 0) 
        return;
    const float *spine = m_extrusion->spine()->getValues(); 
    int startHandles = controlPoint()->getSize() / 3;
         
    glPushMatrix();
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushName(0);
        
    state.startDrawHandles();
    for (int i = 0; i < 4; i++) {
        float x = 0;
        float y = 0;
        float z = 0;
        switch (i) {
          case 0:
            x = -fa;
            break;
          case 1:
            x = fa;
            break;
          case 2:
            z = -fa;
            break;
          case 3:
            z = fa;
            break;
        }
        Vec3f handlePosition(spine[0] + x, spine[1] + y, spine[2] + z);
        state.setHandleColor(m_scene, startHandles + i);
        glLoadName(startHandles + i);
        state.drawHandle(handlePosition);
    }
    state.endDrawHandles();
    glPopName();
    glPopAttrib();
    glPopMatrix(); 
}

void
NodeSuperExtrusion::drawScaleHandles()
{
    RenderState state;

    int spineLen = m_extrusion->spine()->getSFSize();
    if (spineLen <= 0) 
        return;
    const float *spine = m_extrusion->spine()->getValues(); 

    int scaleLen = scale()->getSFSize();
    if (scaleLen <= 1) 
        return;
    const float *fscale = scale()->getValues(); 
    int startHandles = controlPoint()->getSize() / 3 + 4;
         
    glPushMatrix();
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushName(0);

    float yLen = spine[(spineLen - 1) * 3 + 1] - spine[1];        
    state.startDrawHandles();
    for (int i = 0; i < scaleLen; i++) {
        float x = fscale[i * 2];
        float y = spine[1] + yLen * i / (scaleLen - 1);
        float z = fscale[i * 2 + 1];
        Vec3f handlePosition(spine[0] + x, spine[1] + y, spine[2] + z);
        state.setHandleColor(m_scene, startHandles + i);
        glLoadName(startHandles + i);
        state.drawHandle(handlePosition);
    }
    state.endDrawHandles();
    glPopName();
    glPopAttrib();
    glPopMatrix(); 
}

void  
NodeSuperExtrusion::drawHandles()
{
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }

    if (!m_extrusion) return;

    drawScaleHandles();
    m_nurbsCurve->drawHandles();
    drawAHandles();
}

Vec3f
NodeSuperExtrusion::getHandle(int handle, int *constraint, int *field)
{
    *field = -1;

    int numControlPoints = controlPoint()->getSize() / 3;
    if (handle >= 0 && handle < numControlPoints) {
        *field = controlPoint_Field();
        Vec3f ret((Vec3f)controlPoint()->getValue(handle) / 
                   weight()->getValue(handle));
        TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, "controlPoint", 
                                          handle, ret);
        return ret;
    } else if ((handle >= numControlPoints) && 
               (handle < (numControlPoints + 4))) {
        *field = a_Field();
        float x = 0;
        float z = 0;
        float fa = a()->getValue();
        switch (handle - numControlPoints) {
          case 0:
            x = -fa;
            break; 
          case 1:
            x = fa;
            break;
          case 2:
            z = -fa;
            break;
          case 3:
            z = fa; 
            break;    
        }
        Vec3f ret(x, 0, z);
        return ret;
    } else if (handle < (numControlPoints + 4 + scale()->getSFSize())) {
        *field = scale_Field();
        const float *fscale = scale()->getValues(); 
        int scaleHandle = handle - numControlPoints - 4;
        const float *spine = m_extrusion->spine()->getValues(); 
        Vec3f ret(fscale[scaleHandle * 2] - spine[0], 
                  0, 
                  fscale[scaleHandle * 2 + 1] - spine[2]);
        return ret;        
    }
    *field = getProto()->metadata_Field();
    return Vec3f(0.0f, 0.0f, 0.0f);
}


void
NodeSuperExtrusion::setHandle(int handle, const Vec3f &v) 
{
    int numControlPoints = controlPoint()->getSize() / 3;

    if (handle >= 0 && handle < numControlPoints) {
        Vec3f v2 = v * weight()->getValue(handle); 
        const float *fcontrolPoint = controlPoint()->getValues(); 
        MFVec3f *newValue = new MFVec3f();
        for (int i = 0; i < controlPoint()->getSFSize(); i++)
            newValue->setSFValue(i, fcontrolPoint[i * 3], 
                                    fcontrolPoint[i * 3 + 1],
                                    fcontrolPoint[i * 3 + 2]);
        m_extrusionDirty = true;
        newValue->setValue(handle * 3, v2.x);
        newValue->setValue(handle * 3 + 1, v2.y);
        newValue->setValue(handle * 3 + 2, v2.z);
        m_scene->setField(this, controlPoint_Field(), newValue);
    } else if ((handle >= numControlPoints) &&
               (handle < (numControlPoints + 4))) {
        float fa = a()->getValue();
        switch (handle - numControlPoints) {
          case 0:
            fa = -v.x;
            break; 
          case 1:
            fa = v.x;
            break;
          case 2:
            fa = -v.z;
            break;
          case 3:
            fa = v.z; 
            break;    
        }
        m_scene->setField(this, a_Field(), new SFFloat(fa));
    } else if (handle < (numControlPoints + 4 + scale()->getSFSize())) {
        int scaleHandle = handle - numControlPoints - 4;
        const float *spine = m_extrusion->spine()->getValues(); 
        const float *fscale = scale()->getValues(); 
        MFVec2f *newValue = new MFVec2f();
        for (int i = 0; i < scale()->getSFSize(); i++)
            newValue->setSFValue(i, fscale[i * 2], fscale[i * 2 + 1]);
        newValue->setSFValue(scaleHandle, v.x - spine[0], v.z - spine[2]);
        m_scene->setField(this, scale_Field(), newValue); 

    }
}

Node *
NodeSuperExtrusion::toExtrusion(void)
{
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }

    if (!m_extrusion) 
        return NULL;

    return m_extrusion->copy();
}

Vec3f   
NodeSuperExtrusion::getMinBoundingBox(void)
{
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }
 
    return m_extrusion->getMinBoundingBox();
}

Vec3f   
NodeSuperExtrusion::getMaxBoundingBox(void)
{
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }
 
    return m_extrusion->getMaxBoundingBox();
}

void
NodeSuperExtrusion::flip(int index)
{
    if (controlPoint())
        controlPoint()->flip(index);
    m_extrusionDirty = true;    
}

void
NodeSuperExtrusion::swap(int fromTo)
{
    if (controlPoint())
        controlPoint()->swap(fromTo);
    m_extrusionDirty = true;    
}

int
NodeSuperExtrusion::countPolygons()
{
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }

    if (!m_extrusion) return 0;

    return m_extrusion->countPolygons();
}

Node *
NodeSuperExtrusion::toNurbsCurve(void)
{
  NodeNurbsCurve *node = (NodeNurbsCurve *) m_scene->createNode("NurbsCurve");
  
  float *tmpControlPoints = new float[controlPoint()->getSize()];
  float *tmpWeights = new float[weight()->getSize()];
  float *tmpKnots = new float[knot()->getSize()];
  int tmpOrder = order()->getValue();  
  
  for(int i=0; i<(controlPoint()->getSize()); i++){
    tmpControlPoints[i] = controlPoint()->getValues()[i];
  }

  for(int i=0; i<(weight()->getSFSize()); i++){
    tmpWeights[i] = weight()->getValue(i);
  }
  
  for(int i=0; i<(knot()->getSFSize()); i++){
    tmpKnots[i] = knot()->getValue(i);
  }
    
  node->setField(node->tessellation_Field(), 
                 new SFInt32(spineTessellation()->getValue()));
  node->knot(new MFFloat(tmpKnots, knot()->getSFSize()));
  node->setField(node->order_Field(), new SFInt32(tmpOrder));
  node->setField(node->closed_Field(), new SFBool(true));
  node->createControlPoints(new MFVec3f(tmpControlPoints, 
                                        controlPoint()->getSize()));
  node->weight(new MFFloat(tmpWeights, weight()->getSFSize()));

  return node;
}

Node*
NodeSuperExtrusion::toNurbs(int uTessel, int vTessel, int uDegree, int vDegree)
{   
    NodeNurbsSurface *node = (NodeNurbsSurface *) m_scene->createNode(
                             "NurbsSurface");

    int vOrder = vDegree + 1;
    int uOrder = uDegree + 1;

    int oldUTess = superTessellation()->getValue();
    int oldVTess = spineTessellation()->getValue();

    int uTess = uTessel;
    int vTess = vTessel; 

    superTessellation(new SFInt32(uTess));
    spineTessellation(new SFInt32(vTess));
    delete m_mesh;
    createMesh(false);
    m_meshDirty = true;
    if (m_mesh == NULL)
        return NULL;

    uTess += 1;
    vTess += 1;

    int size = m_mesh->getVertices()->getSize();
    int uDimension = uTess;
    int vDimension = vTess;
    float *controlPoints = new float[size];
    float *weights = new float[size];
    float *uKnots = new float[uDimension + uOrder]; 
    float *vKnots = new float[vDimension + vOrder]; 

    for (int i = 0; i < size; i++) {
        controlPoints[i] = m_mesh->getVertices()->getValues()[i];
        weights[i] = 1;
    }
    //set u-knotvektor
    for(int i = 0; i < uOrder; i++){
        uKnots[i] = 0.0f;
        uKnots[i + uDimension] = (float) (uDimension - uOrder + 1);
    }
    for(int i = 0; i < (uDimension - uOrder); i++){
        uKnots[i + uOrder] = (float) (i + 1);  
    } 
    //set v-knotvektor
    for(int i = 0; i < vOrder; i++){
        vKnots[i] = 0.0f;
        vKnots[i + vDimension] = (float) (vDimension - vOrder + 1);
    }
    for(int i = 0; i < (vDimension - vOrder); i++){
        vKnots[i + vOrder] = (float) (i + 1);  
    } 
    node->setField(node->uDimension_Field(), new SFInt32(uDimension));
    node->setField(node->vDimension_Field(), new SFInt32(vDimension));
    node->uKnot(new MFFloat(uKnots, uDimension + uOrder));
    node->vKnot(new MFFloat(vKnots, vDimension + vOrder));
    node->setField(node->uOrder_Field(), new SFInt32(uOrder));
    node->setField(node->vOrder_Field(), new SFInt32(vOrder));
    node->setField(node->uClosed_Field(), new SFBool(true));
    node->setField(node->vClosed_Field(), new SFBool(true));
    node->createControlPoints(new MFVec3f(controlPoints, 
                                          uDimension * vDimension * 3));
    node->weight(new MFFloat(weights, uDimension * vDimension));
    node->ccw(new SFBool(m_mesh->ccw()));
    node->solid(new SFBool(m_mesh->solid()));

    node->setField(node->uTessellation_Field(), new SFInt32(uTess - 1));
    node->setField(node->vTessellation_Field(), new SFInt32(vTess - 1));

    for (int iteration = 0; iteration < 32; iteration++) {
        node->reInit();
        node->createMesh(false);    
        if (node->getVertices() != NULL) {
            assert(size == node->getVertices()->getSize() -
                           3 * node->getNumExtraVertices());
            float *vert = new float[size];
            MFVec3f *nurbsControlPoints = node->controlPoint();    
            for (int i = 0; i < size; i++) {
                vert[i] = node->getVertices()->getValues()[i];
                float meshValue = m_mesh->getVertices()->getValues()[i];
                float nurbsValue = nurbsControlPoints->getValues()[i];
                if (fabs(vert[i]) > FEPSILON)
                    vert[i] = nurbsValue * meshValue / vert[i];
            }
            for (int i = 0; i < size; i++) {
                nurbsControlPoints->setValue(i, vert[i]);
            }
        }
    }
    node->setField(node->uTessellation_Field(), new SFInt32(0));
    node->setField(node->vTessellation_Field(), new SFInt32(0));

    superTessellation(new SFInt32(oldUTess));
    spineTessellation(new SFInt32(oldVTess));

    return node;   
}

void
NodeSuperExtrusion::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }

    if (!m_extrusion) return;

    m_mesh = m_extrusion->getMesh();
    m_meshDirty = false;
}

void
NodeSuperExtrusion::createMesh(SuperExtrusionData &data)
{
    a(new SFFloat(data.a));
    b(new SFFloat(data.b));
    m(new SFFloat(data.m));
    n1(new SFFloat(data.n1));
    n2(new SFFloat(data.n2));
    n3(new SFFloat(data.n3));
    border(new SFFloat(data.border));
    bottomBorder(new SFFloat(data.bottomBorder));
    float *controlPoints = new float[controlPoint()->getSize()];
    for (int i = 0; i < controlPoint()->getSize(); i++)
         controlPoints[i] = data.controlPoint[i];
    controlPoint(new MFVec3f(controlPoints, controlPoint()->getSize()));
    float *weights = new float[weight()->getSize()];
    for (int i = 0; i < weight()->getSize(); i++)
         weights[i] = data.weight[i];
    weight(new MFFloat(weights, weight()->getSize()));
    float *knots = new float[knot()->getSize()];
    for (int i = 0; i < knot()->getSize(); i++)
        knots[i] = data.knot[i];
    knot(new MFFloat(knots, knot()->getSize()));
    m_extrusionDirty = true;
    createMesh();
}

void
NodeSuperExtrusion::copyData(SuperExtrusionData *data)
{
    data->a = a()->getValue();
    data->b = b()->getValue();
    data->m = m()->getValue();
    data->n1 = n1()->getValue();
    data->n2 = n2()->getValue();
    data->n3 = n3()->getValue();
    data->border = border()->getValue();
    data->bottomBorder = bottomBorder()->getValue();
    MFVec3f *controlPoints = (MFVec3f *) controlPoint()->copy();
    data->controlPoint = (float *) controlPoints->getValues();
    MFFloat *weights = (MFFloat *) weight()->copy();
    data->weight = (float *) weights->getValues();
    MFFloat *knots = (MFFloat *) knot()->copy();
    data->knot = (float *) knots->getValues();
}

void *
NodeSuperExtrusion::initializeData(void)
{
    SuperExtrusionData *data = new SuperExtrusionData();

    copyData(data);
    copyData(&m_tempStoreData);

    return data;
}

void
NodeSuperExtrusion::loadDataFromInterpolators(void *superExtrusionData, 
                                              Interpolator *inter,
                                              int field, float key)
{
    SuperExtrusionData *data = (SuperExtrusionData *)superExtrusionData;
    if (field == a_Field())
        inter->interpolate(key, &(data->a));
    else if (field == b_Field())
        inter->interpolate(key, &(data->b));
    else if (field == m_Field())
        inter->interpolate(key, &(data->m));
    else if (field == n1_Field())
        inter->interpolate(key, &(data->n1));
    else if (field == n2_Field())
        inter->interpolate(key, &(data->n2));
    else if (field == n3_Field())
        inter->interpolate(key, &(data->n3));
    else if (field == border_Field())
        inter->interpolate(key, &(data->border));
    else if (field == bottomBorder_Field())
        inter->interpolate(key, &(data->bottomBorder));
    else if (field == controlPoint_Field())
        inter->interpolate(key, data->controlPoint);
}

void
NodeSuperExtrusion::createMeshFromData(void* superExtrusionData, bool optimize)
{
    m_meshDirty = true;
    SuperExtrusionData *data = (SuperExtrusionData *)superExtrusionData;
    createMesh(*data);
}

void
NodeSuperExtrusion::finalizeData(void* data)
{
    m_extrusionDirty = true;
    createMesh(m_tempStoreData);
    delete (SuperExtrusionData *)data;
}

    
Node *
NodeSuperExtrusion::degreeElevate(int newDegree)
{
    //Nothing but simple application of existing class "NurbsCurveDegreeElevate" 

    if (newDegree <= ((order()->getValue())-1)){
        return NULL;
    }

    NodeSuperExtrusion *node = (NodeSuperExtrusion *)
                               m_scene->createNode("SuperExtrusion");

    NodeNurbsCurve *curve = (NodeNurbsCurve *)
                            m_nurbsCurve->degreeElevate(newDegree);

    if (curve == NULL)
        return NULL;

    //load new node
    node->a(new SFFloat(a()->getValue()));
    node->b(new SFFloat(b()->getValue()));
    node->m(new SFFloat(m()->getValue()));
    node->n1(new SFFloat(n1()->getValue()));
    node->n2(new SFFloat(n2()->getValue()));
    node->n3(new SFFloat(n3()->getValue()));
    node->border(new SFFloat(border()->getValue()));
    node->bottomBorder(new SFFloat(bottomBorder()->getValue()));
    node->superTessellation(new SFInt32(superTessellation()->getValue()));

    node->spineTessellation(new SFInt32(spineTessellation()->getValue()));
    node->controlPoint(new MFVec3f(curve->getControlPoints()));
    MFFloat *w = (MFFloat *)(curve->weight());
    node->weight(new MFFloat(w->getValues(), w->getSize()));
    MFFloat *k = (MFFloat *)(curve->knot());
    node->knot(new MFFloat(k->getValues(), k->getSize()));
    node->order(new SFInt32(curve->order()->getValue()));

    node->creaseAngle(new SFFloat(creaseAngle()->getValue()));
    node->beginCap(new SFBool(beginCap()->getValue()));
    node->endCap(new SFBool(endCap()->getValue()));
    MFVec2f *s = (MFVec2f *)(scale()->copy());
    node->scale(new MFVec2f((float *)s->getValues(), s->getSize()));
 
    return node;
}

void            
NodeSuperExtrusion::reInit(void) 
{
    m_extrusion = NULL;
    m_extrusionDirty = true; 
    Node::reInit();
}

int     
NodeSuperExtrusion::writeRib(int filedes, int indent)
{
    if (m_extrusionDirty) {
        createExtrusion();
        m_extrusionDirty = false;
    }

    if (!m_extrusion) return 0;

    m_extrusion->writeRib(filedes, indent);

    return 0;
}

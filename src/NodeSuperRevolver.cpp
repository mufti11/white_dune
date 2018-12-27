/*
 * NodeSuperRevolver.cpp
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

#include "NodeSuperRevolver.h"
#include "Proto.h"
#include "DuneApp.h"
#include "Scene.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFInt32.h"
#include "SFBool.h"
#include "Vec2f.h"
#include "NodeNurbsSurface.h"
#include "NurbsCurveDegreeElevate.h"
#include "MyMesh.h"
#include "Util.h"
#include "RenderState.h"
#include "resource.h"

ProtoSuperRevolver::ProtoSuperRevolver(Scene *scene)
  : Proto(scene, "SuperRevolver")
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
    nurbsTessellation.set(
          addExposedField(SFINT32, "nurbsTessellation", new SFInt32(0)));
    setFieldFlags(nurbsTessellation, FF_4KIDS);

    controlPoint.set(
          addExposedField(MFVEC2F, "controlPoint", new MFVec2f()));
    weight.set(
          addExposedField(MFFLOAT, "weight", new MFFloat(), new SFFloat(0.0f)));
    knot.set(
          addField(MFFLOAT, "knot", new MFFloat()));
    order.set(
          addField(SFINT32, "order", new SFInt32(3), new SFInt32(2)));

    creaseAngle.set( 
          addField(SFFLOAT, "creaseAngle", new SFFloat(M_PI), 
                   new SFFloat(0.0f)));
    setFieldFlags(creaseAngle, FF_4KIDS);
    ccw.set( 
          addField(SFBOOL, "ccw", new SFBool(false)));
    setFieldFlags(ccw, FF_4KIDS);
    solid.set( 
          addField(SFBOOL, "solid", new SFBool(false)));
    setFieldFlags(solid, FF_4KIDS);

    pieceOfCake.set( 
          addField(SFBOOL, "pieceOfCake", new SFBool(false)));
    setFieldFlags(pieceOfCake, FF_4KIDS);    

    addURLs(URL_SCRIPTED_NODES);
}

Node *
ProtoSuperRevolver::create(Scene *scene)
{
    return new NodeSuperRevolver(scene, this); 
}

NodeSuperRevolver::NodeSuperRevolver(Scene *scene, Proto *def)
  : MeshMorphingNode(scene, def)
{
    m_meshDirty = true;
    m_nurbsCurve = (NodeNurbsCurve *) scene->createNode("NurbsCurve");
    m_nurbsCurve->setInternal(true);
}

NodeSuperRevolver::~NodeSuperRevolver()
{
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
NodeSuperRevolver::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    int superTess = superTessellation()->getValue();
    int nurbsTess = nurbsTessellation()->getValue();

    if (superTess <= 0) superTess = TheApp->getTessellation();
    if (nurbsTess <= 0) nurbsTess = TheApp->getTessellation();

    superTess++;

    if (superTess < 3) return;
    if (nurbsTess < 3) return;

    float low = bottomBorder()->getValue();
    float high = border()->getValue();
    
    float inc1 = (high - low) / (superTess-1);
    if (inc1 < FEPSILON) return;
    float fa = a()->getValue();
    float fb = b()->getValue();
    float fm = m()->getValue();
    float fn1 = n1()->getValue();
    float fn2 = n2()->getValue();
    float fn3 = n3()->getValue();
    Array<float> r1(superTess);
    Array<float> c1(superTess);
    Array<float> s1(superTess);
    for (int a1 = 0; a1 < superTess; a1++) {
        float angle1 = low + a1 * inc1;
        r1[a1] = superFormula(angle1, fa, fb, fm, fn1, fn2, fn3);
        c1[a1] = cos(angle1 + M_PI / 2.0);
        s1[a1] = sin(angle1 + M_PI / 2.0);
    }

    if (m_nurbsCurve == NULL)
        m_nurbsCurve = (NodeNurbsCurve *) m_scene->createNode("NurbsCurve");
    int len = controlPoint()->getSFSize();
    float *points = new float[len * 3];
    for (int i = 0; i < len; i++) {
         points[i * 3] = controlPoint()->getValues()[2 * i];
         points[i * 3 + 1] = controlPoint()->getValues()[2 * i + 1];
         points[i * 3 + 2] = 0;
    }
    m_nurbsCurve->setControlPoints(new MFVec3f(points, len * 3));
    float *weights = new float[weight()->getSize()];
    for (int i = 0; i < weight()->getSize(); i++)
         weights[i] = weight()->getValues()[i];
    m_nurbsCurve->weight(new MFFloat(weights, weight()->getSize()));
    float *knots = new float[knot()->getSize()];
    for (int i = 0; i < knot()->getSize(); i++)
         knots[i] = knot()->getValues()[i];
    m_nurbsCurve->knot(new MFFloat(knots, knot()->getSize()));
    m_nurbsCurve->tessellation(new SFInt32(nurbsTess - 1));
    m_nurbsCurve->order(new SFInt32(order()->getValue()));
    const Vec3f *chain = m_nurbsCurve->getChain();
    int chainLength = m_nurbsCurve->getChainLength();
    Array<float> fchain(chainLength * 2);
    for (int i = 0; i < chainLength; i++) {
         fchain[i * 2    ] = chain[i].x;
         fchain[i * 2 + 1] = chain[i].y;
    }   
    int numVert = nurbsTess * superTess;
    int numCi = nurbsTess * superTess * 5;
    bool sidesClosed = false;    
    if ((border()->getValue() != (float) M_PI) &&
        (bottomBorder()->getValue() != (float) -M_PI))
        if (pieceOfCake()->getValue()) {
            sidesClosed = true;
            numVert += chainLength; 
            numCi += chainLength * 4 + 2; 
        }
    
    MFVec3f *vert = new MFVec3f(numVert);
    int* ci = new int[numCi];

    int index = 0;
    int cindex = 0;
    for (int a2 = 0; a2 < nurbsTess; a2++) {
        for (int a1 = 0; a1 < superTess; a1++) {
            float x = fchain[a2 * 2] * r1[a1] * c1[a1];
            float y = fchain[a2 * 2 + 1];
            float z = fchain[a2 * 2] * r1[a1] * s1[a1];
            vert->setSFValue(index, x, y, z);
            index++;
            int n1 = a1 + 1;
            bool closed = false;
            if (n1 == superTess) {
                n1 = 0;
                if (sidesClosed)
                    closed = true;
            }
            int n2 = a2 + 1;
            if (n2 < nurbsTess) {
                if (closed) {
                    ci[cindex++] = -1;
                } else {
                    if (a1 < superTess / 2) {
                        ci[cindex++] = a1 + a2 * superTess;
                        ci[cindex++] = n1 + a2 * superTess;
                        ci[cindex++] = n1 + n2 * superTess;
                        ci[cindex++] = a1 + n2 * superTess;
                        ci[cindex++] = -1;
                    } else {
                        ci[cindex++] = n1 + a2 * superTess;
                        ci[cindex++] = n1 + n2 * superTess;
                        ci[cindex++] = a1 + n2 * superTess;
                        ci[cindex++] = a1 + a2 * superTess;
                        ci[cindex++] = -1;
                    }
                }
            }
        }
    }
    if (sidesClosed) {
        float highest = chain[chainLength - 1].y;
        float lowest = chain[0].y;
        if (lowest > highest) {
            highest = chain[0].y;
            lowest = chain[chainLength - 1].y;
        }
        for (int i = 0; i < chainLength; i++)
             ci[cindex++] = superTess * i;
        for (int i = chainLength - 1; i >= 0; i--) {
             float y = chain[i].y;
             if (y > highest)
                 y = highest;
             if (y < lowest)
                 y = lowest;
             vert->setSFValue(nurbsTess * superTess + i, 0, y, 0);
             ci[cindex++] = nurbsTess * superTess + i;
        }
        ci[cindex++] = -1;
        for (int i = chainLength - 1; i >= 0; i--)
             ci[cindex++] = superTess * i + (superTess - 1);
        for (int i = 0; i < chainLength; i++)
             ci[cindex++] = nurbsTess * superTess + i;
        ci[cindex++] = -1;
    }

    cindex = optimizeNormals(ci, vert, NULL, cindex, false, 
                             cleanDoubleVertices);    

    MFInt32 *coordIndex = new MFInt32(ci, cindex);
    MFVec2f *texCoord = generateTextureCoordinates(vert, coordIndex);
    Array<MFVec2f *> texCoords;
    texCoords.append(texCoord);

    bool bsolid = solid()->getValue();
    int meshFlags = ccw()->getValue() ? MESH_CCW : 0;
    if (bsolid)
        meshFlags |= MESH_SOLID;
    meshFlags |= MESH_COLOR_PER_VERTEX;
    meshFlags |= MESH_NORMAL_PER_VERTEX;

    m_mesh = new MyMesh(this, vert, coordIndex, NULL, NULL, NULL, NULL, 
                        texCoords, NULL, creaseAngle()->getValue(), meshFlags);
    if (sidesClosed)
        m_mesh->triangulate(this);
    m_meshDirty = false;
}

int
NodeSuperRevolver::writeProto(int f)
{
    return ((Node *)this)->writeProto(f, "", "scripted_Nodes"
#ifdef HAVE_SCRIPTED_NODES_PROTO_URL
                                            , HAVE_SCRIPTED_NODES_PROTO_URL
#endif
                                            );
}

void
NodeSuperRevolver::setField(int index, FieldValue *value, int cf)
{
    m_meshDirty = true;
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
NodeSuperRevolver::drawAHandles()
{
    float fa = a()->getValue();
    RenderState state;

    glPushMatrix();
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushName(0);

    int numControlPoints = controlPoint()->getSize() / 2;
        
    state.startDrawHandles();
    for (int i = 0; i < 4; i++) {
        float x = 0;
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
        glLoadName(numControlPoints + 4 + i);
        state.drawHandle(Vec3f(x, 0, z));
    }
    state.endDrawHandles();
    glPopName();
    glEnable(GL_LIGHTING);
    glPopAttrib();
    glPopMatrix(); 
}

void  
NodeSuperRevolver::drawHandles()
{
    if (meshDirty()) {
        createMesh();
        m_meshDirty = false;
    }

    if (m_mesh == NULL) return;

    m_nurbsCurve->drawHandles();
    drawAHandles();
}

Vec3f
NodeSuperRevolver::getHandle(int handle, int *constraint, int *field)
{
    *field = controlPoint_Field() ;

    int numControlPoints = controlPoint()->getSize() / 2;
    if (handle >= 0 && handle < numControlPoints) {
        *field = controlPoint_Field();
        Vec3f ret(0, 0, 0);
        float fweight = weight()->getValue(handle);
        ret.x = controlPoint()->getValues()[handle * 2] / fweight;
        ret.y = controlPoint()->getValues()[handle * 2 + 1] / fweight;
        TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, "controlPoint", 
                                          handle, ret);
        return ret;
    } else if (handle < (numControlPoints + 4)) {
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
    } else {
        return Vec3f(0.0f, 0.0f, 0.0f);
    }
}


void
NodeSuperRevolver::setHandle(int handle, const Vec3f &v) 
{
    MFVec2f *oldValue = controlPoint();
    int numControlPoints = controlPoint()->getSize() / 2;

    if (handle >= 0 && handle < numControlPoints) {
        MFVec2f *newValue = (MFVec2f *)oldValue->copy();
        Vec3f v2 = v * weight()->getValue(handle); 
        m_meshDirty = true;
        newValue->setValue(handle * 2, v2.x);
        newValue->setValue(handle * 2 + 1, v2.y);
        m_scene->setField(this, controlPoint_Field(), newValue);
    } else {
        float fa = a()->getValue();
        switch (handle - numControlPoints) {
          case 0:
            fa = v.x;
            break; 
          case 1:
            fa = -v.x;
            break;
          case 2:
            fa = -v.z;
            break;
          case 3:
            fa = v.z; 
            break;    
        }
        m_scene->setField(this, a_Field(), new SFFloat(fa));
    }
}

void
NodeSuperRevolver::flip(int index)
{
    float epsilon = TheApp->GetHandleEpsilon();
    if (index == 1) {
        if (controlPoint())
            controlPoint()->flip(index);
    } else if ((index == 2) && 
               ((border()->getValue() < (M_PI - epsilon)) || 
                (bottomBorder()->getValue() > (-M_PI + epsilon)))) {
        Node *mesh = m_scene->convertToIndexedFaceSet(this);
        if (mesh != NULL)
            mesh->flip(index);
    }
    m_meshDirty = true;    
}

void
NodeSuperRevolver::swap(int fromTo)
{
    Node *mesh = m_scene->convertToIndexedFaceSet(this);
    if (mesh != NULL)
        mesh->swap(fromTo);
    m_meshDirty = true;    
}

Node*
NodeSuperRevolver::toNurbsCurve(void)
{
  NodeNurbsCurve *node = (NodeNurbsCurve *) m_scene->createNode("NurbsCurve");
  
  float *tmpControlPoints = new float[controlPoint()->getSFSize() * 3];
  float *tmpWeights = new float[weight()->getSize()];
  float *tmpKnots = new float[knot()->getSize()];
  int tmpOrder = order()->getValue();  
  
  for(int i=0; i<(controlPoint()->getSFSize()); i++) {
    tmpControlPoints[3 * i] = controlPoint()->getValues()[2 * i];
    tmpControlPoints[3 * i + 1] = controlPoint()->getValues()[2 * i + 1];
    tmpControlPoints[3 * i + 2] = 0.0f;
  }

  for(int i=0; i<(weight()->getSFSize()); i++){
    tmpWeights[i] = weight()->getValue(i);
  }
  
  for(int i=0; i<(knot()->getSFSize()); i++){
    tmpKnots[i] = knot()->getValue(i);
  }
    
  node->setField(node->tessellation_Field(), 
                 new SFInt32(nurbsTessellation()->getValue()));
  node->knot(new MFFloat(tmpKnots, knot()->getSFSize()));
  node->setField(node->order_Field(), new SFInt32(tmpOrder));
  node->createControlPoints(new MFVec3f(tmpControlPoints, 
                                        controlPoint()->getSFSize() * 3));
  node->setField(node->closed_Field(), new SFBool(true));
  node->weight(new MFFloat(tmpWeights, weight()->getSFSize()));

  return node;
}

Node*
NodeSuperRevolver::degreeElevate(int newDegree)
{
  //Nothing but simple application of existing class "NurbsCurveDegreeElevate" 

  if(newDegree <= ((order()->getValue())-1)){
    return NULL;
  }

  NodeSuperRevolver *node = (NodeSuperRevolver *)
                            m_scene->createNode("SuperRevolver");

  if(newDegree > ((order()->getValue())-1)){
    
    //load old values
    int dimension = controlPoint()->getSFSize();
    Vec3f *tPoints = new Vec3f[dimension];
    float *tWeights = new float[dimension];
    int knotSize = knot()->getSize();
    Array<float> tKnots(knotSize);
    int deg = order()->getValue() - 1;
    int upDegree = newDegree - deg;

    for (int i=0; i<dimension; i++){
      tPoints[i].x = controlPoint()->getVec(i).x;
      tPoints[i].y = controlPoint()->getVec(i).y;
      tPoints[i].z = 0;
      tWeights[i] =weight()->getValue(i);
    }
    
    for (int i=0; i<knotSize; i++){
      tKnots[i] = knot()->getValue(i);
    }

    //elevate curve
    NurbsCurveDegreeElevate elevatedCurve(tPoints, tWeights, tKnots, dimension, deg, upDegree);

    //get new values
    int newDimension = elevatedCurve.getPointSize();
    int newKnotSize = elevatedCurve.getKnotSize();
    int newOrder = newDegree + 1;

    float *newControlPoints = new float[newDimension * 2];
    float *newWeights = new float[newDimension];
    float *newKnots = new float[newDimension + newOrder]; 

    for (int i=0; i<newDimension; i++){
      newControlPoints[(i*2)]   = elevatedCurve.getControlPoints(i).x;
      newControlPoints[(i*2)+1] = elevatedCurve.getControlPoints(i).y;
      newWeights[i] = elevatedCurve.getWeights(i);
    }
    
    for (int i=0; i<newKnotSize; i++){
      newKnots[i] = elevatedCurve.getKnots(i);
    }

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

    node->nurbsTessellation(new SFInt32(nurbsTessellation()->getValue()));
    node->controlPoint(new MFVec2f(newControlPoints, newDimension * 2));
    node->weight(new MFFloat(newWeights, newDimension));
    node->knot(new MFFloat(newKnots, newDimension + newOrder));
    node->order(new SFInt32(newOrder));

    node->creaseAngle(new SFFloat(creaseAngle()->getValue()));
    node->pieceOfCake(new SFBool(pieceOfCake()->getValue()));
    node->ccw(new SFBool(ccw()->getValue()));
    node->solid(new SFBool(solid()->getValue()));
 
    return node;
  }
  return NULL;
}

Node*
NodeSuperRevolver::toNurbs(int uTessel, int vTessel, int uDegree, int vDegree)
{   
    NodeNurbsSurface *node = (NodeNurbsSurface *) m_scene->createNode(
                             "NurbsSurface");

    int vOrder = vDegree + 1;
    int uOrder = uDegree + 1;

    int oldUTess = superTessellation()->getValue();
    int oldVTess = nurbsTessellation()->getValue();

    int uTess = uTessel;
    int vTess = vTessel; 

    superTessellation(new SFInt32(uTess));
    nurbsTessellation(new SFInt32(vTess));
    createMesh(false);
    m_meshDirty = true;
    if (m_mesh == NULL)
        return NULL;

    uTess += 1;

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

    node->setField(node->uTessellation_Field(), new SFInt32(uTess-1));
    node->setField(node->vTessellation_Field(), new SFInt32(vTess-1));

    for (int iteration = 0; iteration < 32; iteration++) {
        node->reInit();
        node->createMesh(false);    
        if (node->getVertices() != NULL) {
            assert(size == node->getVertices()->getSize() - 
                           node->getNumExtraVertices() * 3);
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
    nurbsTessellation(new SFInt32(oldVTess));

    return node;   
}

void
NodeSuperRevolver::createMesh(SuperRevolverData &data)
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
    controlPoint(new MFVec2f(controlPoints, controlPoint()->getSize()));
    float *weights = new float[weight()->getSize()];
    for (int i = 0; i < weight()->getSize(); i++)
         weights[i] = data.weight[i];
    weight(new MFFloat(weights, weight()->getSize()));
    float *knots = new float[knot()->getSize()];
    for (int i = 0; i < knot()->getSize(); i++)
        knots[i] = data.knot[i];
    knot(new MFFloat(knots, knot()->getSize()));
    createMesh();
}

void
NodeSuperRevolver::copyData(SuperRevolverData *data)
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
NodeSuperRevolver::initializeData(void)
{
    SuperRevolverData *data = new SuperRevolverData();

    copyData(data);
    copyData(&m_tempStoreData);

    return data;
}

void
NodeSuperRevolver::loadDataFromInterpolators(void *superRevolverData, 
                                             Interpolator *inter,
                                             int field, float key)
{
    SuperRevolverData *data = (SuperRevolverData *)superRevolverData;
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
    else if (field == weight_Field())
        inter->interpolate(key, data->weight);
    else if (field == knot_Field())
        inter->interpolate(key, data->knot);
}

void
NodeSuperRevolver::createMeshFromData(void* superRevolverData, bool optimize)
{
    m_meshDirty = true;
    SuperRevolverData *data = (SuperRevolverData *)superRevolverData;
    createMesh(*data);
}

void
NodeSuperRevolver::finalizeData(void* data)
{
    m_meshDirty = true;
    createMesh(&m_tempStoreData);
    delete (SuperRevolverData *)data;
}


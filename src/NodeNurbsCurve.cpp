/*
 * NodeNurbsCurve.cpp
 *
 * Copyright (C) 2003 Th. Rothermel, 2019 J. "MUFTI" Scheurich
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

#ifndef _WIN32
# include "stdlib.h"
#endif

#include "stdafx.h"
#include "NodeNurbsCurve.h"
#include "Scene.h"
#include "MoveCommand.h"
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
#include "NodeNurbsSurface.h"
#include "NurbsMakeRevolvedSurface.h"
#include "NurbsCurveDegreeElevate.h"
#include "Util.h"
#include "NodeIndexedLineSet.h"
#include "NodePositionInterpolator.h"
#include "NodeOrientationInterpolator.h"
#include "NodeCurveAnimation.h"
#include "NodeSuperExtrusion.h"
#include "NodeSuperRevolver.h"
#include "NodeNurbsPositionInterpolator.h"
#include "NodeNurbsOrientationInterpolator.h"
#include "NodeCoordinateDouble.h"
#include "NodeCoordinateInterpolator.h"
#include "EventIn.h"
#include "EventOut.h"
#include "resource.h"


void 
ProtoNurbsCurve::addElements(bool curveAnimation)
{
    controlPoint.set(
          addExposedField(MFVEC3F, "controlPoint", new MFVec3f()));
    setFieldFlags(controlPoint, curveAnimation ? FF_ALWAYS : FF_VRML_ONLY);
    controlPointX3D.set(
          addExposedField(SFNODE, "controlPoint", new SFNode(NULL), 
                          COORDINATE_NODE));
    setFieldFlags(controlPointX3D, curveAnimation ? FF_NEVER : FF_X3D_ONLY);
    tessellation.set(
          addExposedField(SFINT32, "tessellation", new SFInt32(0)));
    weight.set(
          addExposedField(MFFLOAT, "weight", new MFFloat(), new SFFloat(0.0f)));
    knot.set(
          addField(MFFLOAT, "knot", new MFFloat()));
    order.set(
          addField(SFINT32, "order", new SFInt32(3), new SFInt32(2)));
    closed.set(
          addField(SFBOOL, "closed", new SFBool(false)));
    setFieldFlags(closed, FF_X3D_ONLY);

    addURLs(URL_X3D);
}

ProtoNurbsCurve::ProtoNurbsCurve(Scene *scene)
  : Proto(scene, "NurbsCurve")
{
    addElements(false);
}

ProtoNurbsCurve::ProtoNurbsCurve(Scene *scene, const char* name,
                                 bool curveAnimation)
  : Proto(scene, name)
{
    addElements(curveAnimation);
}

Node *
ProtoNurbsCurve::create(Scene *scene)
{
    return new NodeNurbsCurve(scene, this); 
}

NodeNurbsCurve::NodeNurbsCurve(Scene *scene, Proto *proto)
  : ChainBasedGeometryNode(scene, proto)
{
    m_inRepair = false;
    m_isInternal = false;
    repairKnotAndWeight();
}


int 
NodeNurbsCurve::repairField(int field) const
{
    bool x3d = (m_scene->isX3d()) && 
               (m_proto->getType() != DUNE_CURVE_ANIMATION);
    if (x3d && (field == controlPoint_Field()))
        return controlPointX3D_Field();
    else if (!x3d && (field == controlPointX3D_Field()))
        return controlPoint_Field();
    return field;
}

// store controlPoint information (MFVec3f *) both in 
// controlPoint (VRML97 Amendment 1) and controlPoint.point (X3D)
void
NodeNurbsCurve::setField(int index, FieldValue *value, int cf)
{
    m_chainDirty = true;
    if (index == controlPoint_Field()) {
        if (controlPointX3D()->getType() == X3D_COORDINATE_DOUBLE) {
            NodeCoordinateDouble *coord = (NodeCoordinateDouble *)
                                          controlPointX3D()->getValue();
            if (coord == NULL) {
                createControlPoints(((MFVec3d *)value)->getMFVec3f());
                return;
            }
            m_scene->setField(coord, coord->point_Field(), value);
        } else if (controlPointX3D()->getType() == VRML_COORDINATE) {
            NodeCoordinate *coord = (NodeCoordinate *)
                                    controlPointX3D()->getValue();
            if (coord == NULL) {
                createControlPoints((MFVec3f *)value);
                return;
            }
            m_scene->setField(coord, coord->point_Field(), value);
        }
    } 
    if (index == controlPointX3D_Field()) {
        if ((value != NULL) && (value->getType() == SFNODE)) {
            Node *node = ((SFNode *)value)->getValue();
            if (node->getType() == X3D_COORDINATE_DOUBLE) {
                NodeCoordinateDouble *coord = (NodeCoordinateDouble *) node;
                Node::setField(controlPoint_Field(), 
                               coord->point()->getMFVec3f(), cf);
            } else if (node->getType() == VRML_COORDINATE) {
                NodeCoordinate *coord = (NodeCoordinate *) node;
                Node::setField(controlPoint_Field(), coord->point(), cf);
            }
        }
    } 
    Node::setField(index, value, cf);
    if (m_isInternal && hasParent()) {
        for (int i = 0; i < getNumParents(); i++)
            getParent(i)->update();
    }
    repairKnotAndWeight();
}

void
NodeNurbsCurve::update()
{
    repairKnotAndWeight();
    m_chainDirty = true;
    if (m_isInternal || hasParent())
        getParents().update();
}

MFVec3f *
NodeNurbsCurve::getControlPoints(void)
{
    return controlPoint();
}

void
NodeNurbsCurve::setControlPoints(MFVec3f *points)
{
    if (controlPointX3D()->getType() == X3D_COORDINATE_DOUBLE) {
        NodeCoordinateDouble *coord = (NodeCoordinateDouble *)
                                      controlPointX3D()->getValue();
        if (coord == NULL) {
            createControlPoints(points);
            return;
        }
        Node::setField(controlPoint_Field(), points);
        ((Node *)coord)->Node::setField(coord->point_Field(), points);
    } else if (controlPointX3D()->getType() == VRML_COORDINATE) {
        NodeCoordinate *coord = (NodeCoordinate *)controlPointX3D()->getValue();
        if (coord == NULL) {
            createControlPoints(points);
            return;
        }
        Node::setField(controlPoint_Field(), points);
        ((Node *)coord)->Node::setField(coord->point_Field(), points);
    } else {
        NodeCoordinate *coord = (NodeCoordinate *)controlPointX3D()->getValue();
        if (coord == NULL) {
            createControlPoints(points);
            return;
        }
        Node::setField(controlPoint_Field(), points);
        ((Node *)coord)->Node::setField(coord->point_Field(), points);
    }

    m_chainDirty = true;
}

void
NodeNurbsCurve::createControlPoints(MFVec3f *points)
{
    controlPointX3D(new SFNode(m_scene->createNode("Coordinate",
                                                   NODE_FLAG_X3D_ONLY))); 
    setControlPoints(points);
}

Node *
NodeNurbsCurve::convert2X3d(void)
{
    NodeCoordinate *coord = (NodeCoordinate *)controlPointX3D()->getValue();
    if (coord != NULL) {
        coord->getVariableName();
        m_scene->changeRoutes(coord, coord->point_Field(), 
                              this, controlPoint_Field());
    } else {
        MFVec3f *points = (MFVec3f *)getUntranslatedField(
           ((ProtoNurbsSurface *)getPrimaryProto())->controlPoint);
        if (points->getSize() > 0) {
            points->ref();
            createControlPoints(points);
            coord = (NodeCoordinate *)controlPointX3D()->getValue();
            m_scene->changeRoutes(coord, coord->point_Field(), 
                                  this, controlPoint_Field());
        }
    }
    return NULL;
}

Node *
NodeNurbsCurve::convert2Vrml(void) 
{
    if (controlPointX3D()->getType() == X3D_COORDINATE_DOUBLE) {
        NodeCoordinateDouble *coord = (NodeCoordinateDouble *) 
                                      controlPointX3D()->getValue();
        if (coord != NULL) {
            m_scene->changeRoutes(this, controlPoint_Field(),
                                  coord, coord->point_Field());
        }
    } else {
        NodeCoordinate *coord = (NodeCoordinate *)controlPointX3D()->getValue();
        if (coord != NULL) {
            m_scene->changeRoutes(this, controlPoint_Field(),
                                  coord, coord->point_Field());
        }
    }
    return NULL;
}

void
NodeNurbsCurve::createChain()
{
    createChain((Vec3f *) getControlPoints()->getValues());
}  

void
NodeNurbsCurve::createChain(void *data)
{
    Vec3f* cpoints = (Vec3f *)data;
    if (getControlPoints() == NULL)
        return;
    int iTess = 32;
    if (tessellation())
        iTess = tessellation()->getValue();
    float *weights = NULL;
    int iDimension = getControlPoints()->getSize() / 3;

    if ((weight() == NULL) || (weight() && weight()->getSize() == 0)) {
        weights = new float[iDimension];
        for (int i=0; i<iDimension; i++){
            weights[i] = 1.0f;
        }
    } 
    else if(weight() && weight()->getSize() != iDimension) {
        return;
    }
    
    if (iTess<=0)
        iTess = TheApp->getTessellation();

    int size = iTess + 1;
    Vec3f *tess = new Vec3f[size];
    
    if (knot()->getSize() == 0)
        return;

    const float *knots = knot()->getValues();
    
    float inc = (knots[knot()->getSize()-1] - knots[0]) / iTess;
    const float *w = weights ? weights : weight()->getValues();
    int i;
    float u;
    
    for (i = 0, u = knots[0]; i <= iTess; i++, u = u + inc) {
        tess[i] = curvePoint(iDimension, order()->getValue(), knots, cpoints,
                             w, u);
    }

    m_chain.resize(size);  
    for (int i=0; i<size; i++)
        m_chain[i] = tess[i];
    m_chainDirty = false;
}

void
NodeNurbsCurve::drawAsHandle(int handle)
{
    glPushName(0);
    glDisable(GL_LIGHTING);
    float c[4];
    c[0] = c[1] = c[2] = c[3] = 1;
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);
    glLoadName(NO_HANDLE);
    NodeNurbsCurve::draw();
    glPopName();
    glEnable(GL_LIGHTING);
} 

void
NodeNurbsCurve::drawHandles()
{
    if (getControlPoints() == NULL)
        return;
    int iDimension = getControlPoints()->getSize() / 3;
    RenderState state;

    if (weight()->getSize() != iDimension) {
        return;
    }

    glPushName(0);
    glDisable(GL_LIGHTING);
    Util::myGlColor3f(1.0f, 1.0f, 1.0f);

    glLoadName(NO_HANDLE);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < iDimension; i++) {
        const float *v = getControlPoints()->getValue(i);
        float w = weight()->getValue(i);
        glVertex3f(v[0] / w, v[1] / w, v[2] / w);
    }
    glEnd();
    
    state.startDrawHandles();
    for (int ci = 0; ci < iDimension; ci++) {
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
NodeNurbsCurve::getHandle(int handle, int *constraint, int *field)
{
    *field = controlPoint_Field() ;

    if (handle >= 0 && handle < getControlPoints()->getSize() / 3) {
        Vec3f ret((Vec3f)getControlPoints()->getValue(handle) / 
                   weight()->getValue(handle));
        TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, "controlPoint", 
                                          handle, ret);
        return ret;
    } else {
        *field = -1;
        return Vec3f(0.0f, 0.0f, 0.0f);
    }
}

void
NodeNurbsCurve::setHandle(int handle, const Vec3f &v)
{
    MFVec3f *newValue = new MFVec3f(*getControlPoints());

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
    }
    if (m_isInternal && hasParent()) {
        for (int i = 0; i < getNumParents(); i++)
            getParent(i)->update();
    }
}

void
NodeNurbsCurve::setHandle(MFVec3f *value, int handle, float newWeight,
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
        m_chainDirty = true;
        if (controlPointX3D()->getType() == X3D_COORDINATE_DOUBLE) {
            NodeCoordinateDouble *pointNode = (NodeCoordinateDouble *)
                                              controlPointX3D()->getValue();
            if (pointNode == NULL) {
                createControlPoints(newValue);
                return;
            }
            m_scene->setField(this, controlPoint_Field(), newValue);
            m_scene->setField(pointNode, pointNode->point_Field(), 
                              newValue->copy());
        } else {
            NodeCoordinate *pointNode = (NodeCoordinate *)
                                        controlPointX3D()->getValue();
            if (pointNode == NULL) {
                createControlPoints(newValue);
                return;
            }
            m_scene->setField(this, controlPoint_Field(), newValue);
            m_scene->setField(pointNode, pointNode->point_Field(), 
                              newValue->copy());
        }
    }
}

void
NodeNurbsCurve::setHandle(float newWeight, 
                            const Vec3f &newV, const Vec3f &oldV)
{
    setHandle(controlPoint(), -1, newWeight, newV, oldV);
}

int
NodeNurbsCurve::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f, "EXTERNPROTO NurbsCurve[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f, " ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:web3d:vrml97:node:NurbsCurve\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:inet:blaxxun.com:node:NurbsCurve\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:ParaGraph:NurbsCURVE\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"NurbsCurvePROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#ifdef HAVE_VRML97_AMENDMENT1_PROTO_URL
    RET_ONERROR( mywritestr(f, " \"") )
    RET_ONERROR( mywritestr(f, HAVE_VRML97_AMENDMENT1_PROTO_URL) )
    RET_ONERROR( mywritestr(f, "/NurbsCurvePROTO.wrl") )
    RET_ONERROR( mywritestr(f, "\"\n") )
    TheApp->incSelectionLinenumber();
#else
    RET_ONERROR( mywritestr(f, " \"NurbsCurvePROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#endif
    RET_ONERROR( mywritestr(f, " \"http://wdune.ourproject.org/docs/vrml97Amendment1/NurbsCurvePROTO.wrl\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int             
NodeNurbsCurve::write(int filedes, int indent)
{
    if (m_scene->isPureVRML()) {
        Node *node = toIndexedLineSet();
        addCoordinateInterpolator(node, true);
        RET_ONERROR( node->write(filedes, indent) )
        node->unref();
    } else
        RET_ONERROR( Node::write(filedes, indent) )
    return 0;
}

Node*
NodeNurbsCurve::toNurbs(int narcs, int pDegree, float rDegree, 
                        Vec3f &P1, Vec3f &P2)
{
  NodeNurbsSurface *node = (NodeNurbsSurface *)
                            m_scene->createNode("NurbsSurface");
  
  Vec3f *tmpControlPoints = new Vec3f[getControlPoints()->getSFSize()];
  float *tmpWeights = new float[weight()->getSize()];
  float *vKnots = new float [knot()->getSize()];
  int vOrder = order()->getValue();
  int vDimension = weight()->getSize();
  
  for(int i=0; i<(getControlPoints()->getSFSize()); i++){
    tmpControlPoints[i] = getControlPoints()->getValue(i);
  }

  for(int i=0; i<(weight()->getSFSize()); i++){
    tmpWeights[i] = weight()->getValue(i);
  }
  
  for(int i=0; i<(knot()->getSFSize()); i++){
    vKnots[i] = knot()->getValue(i);
  }
    
  Vec3f point = P1;
  Vec3f vector = P2 - P1;
  vector.normalize();
  if((vector.x==0) && (vector.y==0) && (vector.z==0)) {return NULL;}
  
  NurbsMakeRevolvedSurface surface(tmpControlPoints, tmpWeights, vDimension, 
                                   narcs, rDegree, pDegree, point, vector);
  if (!surface.isValid())
      return NULL;

  int uOrder = pDegree + 1;
  float *controlPoints = new float[surface.getPointSize()];
  float *weights = new float[surface.getWeightSize()];
  int uDimension = surface.getWeightSize() / vDimension;
  float *uKnots = new float[uDimension + uOrder]; 

  //get results of rotation
  //get control points
  for(int i=0; i<(surface.getPointSize()); i++){
    controlPoints[i] = surface.getControlPoints(i);
  }
  //weights
  for(int i=0; i<(surface.getWeightSize()); i++){
    weights[i] = surface.getWeights(i);
  }
  if (pDegree == 1){
    //set u-knotvektor
    for(int i=0; i<uOrder; i++){
      uKnots[i] = 0.0f;
      uKnots[i+uDimension] = (float) (uDimension - uOrder +1);
    }
    for(int i=0; i<(uDimension-uOrder); i++){
      uKnots[i+uOrder] = (float) (i + 1);  
    } 
  }  
  else {
    //u-knotenvector
    for(int i=0; i<(surface.getKnotSize()); i++){
      uKnots[i] = surface.getKnots(i);
    }
  }
  
  node->setField(node->uDimension_Field(), new SFInt32(uDimension));
  node->setField(node->vDimension_Field(), new SFInt32(vDimension));
  node->uKnot(new MFFloat(uKnots, uDimension + uOrder));
  node->vKnot(new MFFloat(vKnots, vDimension + vOrder));
  node->setField(node->uOrder_Field(), new SFInt32(uOrder));
  node->setField(node->vOrder_Field(), new SFInt32(vOrder));
  node->createControlPoints(new MFVec3f(controlPoints, 
                                        uDimension * vDimension * 3));
  node->weight(new MFFloat(weights, uDimension * vDimension));

  return node;
}  

Node*
NodeNurbsCurve::toSuperExtrusion(void)
{
  NodeSuperExtrusion *node = (NodeSuperExtrusion *)
                            m_scene->createNode("SuperExtrusion");
  
  float *tmpControlPoints = new float[getControlPoints()->getSize()];
  float *tmpWeights = new float[weight()->getSize()];
  float *tmpKnots = new float[knot()->getSize()];
  int tmpOrder = order()->getValue();  
  
  for(int i=0; i<(getControlPoints()->getSize()); i++){
    tmpControlPoints[i] = getControlPoints()->getValues()[i];
  }

  for(int i=0; i<(weight()->getSFSize()); i++){
    tmpWeights[i] = weight()->getValue(i);
  }
  
  for(int i=0; i<(knot()->getSFSize()); i++){
    tmpKnots[i] = knot()->getValue(i);
  }
    
  node->setField(node->spineTessellation_Field(), 
                 new SFInt32(tessellation()->getValue()));
  node->knot(new MFFloat(tmpKnots, knot()->getSFSize()));
  node->setField(node->order_Field(), new SFInt32(tmpOrder));
  node->controlPoint(new MFVec3f(tmpControlPoints, 
                                 getControlPoints()->getSize()));
  node->weight(new MFFloat(tmpWeights, weight()->getSFSize()));

  return node;
}  

Node*
NodeNurbsCurve::toSuperRevolver(void)
{
  NodeSuperRevolver *node = (NodeSuperRevolver *)
                            m_scene->createNode("SuperRevolver");
  
  float *tmpControlPoints = new float[getControlPoints()->getSFSize() * 2];
  float *tmpWeights = new float[weight()->getSize()];
  float *tmpKnots = new float[knot()->getSize()];
  int tmpOrder = order()->getValue();  
  
  for(int i=0; i<(getControlPoints()->getSFSize()); i++){
    tmpControlPoints[2 * i] = getControlPoints()->getValues()[3 * i];
    tmpControlPoints[2 * i + 1] = getControlPoints()->getValues()[3 * i + 1];
  }

  for(int i=0; i<(weight()->getSFSize()); i++){
    tmpWeights[i] = weight()->getValue(i);
  }
  
  for(int i=0; i<(knot()->getSFSize()); i++){
    tmpKnots[i] = knot()->getValue(i);
  }
    
  node->setField(node->nurbsTessellation_Field(), 
                 new SFInt32(tessellation()->getValue()));
  node->knot(new MFFloat(tmpKnots, knot()->getSFSize()));
  node->setField(node->order_Field(), new SFInt32(tmpOrder));
  node->controlPoint(new MFVec2f(tmpControlPoints, 
                                 getControlPoints()->getSFSize() * 2));
  node->weight(new MFFloat(tmpWeights, weight()->getSFSize()));

  return node;
}  

Node *
NodeNurbsCurve::toCurveAnimation(void)
{
    NodeCurveAnimation *node = (NodeCurveAnimation *)
                                m_scene->createNode("CurveAnimation");
  
    float *tmpControlPoints = new float[getControlPoints()->getSize()];
    float *tmpWeights = new float[weight()->getSize()];
    float *tmpKnots = new float[knot()->getSize()];
    int tmpOrder = order()->getValue();  
  
    for (int i = 0; i < getControlPoints()->getSize(); i++)
        tmpControlPoints[i] = getControlPoints()->getValues()[i];

    for (int i = 0; i < weight()->getSFSize(); i++)
        tmpWeights[i] = weight()->getValue(i);

    for (int i = 0; i < knot()->getSFSize(); i++)
        tmpKnots[i] = knot()->getValue(i);
    
    node->setField(node->tessellation_Field(), 
                   new SFInt32(tessellation()->getValue()));
    node->knot(new MFFloat(tmpKnots, knot()->getSFSize()));
    node->setField(node->order_Field(), new SFInt32(tmpOrder));
    node->createControlPoints(new MFVec3f(tmpControlPoints, 
                              getControlPoints()->getSize()));
    node->weight(new MFFloat(tmpWeights, weight()->getSFSize()));

    return node;
}

Node*
NodeNurbsCurve::toNurbsPositionInterpolator(void)
{
  NodeNurbsPositionInterpolator *node = (NodeNurbsPositionInterpolator *)
      m_scene->createNode("NurbsPositionInterpolator");
  
  float *tmpControlPoints = new float[getControlPoints()->getSize()];
  double *tmpWeights = new double[weight()->getSize()];
  double *tmpKnots = new double[knot()->getSize()];
  int tmpOrder = order()->getValue();  
  
  for(int i=0; i<(getControlPoints()->getSize()); i++){
    tmpControlPoints[i] = getControlPoints()->getValues()[i];
  }

  for(int i=0; i<(weight()->getSFSize()); i++){
    tmpWeights[i] = weight()->getValue(i);
  }
  
  for(int i=0; i<(knot()->getSFSize()); i++){
    tmpKnots[i] = knot()->getValue(i);
  }
    
  node->knot(new MFDouble(tmpKnots, knot()->getSFSize()));
  node->setField(node->order_Field(), new SFInt32(tmpOrder));
  node->setControlPoints(new MFVec3f(tmpControlPoints, 
                                     getControlPoints()->getSize()));
  node->weight(new MFDouble(tmpWeights, weight()->getSFSize()));

  return node;
}  


Node*
NodeNurbsCurve::toNurbsOrientationInterpolator(void)
{
  NodeNurbsOrientationInterpolator *node = (NodeNurbsOrientationInterpolator *)
      m_scene->createNode("NurbsOrientationInterpolator");
  
  float *tmpControlPoints = new float[getControlPoints()->getSize()];
  double *tmpWeights = new double[weight()->getSize()];
  double *tmpKnots = new double[knot()->getSize()];
  int tmpOrder = order()->getValue();  
  
  for(int i=0; i<(getControlPoints()->getSize()); i++){
    tmpControlPoints[i] = getControlPoints()->getValues()[i];
  }

  for(int i=0; i<(weight()->getSFSize()); i++){
    tmpWeights[i] = weight()->getValue(i);
  }
  
  for(int i=0; i<(knot()->getSFSize()); i++){
    tmpKnots[i] = knot()->getValue(i);
  }
    
  node->knot(new MFDouble(tmpKnots, knot()->getSFSize()));
  node->setField(node->order_Field(), new SFInt32(tmpOrder));
  node->setControlPoints(new MFVec3f(tmpControlPoints, 
                                     getControlPoints()->getSize()));
  node->weight(new MFDouble(tmpWeights, weight()->getSFSize()));

  return node;
}  

void 
NodeNurbsCurve::revolveFlatter(int change, int zero)
{
    MFVec3f *values = getControlPoints();
    for (int i = 0; i < values->getSFSize(); i++) {
        SFVec3f vec(values->getValue(i));
        float len = sqrt(vec.getValue(change) * vec.getValue(change)  
                       + vec.getValue(zero)   * vec.getValue(zero));
        vec.setValue(change,  len);
        vec.setValue(zero, 0);
        SFVec3f result(vec.getValue(0), vec.getValue(1), vec.getValue(2));
        values->setSFValue(i, &result);
    } 
}

bool
NodeNurbsCurve::revolveFlatten(int direction)
{
    switch(direction) {
      case NURBS_ROT_X_AXIS:
        revolveFlatter(2, 1);
        break;
      case NURBS_ROT_Y_AXIS:
        revolveFlatter(0, 2);
        break;
      case NURBS_ROT_Z_AXIS:
        revolveFlatter(1, 0);
        break;
      case NURBS_ROT_POINT_TO_POINT:
        return false; 
    }
    return true;
}

void
NodeNurbsCurve::flip(int index)
{
    if (getControlPoints())
        getControlPoints()->flip(index);
    m_chainDirty = true;
    
}

void
NodeNurbsCurve::swap(int fromTo)
{
    if (getControlPoints())
        getControlPoints()->swap(fromTo);
    m_chainDirty = true;
    
}

Node*
NodeNurbsCurve::degreeElevate(int newDegree)
{
  //Nothing but simple application of existing class "NurbsCurveDegreeElevate" 

  if(newDegree <= ((order()->getValue())-1)){
    return NULL;
  }

  NodeNurbsCurve *node = (NodeNurbsCurve *)
                            m_scene->createNode("NurbsCurve");

  if(newDegree > ((order()->getValue())-1)){
    
    //load old values
    int dimension = getControlPoints()->getSize() / 3;
    Vec3f *tPoints = new Vec3f[dimension];
    float *tWeights = new float[dimension];
    int knotSize = knot()->getSize();
    MyArray<float> tKnots(knotSize);
    int deg = order()->getValue() - 1;
    int upDegree = newDegree - deg;

    for (int i=0; i<dimension; i++){
      tPoints[i] = getControlPoints()->getValue(i);
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

    float *newControlPoints = new float[newDimension * 3];
    float *newWeights = new float[newDimension];
    float *newKnots = new float[newDimension + newOrder]; 

    for (int i=0; i<newDimension; i++){
      newControlPoints[(i*3)]   = elevatedCurve.getControlPoints(i).x;
      newControlPoints[(i*3)+1] = elevatedCurve.getControlPoints(i).y;
      newControlPoints[(i*3)+2] = elevatedCurve.getControlPoints(i).z;
      newWeights[i] = elevatedCurve.getWeights(i);
    }
    
    for (int i=0; i<newKnotSize; i++){
      newKnots[i] = elevatedCurve.getKnots(i);
    }

    //load new node
    node->createControlPoints(new MFVec3f(newControlPoints, newDimension * 3));
    node->weight(new MFFloat(newWeights, newDimension));
    node->knot(new MFFloat(newKnots, newDimension + newOrder));
    node->order(new SFInt32(newOrder));
    node->tessellation(new SFInt32(tessellation()->getValue()));
 
    return node;
  }
  return NULL;
}

void
NodeNurbsCurve::backupFieldsAppend(int field)
{
    m_scene->addNextCommand();
    m_scene->backupFieldsAppend(this, field);
}

void
NodeNurbsCurve::repairKnotAndWeight()
{
    if (m_inRepair)
        return;

    m_inRepair = true;

    MFVec3f *controlPoints = getControlPoints();
    int iorder = order()->getValue();

    if (controlPoints && controlPoints->getSFSize() > 0) { 
        if (weight() && controlPoints->getSFSize() > weight()->getSize()) {
            MFFloat *oldWeights = weight();
            MFFloat *newWeights = new MFFloat();
            for (int i = 0; i < oldWeights->getSize(); i++)
                newWeights->appendSFValue(oldWeights->getValue(i));
            for (int i = oldWeights->getSize(); i < controlPoints->getSFSize();
                 i++)
                newWeights->appendSFValue(1);
            weight(newWeights);
        }     
        if (controlPoints->getSFSize() > knot()->getSize() - iorder) {
            MFFloat *newKnots = new MFFloat();
            for (int i = 0; i < controlPoints->getSFSize() +
                                                  iorder; i++) {
                if (i < iorder - 1)
                    newKnots->appendSFValue(0);
                else if ((i - iorder + 2) < controlPoints->getSFSize()) {
                    if ((i - iorder + 1) > -1)
                        newKnots->appendSFValue(i - iorder + 1);
                    else
                        newKnots->appendSFValue(0);
                } else 
                    newKnots->appendSFValue(controlPoints->getSFSize() - 
                                            iorder + 1);
            }
            knot(newKnots);
        }
    }
    m_inRepair = false;
}

void
NodeNurbsCurve::extrudePoints(int from, int to, int points, bool xSymetric)
{
    if (xSymetric)
        m_scene->removeSelectedHandles();
    MFVec3f *controlPoints = (MFVec3f *)getControlPoints()->copy();
    MFFloat *weights = (MFFloat *)weight()->copy();
    Vec3f vecFrom = controlPoints->getVec(from);
    Vec3f vecTo   = controlPoints->getVec(to);
    Vec3f inc;
    Vec3f mdiff = vecFrom - vecTo;
    float min = fabs(mdiff.x);
    int direction = 0;
    if (fabs(mdiff.y) < min) {
        direction = 1;
        min = mdiff.y;
    }
    if (fabs(mdiff.z) < min) {
        direction = 2;
        min = mdiff.z;
    }
    switch (direction) {
      case 0:
        inc.x = TheApp->getExtrusionAmount();
        break;
      case 1: 
        inc.y = TheApp->getExtrusionAmount();;
        break;
      case 2:
        inc.z = TheApp->getExtrusionAmount();;
        break;
    }
    Vec3f vecStart = vecFrom + inc;
    Vec3f vecEnd = vecTo + inc;
    Vec3f diff = (vecEnd - vecStart) / points;
    for (int i = 0; i < points; i++) {
        Vec3f vec = vecStart + diff * i;
        if (xSymetric)
            controlPoints->insertSFValue(from + i + 1, vec.x, vec.y, vec.z);
        else
            controlPoints->insertSFValue(from, vec.x, vec.y, vec.z);
    }
    for (int i = from; i < from + points; i++)
        weights->insertSFValue(from, 1);
    controlPoints->ref();
    setControlPoints(controlPoints);
    for (int i = 0; i < points; i++) {
        if (xSymetric)
            m_scene->addSelectedHandle(from + i + 1);
        else
            m_scene->addSelectedHandle(from + i);
    }
    weights->ref();
    weight(new MFFloat(weights));
    knot(new MFFloat());
    repairKnotAndWeight();    
    controlPoints = getControlPoints();
    if (TheApp->GetXSymetricMode() && xSymetric) {
        int symFrom = -1;
        int symTo = -1;
        float eps = TheApp->GetHandleEpsilon();
        for (int i = 0; i < controlPoints->getSFSize(); i++) {
            if (i != from)
                if (((controlPoints->getVec(i).x + vecFrom.x) < eps) &&
                    ((controlPoints->getVec(i).y - vecFrom.y) < eps) &&
                    ((controlPoints->getVec(i).z - vecFrom.z) < eps))
                    symFrom = i;
            if (i != to)
                if (((controlPoints->getVec(i).x + vecTo.x) < eps) &&
                    ((controlPoints->getVec(i).y - vecTo.y) < eps) &&
                    ((controlPoints->getVec(i).z - vecTo.z) < eps))
                    symTo = i;
        }
        if ((symFrom > -1) && (symTo > -1)) {
            extrudePoints(symFrom, symTo, points, false);
        }
    }
}

void      
NodeNurbsCurve::addToConvertedNodes(int writeFlags)
{
    Node *node = toIndexedLineSet();
    if (node != NULL) {
        node->setVariableName(strdup(getVariableName()));
        node->addParent(getParent(), getParentField());
        m_convertedNodes.append(node);
        addCoordinateInterpolator(node, false);
    }
}

bool
NodeNurbsCurve::addCoordinateInterpolator(Node *node, bool appendToScene)
{
    // find interpolators which have routes to the morphing node
    InterpolatorInfo info;

    findInterpolators(info);

    if (info.interpolator.size() == 0) {
        // nothing to do
        return true;
    }

    // get name of output of used input of interpolators
    // only the first found input is used
    MyString interOutputName;
    MyString interOutputEventName;
    Node *interOutput = NULL;
    int interOutputEvent = -1;
    bool foundInterOutput = false;
    for (size_t k = 0; k < info.interpolator.size(); k++) {
        Node *node = info.interpolator[k];
        int eventIn = node->lookupEventIn("set_fraction", false);
        if (eventIn == INVALID_INDEX)
            continue;

        // use first eventIn found to drive Interpolator
        SocketList::Iterator *iter= node->getInput(eventIn).first();
        if (iter && iter->item().getNode()) {
            interOutput = iter->item().getNode();
            interOutputName = interOutput->getNameOrNewName();
            interOutputEvent = iter->item().getField();
            interOutputEventName = interOutput->getProto()->
                                       getEventOut(interOutputEvent)->
                                       getName(interOutput->getScene()->isX3d());
            foundInterOutput = true;
            break;
        }
    }
        
    // sort list of keys
    MyArray<float> keys;
    MyArray<float> coordKeyValues;
    MyArray<float> normalKeyValues;
    bool foundMinKey = false;
    float currentKey = -1;
    do {
       foundMinKey = false;
       float minKey = 0;
       for (size_t i = 0; i < info.interpolator.size(); i++)
           for (int j = 0; j < info.interpolator[i]->getNumKeys(); j++)
               if (info.interpolator[i]->getKey(j) > currentKey) {
                   if (foundMinKey == false) {
                       minKey = info.interpolator[i]->getKey(j);
                       foundMinKey = true;
                   } else if (info.interpolator[i]->getKey(j) < minKey)
                       minKey = info.interpolator[i]->getKey(j);
               }
       if (foundMinKey) {
           keys.append(minKey);
           currentKey = minKey;
       }
       if (!foundMinKey)
           break;
    } while (foundMinKey);

    // get data from Interpolators and generate chain

    void *data = initializeData();
    for (size_t i = 0; i < keys.size(); i++) {
        for (size_t j = 0; j < info.interpolator.size(); j++) {
            Interpolator *inter = info.interpolator[j];
            int field = info.field[j];
            if (inter != NULL)
                loadDataFromInterpolators(data, inter, field, keys[i]);
        }

        m_chainDirty = true;
        createChain(data);
        m_chainDirty = true;

        MFVec3f *vertices = new MFVec3f();
        for (size_t n = 0; n < m_chain.size(); n++)
             vertices->appendVec(m_chain[n]);
        for (int k = 0; k < vertices->getSize(); k++)
            coordKeyValues.append(vertices->getValues()[k]);
    }
    finalizeData(data);
    data = NULL;    

    NodeCoordinateInterpolator *coordInter = (NodeCoordinateInterpolator *) 
          m_scene->createNode("CoordinateInterpolator");

    // write CoordinateInterpolator
    MFFloat *mfkeys = new MFFloat((float *)keys.getData(), keys.size());      
    coordInter->key((MFFloat *)mfkeys->copy());

    MFVec3f *mfCoordKeyValues = new MFVec3f((float *)coordKeyValues.getData(),
                                             coordKeyValues.size());
    coordInter->keyValue((MFVec3f *)mfCoordKeyValues->copy());
    if (appendToScene)
        m_scene->addDelayedWriteNode(coordInter); 
    else {
        m_convertedNodes.append(coordInter);
        coordInter->setVariableName(strdup(coordInter->getName()));
        coordInter->addParent(getParent(), getParentField()); 
    }

    // write route
    Node *ncoord = ((NodeIndexedLineSet *)node)->coord()->getValue();
    ncoord->setVariableName(strdup(ncoord->getVariableName()));
    if (appendToScene)
        m_scene->addRouteString(m_scene->createRouteString(
               coordInter->getNameOrNewName(), "value_changed",
               ncoord->getNameOrNewName(), "set_point"));
    else
        m_scene->addRoute(coordInter, coordInter->value_changed_Field(), 
                          ncoord, ((NodeCoordinate *)ncoord)->point_Field());
    if (foundInterOutput) {
        if (appendToScene)
            m_scene->addEndRouteString(m_scene->createRouteString(
                   interOutputName, interOutputEventName,
                   coordInter->getNameOrNewName(), "set_fraction"));
        else
            m_scene->addRoute(interOutput, interOutputEvent,
                             coordInter, coordInter->set_fraction_Field());
    }
    return true;
} 

void *
NodeNurbsCurve::initializeData() 
{
    int size = getControlPoints()->getSFSize();
    return new Vec3f[size];
}

void    
NodeNurbsCurve::finalizeData(void* data)
{
    delete [] (Vec3f *)data;
}

void 
NodeNurbsCurve::findInterpolators(InterpolatorInfo& info)
{
    Node *node = this;
    int field = controlPoint_Field();
    if (m_scene->isX3d()) {
        NodeCoordinate *coordinate = (NodeCoordinate *) 
                                     controlPointX3D()->getValue();
        field = coordinate->point_Field();
    }

    Interpolator *inter = m_scene->findUpstreamInterpolator(node, field); 
    if (inter != NULL) {
        info.interpolator.append(inter);
        info.field.append(field);
    }
}

void
NodeNurbsCurve::loadDataFromInterpolators(void *nurbsCurveData, 
                                          Interpolator *inter,
                                          int field, float key)
{
    NurbsCurveData *data = (NurbsCurveData *)nurbsCurveData;
    if (field == controlPoint_Field())
        inter->interpolate(key, (float *)data);
}

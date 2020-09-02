/*
 * NodeCurveAnimation.cpp
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
#include <math.h>
#include "stdafx.h"

#include "NodeCurveAnimation.h"
#include "Proto.h"
#include "DuneApp.h"
#include "Scene.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFInt32.h"
#include "SFBool.h"
#include "SFNode.h"
#include "NodeScalarInterpolator.h"
#include "Util.h"
#include "EventIn.h"
#include "EventOut.h"
#include "NurbsCurveDegreeElevate.h"

ProtoCurveAnimation::ProtoCurveAnimation(Scene *scene)
  : ProtoNurbsCurve(scene, "CurveAnimation", true)
{
    addEventIn(SFFLOAT, "set_fraction", EIF_RECOMMENDED);

    rotationAxis.set(
          addExposedField(SFVEC3F, "rotationAxis", new SFVec3f(0, 1, 0)));
    setFieldFlags(rotationAxis, FF_4KIDS);
    enableRotation.set(
          addExposedField(SFBOOL, "enableRotation", new SFBool(true)));
    setFieldFlags(enableRotation, FF_4KIDS);
    hover.set(
          addExposedField(SFBOOL, "hover", new SFBool(true)));
    setFieldFlags(hover, FF_4KIDS);

    addEventOut(SFVEC3F, "position_changed", EOF_RECOMMENDED);
    addEventOut(SFROTATION, "orientation_changed", EOF_RECOMMENDED);

    addURLs(URL_SCRIPTED_NODES);
}

Node *
ProtoCurveAnimation::create(Scene *scene)
{
    return new NodeCurveAnimation(scene, this); 
}

NodeCurveAnimation::NodeCurveAnimation(Scene *scene, Proto *def)
  : NodeNurbsCurve(scene, def)
{
    m_set_fractionField.set(getProto()->lookupEventIn("set_fraction"));

    m_position_changedField.set(getProto()->lookupEventOut("position_changed"));
    m_orientation_changedField.set(getProto()->lookupEventOut(
                                  "orientation_changed"));

    m_curveDirty = true;

    m_positionInterpolator = (NodePositionInterpolator *)
                             scene->createNode("PositionInterpolator");
    m_positionInterpolator->setConvertedInCurveAnimaton();
    m_positionInterpolator->getVariableName();

    m_orientationInterpolator = (NodeOrientationInterpolator *)
                                scene->createNode("OrientationInterpolator");
    m_orientationInterpolator->setConvertedInCurveAnimaton();
    m_orientationInterpolator->getVariableName();
}

Node *
NodeCurveAnimation::copyFromNurbsCurve(Node *curve)
{
    if (curve->getType() == VRML_NURBS_CURVE) {
        NodeNurbsCurve *nurbsCurve = (NodeNurbsCurve *) curve;
        NodeCurveAnimation *node = (NodeCurveAnimation *) 
              nurbsCurve->getScene()->createNode("CurveAnimation");
        int order = nurbsCurve->order()->getValue();
        node->setField(node->order_Field(), new SFInt32(order));
        int dimension = nurbsCurve->knot()->getSize() - order;
        MFFloat *mfknot = (MFFloat *) nurbsCurve->knot()->copy();
        const float *knots = mfknot->getValues();
        node->knot(new MFFloat(knots, dimension + order));
        MFVec3f *mfControlPoint = (MFVec3f *)
                                  nurbsCurve->getControlPoints()->copy();
        const float *controlPoint = mfControlPoint->getValues();
        node->createControlPoints(new MFVec3f(controlPoint, dimension * 3));
        MFFloat *mfWeight = (MFFloat *) nurbsCurve->weight()->copy();
        const float *weights = mfWeight->getValues();
        node->weight(new MFFloat(weights, dimension));
        return node;
     }
     return NULL;
}


NodeCurveAnimation::~NodeCurveAnimation()
{
    delete m_positionInterpolator;
    delete m_orientationInterpolator;
}

void
NodeCurveAnimation::createNurbsCurve()
{
    m_positionInterpolator->key(new MFFloat());
    m_positionInterpolator->keyValue(new MFVec3f());
    m_orientationInterpolator->key(new MFFloat());
    m_orientationInterpolator->keyValue(new MFRotation());

    toPositionInterpolator(m_positionInterpolator); 
    toOrientationInterpolator(m_orientationInterpolator, 
                              rotationAxis()->getValue());

    m_curveDirty = false;
}

void    
NodeCurveAnimation::toPositionAndOrientationInterpolators(NodeList *nodes)
{
    nodes->append(m_positionInterpolator->copy());
    nodes->append(m_orientationInterpolator->copy());
}

int
NodeCurveAnimation::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    return ((Node *)this)->writeProto(f, "", "scripted_Nodes"
#ifdef HAVE_SCRIPTED_NODES_PROTO_URL
                                            , HAVE_SCRIPTED_NODES_PROTO_URL
#endif
                                            );
}

void 
NodeCurveAnimation::createRoutes(Interpolator *node, bool appendToScene) 
{
    int eventIn = node->lookupEventIn("set_fraction", false);
    if (eventIn == INVALID_INDEX)
        return;

    SocketList::Iterator *iter;
    for (iter = node->getInput(eventIn).first(); iter != NULL;
         iter = iter->next()) {
        if (iter->item().getNode()) {
            Node *interOutput = iter->item().getNode();
            int interOutputEvent = iter->item().getField();
            MyString interOutputEventName = interOutput->getProto()->
                  getEventOut(interOutputEvent)->getName(
                  interOutput->getScene()->isX3d());
            if (appendToScene)
                m_scene->addRouteString(m_scene->createRouteString(
                      interOutput->getNameOrNewName(), interOutputEventName,
                      node->getNameOrNewName(), "set_fraction"));
            else
               m_scene->addRoute(interOutput, interOutputEvent,
                                 node, node->set_fraction_Field());

        }
    }
    if (node->getType() == VRML_POSITION_INTERPOLATOR) {
        NodePositionInterpolator *pos = (NodePositionInterpolator *)node;       
        int eventOut = node->lookupEventOut("value_changed", false);
        if (eventOut == INVALID_INDEX)
           return;

        Node* posOut = NULL;
        SocketList::Iterator *iter;
        for (iter = node->getOutput(eventOut).first(); iter != NULL;
            iter = iter->next()) {
            if (iter->item().getNode()) {
                eventOut = iter->item().getField();
                posOut = iter->item().getNode();
                break;
            }
        }
        if (posOut) {
            if (appendToScene)
                m_scene->addRouteString(m_scene->createRouteString(
                                       node->getNameOrNewName(), 
                                       "value_changed",
                                       posOut->getNameOrNewName(), 
                   posOut->getProto()->getEventOut(eventOut)->getName(true)));
        else
            m_scene->addRoute(node, pos->value_changed_Field(),
                              posOut, eventOut);
        }
    }
    if (node->getType() == VRML_ORIENTATION_INTERPOLATOR) {
        NodeOrientationInterpolator *ori = (NodeOrientationInterpolator *)node;

        int eventOut = node->lookupEventOut("value_changed", false);
        if (eventOut == INVALID_INDEX)
           return;

        Node* oriOut = NULL;
        SocketList::Iterator *iter;
        for (iter = node->getOutput(eventOut).first(); iter != NULL;
            iter = iter->next()) {
            if (iter->item().getNode()) {
                eventOut = iter->item().getField();
                oriOut = iter->item().getNode();
                break;
            }
        }
        if (oriOut) {
            if (appendToScene )
                m_scene->addRouteString(m_scene->createRouteString(
                                        node->getNameOrNewName(), 
                                        "value_changed",
                                        oriOut->getNameOrNewName(), 
                   oriOut->getProto()->getEventOut(eventOut)->getName(true)));
           else
               m_scene->addRoute(node, ori->value_changed_Field(),
                              oriOut, eventOut);
        }
    }
}

int             
NodeCurveAnimation::writeXml(int f, int indent, int containerField,
                             bool avoidUse) 
{
    int flags = m_scene->getWriteFlags();
    if (flags & (PURE_VRML97 | PURE_X3DV | X3DOM)) {
        if (isX3dXml(flags)) {
            RET_ONERROR( m_positionInterpolator->writeXml(f, indent, -1, 
                                                          true) )
            RET_ONERROR( m_orientationInterpolator->writeXml(f, indent, -1, 
                                                            true) )
        } else {
            RET_ONERROR( m_positionInterpolator->write(f, indent, avoidUse) )
            RET_ONERROR( m_orientationInterpolator->write(f, indent, avoidUse) )
        }
        createRoutes(m_positionInterpolator, true);
        createRoutes(m_orientationInterpolator, true);
    } else {
        if (isX3dXml(flags))
            RET_ONERROR( Node::writeXml(f, indent, -1, avoidUse) )
        else
            RET_ONERROR( Node::write(f, indent, avoidUse) )
    }
    return 0;
}


int             
NodeCurveAnimation::write(int f, int indent, bool avoidUse) 
{
    int flags = m_scene->getWriteFlags();
    if (flags & (PURE_VRML97 | PURE_X3DV | X3DOM)) {
        if (isX3dXml(flags)) {
            RET_ONERROR( m_positionInterpolator->writeXml(f, indent, -1, 
                                                          true) )
            RET_ONERROR( m_orientationInterpolator->writeXml(f, indent, -1, 
                                                            true) )
        } else {
            RET_ONERROR( m_positionInterpolator->write(f, indent, avoidUse) )
            RET_ONERROR( m_orientationInterpolator->write(f, indent, avoidUse) )
        }
        createRoutes(m_positionInterpolator, true);
        createRoutes(m_orientationInterpolator, true);
    } else {
        if (isX3dXml(flags))
            RET_ONERROR( Node::writeXml(f, indent, -1, avoidUse) )
        else
            RET_ONERROR( Node::write(f, indent, avoidUse) )
    }
    return 0;
}

int     
NodeCurveAnimation::writeCDeclaration(int filedes, int languageFlag)
{
/*
    if ((!m_positionInterpolator) && (!m_orientationInterpolator)) return 0;
    m_positionInterpolator->getProto()->writeCDeclaration(filedes, 
                                                          languageFlag);
    m_orientationInterpolator->getProto()->writeCDeclaration(filedes, 
                                                             languageFlag);
*/
    return 0;
}

int     
NodeCurveAnimation::writeC(int filedes, int languageFlag)
{
    if (m_positionInterpolator) {
        RET_ONERROR( m_positionInterpolator->writeC(filedes, languageFlag) )
    }
    if (m_orientationInterpolator) {
        RET_ONERROR( m_orientationInterpolator->writeC(filedes, languageFlag) )
    }
    return 0;
}

void
NodeCurveAnimation::addToConvertedNodes(long flags)
{ 
    if (m_convertedNodes.size() > 0)
        return;

    Interpolator *positionInterpolator = (Interpolator *)
                                         m_positionInterpolator->copy();
    const char *name = strdup(m_positionInterpolator->getVariableName());
    positionInterpolator->setVariableName(name);
    positionInterpolator->addParent(getParent(), getParentField()); 
    createRoutes(positionInterpolator, false);
    positionInterpolator->setConvertedInCurveAnimaton();
    m_convertedNodes.append(positionInterpolator);

    Interpolator *orientationInterpolator = (Interpolator *)
                                            m_orientationInterpolator->copy();
    name = strdup(m_orientationInterpolator->getVariableName());
    orientationInterpolator->setVariableName(name);
    orientationInterpolator->addParent(getParent(), getParentField()); 
    createRoutes(orientationInterpolator, false);
    orientationInterpolator->setConvertedInCurveAnimaton();
    m_convertedNodes.append(orientationInterpolator);

    return; 
}

void
NodeCurveAnimation::setField(int index, FieldValue *value, int cf)
{
    if (index < rotationAxis_Field())
        NodeNurbsCurve::setField(index, value, cf);
    else
        Node::setField(index, value, cf);
    createNurbsCurve();
    update();
}

void  
NodeCurveAnimation::drawHandles()
{
    if (m_curveDirty) {
        createNurbsCurve();
        m_curveDirty = false;
    }
    NodeNurbsCurve::drawHandles();
    NodeNurbsCurve::drawAsHandle(0);
}

Vec3f
NodeCurveAnimation::getHandle(int handle, int *constraint, int *field)
{
    if (m_curveDirty) {
        createNurbsCurve();
        m_curveDirty = false;
    }

    return NodeNurbsCurve::getHandle(handle, constraint, field);
}


void
NodeCurveAnimation::setHandle(int handle, const Vec3f &v) 
{
    if (m_curveDirty) {
        createNurbsCurve();
        m_curveDirty = false;
    }

    MFVec3f *mfControlPoints = getControlPoints();
    if (mfControlPoints == NULL)
        return;
    if (handle >= 0 && handle < (mfControlPoints->getSize() / 3)) {
        float w = weight()->getValue(handle);
        MFVec3f *newValue = (MFVec3f *) mfControlPoints->copy();
        newValue->setVec(handle, v * w);
        setControlPoints(newValue);
    } else
        NodeNurbsCurve::setHandle(handle, v);

    m_curveDirty = true;
}

Vec3f   
NodeCurveAnimation::getMinBoundingBox(void)
{
    if (m_curveDirty) {
        createNurbsCurve();
        m_curveDirty = false;
    }

    return NodeNurbsCurve::getMinBoundingBox();
}

Vec3f   
NodeCurveAnimation::getMaxBoundingBox(void)
{
    if (m_curveDirty) {
        createNurbsCurve();
        m_curveDirty = false;
    }

    return NodeNurbsCurve::getMinBoundingBox();
}

void
NodeCurveAnimation::flip(int index)
{
    if (m_curveDirty) {
        createNurbsCurve();
        m_curveDirty = false;
    }

    rotationAxis()->flip(index);

    NodeNurbsCurve::flip(index);

    m_curveDirty = true;
}

void
NodeCurveAnimation::swap(int fromTo)
{
    if (m_curveDirty) {
        createNurbsCurve();
        m_curveDirty = false;
    }

    rotationAxis()->swap(fromTo);

    NodeNurbsCurve::swap(fromTo);

    m_curveDirty = true;
}

void            
NodeCurveAnimation::sendPosition(double timestamp, float fraction)
{
    m_positionInterpolator->sendInterpolatedEvent(timestamp, fraction);
}

void            
NodeCurveAnimation::sendOrientation(double timestamp, float f)
{
    FieldValue *value = m_orientationInterpolator->getInterpolatedFieldValue(f);
    if (enableRotation()->getValue())
        sendEvent(m_orientation_changedField, timestamp, value);
}

void
NodeCurveAnimation::receiveEvent(int eventIn, double timestamp, 
                                 FieldValue *value)
{
    if (m_curveDirty) {
        createNurbsCurve();
        m_curveDirty = false;
    }

    float fraction = ((SFFloat *) value)->getValue();
    if (eventIn == m_set_fractionField) {
        sendPosition(timestamp, fraction);
        sendOrientation(timestamp, fraction);
    } else
        Node::receiveEvent(eventIn, timestamp, value);
}

void    
NodeCurveAnimation::addInput(int eventIn, Node *src, int eventOut)
{
    NodeData::addInput(eventIn, src, eventOut);
    if (eventIn == m_set_fractionField) {
        m_positionInterpolator->addInput(
              m_positionInterpolator->set_fraction_Field(), src, eventOut);
        m_orientationInterpolator->addInput(
              m_orientationInterpolator->set_fraction_Field(), src, eventOut);
    }
}

void    
NodeCurveAnimation::addOutput(int eventOut, Node *dst, int eventIn)
{
    NodeData::addOutput(eventOut, dst, eventIn);
    if (eventOut == m_position_changedField)
        m_positionInterpolator->addOutput(
              m_positionInterpolator->value_changed_Field(), dst, eventIn);
    if (eventOut == m_orientation_changedField)
        m_orientationInterpolator->addOutput(
              m_orientationInterpolator->value_changed_Field(), dst, eventIn);
}

void    
NodeCurveAnimation::removeInput(int eventIn, Node *src, int eventOut)
{
    NodeData::removeInput(eventIn, src, eventOut);
    if (eventIn == m_set_fractionField) {
        m_positionInterpolator->removeInput(
              m_positionInterpolator->set_fraction_Field(), src, eventOut);
        m_orientationInterpolator->removeInput(
              m_orientationInterpolator->set_fraction_Field(), src, eventOut);
    }
}

void    
NodeCurveAnimation::removeOutput(int eventOut, Node *dst, int eventIn)
{
    NodeData::removeOutput(eventOut, dst, eventIn);
    if (eventOut == m_position_changedField)
        m_positionInterpolator->removeOutput(
              m_positionInterpolator->value_changed_Field(), dst, eventIn);
    if (eventOut == m_orientation_changedField)
        m_orientationInterpolator->removeOutput(
              m_orientationInterpolator->value_changed_Field(), dst, eventIn);
}

Node *
NodeCurveAnimation::toNurbsCurve(void)
{
     NodeNurbsCurve *ret = (NodeNurbsCurve *)m_scene->createNode("NurbsCurve");
     ret->setControlPoints(controlPoint());
     m_scene->setField(ret, tessellation_Field(), tessellation());
     m_scene->setField(ret, weight_Field(), weight());
     m_scene->setField(ret, knot_Field(), knot());
     m_scene->setField(ret, order_Field(), order());
     m_scene->setField(ret, closed_Field(), closed());
     return ret;
}

Node*
NodeCurveAnimation::degreeElevate(int newDegree)
{
  //Nothing but simple application of existing class "NurbsCurveDegreeElevate" 

  if(newDegree <= ((order()->getValue())-1)){
    return NULL;
  }

  NodeCurveAnimation *node = (NodeCurveAnimation *)
                             m_scene->createNode("CurveAnimation");

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

    node->createRoutes(m_positionInterpolator, true);
    node->createRoutes(m_orientationInterpolator, true);

    int eventIns = getProto()->getNumEventIns();
    int eventOuts = getProto()->getNumEventOuts();
 
    for (int i = 0; i < eventIns; i++) {
        for (SocketList::Iterator *j = getInput(i).first(); j != NULL;
             j = j->next()) {
            RouteSocket s = j->item();
            m_scene->addRoute(s.getNode(), s.getField(), node, i);
        }
    }
    for (int i = 0; i < eventOuts; i++) {
        for (SocketList::Iterator *j = getOutput(i).first(); j != NULL;
             j = j->next()) {
            RouteSocket s = j->item();
            m_scene->addRoute(node, i, s.getNode(), s.getField());
        }
    }
    return node;
  }
  return NULL;
}

void            
NodeCurveAnimation::reInit(void) 
{ 
    m_curveDirty = true; 
    Node::reInit();
}


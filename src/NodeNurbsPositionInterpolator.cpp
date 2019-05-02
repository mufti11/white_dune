/*
 * NodeNurbsPositionInterpolator.cpp
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

#include "NodeNurbsPositionInterpolator.h"
#include "NodeNurbsCurve.h"
#include "NodeCoordinate.h"
#include "Proto.h"
#include "DuneApp.h"
#include "Scene.h"
#include "FieldValue.h"
#include "Field.h"
#include "SFFloat.h"
#include "SFInt32.h"
#include "SFBool.h"
#include "Vec2f.h"
#include "MoveCommand.h"
#include "RenderState.h"
#include "Util.h"

ProtoNurbsPositionInterpolator::ProtoNurbsPositionInterpolator(Scene *scene)
  : Proto(scene, "NurbsPositionInterpolator")
{
    dimension.set(
          addExposedField(SFINT32, "dimension", new SFInt32(0)));
    setFieldFlags(dimension, FF_VRML_ONLY);
    controlPoint.set(
          addExposedField(SFNODE, "controlPoint", new SFNode(NULL), 
                          COORDINATE_NODE));
    setFieldFlags(controlPoint, FF_X3D_ONLY);
    weight.set(
          addExposedField(MFDOUBLE, "keyWeight", new MFFloat(), 
                          new MFDouble(0.0f), NULL, "weight"));
    knot.set(
          addField(MFDOUBLE, "knot", new MFDouble()));
    order.set(
          addField(SFINT32, "order", new SFInt32(4), new SFInt32(2)));
    getField(order)->addX3dDefault(new SFInt32(3));
    addEventIn(SFFLOAT, "set_fraction", EIF_RECOMMENDED);
    addEventOut(SFVEC3F, "value_changed", EOF_RECOMMENDED);

    addURLs(URL_VRML97_AMENDMENT1);
}

Node *
ProtoNurbsPositionInterpolator::create(Scene *scene)
{
    return new NodeNurbsPositionInterpolator(scene, this); 
}

void
NodeNurbsPositionInterpolator::setField(int fieldIndex, FieldValue *value, 
                                        int cf)
{
    m_nurbsCurveDirty = true;
    Node::setField(fieldIndex, value, cf);   
}

NodeNurbsPositionInterpolator::NodeNurbsPositionInterpolator(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_nurbsCurveDirty = true;
    m_nurbsCurve = (NodeNurbsCurve *) scene->createNode("NurbsCurve");
    m_nurbsCurve->setInternal(true);
    m_set_fractionField.set(getProto()->lookupEventIn("set_fraction"));
    m_value_changedField.set(getProto()->lookupEventOut("value_changed"));
}

NodeNurbsPositionInterpolator::~NodeNurbsPositionInterpolator()
{
    delete m_nurbsCurve;
}

MFVec3f *
NodeNurbsPositionInterpolator::getControlPoints(void)
{
    Node *pointNode = controlPoint()->getValue();
    if (pointNode)
        return ((NodeCoordinate *)pointNode)->point();
    return NULL;
}

void
NodeNurbsPositionInterpolator::setControlPoints(const MFVec3f *points)
{
    NodeCoordinate *coord = (NodeCoordinate *)controlPoint()->getValue();
    if (!coord)
         coord = (NodeCoordinate *) m_scene->createNode("Coordinate");
    if (coord != NULL) {
        controlPoint(new SFNode(coord));
        coord->setField(coord->point_Field(), new MFVec3f(points));
    }
}

void
NodeNurbsPositionInterpolator::createNurbsCurve()
{
    float *points = new float[getControlPoints()->getSize()];
    for (int i = 0; i < getControlPoints()->getSize(); i++)
         points[i] = getControlPoints()->getValues()[i];
    m_nurbsCurve->setControlPoints(new MFVec3f(points, 
                                               getControlPoints()->getSize()));
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
    m_chainLength = 0;
    float *fchain = new float[chainLength * 3];
    for (int i = 0; i < chainLength; i++) {
         fchain[i * 3    ] = chain[i].x;
         fchain[i * 3 + 1] = chain[i].y;
         fchain[i * 3 + 2] = chain[i].z;
         if (i > 0)
             m_chainLength += (chain[i] - chain[i - 1]).length();
    }   
    m_nurbsCurveDirty = false;
}

int
NodeNurbsPositionInterpolator::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f ,"EXTERNPROTO NurbsPositionInterpolator[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) );
    RET_ONERROR( mywritestr(f ," ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:web3d:vrml97:node:NurbsPositionInterpolator\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:inet:blaxxun.com:node:NurbsPositionInterpolator\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:ParaGraph:NurbsPositionInterpolator\",\n") )
    TheApp->incSelectionLinenumber();
#ifdef HAVE_VRML97_AMENDMENT1_PROTO_URL
    RET_ONERROR( mywritestr(f ," \"") )
    RET_ONERROR( mywritestr(f ,HAVE_VRML97_AMENDMENT1_PROTO_URL) )
    RET_ONERROR( mywritestr(f ,"/NurbsPositionInterpolatorPROTO.wrl") )
    RET_ONERROR( mywritestr(f ,"\"\n") )
    TheApp->incSelectionLinenumber();
#else
    RET_ONERROR( mywritestr(f ," \"NurbsPositionInterpolatorPROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#endif
    RET_ONERROR( mywritestr(f ," \"http://wdune.ourproject.org/docs/vrml97Amendment1/NurbsPositionInterpolatorPROTO.wrl\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int             
NodeNurbsPositionInterpolator::write(int filedes, int indent) 
{
    if (m_scene->isPureVRML()) {
        Node *node = m_nurbsCurve->toPositionInterpolator();
        if (node == NULL) 
           return 1;

        // fixme: read Route connects of node and 
        // build routes to new node instead

        RET_ONERROR( node->write(filedes, indent) )
        node->unref();
    } else
        RET_ONERROR( Node::write(filedes, indent) )
    return 0;
}

void
NodeNurbsPositionInterpolator::draw()
{
    if (m_nurbsCurveDirty) {
        createNurbsCurve();
        m_nurbsCurve->createChain();
        m_nurbsCurveDirty = false;
    }

    if (!m_nurbsCurve) return;

    float c[3] = { 1.0f, 1.0f, 1.0f };
    Util::myGlMaterial3fv(GL_FRONT, GL_EMISSION, c);
    m_nurbsCurve->draw();
}

void
NodeNurbsPositionInterpolator::drawHandles()
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
    draw();
    glPopName();
    glEnable(GL_LIGHTING);
}

Vec3f
NodeNurbsPositionInterpolator::getHandle(int handle, int *constraint,
                                         int *field)
{
    *field = controlPoint_Field() ;

    if (handle >= 0 && handle < getControlPoints()->getSize() / 3) {
        Vec3f ret((Vec3f)getControlPoints()->getValue(handle) / 
                   weight()->getValue(handle));
        return ret;
    } else {
        return Vec3f(0.0f, 0.0f, 0.0f);
    }
}

void
NodeNurbsPositionInterpolator::setHandle(int handle, const Vec3f &v) 
{
    MFVec3f *oldValue = getControlPoints();
    MFVec3f *newValue = (MFVec3f *)oldValue->copy();

    if (handle >= 0 && handle < oldValue->getSize() / 3) {
        Vec3f v2 = v * weight()->getValue(handle); 
        m_nurbsCurveDirty = true;
        newValue->setValue(handle * 3, v2.x);
        newValue->setValue(handle * 3+1, v2.y);
        newValue->setValue(handle * 3+2, v2.z);
        setControlPoints(newValue);
    }
}

void
NodeNurbsPositionInterpolator::flip(int index)
{
    if (getControlPoints())
        getControlPoints()->flip(index);
    m_nurbsCurveDirty = true;    
}

void
NodeNurbsPositionInterpolator::swap(int fromTo)
{
    if (getControlPoints())
        getControlPoints()->swap(fromTo);
    m_nurbsCurveDirty = true;    
}

Node   *
NodeNurbsPositionInterpolator::toNurbsCurve(void)
{
  NodeNurbsCurve *node = (NodeNurbsCurve *) m_scene->createNode("NurbsCurve");
  
  float *tmpControlPoints = new float[getControlPoints()->getSize()];
  float *tmpWeights = new float[weight()->getSize()];
  float *tmpKnots = new float[knot()->getSize()];
  int tmpOrder = order()->getValue();  
  
  for(int i=0; i<getControlPoints()->getSize(); i++){
    tmpControlPoints[i] = getControlPoints()->getValues()[i];
  }

  for(int i=0; i<(weight()->getSFSize()); i++){
    tmpWeights[i] = weight()->getValue(i);
  }
  
  for(int i=0; i<(knot()->getSFSize()); i++){
    tmpKnots[i] = knot()->getValue(i);
  }
    
  node->knot(new MFFloat(tmpKnots, knot()->getSFSize()));
  node->setField(node->order_Field(), new SFInt32(tmpOrder));
  node->setControlPoints(new MFVec3f(tmpControlPoints, 
                                     getControlPoints()->getSize()));
  node->weight(new MFFloat(tmpWeights, weight()->getSFSize()));

  return node;
}

void
NodeNurbsPositionInterpolator::receiveEvent(int eventIn, double timestamp, 
                                            FieldValue *value)
{
    if (m_nurbsCurveDirty) {
        createNurbsCurve();
        m_nurbsCurve->createChain();
        m_nurbsCurveDirty = false;
    }

    if (!m_nurbsCurve) return;

    if (eventIn == m_set_fractionField) {
        float fraction = ((SFFloat *) value)->getValue();
        float posChain = fraction * m_chainLength;
        float pos = 0;
        float oldPos = pos;
        const Vec3f *chain = m_nurbsCurve->getChain();
        int chainLength = m_nurbsCurve->getChainLength();
        for (int i = 0; i < chainLength; i++) {
             if (pos >= posChain) {
                 if (i == 0) {
                     sendEvent(m_value_changedField, timestamp, 
                               new SFVec3f(chain[0].x, chain[0].y, chain[0].z));
                     break;
                 } else {
                     Vec3f vec = chain[i - 1] + ((chain[i] - chain[i - 1]) *
                                                 (pos - oldPos));
                     sendEvent(m_value_changedField, timestamp, 
                               new SFVec3f(vec.x, vec.y, vec.z));
                     break;
                 }
             }
             if (i > 0)
                 pos += (chain[i] - chain[i - 1]).length();
             oldPos = pos;
        }
    } else
        Node::receiveEvent(eventIn, timestamp, value);
}


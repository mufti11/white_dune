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

ProtoNurbsPositionInterpolator::ProtoNurbsPositionInterpolator(Scene *scene)
  : Proto(scene, "NurbsPositionInterpolator")
{
    dimension.set(
          addExposedField(SFINT32, "dimension", new SFInt32(0)));
    setFieldFlags(dimension, FF_VRML_ONLY);
    keyValue.set(
          addExposedField(MFVEC3F, "keyValue", new MFVec3f()));
    setFieldFlags(keyValue, FF_VRML_ONLY | EIF_RECOMMENDED);
    controlPoint.set(
          addExposedField(SFNODE, "controlPoint", new SFNode(NULL), 
                          COORDINATE_NODE));
    setFieldFlags(controlPoint, FF_X3D_ONLY);
    keyWeight.set(
          addExposedField(MFFLOAT, "keyWeight", new MFFloat(), 
                          new SFFloat(0.0f), NULL, "weight"));
    knot.set(
          addField(MFFLOAT, "knot", new MFFloat()));
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
    if (fieldIndex == keyValue_Field()) 
        setControlPoints((MFVec3f *)value);
    if (fieldIndex == controlPoint_Field()) {
        Node *pointNode = controlPoint()->getValue();
        if (!pointNode)
            pointNode = (NodeCoordinate *) m_scene->createNode("Coordinate");
        if (pointNode != NULL)
            value = ((NodeCoordinate *)pointNode)->point();
    }
    m_nurbsCurveDirty = true;
    Node::setField(fieldIndex, value, cf);   
}

NodeNurbsPositionInterpolator::NodeNurbsPositionInterpolator(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_nurbsCurveDirty = true;
    m_nurbsCurve = (NodeNurbsCurve *) scene->createNode("NurbsCurve");
    m_nurbsCurve->setInternal(true);
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
    return keyValue();
}

void
NodeNurbsPositionInterpolator::setControlPoints(const MFVec3f *points)
{
    NodeCoordinate *coord = (NodeCoordinate *)controlPoint()->getValue();
    if (!coord)
         coord = (NodeCoordinate *) m_scene->createNode("Coordinate");
    if (coord != NULL)
        coord->setField(coord->point_Field(), new MFVec3f(points));
    Node::setField(keyValue_Field(), new MFVec3f(points));
}

void
NodeNurbsPositionInterpolator::createNurbsCurve()
{
    float *points = new float[getControlPoints()->getSize()];
    for (int i = 0; i < getControlPoints()->getSize(); i++)
         points[i] = getControlPoints()->getValues()[i];
    setControlPoints(new MFVec3f(points, getControlPoints()->getSize()));
    float *weights = new float[keyWeight()->getSize()];
    for (int i = 0; i < keyWeight()->getSize(); i++)
         weights[i] = keyWeight()->getValues()[i];
    m_nurbsCurve->weight(new MFFloat(weights, keyWeight()->getSize()));
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
/*        
        Node *node = m_nurbsCurve->toPositionInterpolator();
        if (node == NULL) 
           return 1;

        // fixme: read Route connects of node and 
        // build routes to new node instead

        RET_ONERROR( node->write(filedes, indent) )
        node->unref();
*/
    } else
        RET_ONERROR( Node::write(filedes, indent) )
    return 0;
}

void
NodeNurbsPositionInterpolator::draw()
{
    if (m_nurbsCurveDirty) {
        createNurbsCurve();
        m_nurbsCurveDirty = false;
    }

    if (!m_nurbsCurve) return;

    m_nurbsCurve->draw();
}

void  
NodeNurbsPositionInterpolator::drawHandles()
{
    if (m_nurbsCurveDirty) {
        createNurbsCurve();
        m_nurbsCurveDirty = false;
    }

    if (!m_nurbsCurve) return;

    m_nurbsCurve->drawHandles();
}

Vec3f
NodeNurbsPositionInterpolator::getHandle(int handle, int *constraint,
                                         int *field)
{
    *field = keyValue_Field() ;

    if (handle >= 0 && handle < getControlPoints()->getSize() / 3) {
        Vec3f ret((Vec3f)getControlPoints()->getValue(handle) / 
                   keyWeight()->getValue(handle));
        return ret;
    } else {
        return Vec3f(0.0f, 0.0f, 0.0f);
    }
}

void
NodeNurbsPositionInterpolator::setHandle(int handle, const Vec3f &v) 
{
    MFVec3f    *oldValue = getControlPoints();
    MFVec3f    *newValue = (MFVec3f *)oldValue->copy();

    if (handle >= 0 && handle < oldValue->getSize() / 3) {
        Vec3f v2 = v * keyWeight()->getValue(handle); 
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
  float *tmpWeights = new float[keyWeight()->getSize()];
  float *tmpKnots = new float[knot()->getSize()];
  int tmpOrder = order()->getValue();  
  
  for(int i=0; i<getControlPoints()->getSize(); i++){
    tmpControlPoints[i] = getControlPoints()->getValues()[i];
  }

  for(int i=0; i<(keyWeight()->getSFSize()); i++){
    tmpWeights[i] = keyWeight()->getValue(i);
  }
  
  for(int i=0; i<(knot()->getSFSize()); i++){
    tmpKnots[i] = knot()->getValue(i);
  }
    
  node->knot(new MFFloat(tmpKnots, knot()->getSFSize()));
  node->setField(node->order_Field(), new SFInt32(tmpOrder));
  node->setControlPoints(new MFVec3f(tmpControlPoints, keyValue()->getSize()));
  node->weight(new MFFloat(tmpWeights, keyWeight()->getSFSize()));

  return node;
}

Node *
NodeNurbsPositionInterpolator::convert2X3d(void)
{
    MFVec3f *points = keyValue();
    int len = points->getSize();
    float *floats = new float[len];
    for (int i = 0; i < len; i++)
        floats[i] = points->getValues()[i];
    setControlPoints(new MFVec3f(floats, len));
    NodeCoordinate *node = (NodeCoordinate *)controlPoint()->getValue();
    if (node)
        m_scene->changeRoutes(node, node->point_Field(), this, keyValue_Field());
    m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
    return NULL;
}

Node *
NodeNurbsPositionInterpolator::convert2Vrml(void) 
{
    NodeCoordinate *node = (NodeCoordinate *)controlPoint()->getValue();
    if (node != NULL) {
        int len = node->point()->getSize();
        float *floats = new float[len];
        for (int i = 0; i < len; i++)
            floats[i] = node->point()->getValues()[i];
        setControlPoints(new MFVec3f(floats, len));
        setControlPoints(new MFVec3f(floats, len));
        m_scene->changeRoutes(this, keyValue_Field(), node, node->point_Field());
        m_scene->execute(new MoveCommand(node, this, controlPoint_Field(),
                                        NULL, -1));
    } else
        keyValue(new MFVec3f());
    m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
    return NULL;
}



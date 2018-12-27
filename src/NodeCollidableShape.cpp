/*
 * NodeCollidableShape.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2003 J. "MUFTI" Scheurich
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
#ifndef FLT_MAX
# include <float.h>
#endif
#include "stdafx.h"

#include "NodeCollidableShape.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFBool.h"
#include "DuneApp.h"
#include "Scene.h"

ProtoCollidableShape::ProtoCollidableShape(Scene *scene)
  : X3DNBodyCollidableProto(scene, "CollidableShape")
{
    shape.set(addField(SFNODE, "shape", new SFNode(NULL), VRML_SHAPE));

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);

    transform2.set(
        addExposedField(SFNODE, "transform", new SFNode(NULL), GROUPING_NODE));
    setFieldFlags(transform2, FF_X3DOM_ONLY);
}

Node *
ProtoCollidableShape::create(Scene *scene)
{ 
    return new NodeCollidableShape(scene, this); 
}

NodeCollidableShape::NodeCollidableShape(Scene *scene, Proto *def)
  : X3DNBodyCollidableNode(scene, def)
{
    m_inSetup = true;
}

void 
NodeCollidableShape::setField(int index, FieldValue *value, int cf)
{
     Node::setField(index, value, cf);
}

void
NodeCollidableShape::transformBranch()
{
    Node *nshape = shape()->getValue();

    if (nshape != NULL) {
        glPushMatrix();
        transform();
        nshape->transformBranch();
        glPopMatrix();
    }
}

static bool testAllParents(Node *node, void *data) {
    if (node->getType() == X3D_RIGID_BODY)
        if (node->isInScene(node->getScene()))
            node->transform();
    return true;
}

void 
NodeCollidableShape::RigidBodyTransform()
{
    // In case of the "RigidBodyPhysics" component, a shape is rendered
    // differently, if the shape is a descendant of a RigidBody node.
    // Before using draw() the following method is used :
    // The transform() method of the RigidBody node is called.
    
    doWithParents(testAllParents, NULL);    
}

void
NodeCollidableShape::preDraw()
{
    Node *nshape = shape()->getValue();

    if (nshape != NULL) {
        glPushMatrix();
        transform();
        nshape->preDraw();
        glPopMatrix();
    }
}

void
NodeCollidableShape::draw(int pass)
{
    Node *nshape = shape()->getValue();

    if (nshape != NULL) {
        glPushMatrix();
        transform();

        nshape->bind();

        glPushName(shape_Field());  // field
        glPushName(0);                 // index

        nshape->draw(pass);

        glPopName();
        glPopName();

        nshape->unbind();

        glPopMatrix();
    }
}

void
NodeCollidableShape::flip(int index)
{
    Node *nshape = shape()->getValue();

    if (nshape != NULL)
        nshape->flip(index);
}

void
NodeCollidableShape::swap(int fromTo)
{
    Node *nshape = shape()->getValue();

    if (nshape != NULL)
        nshape->swap(fromTo);
}

int         
NodeCollidableShape::countPolygons(void)
{
    int ret = 0;    
    Node *nshape = shape()->getValue();
    
    if (nshape != NULL)
        ret += nshape->countPolygons();
    return ret;
}

int         
NodeCollidableShape::countPrimitives(void)
{
    int ret = 0;    
    Node *nshape = shape()->getValue();
    
    if (nshape != NULL)
        ret += nshape->countPrimitives();
    return ret;
}

int         
NodeCollidableShape::countPolygons1Sided(void)
{
    int ret = 0;    
    Node *nshape = shape()->getValue();
    
    if (nshape != NULL)
        ret += nshape->countPolygons1Sided();
    return ret;
}

int         
NodeCollidableShape::countPolygons2Sided(void)
{
    int ret = 0;    
    Node *nshape = shape()->getValue();
    
    if (nshape != NULL)
        ret += nshape->countPolygons2Sided();
    return ret;
}

float   
NodeCollidableShape::getTransparency(void)
{
    float ret = 0;    
    Node *nshape = shape()->getValue();
    
    if (nshape != NULL)
        ret += nshape->countPolygons2Sided();
    return ret;
}

Node *
NodeCollidableShape::convert2X3d(void)
{
    return NULL;
}

Node *
NodeCollidableShape::convert2Vrml(void) 
{
    return NULL;
}

bool
NodeCollidableShape::canWriteAc3d()
{
    Node *nshape = shape()->getValue();
    if (nshape)
        return nshape->canWriteAc3d();
    return false;
}

bool
NodeCollidableShape::canWriteCattGeo()
{
    Node *nshape = shape()->getValue();
    if (nshape)
        return nshape->canWriteCattGeo();
    return false;
}

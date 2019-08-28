/*
 * NodeHAnimJoint.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "NodeHAnimJoint.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFString.h"
#include "SFInt32.h"
#include "MFInt32.h"
#include "SFFloat.h"
#include "MFFloat.h"
#include "SFVec3f.h"
#include "MyMesh.h"
#include "NodeHAnimHumanoid.h"
#include "Scene.h"
#include "RenderState.h"
#include "Path.h"


ProtoHAnimJoint::ProtoHAnimJoint(Scene *scene)
  : ProtoTransform(scene, "HAnimJoint", HANIM_CHILD_NODE)
{
    displacers.set(
          addExposedField(MFNODE, "displacers", new MFNode(), 
                          X3D_HANIM_DISPLACER));
    limitOrientation.set(
          addExposedField(SFROTATION, "limitOrientation", 
                          new SFRotation(0.0f, 0.0f, 1.0f, 0.0f)));
    llimit.set(
          addExposedField(MFFLOAT, "llimit", new MFFloat()));
    name.set(
          addExposedField(SFSTRING, "name", new SFString()));
    skinCoordIndex.set(
          addExposedField(MFINT32, "skinCoordIndex", new MFInt32()));
    skinCoordWeight.set(
          addExposedField(MFFLOAT, "skinCoordWeight", new MFFloat()));
    stiffness.set(
          addExposedField(MFFLOAT, "stiffness", new MFFloat(),
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    ulimit.set(
          addExposedField(MFFLOAT, "ulimit", new MFFloat()));
}

Node *
ProtoHAnimJoint::create(Scene *scene)
{ 
    return new NodeHAnimJoint(scene, this); 
}

NodeHAnimJoint::NodeHAnimJoint(Scene *scene, Proto *def)
  : NodeTransform(scene, def)
{
    m_jointMatrix = Matrix::identity();
    m_coord = NULL;
}

void
NodeHAnimJoint::setField(int field, FieldValue *value, int cf)
{
    if (field != getChildrenField()) 
        m_matrixDirty = true;

    if (field < displacers_Field())
        TransformNode::setField(field, value, cf);
    else
        Node::setField(field, value, cf);
}

int
NodeHAnimJoint::getComponentLevel(void) const
{
    return 1;
}

const char* 
NodeHAnimJoint::getComponentName(void) const
{
    static const char* name = "H-Anim";
    return name;
}

static bool searchCoord(Node *node, void *data)
{
    NodeCoordinate **ret = (NodeCoordinate **)data;
    if (node->getType() == VRML_COORDINATE) {
        NodeCoordinate *coord = (NodeCoordinate *)node;
        coord->getScene()->setInfoHandles(true);
        coord->drawHandles();
        coord->getScene()->setInfoHandles(false);
        *ret = coord;
    }
    return true;     
}

void    
NodeHAnimJoint::drawHandles(void)
{
    if (m_scene->getSelection())
        if (m_scene->getSelection()->getNode() == this) {
            NodeHAnimHumanoid *human = this->getHumanoid();
            if (human) {
                m_scene->setLastSelectedHAnimJoint(this);
                m_scene->setLastSelectedHAnimJointOrHumanoid(this);
                human->doWithBranch(searchCoord, &m_coord, false, false);
                human->drawJointHandle(TheApp->GetHandleScale(), this);
            }
            return;
        }
    updateHandles();    
}

void    
NodeHAnimJoint::setHandle(int handle, const Vec3f &v) 
{
    if (handle > NO_HANDLE)
        NodeTransform::setHandle(handle, v);
}


void
NodeHAnimJoint::drawJointHandles(float scale, Node *parent, Node *that)
{
    if ((that == NULL) || (that == this)) {
        RenderState state;
        TransformMode* tm=m_scene->getTransformMode();
        if ((tm->tmode == TM_CENTER) && (that == this))
            state.drawJointHandles(this, m_jointMatrix, scale, CENTER);
        else
            state.drawJointHandles(this, m_jointMatrix, scale);
        if (that == this)
            return;
    } else
        preDraw();

    if (that == NULL) {
        glPushName(getBasicChildrenField());  // field
        glPushName(0);                        // index
    }

    NodeList *childList = getBasicChildren()->getValues();
    for (size_t i = 0; i < childList->size(); i++) {
        if (childList->get(i) && 
            (childList->get(i)->getType() == X3D_HANIM_JOINT)) {
            if (that == NULL)
                glLoadName(i);
            childList->get(i)->drawJointHandles(scale, this, that);
        }
    }

    if (that == NULL) {
        glPopName();
        glPopName();
    }
}

void 
NodeHAnimJoint::accountJointMatrix(Node *parent)
{   
    glPushMatrix();
    glLoadIdentity();
    m_jointMatrix = Matrix::identity();
    Matrix thisMatrix = Matrix::identity();
    transform();
    if (parent->getType() == X3D_HANIM_JOINT) {
        NodeHAnimJoint *joint = (NodeHAnimJoint *)parent;
        Matrix matrix;
        joint->getJointMatrix(matrix);
        getMatrix(thisMatrix);  
        m_jointMatrix = matrix * thisMatrix;
    } else if (parent && parent->getType() == X3D_HANIM_HUMANOID) {
        NodeHAnimHumanoid *human = (NodeHAnimHumanoid *)parent;
        Matrix matrix;
        human->getMatrix(matrix);
        getMatrix(thisMatrix);  
        m_jointMatrix = matrix * thisMatrix;
    }
    glPopMatrix();
}   

void
NodeHAnimJoint::applyJoint(int skinNum, MyMesh *mesh, MFVec3f *origVertices, 
                           Node *parent)
{
    accountJointMatrix(parent);
    MFInt32 *mfskinCoordIndex = skinCoordIndex();
    MFFloat *mfskinCoordWeight = skinCoordWeight();
    MFVec3f *vertices = mesh->getVertices();
    for (int i = 0; i < mfskinCoordIndex->getSize(); i++) {
        if (mfskinCoordIndex->getValue(i) > -1) {
            int index = mfskinCoordIndex->getValue(i);
            if (index >= origVertices->getSFSize())
                continue;
            mesh->setValidVertex(index);
            Vec3f point = origVertices->getVec(index);
            Vec3f newpoint = vertices->getVec(index);
            int sCwI = MIN(i, mfskinCoordWeight->getSize() - 1);
            float weight = mfskinCoordWeight->getValue(sCwI);
            if (weight == 1)
                point = m_jointMatrix * point ;
            else {
                point =  (m_jointMatrix * point) * weight;
            }
            newpoint += point;
            vertices->setVec(index, newpoint);
        }
    }
   
    for (int i = 0; i < children()->getSize(); i++) {
        children()->getValue(i)->applyJoint(skinNum, mesh, origVertices, this
                                           );
    }
}

Node *
NodeHAnimJoint::getJointParent(Node *node, Node *parent)
{
    if (node == this)
        return parent;
    for (int i = 0; i < children()->getSize(); i++) {
        if (children()->getValue(i)->getType()  == X3D_HANIM_JOINT) {
            NodeHAnimJoint *joint = (NodeHAnimJoint *)children()->getValue(i);
            return joint->getJointParent(node, this);
        }      
    }
    return NULL;
}

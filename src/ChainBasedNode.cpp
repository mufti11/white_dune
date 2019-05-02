/*
 * ChainBasedNode.cpp
 *
 * Copyright (C) 2003 Th. Rothermel, 2007 J. "MUFTI" Scheurich
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

#include "stdafx.h"
#include "Scene.h"
#include "Util.h"
#include "MoveCommand.h"
#include "MFFloat.h"
#include "MFVec3f.h"
#include "ChainBasedNode.h"
#include "NodeCoordinate.h"
#include "NodeShape.h"
#include "NodeIndexedLineSet.h"
#include "NodePositionInterpolator.h"
#include "NodeOrientationInterpolator.h"
#include "NodeCurveAnimation.h"
#include "LdrawDefines.h"

ChainBasedNode::ChainBasedNode(Scene *scene, Proto *proto)
  : Node(scene, proto)
{
    m_chain.resize(0);
    m_chainDirty = true;
}

ChainBasedNode::~ChainBasedNode()
{
    m_chain.resize(0);
}

void
ChainBasedNode::setField(int index, FieldValue *value, int cf) 
{
    m_chainDirty = true;
    Node::setField(index, value, cf);
}

void
ChainBasedNode::draw()
{
    if(m_chainDirty) {
        createChain();
        m_chainDirty = false;
    }
    
    if (m_chain.size() == 0) 
        createChain();

    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glLineWidth(1);
    
    //taken from IndexedLineSet if colors == NULL

    float c[4];
    glGetMaterialfv(GL_FRONT, GL_EMISSION, c);
    Util::myGlColor4fv(c);

    glBegin(GL_LINE_STRIP);
    for (int i=0; i<m_chain.size(); i++)
        glVertex3f(m_chain[i].x, m_chain[i].y, m_chain[i].z);
    glEnd();
    glEnable(GL_LIGHTING);
    glPopAttrib();
}

Node * 
ChainBasedNode::toIndexedLineSet(void)
{
    if(m_chainDirty) {
       createChain();
       m_chainDirty = false;
    }
  
    if (m_chain.size() == 0) 
        createChain();

    NodeCoordinate *ncoord = (NodeCoordinate *)m_scene->createNode("Coordinate");

    int chainLength = m_chain.size();
    float *chainValues = new float[chainLength * 3];
    for (int i = 0; i < chainLength; i++) {
        chainValues[i * 3    ] = m_chain[i].x;
        chainValues[i * 3 + 1] = m_chain[i].y;
        chainValues[i * 3 + 2] = m_chain[i].z;
    }
    ncoord->point(new MFVec3f(chainValues, chainLength * 3));
    NodeIndexedLineSet *node = (NodeIndexedLineSet *)
                               m_scene->createNode("IndexedLineSet");
    node->coord(new SFNode(ncoord));
    
    int *chainIndex = new int[chainLength];
    for (int i = 0; i < chainLength; i++)
        chainIndex[i] = i;
    node->coordIndex(new MFInt32(chainIndex, chainLength));
    m_chain.resize(0);
    m_chainDirty = true;
    return node;
}

int             
ChainBasedNode::write(int f, int indent) 
{
    if (TheApp->isWonderlandModuleExport()) {
        NodeIndexedLineSet *chainNode = (NodeIndexedLineSet *)
                                        toIndexedLineSet();
        return chainNode->write(f, indent);
    }
    return Node::write(f, indent);
}

Vec3f               
ChainBasedNode::getMinBoundingBox(void)
{
    if (m_chainDirty) {
        createChain();
        m_chainDirty = false;
    }
  
    if (m_chain.size() == 0) 
        createChain();

    int chainLength = m_chain.size();
    float *chainValues = new float[chainLength * 3];
    for (int i = 0; i < chainLength; i++) {
        chainValues[i * 3    ] = m_chain[i].x;
        chainValues[i * 3 + 1] = m_chain[i].y;
        chainValues[i * 3 + 2] = m_chain[i].z;
    }
    MFVec3f chain(chainValues, chainLength * 3);
    return chain.getMinBoundingBox();
}

Vec3f               
ChainBasedNode::getMaxBoundingBox(void)
{
    if (m_chainDirty) {
        createChain();
        m_chainDirty = false;
    }
  
    if (m_chain.size() == 0) 
        createChain();

    int chainLength = m_chain.size();
    float *chainValues = new float[chainLength * 3];
    for (int i = 0; i < chainLength; i++) {
        chainValues[i * 3    ] = m_chain[i].x;
        chainValues[i * 3 + 1] = m_chain[i].y;
        chainValues[i * 3 + 2] = m_chain[i].z;
    }
    MFVec3f chain(chainValues, chainLength * 3);
    return chain.getMaxBoundingBox();
}

void
ChainBasedNode::flip(int index)
{
    Node *lineSet = m_scene->convertToIndexedLineSet(this);
    if (lineSet != NULL)
        lineSet->flip(index);
}

void
ChainBasedNode::swap(int fromTo)
{
    Node *lineSet = m_scene->convertToIndexedLineSet(this);
    if (lineSet != NULL)
        lineSet->swap(fromTo);
}

Node * 
ChainBasedNode::toPositionInterpolator(void)
{
    NodePositionInterpolator *node = (NodePositionInterpolator *)
                                     m_scene->createNode("PositionInterpolator");
    toPositionInterpolator(node);
    return node;
}

Node *
ChainBasedNode::toPositionInterpolator(NodePositionInterpolator *node)
{
    if(m_chainDirty) {
       createChain();
       m_chainDirty = false;
    }
  
    if (m_chain.size() == 0) 
        createChain();

    int chainLength = m_chain.size();
    float *chainValues = new float[chainLength * 3];
    for (int i = 0; i < chainLength; i++) {
        chainValues[i * 3    ] = m_chain[i].x;
        chainValues[i * 3 + 1] = m_chain[i].y;
        chainValues[i * 3 + 2] = m_chain[i].z;
    }
    node->keyValue(new MFVec3f(chainValues, chainLength * 3));
    float *chainKeyes = new float[chainLength];
    for (int i = 0; i < chainLength; i++)
        if (chainLength == 1)
            chainKeyes[i] = 0;
        else
            chainKeyes[i] = (1.0 / (chainLength - 1)) * i;
    node->key(new MFFloat(chainKeyes, chainLength));
    m_chain.resize(0);
    m_chainDirty = true;
    return node;
}

Vec3f 
ChainBasedNode::accountProjection(Vec3f vector, Vec3f normal) 
{
    float angle = accountAngle(vector, normal);
    float vectorToProjectionLength = vector.length() * cos(angle);
    return vector - normal * vectorToProjectionLength;
}

float
ChainBasedNode::accountAngle(Vec3f vec1, Vec3f vec2) 
{
    float vec1len = vec1.length();
    float vec2len = vec2.length();
    if ((vec1len == 0) || (vec2len == 0))
        return 0;
    float sinangle = vec1.cross(vec2).length() / vec1len / vec2len;
    float cosangle = vec1.dot(vec2) / vec1len / vec2len;
    float angle = 0;
    if (sinangle != 0)
        angle = atan2(sinangle, cosangle);
    return angle;
}

Quaternion
ChainBasedNode::accountQuat(Vec3f vec1, Vec3f vec2) 
{
    Vec3f rotationAxis = vec1.cross(vec2);
    float angle = accountAngle(vec1, vec2); 
    Vec3f axis(rotationAxis.x, rotationAxis.y, rotationAxis.z);
    axis.normalize();
    Quaternion quat(axis, angle);
    quat.normalize();
    return quat;
}

Node * 
ChainBasedNode::toOrientationInterpolator(Direction direction)
{
    NodeOrientationInterpolator *node = (NodeOrientationInterpolator *)
                                  m_scene->createNode("OrientationInterpolator");
    float normal[] = { 0, 0, 0 };
    switch (direction) {
      case XZ_DIRECTION:
        normal[1] = 1;
        break;
      case YZ_DIRECTION:
        normal[0] = 1;
        break;
      case XY_DIRECTION:
        normal[2] = 1;
        break;
    }
    toOrientationInterpolator(node, normal);
    return node;
}     

void
ChainBasedNode::toOrientationInterpolator(NodeOrientationInterpolator *
                                          rotationInterpolator,
                                          const float *rotationAxis)
{
    if(m_chainDirty) {
       createChain();
       m_chainDirty = false;
    }
  
    if (m_chain.size() == 0) 
        createChain();

    int chainLength = m_chain.size();
    if (chainLength < 3)
        return;
    float *chainRot = new float[chainLength * 4];
    Vec3f vecZ(0, 0, 1);
    Quaternion oldQuat(0, 0, 1, 0);
    Quaternion correctionX(1, 0, 0, cos(0.5 * M_PI));
    Quaternion correctionY(0, 1, 0, cos(0.5 * M_PI));
    Quaternion climbQuat;
    for (int j = 0; j < (chainLength - 1); j++) {
        Vec3f normal(rotationAxis);
        Vec3f vector1(0, 0, 1);
        Vec3f point3(m_chain[j + 1].x,  m_chain[j + 1].y, m_chain[j + 1].z);
        Vec3f point2(m_chain[j    ].x,  m_chain[j    ].y, m_chain[j    ].z);
        if (j > 0) {
            Vec3f point1(m_chain[j - 1].x,  m_chain[j - 1].y, m_chain[j - 1].z);
            vector1 = point2 - point1;
        }
        Vec3f vector2(point3 - point2);
        Vec3f vec1 = accountProjection(vector1, normal);
        Vec3f vec2 = accountProjection(vector2, normal);
        Quaternion quat = accountQuat(vec1, vec2);
        quat = quat * oldQuat;
        if (j == 0) {
            quat = quat * correctionY; 
            quat = quat * correctionX; 
        }
        SFRotation rot(quat);
        for (int i = 0; i < 4; i++)
            chainRot[j * 4 + i] =  rot.getValue()[i];
        oldQuat = quat;
        Vec3f plainVec;
        plainVec = vecZ * quat;
        if (getType() == DUNE_CURVE_ANIMATION) {
            bool hashover = ((NodeCurveAnimation *)this)->hover()->getValue();
            if (!hashover) {
                climbQuat = accountQuat(plainVec, vector2);
                oldQuat = oldQuat * climbQuat;
            }
        }
    }
    
    if (chainLength > 0) {
        SFRotation rot(oldQuat);
        for (int i = 0; i < 4; i++)
            chainRot[(chainLength - 1) * 4 + i] = rot.getValue()[i];
    }
    rotationInterpolator->keyValue(new MFRotation(chainRot, chainLength * 4));
    float *chainKeyes = new float[chainLength];
    for (int i = 0; i < chainLength; i++)
        if (chainLength == 1)
            chainKeyes[i] = 0;
        else
            chainKeyes[i] = (1.0 / (chainLength - 1)) * i;
    rotationInterpolator->key(new MFFloat(chainKeyes, chainLength));
}

const Vec3f *
ChainBasedNode::getChain()
{
    if(m_chainDirty) {
       createChain();
       m_chainDirty = false;
    }
    return m_chain.getData();
}

int
ChainBasedNode::getChainLength()
{
    if(m_chainDirty) {
       createChain();
       m_chainDirty = false;
    }
    return m_chain.size();
}

int 
ChainBasedNode::writeLdrawDat(int f, int indent)
{
    if(m_chainDirty) {
       createChain();
       m_chainDirty = false;
    }
  
    if (m_chain.size() == 0) 
        createChain();

    Matrix matrix;
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) matrix);
    RET_ONERROR( mywritestr(f, "0 //") )
    if (hasName())
        RET_ONERROR( mywritef(f, " %s", (const char *)getName()))
    RET_ONERROR( mywritef(f, " %s\n", (const char *)getProto()->getName(false)))

    if (hasName())
        RET_ONERROR( mywritef(f, "name \"%s\"\n", getName().getData()) )
        
    for (int i = 0; i < m_chain.size(); i++) {
        if (i > 0) {
            RET_ONERROR( mywritef(f, "2 24 ") )
            for (int j = 0; j < 2; j++)  {
                Vec3f v(m_chain[i - j].x, m_chain[i - j].y, m_chain[i - j].z);
                v = matrix * v;
                RET_ONERROR( mywritef(f, " %f %f %f",  v.z * LDRAW_SCALE, 
                                                      -v.y * LDRAW_SCALE, 
                                                       v.x * LDRAW_SCALE) )
            }
            RET_ONERROR( mywritestr(f, "\n") )
        }
    }
    return 0;
}

void    
ChainBasedNode::reInit(void)
{ 
    m_chain.resize(0); 
    m_chainDirty = true;
    Node::reInit();
}

ChainBasedGeometryNode::ChainBasedGeometryNode(Scene *scene, Proto *proto)
  : ChainBasedNode(scene, proto)
{
}

ChainBasedGeometryNode::~ChainBasedGeometryNode()
{
}

GeometryNodeMacros(ChainBasedGeometryNode)

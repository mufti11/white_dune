/*
 * NodeUniversalJoint.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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

#include "NodeUniversalJoint.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFBool.h"
#include "DuneApp.h"
#include "Scene.h"
#include "RenderState.h"

ProtoUniversalJoint::ProtoUniversalJoint(Scene *scene)
  : AnchoredJointProto(scene, "UniversalJoint")
{
    axis1.set(addExposedField(SFVEC3F, "axis1", new SFVec3f()));
    axis2.set(addExposedField(SFVEC3F, "axis2", new SFVec3f()));
    stop1Bounce.set(addExposedField(SFFLOAT, "stop1Bounce", new SFFloat(0), 
                                    new SFFloat(0), new SFFloat(1)));
    stop1ErrorCorrection.set(addExposedField(SFFLOAT, 
                                              "stop1ErrorCorrection", 
                                              new SFFloat(0.8), 
                                              new SFFloat(0), new SFFloat(1)));
    stop2Bounce.set(addExposedField(SFFLOAT, "stop2Bounce", new SFFloat(0), 
                                    new SFFloat(0), new SFFloat(1)));
    stop2ErrorCorrection.set(addExposedField(SFFLOAT, 
                                              "stop2ErrorCorrection", 
                                              new SFFloat(0.8), 
                                              new SFFloat(0), new SFFloat(1)));
    addEventOut(SFVEC3F, "body1Axis");
    addEventOut(SFVEC3F, "body2Axis");
}

Node *
ProtoUniversalJoint::create(Scene *scene)
{ 
    return new NodeUniversalJoint(scene, this); 
}

NodeUniversalJoint::NodeUniversalJoint(Scene *scene, Proto *def)
  : AnchoredJointNode(scene, def)
{
}

Vec3f   
NodeUniversalJoint::getHandle(int handle, int *constraint, int *field)
{
    const float *v = NULL;
    switch (handle) {
      case TRANSLATION:    
      case TRANSLATION_X:    
      case TRANSLATION_Y:    
      case TRANSLATION_Z:    
        return AnchoredJointNode::getHandle(handle, constraint, field);
      case AXIS1:
        *constraint = CONSTRAIN_NONE;
        *field = axis1_Field();
        v = axis1()->getValue();
        return Vec3f(v[0], v[1], v[2]);
      case AXIS2:
        *constraint = CONSTRAIN_NONE;
        *field = axis2_Field();
        v = axis2()->getValue();
        return Vec3f(v[0], v[1], v[2]);
      default:
        *constraint = CONSTRAIN_NONE;
        *field = getProto()->metadata_Field();
        return Vec3f(0.0f, 0.0f, 0.0f);        
    }
}

void    
NodeUniversalJoint::setHandle(int handle, const Vec3f &v)
{
    Vec3f normedV(0, 0, 0);
    Vec3f vAxis1(axis1()->getValue());
    Vec3f vAxis2(axis2()->getValue());
    switch (handle) {
      case TRANSLATION:    
      case TRANSLATION_X:    
      case TRANSLATION_Y:    
      case TRANSLATION_Z:    
        AnchoredJointNode::setHandle(handle, v);
        break;
      case AXIS1:
        if (m_scene->getXonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.x = v.x;
        if (m_scene->getYonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.y = v.y;
        if (m_scene->getZonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.z = v.z;
        normedV.normalize();
        if (vAxis1.squaredNorm() != 0) {
            Vec3f oldAxis(vAxis1);
            oldAxis.normalize();
            Vec3f otherAxis(vAxis2);
            otherAxis.normalize();
            float angle = acos(normedV.dot(oldAxis));
            Vec3f v = oldAxis.cross(otherAxis); 
            Quaternion q(otherAxis, angle);
            normedV = v * q;
            Quaternion q2(otherAxis, M_PI / 2.0f);
            normedV = normedV * q2;
            normedV.normalize();
        }
        m_scene->setField(this, axis1_Field(),
                         new SFVec3f(normedV.x, normedV.y, normedV.z));
        break;
      case AXIS2:
        if (m_scene->getXonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.x = v.x;
        if (m_scene->getYonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.y = v.y;
        if (m_scene->getZonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.z = v.z;
        normedV.normalize();
        if (vAxis2.squaredNorm() != 0) {
            Vec3f oldAxis(vAxis2);
            oldAxis.normalize();
            Vec3f otherAxis(vAxis1);
            otherAxis.normalize();
            float angle = acos(normedV.dot(oldAxis));
            Vec3f v = oldAxis.cross(otherAxis); 
            Quaternion q(otherAxis, angle);
            normedV = v * q;
            Quaternion q2(otherAxis, M_PI / 2.0f);
            normedV = normedV * q2;
            normedV.normalize();
            m_scene->setField(this, axis2_Field(),
                             new SFVec3f(normedV.x, normedV.y, normedV.z));
        }
        break;
    }
}

void
NodeUniversalJoint::drawHandles() {

    AnchoredJointNode::drawHandles(true);

    glPushMatrix();
    transformForHandle(AXIS1);
  
    RenderState state;
    state.setVectorStart(0.1);
    const float *faxis1 = axis1()->getValue();
    state.drawVectorHandle(AXIS1, faxis1[0], faxis1[1], faxis1[2], 
                           1.0f, 0.5f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    transformForHandle(AXIS2);
  
    const float *faxis2 = axis2()->getValue();
    state.drawVectorHandle(AXIS2, faxis2[0], faxis2[1], faxis2[2], 
                           1.0f, 0.0f, 0.5f);
    glPopMatrix();
}


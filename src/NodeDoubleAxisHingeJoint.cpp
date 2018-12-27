/*
 * NodeDoubleAxisHingeJoint.cpp
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

#include "NodeDoubleAxisHingeJoint.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFBool.h"
#include "DuneApp.h"
#include "Scene.h"
#include "RenderState.h"

ProtoDoubleAxisHingeJoint::ProtoDoubleAxisHingeJoint(Scene *scene)
  : AnchoredJointProto(scene, "DoubleAxisHingeJoint")
{
    double unitAngle = scene->getUnitAngle();

    axis1.set(addExposedField(SFVEC3F, "axis1", new SFVec3f()));

    axis2.set(addExposedField(SFVEC3F, "axis2", new SFVec3f()));

    desiredAngularVelocity1.set(addExposedField(SFFLOAT, 
                                                "desiredAngularVelocity1",
                                                new SFFloat()));

    desiredAngularVelocity2.set(addExposedField(SFFLOAT, 
                                                "desiredAngularVelocity2",
                                                new SFFloat()));

    maxAngle1.set( 
          addExposedField(SFFLOAT, "maxAngle1", new SFFloat(M_PI / unitAngle),
                          new SFFloat(-M_PI / unitAngle),
                          new SFFloat( M_PI / unitAngle)));

    maxTorque1.set(addExposedField(SFFLOAT, "maxTorque1", new SFFloat()));

    maxTorque2.set(addExposedField(SFFLOAT, "maxTorque2", new SFFloat()));

    minAngle1.set( 
          addExposedField(SFFLOAT, "minAngle1", new SFFloat(-M_PI / unitAngle),
                          new SFFloat(-M_PI / unitAngle), 
                          new SFFloat( M_PI / unitAngle)));

    stopBounce1.set(addExposedField(SFFLOAT, "stopBounce1", new SFFloat(0), 
                                    new SFFloat(0), new SFFloat(1)));

    stopConstantForceMix1.set(addExposedField(SFFLOAT, 
                                              "stopConstantForceMix1", 
                                              new SFFloat(0.001), 
                                              new SFFloat(0)));

    stopErrorCorrection1.set(addExposedField(SFFLOAT, 
                                              "stopErrorCorrection1", 
                                              new SFFloat(0.8), 
                                              new SFFloat(0), new SFFloat(1)));

    suspensionErrorCorrection.set(addExposedField(SFFLOAT, 
                                                  "suspensionErrorCorrection", 
                                                  new SFFloat(0.8), 
                                                  new SFFloat(0), 
                                                  new SFFloat(1)));

    suspensionForce.set(addExposedField(SFFLOAT, "suspensionForce", 
                                        new SFFloat(0), new SFFloat(0)));

    addEventOut(SFVEC3F, "body1Axis");
    addEventOut(SFVEC3F, "body2Axis");
    addEventOut(SFFLOAT, "hinge1Angle");
    addEventOut(SFFLOAT, "hinge2Angle");
    addEventOut(SFFLOAT, "hinge1AngleRate");
    addEventOut(SFFLOAT, "hinge2AngleRate");
}

Node *
ProtoDoubleAxisHingeJoint::create(Scene *scene)
{ 
    return new NodeDoubleAxisHingeJoint(scene, this); 
}

NodeDoubleAxisHingeJoint::NodeDoubleAxisHingeJoint(Scene *scene, Proto *def)
  : AnchoredJointNode(scene, def)
{
}

Vec3f   
NodeDoubleAxisHingeJoint::getHandle(int handle, int *constraint, int *field)
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
NodeDoubleAxisHingeJoint::setHandle(int handle, const Vec3f &v)
{
    Vec3f normedV(0, 0, 0);
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
        m_scene->setField(this, axis2_Field(),
                         new SFVec3f(normedV.x, normedV.y, normedV.z));
        break;
    }
}

void
NodeDoubleAxisHingeJoint::drawHandles(void) 
{
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



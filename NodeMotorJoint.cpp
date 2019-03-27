/*
 * NodeMotorJoint.cpp
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

#include "NodeMotorJoint.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFBool.h"
#include "DuneApp.h"
#include "Scene.h"
#include "RenderState.h"

ProtoMotorJoint::ProtoMotorJoint(Scene *scene)
  : X3DRigidJointProto(scene, "MotorJoint")
{
    addElements(scene);
}

ProtoMotorJoint::ProtoMotorJoint(Scene *scene, const char *name)
  : X3DRigidJointProto(scene, name)
{
    addElements(scene);
}

void 
ProtoMotorJoint::addElements(Scene *scene) 
{
    double unitAngle = scene->getUnitAngle();

    axis1Angle.set(addExposedField(SFFLOAT, "axis1Angle", new SFFloat(0),
                              new SFFloat(-M_PI / unitAngle), 
                              new SFFloat( M_PI / unitAngle)));
    axis1Torque.set(addExposedField(SFFLOAT, "axis1Torque", new SFFloat(0)));
    axis2Angle.set(addExposedField(SFFLOAT, "axis2Angle", new SFFloat(0),
                              new SFFloat(-M_PI / unitAngle), 
                              new SFFloat( M_PI / unitAngle)));
    axis2Torque.set(addExposedField(SFFLOAT, "axis2Torque", new SFFloat(0)));
    axis3Angle.set(addExposedField(SFFLOAT, "axis3Angle", new SFFloat(0),
                              new SFFloat(-M_PI / unitAngle),
                              new SFFloat( M_PI / unitAngle)));
    axis3Torque.set(addExposedField(SFFLOAT, "axis3Torque", new SFFloat(0)));
    enabledAxes.set(addExposedField(SFINT32, "enabledAxes", new SFInt32(1)));
    motor1Axis.set(addExposedField(SFVEC3F, "motor1Axis", new SFVec3f())); 
    motor2Axis.set(addExposedField(SFVEC3F, "motor2Axis", new SFVec3f())); 
    motor3Axis.set(addExposedField(SFVEC3F, "motor3Axis", new SFVec3f())); 
    stop1Bounce.set(addExposedField(SFFLOAT, "stop1Bounce", new SFFloat(0), 
                                    new SFFloat(0), new SFFloat(1)));
    stop1ErrorCorrection.set(addExposedField(SFFLOAT, "stop1ErrorCorrection", 
                                             new SFFloat(0.8), 
                                             new SFFloat(0), new SFFloat(1)));
    stop2Bounce.set(addExposedField(SFFLOAT, "stop2Bounce", new SFFloat(0), 
                                    new SFFloat(0), new SFFloat(1)));
    stop2ErrorCorrection.set(addExposedField(SFFLOAT, "stop2ErrorCorrection", 
                                             new SFFloat(0.8), 
                                             new SFFloat(0), new SFFloat(1)));
    stop3Bounce.set(addExposedField(SFFLOAT, "stop3Bounce", new SFFloat(0), 
                                    new SFFloat(0), new SFFloat(1)));
    stop3ErrorCorrection.set(addExposedField(SFFLOAT, "stop3ErrorCorrection", 
                                             new SFFloat(0.8), 
                                             new SFFloat(0), new SFFloat(1)));
    autoCalc.set(addField(SFBOOL, "autoCalc", new SFBool(false))); 
    addEventOut(SFFLOAT, "motor1Angle");
    addEventOut(SFFLOAT, "motor1AngleRate");
    addEventOut(SFFLOAT, "motor2Angle");
    addEventOut(SFFLOAT, "motor2AngleRate");
    addEventOut(SFFLOAT, "motor3Angle");
    addEventOut(SFFLOAT, "motor3AngleRate");
}

Node *
ProtoMotorJoint::create(Scene *scene)
{ 
    return new NodeMotorJoint(scene, this); 
}

NodeMotorJoint::NodeMotorJoint(Scene *scene, Proto *def)
  : X3DRigidJointNode(scene, def)
{
}

Vec3f   
NodeMotorJoint::getHandle(int handle, int *constraint, int *field)
{
    const float *v = NULL;
    switch (handle) {
      case AXIS1:
        *constraint = CONSTRAIN_NONE;
        *field = motor1Axis_Field();
        v = motor1Axis()->getValue();
        return Vec3f(v[0], v[1], v[2]);
      case AXIS2:
        *constraint = CONSTRAIN_NONE;
        *field = motor2Axis_Field();
        v = motor2Axis()->getValue();
        return Vec3f(v[0], v[1], v[2]);
      case AXIS3:
        *constraint = CONSTRAIN_NONE;
        *field = motor3Axis_Field();
        v = motor3Axis()->getValue();
        return Vec3f(v[0], v[1], v[2]);
      default:
        *constraint = CONSTRAIN_NONE;
        *field = getProto()->metadata_Field();   
        return Vec3f(0.0f, 0.0f, 0.0f);        
    }
}

void    
NodeMotorJoint::setHandle(int handle, const Vec3f &v)
{
    Vec3f normedV(0, 0, 0);
    switch (handle) {
      case AXIS1:
        if (m_scene->getXonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.x = v.x;
        if (m_scene->getYonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.y = v.y;
        if (m_scene->getZonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.z = v.z;
        normedV.normalize();
        m_scene->setField(this, motor1Axis_Field(),
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
        m_scene->setField(this, motor2Axis_Field(),
                         new SFVec3f(normedV.x, normedV.y, normedV.z));
        break;
      case AXIS3:
        if (m_scene->getXonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.x = v.x;
        if (m_scene->getYonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.y = v.y;
        if (m_scene->getZonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            normedV.z = v.z;
        normedV.normalize();
        m_scene->setField(this, motor3Axis_Field(),
                         new SFVec3f(normedV.x, normedV.y, normedV.z));
        break;
    }
}

void
NodeMotorJoint::drawHandles() {

    glPushMatrix();
    transformForHandle(AXIS1);
  
    RenderState state;
    state.setVectorStart(0.1);
    const float *faxis1 = motor1Axis()->getValue();
    state.drawVectorHandle(AXIS1, faxis1[0], faxis1[1], faxis1[2], 
                           1.0f, 0.5f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    transformForHandle(AXIS2);
  
    const float *faxis2 = motor2Axis()->getValue();
    state.drawVectorHandle(AXIS2, faxis2[0], faxis2[1], faxis2[2], 
                           1.0f, 0.0f, 0.5f);
    glPopMatrix();

    glPushMatrix();
    transformForHandle(AXIS3);
  
    const float *faxis3 = motor3Axis()->getValue();
    state.drawVectorHandle(AXIS3, faxis3[0], faxis3[1], faxis3[2], 
                           1.0f, 0.25f, 0.25f);
    glPopMatrix();
}




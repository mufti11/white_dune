/*
 * NodeMotorJoint.h
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

#pragma once

#include "X3DRigidJointNode.h"
#include "SFMFTypes.h"

class ProtoMotorJoint : public X3DRigidJointProto {
public:
                    ProtoMotorJoint(Scene *scene);
                    ProtoMotorJoint(Scene *scene, const char *name);
    void            addElements(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_MOTOR_JOINT; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    needUpdate(void) { return true; }

    FieldIndex axis1Angle;
    FieldIndex axis1Torque;
    FieldIndex axis2Angle;
    FieldIndex axis2Torque;
    FieldIndex axis3Angle;
    FieldIndex axis3Torque;
    FieldIndex enabledAxes;
    FieldIndex motor1Axis;
    FieldIndex motor2Axis;
    FieldIndex motor3Axis;
    FieldIndex stop1Bounce;
    FieldIndex stop1ErrorCorrection;
    FieldIndex stop2Bounce;
    FieldIndex stop2ErrorCorrection;
    FieldIndex stop3Bounce;
    FieldIndex stop3ErrorCorrection;
    FieldIndex autoCalc;
};

class NodeMotorJoint : public X3DRigidJointNode {
public:
                    NodeMotorJoint(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 2; }
    virtual Node   *copy() const { return new NodeMotorJoint(*this); }

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    void            drawHandles(void);

    fieldMacros(SFFloat, axis1Angle,           ProtoMotorJoint)
    fieldMacros(SFFloat, axis1Torque,          ProtoMotorJoint)
    fieldMacros(SFFloat, axis2Angle,           ProtoMotorJoint)
    fieldMacros(SFFloat, axis2Torque,          ProtoMotorJoint)
    fieldMacros(SFFloat, axis3Angle,           ProtoMotorJoint)
    fieldMacros(SFFloat, axis3Torque,          ProtoMotorJoint)
    fieldMacros(SFInt32, enabledAxes,          ProtoMotorJoint)
    fieldMacros(SFVec3f, motor1Axis,           ProtoMotorJoint)
    fieldMacros(SFVec3f, motor2Axis,           ProtoMotorJoint)
    fieldMacros(SFVec3f, motor3Axis,           ProtoMotorJoint)
    fieldMacros(SFFloat, stop1Bounce,          ProtoMotorJoint)
    fieldMacros(SFFloat, stop1ErrorCorrection, ProtoMotorJoint)
    fieldMacros(SFFloat, stop2Bounce,          ProtoMotorJoint)
    fieldMacros(SFFloat, stop2ErrorCorrection, ProtoMotorJoint)
    fieldMacros(SFFloat, stop3Bounce,          ProtoMotorJoint)
    fieldMacros(SFFloat, stop3ErrorCorrection, ProtoMotorJoint)
    fieldMacros(SFBool,  autoCalc,             ProtoMotorJoint)
};


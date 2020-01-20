/*
 * NodeDoubleAxisHingeJoint.h
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

#include "AnchoredJointNode.h"
#include "SFMFTypes.h"

class ProtoDoubleAxisHingeJoint : public AnchoredJointProto {
public:
                    ProtoDoubleAxisHingeJoint(Scene *scene);

    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_DOUBLE_AXIS_HINGE_JOINT; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    needUpdate(void) { return true; }

    FieldIndex axis1;
    FieldIndex axis2;
    FieldIndex desiredAngularVelocity1;
    FieldIndex desiredAngularVelocity2;
    FieldIndex maxAngle1;
    FieldIndex maxTorque1;
    FieldIndex maxTorque2;
    FieldIndex minAngle1;
    FieldIndex stopBounce1;
    FieldIndex stopConstantForceMix1;
    FieldIndex stopErrorCorrection1;
    FieldIndex suspensionErrorCorrection;
    FieldIndex suspensionForce;
};

class NodeDoubleAxisHingeJoint : public AnchoredJointNode {
public:
                    NodeDoubleAxisHingeJoint(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 2; }
    virtual Node   *copy() const { return new NodeDoubleAxisHingeJoint(*this); }

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    void            drawHandles(void);

    fieldMacros(SFVec3f, axis1,                     ProtoDoubleAxisHingeJoint)
    fieldMacros(SFVec3f, axis2,                     ProtoDoubleAxisHingeJoint)
    fieldMacros(SFFloat, desiredAngularVelocity1,   ProtoDoubleAxisHingeJoint)
    fieldMacros(SFFloat, desiredAngularVelocity2,   ProtoDoubleAxisHingeJoint)
    fieldMacros(SFFloat, maxAngle1,                 ProtoDoubleAxisHingeJoint)
    fieldMacros(SFFloat, maxTorque1,                ProtoDoubleAxisHingeJoint)
    fieldMacros(SFFloat, maxTorque2,                ProtoDoubleAxisHingeJoint)
    fieldMacros(SFFloat, minAngle1,                 ProtoDoubleAxisHingeJoint)
    fieldMacros(SFFloat, stopBounce1,               ProtoDoubleAxisHingeJoint)
    fieldMacros(SFFloat, stopConstantForceMix1,     ProtoDoubleAxisHingeJoint)
    fieldMacros(SFFloat, stopErrorCorrection1,      ProtoDoubleAxisHingeJoint)
    fieldMacros(SFFloat, suspensionErrorCorrection, ProtoDoubleAxisHingeJoint)
    fieldMacros(SFFloat, suspensionForce,           ProtoDoubleAxisHingeJoint)
};


/*
 * NodeSingleAxisHingeJoint.h
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

#ifndef _NODE_SINGLE_AXIS_HINGE_JOINT_H
#define _NODE_SINGLE_AXIS_HINGE_JOINT_H

#include "AnchoredJointNode.h"

#include "SFMFTypes.h"

class Quaternion;

class ProtoSingleAxisHingeJoint : public AnchoredJointProto {
public:
                    ProtoSingleAxisHingeJoint(Scene *scene);
                    ProtoSingleAxisHingeJoint(Scene *scene, const char * name);

    void            addElements(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_SINGLE_AXIS_HINGE_JOINT; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    needUpdate(void) { return true; }

    FieldIndex axis;
    FieldIndex maxAngle;
    FieldIndex minAngle;
    FieldIndex stopBounce;
    FieldIndex stopErrorCorrection;
};

class NodeSingleAxisHingeJoint : public AnchoredJointNode {
public:
                    NodeSingleAxisHingeJoint(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 2; } 
    virtual Node   *copy() const { return new NodeSingleAxisHingeJoint(*this); }

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    void            drawHandles(void);

    fieldMacros(SFVec3f, axis,                ProtoSingleAxisHingeJoint)
    fieldMacros(SFFloat, maxAngle,            ProtoSingleAxisHingeJoint)
    fieldMacros(SFFloat, minAngle,            ProtoSingleAxisHingeJoint)
    fieldMacros(SFFloat, stopBounce,          ProtoSingleAxisHingeJoint)
    fieldMacros(SFFloat, stopErrorCorrection, ProtoSingleAxisHingeJoint)
};

#endif 

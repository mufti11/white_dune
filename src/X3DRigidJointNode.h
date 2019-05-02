/*
 * X3DRigidJointNode.h
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

#ifndef _X3D_RIGID_JOINT_NODE_H
#define _X3D_RIGID_JOINT_NODE_H

#include "RigidBodyPhysicsNode.h"

#include "SFMFTypes.h"

class X3DRigidJointProto : public Proto {
public:
               X3DRigidJointProto(Scene *scene, const char *name);

    virtual int     getNodeClass() const { return RIGID_JOINT_NODE; }

    FieldIndex body1;
    FieldIndex body2;
    FieldIndex forceOutput;
};

class X3DRigidJointNode : public RigidBodyPhysicsNode {
public:
                    X3DRigidJointNode(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
                           { return "RigidBodyPhysics"; }
    virtual int         getComponentLevel(void) const { return 2; }

    virtual bool        showFields() { return true; }

    fieldMacros(SFNode,   body1,       X3DRigidJointProto)
    fieldMacros(SFNode,   body2,       X3DRigidJointProto)
    fieldMacros(MFString, forceOutput, X3DRigidJointProto)
};

#endif


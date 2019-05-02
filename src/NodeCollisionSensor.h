/*
 * NodeCollisionSensor.h
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

#ifndef _NODE_COLLISION_Sensor_H
#define _NODE_COLLISION_Sensor_H

#include "RigidBodyPhysicsNode.h"

#include "SFMFTypes.h"

class ProtoCollisionSensor : public Proto {
public:
                  ProtoCollisionSensor(Scene *scene);
    virtual Node *create(Scene *scene);

    virtual int   getType() const { return X3D_COLLISION_SENSOR; }
    virtual int   getNodeClass() const { return SENSOR_NODE | CHILD_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex collider;
    FieldIndex enabled;
};

class NodeCollisionSensor : public RigidBodyPhysicsNode {
public:
                    NodeCollisionSensor(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
                           { return "RigidBodyPhysics"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int         getX3dVersion(void) const { return 2; }
    virtual Node       *copy() const 
                           { return new NodeCollisionSensor(*this); }

    fieldMacros(SFNode, collider, ProtoCollisionSensor)
    fieldMacros(SFBool, enabled,     ProtoCollisionSensor)
};

#endif


/*
 * NodeCollisionCollection.h
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

#ifndef _NODE_COLLISION_COLLECTION_H
#define _NODE_COLLISION_COLLECTION_H

#include "RigidBodyPhysicsNode.h"

#include "SFMFTypes.h"

class ProtoCollisionCollection : public Proto {
public:
                  ProtoCollisionCollection(Scene *scene);
    virtual Node *create(Scene *scene);

    virtual int   getType() const { return X3D_COLLISION_COLLECTION; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex appliedParameters;
    FieldIndex bounce;
    FieldIndex collidables;
    FieldIndex enabled;
    FieldIndex frictionCoefficients;
    FieldIndex minBounceSpeed;
    FieldIndex slipFactors;
    FieldIndex softnessConstantForceMix;
    FieldIndex softnessErrorCorrection;
    FieldIndex surfaceSpeed;
};

class NodeCollisionCollection : public RigidBodyPhysicsNode {
public:
                    NodeCollisionCollection(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
                           { return "RigidBodyPhysics"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int         getX3dVersion(void) const { return 2; }
    virtual Node       *copy() const 
                           { return new NodeCollisionCollection(*this); }
    fieldMacros(MFString, appliedParameters,        ProtoCollisionCollection)
    fieldMacros(SFFloat,  bounce,                   ProtoCollisionCollection)
    fieldMacros(MFNode,   collidables,              ProtoCollisionCollection)
    fieldMacros(SFBool,   enabled,                  ProtoCollisionCollection)
    fieldMacros(SFVec2f,  frictionCoefficients,     ProtoCollisionCollection)
    fieldMacros(SFFloat,  minBounceSpeed,           ProtoCollisionCollection)
    fieldMacros(SFVec2f,  slipFactors,              ProtoCollisionCollection)
    fieldMacros(SFFloat,  softnessConstantForceMix, ProtoCollisionCollection)
    fieldMacros(SFFloat,  softnessErrorCorrection,  ProtoCollisionCollection)
    fieldMacros(SFVec2f,  surfaceSpeed,             ProtoCollisionCollection)
};

#endif


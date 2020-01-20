/*
 * NodeContact.h
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

#include "RigidBodyPhysicsNode.h"
#include "SFMFTypes.h"

class ProtoContact : public Proto {
public:
                  ProtoContact(Scene *scene);
    virtual Node *create(Scene *scene);

    virtual int   getType() const { return X3D_CONTACT; }

    virtual bool  isX3dInternalProto(void) { return true; }

    FieldIndex appliedParameters;
    FieldIndex body1;
    FieldIndex body2;
    FieldIndex bounce;
    FieldIndex contactNormal;
    FieldIndex depth;
    FieldIndex frictionCoefficients;
    FieldIndex frictionDirection;
    FieldIndex geometry1;
    FieldIndex geometry2;
    FieldIndex minBounceSpeed;
    FieldIndex position;
    FieldIndex slipCoefficients;
    FieldIndex softnessConstantForceMix;
    FieldIndex softnessErrorCorrection;
    FieldIndex surfaceSpeed;
};

class NodeContact : public RigidBodyPhysicsNode {
public:
                    NodeContact(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
                           { return "RigidBodyPhysics"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 2; }
    virtual Node       *copy() const 
                           { return new NodeContact(*this); }

    virtual bool        showFields() { return true; }

    fieldMacros(MFString, appliedParameters,        ProtoContact)
    fieldMacros(SFNode,   body1,                    ProtoContact)
    fieldMacros(SFNode,   body2,                    ProtoContact)
    fieldMacros(SFFloat,  bounce,                   ProtoContact)
    fieldMacros(SFVec3f,  contactNormal,            ProtoContact)
    fieldMacros(SFFloat,  depth,                    ProtoContact)
    fieldMacros(SFVec2f,  frictionCoefficients,     ProtoContact)
    fieldMacros(SFVec3f,  frictionDirection,        ProtoContact)
    fieldMacros(SFNode,   geometry1,                ProtoContact)
    fieldMacros(SFNode,   geometry2,                ProtoContact)
    fieldMacros(SFFloat,  minBounceSpeed,           ProtoContact)
    fieldMacros(SFVec3f,  position,                 ProtoContact)
    fieldMacros(SFVec2f,  slipCoefficients,         ProtoContact)
    fieldMacros(SFFloat,  softnessConstantForceMix, ProtoContact)
    fieldMacros(SFFloat,  softnessErrorCorrection,  ProtoContact)
    fieldMacros(SFVec2f,  surfaceSpeed,             ProtoContact)
};


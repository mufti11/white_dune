/*
 * NodeRigidBody.h
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

#ifndef _NODE_RIGID_BODY_H
#define _NODE_RIGID_BODY_H

#include "RigidBodyPhysicsNode.h"

#include "SFMFTypes.h"

class ProtoRigidBody : public Proto {
public:
                  ProtoRigidBody(Scene *scene);
    virtual Node *create(Scene *scene);

    virtual int   getType() const { return X3D_RIGID_BODY; }

    virtual bool  isX3dInternalProto(void) { return true; }

    FieldIndex angularDampingFactor;
    FieldIndex angularVelocity;
    FieldIndex autoDamp;
    FieldIndex autoDisable;
    FieldIndex centerOfMass;
    FieldIndex disableAngularSpeed;
    FieldIndex disableLinearSpeed;
    FieldIndex disableTime;
    FieldIndex enabled;
    FieldIndex finiteRotationAxis;
    FieldIndex fixed;
    FieldIndex forces;
    FieldIndex geometry;
    FieldIndex inertia;
    FieldIndex linearDampingFactor;
    FieldIndex linearVelocity;
    FieldIndex mass;
    FieldIndex massDensityModel;
    FieldIndex orientation;
    FieldIndex position;
    FieldIndex torques;
    FieldIndex useFiniteRotation;
    FieldIndex useGlobalGravity;
};

class NodeRigidBody : public RigidBodyPhysicsNode {
public:
                    NodeRigidBody(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
                           { return "RigidBodyPhysics"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int         getX3dVersion(void) const { return 2; } 
    virtual Node       *copy() { return new NodeRigidBody(m_scene, m_proto); }

    virtual void       preDraw();
    virtual void       draw(int pass); 

    virtual void       setField(int index, FieldValue *value, int cf = -1);
    virtual void       drawHandles(void);
    virtual void       transform();
    virtual void       transformForHandle(int handle);

    virtual Vec3f      getHandle(int handle, int *constraint, int *field);
    virtual void       setHandle(int handle, const Vec3f &v);

    virtual bool       showFields() { return true; }

    void               drawVelocityHandles(float handleScale);

    virtual void       update() { geometry()->update(); }

    fieldMacros(SFFloat,    angularDampingFactor, ProtoRigidBody)
    fieldMacros(SFVec3f,    angularVelocity,      ProtoRigidBody)
    fieldMacros(SFBool,     autoDamp,             ProtoRigidBody)
    fieldMacros(SFBool,     autoDisable,          ProtoRigidBody)
    fieldMacros(SFVec3f,    centerOfMass,         ProtoRigidBody)
    fieldMacros(SFFloat,    disableAngularSpeed,  ProtoRigidBody)
    fieldMacros(SFFloat,    disableLinearSpeed,   ProtoRigidBody)
    fieldMacros(SFFloat,    disableTime,          ProtoRigidBody)
    fieldMacros(SFBool,     enabled,              ProtoRigidBody)
    fieldMacros(SFVec3f,    finiteRotationAxis,   ProtoRigidBody)
    fieldMacros(SFBool,     fixed,                ProtoRigidBody)
    fieldMacros(MFVec3f,    forces,               ProtoRigidBody)
    fieldMacros(MFNode,     geometry,             ProtoRigidBody)
    fieldMacros(SFMatrix3f, inertia,              ProtoRigidBody)
    fieldMacros(SFFloat,    linearDampingFactor,  ProtoRigidBody)
    fieldMacros(SFVec3f,    linearVelocity,       ProtoRigidBody)
    fieldMacros(SFFloat,    mass,                 ProtoRigidBody)
    fieldMacros(SFNode,     massDensityModel,     ProtoRigidBody)
    fieldMacros(SFRotation, orientation,          ProtoRigidBody)
    fieldMacros(SFVec3f,    position,             ProtoRigidBody)
    fieldMacros(MFVec3f,    torques,              ProtoRigidBody)
    fieldMacros(SFBool,     useFiniteRotation,    ProtoRigidBody)
    fieldMacros(SFBool,     useGlobalGravity,     ProtoRigidBody)

private:
    bool              m_matrixDirty;
    Matrix            m_matrix;
    float             m_handleScale;
    int               m_handleNameLinearVelocity;
    int               m_handleNameAngularVelocityX1;
    int               m_handleNameAngularVelocityY1;
    int               m_handleNameAngularVelocityZ1;
    int               m_handleNameAngularVelocityX2;
    int               m_handleNameAngularVelocityY2;
    int               m_handleNameAngularVelocityZ2;
};

#endif


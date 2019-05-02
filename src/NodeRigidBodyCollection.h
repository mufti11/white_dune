/*
 * NodeRigidBodyCollection.h
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

#ifndef _NODE_RIGID_BODY_COLLECTION_H
#define _NODE_RIGID_BODY_COLLECTION_H

#include "RigidBodyPhysicsNode.h"
#include "Scene.h"
#include "SFMFTypes.h"

class ProtoRigidBodyCollection : public Proto {
public:
                  ProtoRigidBodyCollection(Scene *scene);
    virtual Node *create(Scene *scene);

    virtual int   getType() const { return X3D_RIGID_BODY_COLLECTION; }

    virtual bool  isX3dInternalProto(void) { return true; }

    FieldIndex autoDisable;
    FieldIndex bodies;
    FieldIndex constantForceMix;
    FieldIndex contactSurfaceThickness;
    FieldIndex disableAngularSpeed;
    FieldIndex disableLinearSpeed;
    FieldIndex disableTime;
    FieldIndex enabled;
    FieldIndex errorCorrection;
    FieldIndex gravity;
    FieldIndex iterations;
    FieldIndex joints;
    FieldIndex maxCorrectionSpeed;
    FieldIndex preferAccuracy;
    FieldIndex collider;
};

class NodeRigidBodyCollection : public RigidBodyPhysicsNode {
public:
                    NodeRigidBodyCollection(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
                           { return "RigidBodyPhysics"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 2; } 
    virtual Node       *copy() const { return new NodeRigidBodyCollection(*this); }

//    virtual void        preDraw() 
//                           { if (!_scene->isRunning()) bodies()->preDraw(); }
//    virtual void        draw(int pass) 
//                           { 
//                           if (!_scene->isRunning())
//                               bodies()->draw(pass, bodies_Field()); 
//                           }

    virtual bool        showFields() { return true; }

    virtual int         getChildrenField(void) const { return bodies_Field(); }

    fieldMacros(SFBool,  autoDisable,             ProtoRigidBodyCollection)
    fieldMacros(MFNode,  bodies,                  ProtoRigidBodyCollection)
    fieldMacros(SFFloat, constantForceMix,        ProtoRigidBodyCollection)
    fieldMacros(SFFloat, contactSurfaceThickness, ProtoRigidBodyCollection)
    fieldMacros(SFFloat, disableAngularSpeed,     ProtoRigidBodyCollection)
    fieldMacros(SFFloat, disableLinearSpeed,      ProtoRigidBodyCollection)
    fieldMacros(SFFloat, disableTime,             ProtoRigidBodyCollection)
    fieldMacros(SFBool,  enabled,                 ProtoRigidBodyCollection)
    fieldMacros(SFFloat, errorCorrection,         ProtoRigidBodyCollection)
    fieldMacros(SFVec3f, gravity,                 ProtoRigidBodyCollection)
    fieldMacros(SFInt32, iterations,              ProtoRigidBodyCollection)
    fieldMacros(MFNode,  joints,                  ProtoRigidBodyCollection)
    fieldMacros(SFFloat, maxCorrectionSpeed,      ProtoRigidBodyCollection)
    fieldMacros(SFBool,  preferAccuracy,          ProtoRigidBodyCollection)
    fieldMacros(SFNode,  collider,                ProtoRigidBodyCollection)
};

#endif


/*
 * NodeWindPhysicsModel.h
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class ProtoWindPhysicsModel : public Proto {
public:
                    ProtoWindPhysicsModel(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_WIND_PHYSICS_MODEL; }
    virtual int     getNodeClass() const { return PARTICLE_PHYSICS_MODEL_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    isDeclaredInRwd_h() { return true; }

    FieldIndex direction;
    FieldIndex enabled;
    FieldIndex gustiness;
    FieldIndex speed;
    FieldIndex turbulence;
};

class NodeWindPhysicsModel : public Node {
public:
                    NodeWindPhysicsModel(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
                           { return "ParticleSystems"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 2; }    
    virtual Node   *copy() const { return new NodeWindPhysicsModel(*this); }

    Vec3f getForce(void);

    fieldMacros(SFVec3f, direction,  ProtoWindPhysicsModel);
    fieldMacros(SFBool,  enabled,    ProtoWindPhysicsModel);
    fieldMacros(SFFloat, gustiness,  ProtoWindPhysicsModel);
    fieldMacros(SFFloat, speed,      ProtoWindPhysicsModel);
    fieldMacros(SFFloat, turbulence, ProtoWindPhysicsModel);
};


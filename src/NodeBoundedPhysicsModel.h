/*
 * NodeBoundedPhysicsModel.h
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

#include "ExternTheApp.h"
#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class ProtoBoundedPhysicsModel : public Proto {
public:
                    ProtoBoundedPhysicsModel(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_BOUNDED_PHYSICS_MODEL; }
    virtual int     getNodeClass() const { return PARTICLE_PHYSICS_MODEL_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    isDeclaredInRwd_h() { return true; }

    FieldIndex enabled;
    FieldIndex geometry;
};

class NodeBoundedPhysicsModel : public Node {
public:
                    NodeBoundedPhysicsModel(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
                           { return "ParticleSystems"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int         getX3dVersion(void) const { return 2; }
    virtual Node       *copy() { return new NodeBoundedPhysicsModel(
                                        m_scene, m_proto); }

    fieldMacros(SFBool, enabled,  ProtoBoundedPhysicsModel);
    fieldMacros(SFNode, geometry, ProtoBoundedPhysicsModel);
};


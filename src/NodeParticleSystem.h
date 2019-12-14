/*
 * NodeParticleSystem.h
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

#ifndef _NODE_PARTICLE_SYSTEM_H
#define _NODE_PARTICLE_SYSTEM_H

#ifndef _NODE_H
#include "ExternTheApp.h"
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

enum {
    PARTICLES_LINE,
    PARTICLES_POINT,
    PARTICLES_QUAD,
    PARTICLES_SPRITE,
    PARTICLES_TRIANGLE,
    PARTICLES_GEOMETRY,
};

class ProtoParticleSystem : public Proto {
public:
                    ProtoParticleSystem(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_PARTICLE_SYSTEM; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual bool    isDeclaredInRwd_h() { return true; }

    FieldIndex appearance;
    FieldIndex createParticles;
    FieldIndex geometry;
    FieldIndex enabled;
    FieldIndex lifetimeVariation;
    FieldIndex maxParticles;
    FieldIndex particleLifetime;
    FieldIndex particleSize;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex colorRamp;
    FieldIndex colorKey;
    FieldIndex emitter;
    FieldIndex geometryType;
    FieldIndex physics;
    FieldIndex texCoordRamp;
    FieldIndex texCoordKey;
};

class NodeParticleSystem : public Node {
public:
                    NodeParticleSystem(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
                           { return "ParticleSystems"; }
    virtual int         getComponentLevel(void) const;
    virtual int     getX3dVersion(void) const { return 2; } 
    virtual Node   *copy() { return new NodeParticleSystem(m_scene, m_proto); }

    void            init(void);
    virtual void    draw(int pass);
    void            startParticle(int i);
    virtual void    setField(int field, FieldValue *value, int cf = -1)
                        {
                        m_particlesDirty = true;
                        Node::setField(field, value, cf);
                        }

    fieldMacros(SFNode,   appearance,        ProtoParticleSystem);
    fieldMacros(SFBool,   createParticles,   ProtoParticleSystem);
    fieldMacros(SFNode,   geometry,          ProtoParticleSystem);
    fieldMacros(SFBool,   enabled,           ProtoParticleSystem);
    fieldMacros(SFFloat,  lifetimeVariation, ProtoParticleSystem);
    fieldMacros(SFInt32,  maxParticles,      ProtoParticleSystem);
    fieldMacros(SFFloat,  particleLifetime,  ProtoParticleSystem);
    fieldMacros(SFVec2f,  particleSize,      ProtoParticleSystem);
    fieldMacros(SFVec3f,  bboxCenter,        ProtoParticleSystem);
    fieldMacros(SFVec3f,  bboxSize,          ProtoParticleSystem);
    fieldMacros(SFNode,   colorRamp,         ProtoParticleSystem);
    fieldMacros(MFFloat,  colorKey,          ProtoParticleSystem);
    fieldMacros(SFNode,   emitter,           ProtoParticleSystem);
    fieldMacros(SFString, geometryType,      ProtoParticleSystem);
    fieldMacros(MFNode,   physics,           ProtoParticleSystem);
    fieldMacros(SFNode,   texCoordRamp,      ProtoParticleSystem);
    fieldMacros(MFFloat,  texCoordKey,       ProtoParticleSystem);

protected:
    float m_force[3];
    MyArray<Vec3f>  m_internPosition; 
    MyArray<Vec3f>  m_internVector; 
    MyArray<double> m_lifeTime;
    MyArray<double> m_startTime; 
    bool            m_particlesDirty;
    double          m_internTime;
    float           m_mass;
    int             m_geometryType;
    Node           *m_particleNode;
};

#endif

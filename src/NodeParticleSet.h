/*
 * NodeParticleSet.h
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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
#include "DuneApp.h"
#include "GeometryNode.h"
#include "SFMFTypes.h"

class ProtoParticleSet : public Proto {
public:
                    ProtoParticleSet(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_PARTICLE_SET; }

    virtual bool    isX3domProto(void) { return true; }

    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
    FieldIndex color;
    FieldIndex coord;
    FieldIndex drawOrder;
    FieldIndex index;
    FieldIndex mode;
    FieldIndex normal;
    FieldIndex size;
    FieldIndex solid;
    FieldIndex textureZ;
};

class NodeParticleSet : public GeometryNode {
public:
                    NodeParticleSet(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeParticleSet(*this); }

    virtual int     getX3dVersion(void) const { return -1; }

    virtual bool    showFields() { return true; }

    x3domGeometryCommonFieldMacros(ProtoParticleSet) 
    fieldMacros(SFBool,   ccw,       ProtoParticleSet)
    fieldMacros(SFNode,   color,     ProtoParticleSet)
    fieldMacros(SFString, drawOrder, ProtoParticleSet)
    fieldMacros(SFNode,   coord,     ProtoParticleSet)
    fieldMacros(MFInt32,  index,     ProtoParticleSet)
    fieldMacros(SFString, mode,      ProtoParticleSet)
    fieldMacros(SFNode,   normal,    ProtoParticleSet)
    fieldMacros(MFVec3f,  size,      ProtoParticleSet)
    fieldMacros(SFBool,   solid,     ProtoParticleSet)
    fieldMacros(MFFloat,  textureZ,  ProtoParticleSet)
};


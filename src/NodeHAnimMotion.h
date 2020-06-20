/*
 * NodeHAnimMotion.h
 *
 * Copyright (C) 1999 Stephen F. White, 2020 J. "MUFTI" Scheurich
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
#include "Matrix.h"
#include "MyMesh.h"
#include "swt.h"
#include "SFMFTypes.h"
#include "TransformNode.h"

class NodeMaterial;

class ProtoHAnimMotion : public Proto {
public:
                    ProtoHAnimMotion(Scene *scene);

    void            addElements(void);

    virtual int     getType() const { return X3D_HANIM_MOTION; }

    virtual Node   *create(Scene *scene);

//    virtual bool    isDeclaredInRwd_h() { return true; }      

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex      channels;
    FieldIndex      channelsEnabled;
    FieldIndex      description;
    FieldIndex      enabled;
    FieldIndex      frameDuration;
    FieldIndex      frameIncrement;
    FieldIndex      frameIndex;
    FieldIndex      joints;
    FieldIndex      loa;
    FieldIndex      loop;
    FieldIndex      values;    
};

class NodeHAnimMotion : public Node {
public:
                      NodeHAnimMotion(Scene *scene, Proto *proto);

protected:
                     ~NodeHAnimMotion();

public:
    virtual const char* getComponentName(void) const;
    virtual int       getComponentLevel(void) const;
    virtual int       getX3dVersion(void) const { return 4; }
    virtual Node     *copy() const { return new NodeHAnimMotion(*this); }

    virtual bool      showFields() { return true; }

    fieldMacros(SFString,   channels,        ProtoHAnimMotion)
    fieldMacros(MFBool,     channelsEnabled, ProtoHAnimMotion)
    fieldMacros(SFString,   description,     ProtoHAnimMotion)
    fieldMacros(SFBool,     enabled,         ProtoHAnimMotion)
    fieldMacros(SFTime ,    frameDuration,   ProtoHAnimMotion)
    fieldMacros(SFInt32,    frameIncrement,  ProtoHAnimMotion)
    fieldMacros(SFInt32,    frameIndex,      ProtoHAnimMotion)
    fieldMacros(MFString,   joints,          ProtoHAnimMotion)
    fieldMacros(SFInt32,    loa,             ProtoHAnimMotion)
    fieldMacros(SFBool,     loop,            ProtoHAnimMotion)
    fieldMacros(SFFloat,    values,          ProtoHAnimMotion)
};


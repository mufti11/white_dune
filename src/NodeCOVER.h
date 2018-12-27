/*
 * NodeCOVER.h
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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
 * 
 * Implements the Cover/Covise COVER node
 */

#ifndef _NODE_COVER_H
#define _NODE_COVER_H

#ifndef _COVER_NODE_H
#include "CoverNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoCOVER : public Proto {
public:
                    ProtoCOVER(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return COVER_COVER; }

    FieldIndex position1;
    FieldIndex position2;
    FieldIndex position3;
    FieldIndex position4;
    FieldIndex position5;
    FieldIndex position6;
    FieldIndex position7;
    FieldIndex position8;
    FieldIndex orientation1;
    FieldIndex orientation2;
    FieldIndex orientation3;
    FieldIndex orientation4;
    FieldIndex orientation5;
    FieldIndex orientation6;
    FieldIndex orientation7;
    FieldIndex orientation8;
    FieldIndex soundEnvironment;

    virtual bool          isCoverProto(void) { return true; }
};

class NodeCOVER : public CoverNode {
public:
                    NodeCOVER(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeCOVER(*this); }

    fieldMacros(SFVec3f,    position1,        ProtoCOVER)
    fieldMacros(SFVec3f,    position2,        ProtoCOVER)
    fieldMacros(SFVec3f,    position3,        ProtoCOVER)
    fieldMacros(SFVec3f,    position4,        ProtoCOVER)
    fieldMacros(SFVec3f,    position5,        ProtoCOVER)
    fieldMacros(SFVec3f,    position6,        ProtoCOVER)
    fieldMacros(SFVec3f,    position7,        ProtoCOVER)
    fieldMacros(SFVec3f,    position8,        ProtoCOVER)
    fieldMacros(SFRotation, orientation1,     ProtoCOVER)
    fieldMacros(SFRotation, orientation2,     ProtoCOVER)
    fieldMacros(SFRotation, orientation3,     ProtoCOVER)
    fieldMacros(SFRotation, orientation4,     ProtoCOVER)
    fieldMacros(SFRotation, orientation5,     ProtoCOVER)
    fieldMacros(SFRotation, orientation6,     ProtoCOVER)
    fieldMacros(SFRotation, orientation7,     ProtoCOVER)
    fieldMacros(SFRotation, orientation8,     ProtoCOVER)
    fieldMacros(SFInt32,    soundEnvironment, ProtoCOVER)
};

#endif // _NODE_COVER_H

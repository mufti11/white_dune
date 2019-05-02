/*
 * NodeVirtualSoundSource.h
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

// implements the VirtualSoundSource covise/cover plugin

#ifndef _NODE_VIRTUAL_SOUND_SOURCE_H
#define _NODE_VIRTUAL_SOUND_SOURCE_H

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

class ProtoVirtualSoundSource : public Proto {
public:
                    ProtoVirtualSoundSource(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return COVER_VIRTUAL_SOUND_SOURCE; }

    FieldIndex mute;
    FieldIndex gain;
    FieldIndex position;
    FieldIndex orientation;
    FieldIndex soundMode;
    FieldIndex soundType;
    FieldIndex source;

    virtual bool          isCoverProto(void) { return true; }
};

class NodeVirtualSoundSource : public CoverNode {
public:
                    NodeVirtualSoundSource(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeVirtualSoundSource(*this); }
    virtual int     getX3dVersion(void) const { return -1; }    

    virtual void    drawHandles(void);
    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    fieldMacros(SFBool,     mute,        ProtoVirtualSoundSource)
    fieldMacros(SFFloat,    gain,        ProtoVirtualSoundSource)
    fieldMacros(SFVec3f,    position,    ProtoVirtualSoundSource)
    fieldMacros(SFRotation, orientation, ProtoVirtualSoundSource)
    fieldMacros(SFString,   soundMode,   ProtoVirtualSoundSource)
    fieldMacros(SFString,   soundType,   ProtoVirtualSoundSource)
    fieldMacros(SFString,   source,      ProtoVirtualSoundSource)
};

#endif

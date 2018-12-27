/*
 * NodeSound.h
 *
 * Copyright (C) 1999 Stephen F. White
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

#ifndef _NODE_SOUND_H
#define _NODE_SOUND_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "swt.h"

#include "SFMFTypes.h"

class ProtoSound : public Proto {
public:
                    ProtoSound(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_SOUND; }

    FieldIndex direction;
    FieldIndex intensity;
    FieldIndex location;
    FieldIndex maxBack;
    FieldIndex maxFront;
    FieldIndex minBack;
    FieldIndex minFront;
    FieldIndex priority;
    FieldIndex source;
    FieldIndex spatialize;
    FieldIndex doppler;
};

class NodeSound : public Node {
public:
                    NodeSound(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; } 
    virtual Node   *copy() const { return new NodeSound(*this); }

    virtual bool    hasNumbers4kids(void) { return true; }

    virtual int     getAnimationCommentID(void);

    virtual bool    hasCoverFields(void) { return true; }   

    fieldMacros(SFVec3f, direction,  ProtoSound)
    fieldMacros(SFFloat, intensity,  ProtoSound)
    fieldMacros(SFVec3f, location,   ProtoSound)
    fieldMacros(SFFloat, maxBack,    ProtoSound)
    fieldMacros(SFFloat, maxFront,   ProtoSound)
    fieldMacros(SFFloat, minBack,    ProtoSound)
    fieldMacros(SFFloat, minFront,   ProtoSound)
    fieldMacros(SFFloat, priority,   ProtoSound)
    fieldMacros(SFNode,  source,     ProtoSound)
    fieldMacros(SFBool,  spatialize, ProtoSound)
    fieldMacros(SFBool,  doppler,    ProtoSound)
};

#endif // _NODE_SOUND_H

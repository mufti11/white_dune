/*
 * NodeAudioClip.h
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

#pragma once

#include "ExternTheApp.h"
#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"

class ProtoAudioClip : public Proto {
public:
                    ProtoAudioClip(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_AUDIO_CLIP; }
    virtual int     getNodeClass() const 
                       { return AUDIO_CLIP_NODE | URL_NODE; }

    FieldIndex description;
    FieldIndex loop;
    FieldIndex pauseTime;
    FieldIndex pitch;
    FieldIndex resumeTime; 
    FieldIndex startTime;
    FieldIndex stopTime;
    FieldIndex url;
    FieldIndex enabled;
};

class NodeAudioClip : public Node {
public:
                    NodeAudioClip(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual Node   *copy() { return new NodeAudioClip(m_scene, m_proto); }
    virtual int     getX3dVersion(void) const { return 0; }

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual bool    isInvalidChildNode(void) { return true; }
    virtual void    load();

    fieldMacros(SFString, description, ProtoAudioClip)
    fieldMacros(SFBool,   loop,        ProtoAudioClip)
    fieldMacros(SFTime,   pauseTime,   ProtoAudioClip)
    fieldMacros(SFFloat,  pitch,       ProtoAudioClip)
    fieldMacros(SFTime,   resumeTime,  ProtoAudioClip)
    fieldMacros(SFTime,   startTime,   ProtoAudioClip)
    fieldMacros(SFTime,   stopTime,    ProtoAudioClip)
    fieldMacros(MFString, url,         ProtoAudioClip)
    fieldMacros(SFBool,   enabled,     ProtoAudioClip)
};

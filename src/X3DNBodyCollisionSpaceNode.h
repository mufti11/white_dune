/*
 * X3DNBodyCollisionSpaceNode.h
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

#pragma once

#include "RigidBodyPhysicsNode.h"
#include "SFMFTypes.h"

class X3DNBodyCollisionSpaceProto : public Proto {
public:
               X3DNBodyCollisionSpaceProto(Scene *scene, const char *name);

    virtual int     getNodeClass() const 
                       { return CHILD_NODE | BODY_COLLISION_SPACE_NODE; }

    FieldIndex enabled;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
};

class X3DNBodyCollisionSpaceNode : public RigidBodyPhysicsNode {
public:
                    X3DNBodyCollisionSpaceNode(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
                           { return "RigidBodyPhysics"; }
    virtual int         getComponentLevel(void) const { return 1; }

    fieldMacros(SFBool,     enabled,     X3DNBodyCollisionSpaceProto)
    fieldMacros(SFVec3f,    bboxCenter,  X3DNBodyCollisionSpaceProto)
    fieldMacros(SFVec3f,    bboxSize,    X3DNBodyCollisionSpaceProto)
};



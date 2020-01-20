/*
 * X3DNBodyCollidableNode.h
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

class X3DNBodyCollidableProto : public Proto {
public:
               X3DNBodyCollidableProto(Scene *scene, const char *name);

    virtual int     getNodeClass() const 
                       { return CHILD_NODE | BODY_COLLIDABLE_NODE; }

    FieldIndex enabled;
    FieldIndex rotation;
    FieldIndex translation;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
};

class X3DNBodyCollidableNode : public RigidBodyPhysicsNode {
public:
                    X3DNBodyCollidableNode(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
                           { return "RigidBodyPhysics"; }
    virtual int         getComponentLevel(void) const { return 1; }

    virtual int         getNodeClass() const { return CHILD_NODE |
                                                      BOUNDED_OBJECT_NODE; }

    virtual void      setField(int index, FieldValue *value, int cf = -1);
    virtual void      drawHandles(void);
    virtual void      transform();
    virtual void      transformForHandle(int handle);

    virtual Vec3f     getHandle(int handle, int *constraint, int *field);
    virtual void      setHandle(int handle, const Vec3f &v);

    virtual void      receiveEvent(int eventIn, double timestamp, FieldValue *value);

    virtual void      update() { m_matrixDirty = true; }

    fieldMacros(SFBool,     enabled,     X3DNBodyCollidableProto)
    fieldMacros(SFRotation, rotation,    X3DNBodyCollidableProto)
    fieldMacros(SFVec3f,    translation, X3DNBodyCollidableProto)
    fieldMacros(SFVec3f,    bboxCenter,  X3DNBodyCollidableProto)
    fieldMacros(SFVec3f,    bboxSize,    X3DNBodyCollidableProto)

    bool              modelViewIsIdentity(void);
private:
    bool              m_matrixDirty;
    Matrix            m_matrix;
    float             m_handleScale;
};



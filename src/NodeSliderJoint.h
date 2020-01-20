/*
 * NodeSliderJoint.h
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

#include "X3DRigidJointNode.h"
#include "SFMFTypes.h"

class ProtoSliderJoint : public X3DRigidJointProto {
public:
                    ProtoSliderJoint(Scene *scene);
                    ProtoSliderJoint(Scene *scene, const char *name);

    void            addElements();
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_SLIDER_JOINT; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex axis;
    FieldIndex maxSeparation;
    FieldIndex minSeparation;
    FieldIndex stopBounce;
    FieldIndex stopErrorCorrection;
};

class NodeSliderJoint : public X3DRigidJointNode {
public:
                    NodeSliderJoint(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 2; } 
    virtual Node   *copy() const { return new NodeSliderJoint(*this); }

    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    void            drawHandles(void);

    fieldMacros(SFVec3f, axis,                      ProtoSliderJoint)
    fieldMacros(SFFloat, maxSeparation,             ProtoSliderJoint)
    fieldMacros(SFFloat, minSeparation,             ProtoSliderJoint)
    fieldMacros(SFFloat, stopBounce,                ProtoSliderJoint)
    fieldMacros(SFFloat, stopErrorCorrection,       ProtoSliderJoint)
};


/*
 * AnchoredJointNode.h
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

#ifndef _ANCHORED_JOINT_NODE_H
#define _ANCHORED_JOINT_NODE_H

#include "SFMFTypes.h"

#include "X3DRigidJointNode.h"

class AnchoredJointProto : public X3DRigidJointProto {
public:
               AnchoredJointProto(Scene *scene, const char *name);
    FieldIndex anchorPoint;
};

class AnchoredJointNode : public X3DRigidJointNode {
public:
                    AnchoredJointNode(Scene *scene, Proto *proto);

    virtual void    transformForHandle(int handle);
    virtual Vec3f   getHandle(int handle, int *constraint, int *field);
    virtual void    setHandle(int handle, const Vec3f &v);

    void            drawHandles(bool hasAxes);
    virtual void    drawHandles(void) { drawHandles(false); }

    fieldMacros(SFVec3f, anchorPoint, AnchoredJointProto)

protected:
    float           m_handleScale;
};

#endif 

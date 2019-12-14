/*
 * NodeBallJoint.h
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

#include "AnchoredJointNode.h"

#include "SFMFTypes.h"

class ProtoBallJoint : public AnchoredJointProto {
public:
                    ProtoBallJoint(Scene *scene);

    virtual int     getType() const { return X3D_BALL_JOINT; }

    virtual bool    isX3dInternalProto(void) { return true; }

    virtual Node   *create(Scene *scene);
};

class NodeBallJoint : public AnchoredJointNode {
public:
                    NodeBallJoint(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 2; }
    virtual Node   *copy() { return new NodeBallJoint(m_scene, m_proto); }

protected:
    float           m_handleScale;

};


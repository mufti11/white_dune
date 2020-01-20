/*
 * RigidBodyPhysicsNode.h
 *
 * 2007 J. "MUFTI" Scheurich
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
#include "SFMFTypes.h"

class RigidBodyPhysicsNode : public Node {
public:
                    RigidBodyPhysicsNode(Scene *scene, Proto *def) : 
                          Node(scene, def) {}

    virtual int     writeProto(int filedes) 
                       {
                       return ((Node *)this)->writeProto(filedes, 
                                    "urn:web3d:x3d:node:",
                                    "rigidBodyPhysicsNodes"
#ifdef HAVE_RIGID_BODY_PHYSICS_NODES_PROTO_URL
                                    , HAVE_RIGID_BODY_PHYSICS_NODES_PROTO_URL
#endif
                                                              );
                       }

    virtual bool    isRigidBodyPhysicsNode(void) { return true; }
};



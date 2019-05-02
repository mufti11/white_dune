/*
 * NodeCollisionSpace.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2003 J. "MUFTI" Scheurich
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

#include <stdio.h>
#ifndef FLT_MAX
# include <float.h>
#endif
#include "stdafx.h"

#include "NodeCollisionSpace.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFBool.h"
#include "DuneApp.h"
#include "Scene.h"

ProtoCollisionSpace::ProtoCollisionSpace(Scene *scene)
  : X3DNBodyCollisionSpaceProto(scene, "CollisionSpace")
{
    collidables.set(addExposedField(MFNODE, "collidables", new MFNode(),
          BODY_COLLIDABLE_OR_BODY_COLLISION_SPACE_NODE));

    useGeometry.set(addExposedField(SFBOOL, "useGeometry", new SFBool(FALSE)));
}

Node *
ProtoCollisionSpace::create(Scene *scene)
{ 
    return new NodeCollisionSpace(scene, this); 
}

NodeCollisionSpace::NodeCollisionSpace(Scene *scene, Proto *def)
  : X3DNBodyCollisionSpaceNode(scene, def)
{
}

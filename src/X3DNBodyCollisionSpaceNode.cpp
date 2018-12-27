/*
 * X3DNBodyCollisionSpaceNode.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "X3DNBodyCollisionSpaceNode.h"
#include "Proto.h"
#include "ExposedField.h"
#include "Field.h"
#include "DuneApp.h"
#include "Scene.h"

X3DNBodyCollisionSpaceProto::X3DNBodyCollisionSpaceProto(Scene *scene, 
                                                         const char *name)
  : Proto(scene, name)
{
    enabled.set(addExposedField(SFBOOL, "enabled", new SFBool(true)));
    bboxCenter.set(addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1), 
                          new SFFloat(-1.0f)));

}

X3DNBodyCollisionSpaceNode::X3DNBodyCollisionSpaceNode(Scene *scene, Proto *def)
  : RigidBodyPhysicsNode(scene, def)
{
}


/*
 * NodeCollisionSensor.cpp
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

#include "NodeCollisionSensor.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoCollisionSensor::ProtoCollisionSensor(Scene *scene)
  : Proto(scene, "CollisionSensor")
{

    collider.set(addExposedField(SFNODE, "collider", new SFNode(NULL),
                                 X3D_COLLISION_COLLECTION));

    enabled.set(addExposedField(SFBOOL, "enabled", new SFBool(true)));

    addEventOut(MFNODE, "intersections");
    addEventOut(MFNODE, "contacts", EOF_RECOMMENDED);
    addEventOut(SFBOOL, "isActive");
}

Node *
ProtoCollisionSensor::create(Scene *scene)
{ 
    return new NodeCollisionSensor(scene, this); 
}

NodeCollisionSensor::NodeCollisionSensor(Scene *scene, Proto *def)
  : RigidBodyPhysicsNode(scene, def)
{
}


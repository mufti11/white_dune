/*
 * NodeGravityPhysicsModel.cpp
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#include "NodeGravityPhysicsModel.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoGravityPhysicsModel::ProtoGravityPhysicsModel(Scene *scene)
  : Proto(scene, "GravityPhysicsModel")
{
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    force.set(
        addExposedField(SFVEC3F, "force", new SFVec3f(0, -9.8, 0)));
}

Node *
ProtoGravityPhysicsModel::create(Scene *scene)
{ 
    return new NodeGravityPhysicsModel(scene, this); 
}

NodeGravityPhysicsModel::NodeGravityPhysicsModel(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

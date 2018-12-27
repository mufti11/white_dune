/*
 * NodeWindPhysicsModel.cpp
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

#include "NodeWindPhysicsModel.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "DuneApp.h"

ProtoWindPhysicsModel::ProtoWindPhysicsModel(Scene *scene)
  : Proto(scene, "WindPhysicsModel")
{
    direction.set(
        addExposedField(SFVEC3F, "direction", new SFVec3f(0, 0, 0)));
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    gustiness.set(
        addExposedField(SFFLOAT, "gustiness", new SFFloat(0.1)));
    speed.set(
        addExposedField(SFFLOAT, "speed", new SFFloat(0.1)));
    turbulence.set(
        addExposedField(SFFLOAT, "turbulence", new SFFloat(0)));
}

Node *
ProtoWindPhysicsModel::create(Scene *scene)
{ 
    return new NodeWindPhysicsModel(scene, this); 
}

NodeWindPhysicsModel::NodeWindPhysicsModel(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

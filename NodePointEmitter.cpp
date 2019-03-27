/*
 * NodePointEmitter.cpp
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

#include "NodePointEmitter.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "DuneApp.h"

ProtoPointEmitter::ProtoPointEmitter(Scene *scene)
  : EmitterProto(scene, "PointEmitter")
{
    direction.set(
        addExposedField(SFVEC3F, "direction", new SFVec3f(0, 1, 0)));
    position.set(
        addExposedField(SFVEC3F, "position", new SFVec3f(0, 0, 0)));
}

Node *
ProtoPointEmitter::create(Scene *scene)
{ 
    return new NodePointEmitter(scene, this); 
}

NodePointEmitter::NodePointEmitter(Scene *scene, Proto *def)
  : EmitterNode(scene, def)
{
}

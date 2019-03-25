/*
 * NodeConeEmitter.cpp
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

#include "NodeConeEmitter.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFVec3f.h"
#include "Scene.h"

ProtoConeEmitter::ProtoConeEmitter(Scene *scene)
  : EmitterProto(scene, "ConeEmitter")
{
    double unitAngle = scene->getUnitAngle();

    angle.set(
        addExposedField(SFFLOAT, "angle", 
                        new SFFloat(M_PI / 4.0f / unitAngle),
                        new SFFloat(0), new SFFloat(M_PI / unitAngle)));
    direction.set(
        addExposedField(SFVEC3F, "direction", new SFVec3f(0, 1, 0)));
    position.set(
        addExposedField(SFVEC3F, "position", new SFVec3f(0, 0, 0)));
}

Node *
ProtoConeEmitter::create(Scene *scene)
{ 
    return new NodeConeEmitter(scene, this); 
}

NodeConeEmitter::NodeConeEmitter(Scene *scene, Proto *def)
  : EmitterNode(scene, def)
{
}

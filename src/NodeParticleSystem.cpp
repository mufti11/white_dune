/*
 * NodeParticleSystem.cpp
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

#include "NodeParticleSystem.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFNode.h"
#include "SFBool.h"
#include "SFNode.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFInt32.h"
#include "SFFloat.h"
#include "SFVec2f.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "SFNode.h"
#include "MFFloat.h"
#include "SFNode.h"
#include "SFString.h"
#include "MFNode.h"
#include "SFNode.h"
#include "MFFloat.h"
#include "DuneApp.h"

ProtoParticleSystem::ProtoParticleSystem(Scene *scene)
  : Proto(scene, "ParticleSystem")
{
    appearance.set(
        addExposedField(SFNODE, "appearance", new SFNode(), VRML_APPEARANCE));
    createParticles.set(
        addExposedField(SFBOOL, "createParticles", new SFBool(true)));
    geometry.set(
        addExposedField(SFNODE, "geometry", new SFNode(), GEOMETRY_NODE));
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    lifetimeVariation.set(
        addExposedField(SFFLOAT, "lifetimeVariation", new SFFloat(0.25)));
    maxParticles.set(
        addExposedField(SFINT32, "maxParticles", new SFInt32(200)));
    particleLifetime.set(
        addExposedField(SFFLOAT, "particleLifetime", new SFFloat(5)));
    particleSize.set(
        addExposedField(SFVEC2F, "particleSize", new SFVec2f(0.02, 0.02)));
    bboxCenter.set(
        addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(
        addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1)));
    colorRamp.set(
        addField(SFNODE, "colorRamp", new SFNode(), COLOR_NODE));
    colorKey.set(
        addField(MFFLOAT, "colorKey", new MFFloat()));
    emitter.set(
        addField(SFNODE, "emitter", new SFNode(), PARTICLE_EMITTER_NODE));
    geometryType.set(
        addField(SFSTRING, "geometryType", new SFString("QUAD")));
    physics.set(
        addField(MFNODE, "physics", new MFNode(), PARTICLE_PHYSICS_MODEL_NODE));
    texCoordRamp.set(
        addField(SFNODE, "texCoordRamp", new SFNode(), VRML_TEXTURE_COORDINATE));
    texCoordKey.set(
        addField(MFFLOAT, "texCoordKey", new MFFloat()));
}

Node *
ProtoParticleSystem::create(Scene *scene)
{ 
    return new NodeParticleSystem(scene, this); 
}

NodeParticleSystem::NodeParticleSystem(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

int
NodeParticleSystem::getComponentLevel(void) const 
{ 
     const char *type = geometryType()->getValue();
     if ((geometry()->getValue() != NULL) ||
         ((type != NULL) && (strcmp(type, "GEOMETRY") != 0)))
          return 4;
     return 3; 
}
 

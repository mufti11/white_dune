/*
 * NodeCollisionCollection.cpp
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

#include "NodeCollisionCollection.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoCollisionCollection::ProtoCollisionCollection(Scene *scene)
  : Proto(scene, "CollisionCollection")
{
    appliedParameters.set(
          addExposedField(MFSTRING, "appliedParameters", 
                          new MFString("BOUNCE")));

    bounce.set(addExposedField(SFFLOAT, "bounce", new SFFloat(0), 
                               new SFFloat(0), new SFFloat(1)));

    collidables.set(addExposedField(MFNODE, "collidables", new MFNode(),
          BODY_COLLIDABLE_OR_BODY_COLLISION_SPACE_NODE));

    enabled.set(addExposedField(SFBOOL, "enabled", new SFBool(true)));

    frictionCoefficients.set(addExposedField(SFVEC2F, "frictionCoefficients", 
                                             new SFVec2f(0.0f, 0.0f)));

    minBounceSpeed.set(addExposedField(SFFLOAT, "minBounceSpeed", 
                                       new SFFloat(0.1), new SFFloat(0))); 

    slipFactors.set(addExposedField(SFVEC2F, "slipFactors", 
                                    new SFVec2f(0.0f, 0.0f)));

    softnessConstantForceMix.set(addExposedField(SFFLOAT, 
                                                 "softnessConstantForceMix", 
                                                 new SFFloat(0.001), 
                                                 new SFFloat(0), 
                                                 new SFFloat(1)));

    softnessErrorCorrection.set(addExposedField(SFFLOAT, 
                                                "softnessErrorCorrection", 
                                                new SFFloat(0.8), 
                                                new SFFloat(0), 
                                                new SFFloat(1)));

    surfaceSpeed.set(addExposedField(SFVEC2F, "surfaceSpeed", 
                                     new SFVec2f(0.0f, 0.0f)));
}

Node *
ProtoCollisionCollection::create(Scene *scene)
{ 
    return new NodeCollisionCollection(scene, this); 
}

NodeCollisionCollection::NodeCollisionCollection(Scene *scene, Proto *def)
  : RigidBodyPhysicsNode(scene, def)
{
}


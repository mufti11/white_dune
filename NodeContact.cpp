/*
 * NodeContact.cpp
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

#include "NodeContact.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoContact::ProtoContact(Scene *scene)
  : Proto(scene, "Contact")
{
    appliedParameters.set(
          addExposedField(MFSTRING, "appliedParameters", 
                          new MFString("BOUNCE")));

    body1.set(addExposedField(SFNODE, "body1", new SFNode(NULL), 
                              X3D_RIGID_BODY));

    body2.set(addExposedField(SFNODE, "body2", new SFNode(NULL), 
                              X3D_RIGID_BODY));

    bounce.set(addExposedField(SFFLOAT, "bounce", new SFFloat(0), 
                               new SFFloat(0), new SFFloat(1)));

    contactNormal.set(addExposedField(SFVEC3F, "contactNormal", 
                                      new SFVec3f(0.0f, 1.0f, 0.0f)));

    depth.set(addExposedField(SFFLOAT, "depth", new SFFloat(0))); 

    frictionCoefficients.set(addExposedField(SFVEC2F, "frictionCoefficients", 
                                             new SFVec2f(0.0f, 0.0f)));

    frictionDirection.set(addExposedField(SFVEC3F, "frictionDirection", 
                                          new SFVec3f(0.0f, 1.0f, 0.0f)));

    geometry1.set(addExposedField(SFNODE, "geometry1", new SFNode(NULL), 
                                  BODY_COLLIDABLE_NODE));

    geometry2.set(addExposedField(SFNODE, "geometry2", new SFNode(NULL), 
                                  BODY_COLLIDABLE_NODE));

    minBounceSpeed.set(addExposedField(SFFLOAT, "minBounceSpeed", 
                                       new SFFloat(0.1), new SFFloat(0))); 

    position.set(addExposedField(SFVEC3F, "position", new SFVec3f()));

    slipCoefficients.set(addExposedField(SFVEC2F, "slipCoefficients", 
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
ProtoContact::create(Scene *scene)
{ 
    return new NodeContact(scene, this); 
}

NodeContact::NodeContact(Scene *scene, Proto *def)
  : RigidBodyPhysicsNode(scene, def)
{
}


/*
 * NodeRigidBodyCollection.cpp
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

#include "NodeRigidBodyCollection.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoRigidBodyCollection::ProtoRigidBodyCollection(Scene *scene)
  : Proto(scene, "RigidBodyCollection")
{
    autoDisable.set(addExposedField(SFBOOL, "autoDisable", new SFBool(false)));
    bodies.set(addExposedField(MFNODE, "bodies", new MFNode(),
                               X3D_RIGID_BODY));
    constantForceMix.set(addExposedField(SFFLOAT, "constantForceMix", 
                                         new SFFloat(0.0001), new SFFloat(0))); 
    contactSurfaceThickness.set(addExposedField(SFFLOAT, 
                                                "contactSurfaceThickness", 
                                                new SFFloat(0), 
                                                new SFFloat(0))); 
    disableAngularSpeed.set(addExposedField(SFFLOAT, "disableAngularSpeed", 
                                            new SFFloat(0), new SFFloat(0))); 
    disableLinearSpeed.set(addExposedField(SFFLOAT, "disableLinearSpeed", 
                                           new SFFloat(0), new SFFloat(0))); 
    disableTime.set(addExposedField(SFFLOAT, "disableTime", 
                                    new SFFloat(0), new SFFloat(0))); 
    enabled.set(addExposedField(SFBOOL, "enabled", new SFBool(true)));
    errorCorrection.set(addExposedField(SFFLOAT, "errorCorrection", 
                                        new SFFloat(0.8), 
                                        new SFFloat(0), new SFFloat(1)));

    gravity.set(addExposedField(SFVEC3F, "gravity", new SFVec3f(0, -9.8, 0)));
    iterations.set(addExposedField(SFINT32, "iterations", new SFInt32(10)));
    joints.set(addExposedField(MFNODE, "joints", new MFNode(),
                               RIGID_JOINT_NODE));
    maxCorrectionSpeed.set(addExposedField(SFFLOAT, "maxCorrectionSpeed", 
                                    new SFFloat(0), new SFFloat(-1))); 
    preferAccuracy.set(addField(SFBOOL, "preferAccuracy", 
                                new SFBool(false)));
    collider.set(addField(SFNODE, "collider", new SFNode(),
                                 X3D_COLLISION_COLLECTION));

    addEventIn(MFNODE, "set_contacts", EIF_RECOMMENDED);
}

Node *
ProtoRigidBodyCollection::create(Scene *scene)
{ 
    return new NodeRigidBodyCollection(scene, this); 
}

NodeRigidBodyCollection::NodeRigidBodyCollection(Scene *scene, Proto *def)
  : RigidBodyPhysicsNode(scene, def)
{
}


/*
 * X3DRigidJointNode.cpp
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

#include "X3DRigidJointNode.h"
#include "Proto.h"
#include "ExposedField.h"
#include "Field.h"
#include "DuneApp.h"
#include "Scene.h"
#include "NodeRigidBody.h"

X3DRigidJointProto::X3DRigidJointProto(Scene *scene, const char *name)
  : Proto(scene, name)
{
    body1.set(addExposedField(SFNODE, "body1", new SFNode(NULL), 
                              X3D_RIGID_BODY));
    body2.set(addExposedField(SFNODE, "body2", new SFNode(NULL), 
                              X3D_RIGID_BODY));
    static const char *forceOutputs[] = { "ALL", "NONE", NULL};
    StringArray *defaultForceOutput = new StringArray();
    defaultForceOutput->append("NONE");
    forceOutput.set(
          addExposedField(MFSTRING, "forceOutput", 
                          new MFString(defaultForceOutput), 0, forceOutputs));
}

X3DRigidJointNode::X3DRigidJointNode(Scene *scene, Proto *def)
  : RigidBodyPhysicsNode(scene, def)
{
}


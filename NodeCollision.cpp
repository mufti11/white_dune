/*
 * NodeCollision.cpp
 *
 * Copyright (C) 1999 Stephen F. White
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

#include "NodeCollision.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFBool.h"
#include "SFVec3f.h"
#include "SFNode.h"
#include "SFFloat.h"
#include "Field.h"

ProtoCollision::ProtoCollision(Scene *scene)
  : WonderlandExportProto(scene, "Collision")
{
    addEventIn(MFNODE, "addChildren");
    addEventIn(MFNODE, "removeChildren");

    children.set(
          addExposedField(MFNODE, "children", new MFNode(), CHILD_NODE));

    collide.set(
          addExposedField(SFBOOL, "collide", new SFBool(true), NULL, NULL,
                          "enabled"));

    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0.0f, 0.0f, 0.0f)));

    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1.0f, -1.0f, -1.0f), 
                   new SFFloat(-1.0f)));

    proxy.set(
          addField(SFNODE, "proxy", new SFNode(NULL), CHILD_NODE));

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);

    addEventOut(SFTIME, "collideTime", EOF_RECOMMENDED);
    addEventOut(SFBOOL, "isActive", FF_X3D_ONLY);
}

Node *
ProtoCollision::create(Scene *scene)
{
    return new NodeCollision(scene, this); 
}

NodeCollision::NodeCollision(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeCollision::flip(int index)
{
    bboxCenter()->flip(index);
    children()->flip(index);
}

void
NodeCollision::swap(int fromTo)
{
    bboxCenter()->swap(fromTo);
    bboxSize()->swap(fromTo);
    children()->swap(fromTo);
}

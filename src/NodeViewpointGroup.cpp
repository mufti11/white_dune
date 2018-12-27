/*
 * NodeViewpointGroup.cpp
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

#include "NodeViewpointGroup.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "MFNode.h"
#include "SFString.h"
#include "SFBool.h"
#include "SFBool.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoViewpointGroup::ProtoViewpointGroup(Scene *scene)
  : Proto(scene, "ViewpointGroup")
{
    center.set(
        addExposedField(SFVEC3F, "center", new SFVec3f(0, 0, 0)));
    children.set(
        addExposedField(MFNODE, "children", new MFNode(), 
                        VIEWPOINT_OR_VIEWPOINT_GROUP_NODE));
    description.set(
        addExposedField(SFSTRING, "description", new SFString("")));
    displayed.set(
        addExposedField(SFBOOL, "displayed", new SFBool(true)));
    retainUserOffsets.set(
        addExposedField(SFBOOL, "retainUserOffsets", new SFBool(false)));
    size.set(
        addExposedField(SFVEC3F, "size", new SFVec3f(0, 0, 0)));
}

Node *
ProtoViewpointGroup::create(Scene *scene)
{ 
    return new NodeViewpointGroup(scene, this); 
}

NodeViewpointGroup::NodeViewpointGroup(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

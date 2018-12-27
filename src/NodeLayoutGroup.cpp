/*
 * NodeLayoutGroup.cpp
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

#include "NodeLayoutGroup.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFNode.h"
#include "SFNode.h"
#include "SFNode.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoLayoutGroup::ProtoLayoutGroup(Scene *scene)
  : Proto(scene, "LayoutGroup")
{
    addEventIn(MFNODE, "addChildren");
    addEventIn(MFNODE, "removeChildren");
    children.set(
        addExposedField(MFNODE, "children", new MFNode(), CHILD_NODE));
    layout.set(
        addExposedField(SFNODE, "layout", new SFNode(), X3D_LAYOUT));
    viewport.set(
        addExposedField(SFNODE, "viewport", new SFNode(), X3D_VIEWPORT));
    bboxCenter.set(
        addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(
        addField(SFVEC3F, "bboxSize", new SFVec3f(0, 0, 0)));
}

Node *
ProtoLayoutGroup::create(Scene *scene)
{ 
    return new NodeLayoutGroup(scene, this); 
}

NodeLayoutGroup::NodeLayoutGroup(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

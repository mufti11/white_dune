/*
 * GroupNode.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#include "stdafx.h"

#include "swt.h"
#include "GroupNode.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "NodeShape.h"
#include "Scene.h"

GroupProto::GroupProto(Scene *scene, const char *name, 
                       int extraChrildrenNodeClass)
  : WonderlandExportProto(scene, name)
{
     addElements(extraChrildrenNodeClass); 
}

void GroupProto::addElements(int extraChrildrenNodeClass) 
{
    children.set(
          addExposedField(MFNODE, "children", new MFNode(), CHILD_NODE |
                          extraChrildrenNodeClass));
}

GroupNode::GroupNode(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
GroupNode::flip(int index)
{
    children()->flip(index);
}

void
GroupNode::swap(int fromTo)
{
    children()->swap(fromTo);
}




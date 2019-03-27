/*
 * StaticGroupNode.cpp
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
#include "StaticGroupNode.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "NodeShape.h"
#include "Scene.h"

StaticGroupProto::StaticGroupProto(Scene *scene, const char *name, 
                                   int extraChrildrenNodeClass)
  : GroupProto(scene, name, extraChrildrenNodeClass)
{
     addElements(); 
}

StaticGroupProto::StaticGroupProto(Scene *scene)
  : GroupProto(scene, "StaticGroup")
{                    
     addElements();     
}

void StaticGroupProto::addElements(void) 
{
    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0.0f, 0.0f, 0.0f)));
    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1.0f, -1.0f, -1.0f), 
                   new SFFloat(-1.0f)));
}

Node *
StaticGroupProto::create(Scene *scene)
{ 
    return new StaticGroupNode(scene, this); 
}

StaticGroupNode::StaticGroupNode(Scene *scene, Proto *def)
  : GroupNode(scene, def)
{
}

const char* 
StaticGroupNode::getComponentName(void) const
{ 
    if (getType() == X3D_STATIC_GROUP)
        return "Grouping"; 
    return ""; 
}

int         
StaticGroupNode::getComponentLevel(void) const 
{ 
    if (getType() == X3D_STATIC_GROUP)
        return 3; 
    return -1;
}

void
StaticGroupNode::flip(int index)
{
    bboxCenter()->flip(index);
    children()->flip(index);
}

void
StaticGroupNode::swap(int fromTo)
{
    bboxCenter()->swap(fromTo);
    bboxSize()->swap(fromTo);
    children()->swap(fromTo);
}




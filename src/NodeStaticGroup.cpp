/*
 * NodeStaticGroup.cpp
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
#include "NodeStaticGroup.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "NodeShape.h"
#include "Scene.h"

ProtoStaticGroup::ProtoStaticGroup(Scene *scene, const char *name, 
                                   int extraChrildrenNodeClass)
  : GroupProto(scene, name, extraChrildrenNodeClass)
{
     addElements(); 
}

ProtoStaticGroup::ProtoStaticGroup(Scene *scene)
  : GroupProto(scene, "StaticGroup")
{                    
     addElements();     
}

void ProtoStaticGroup::addElements(void) 
{
    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0.0f, 0.0f, 0.0f)));
    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1.0f, -1.0f, -1.0f), 
                   new SFFloat(-1.0f)));

    bvhType.set(
          addExposedField(SFSTRING, "bvhType", new SFString("jsBIH")));
    setFieldFlags(bvhType, FF_X3DOM_ONLY);

    debug.set(
          addExposedField(SFBOOL, "debug", new SFBool(false)));
    setFieldFlags(debug, FF_X3DOM_ONLY);

    maxDepth.set(
          addExposedField(SFINT32, "maxDepth", new SFInt32(-1)));
    setFieldFlags(maxDepth, FF_X3DOM_ONLY);

    maxObjectsPerNode.set(
          addExposedField(SFINT32, "maxObjectsPerNode", new SFInt32(1)));
    setFieldFlags(maxObjectsPerNode, FF_X3DOM_ONLY);

    minRelativeBBoxSize.set(
          addExposedField(SFFLOAT, "minRelativeBBoxSize", new SFFloat(1)));
    setFieldFlags(minRelativeBBoxSize, FF_X3DOM_ONLY);

    render.set(
          addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);

    showDebugBoxVolumes.set(
          addExposedField(SFBOOL, "showDebugBoxVolumes", new SFBool(false)));
    setFieldFlags(showDebugBoxVolumes, FF_X3DOM_ONLY);
}

Node *
ProtoStaticGroup::create(Scene *scene)
{ 
    return new NodeStaticGroup(scene, this); 
}

NodeStaticGroup::NodeStaticGroup(Scene *scene, Proto *def)
  : GroupNode(scene, def)
{
}

const char* 
NodeStaticGroup::getComponentName(void) const
{ 
    if (getType() == X3D_STATIC_GROUP)
        return "Grouping"; 
    return ""; 
}

int         
NodeStaticGroup::getComponentLevel(void) const 
{ 
    if (getType() == X3D_STATIC_GROUP)
        return 3; 
    return -1;
}

void
NodeStaticGroup::flip(int index)
{
    bboxCenter()->flip(index);
    children()->flip(index);
}

void
NodeStaticGroup::swap(int fromTo)
{
    bboxCenter()->swap(fromTo);
    bboxSize()->swap(fromTo);
    children()->swap(fromTo);
}




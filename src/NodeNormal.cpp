/*
 * NodeNormal.cpp
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

#include "NodeNormal.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFVec3f.h"

#include "NodeIndexedFaceSet.h"

ProtoNormal::ProtoNormal(Scene *scene)
  : WonderlandExportProto(scene, "Normal")
{
    vector.set(
          addExposedField(MFVEC3F, "vector", new MFVec3f()));
}

Node *
ProtoNormal::create(Scene *scene)
{
    return new NodeNormal(scene, this); 
}

NodeNormal::NodeNormal(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeNormal::setField(int index, FieldValue *value, int cf)
{
    if (hasParent())
        getParent()->update();
    Node::setField(index, value, cf);
}

void
NodeNormal::drawHandles()
{
    if (hasParent())
        ((MeshBasedNode *)getParent())->drawNormals();
}

void
NodeNormal::flip(int index)
{
    MFVec3f *normals = vector();
    if (normals != NULL)
        normals->flip(index);
    if (hasParent())
        getParent()->update();
}

void
NodeNormal::swap(int fromTo)
{
    MFVec3f *normals = vector();
    if (normals != NULL)
         normals->swap(fromTo);
    if (hasParent())
        getParent()->update();
}

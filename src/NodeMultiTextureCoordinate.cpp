/*
 * NodeMultiTextureCoordinate.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "NodeMultiTextureCoordinate.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MeshBasedNode.h"
#include "Util.h"

ProtoMultiTextureCoordinate::ProtoMultiTextureCoordinate(Scene *scene)
  : Proto(scene, "MultiTextureCoordinate")
{
    texCoord.set(addExposedField(MFNODE, "texCoord", new MFNode(),
                                 TEXTURE_COORDINATE_NODE | NOT_SELF_NODE));
}

Node *
ProtoMultiTextureCoordinate::create(Scene *scene)
{ 
    return new NodeMultiTextureCoordinate(scene, this); 
}

NodeMultiTextureCoordinate::NodeMultiTextureCoordinate(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeMultiTextureCoordinate::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    if (hasParent())
        getParent()->update();
}

void
NodeMultiTextureCoordinate::update()
{
   if (hasParent()) {
       Node *parent = getParent();
       if (parent->isMeshBasedNode()) {
           MeshBasedNode *mesh = (MeshBasedNode *)parent;
           mesh->update();
       }
   }        
}


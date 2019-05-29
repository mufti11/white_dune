/*
 * NodeMultiGeneratedTextureCoordinate.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2010 J. "MUFTI" Scheurich
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

#include "NodeMultiGeneratedTextureCoordinate.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"

ProtoMultiGeneratedTextureCoordinate::ProtoMultiGeneratedTextureCoordinate(Scene *scene)
  : Proto(scene, "MultiGeneratedTextureCoordinate")
{
    texCoord.set(addExposedField(MFNODE, "texCoord", new MFNode(),
                                 GENERATED_TEXTURE_COORDINATE_NODE | 
                                 NOT_SELF_NODE));
}

Node *
ProtoMultiGeneratedTextureCoordinate::create(Scene *scene)
{ 
    return new NodeMultiGeneratedTextureCoordinate(scene, this); 
}

NodeMultiGeneratedTextureCoordinate::NodeMultiGeneratedTextureCoordinate(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeMultiGeneratedTextureCoordinate::setField(int index, FieldValue *value,
                                              int cf)
{
    Node::setField(index, value, cf);
    if (hasParent())
        getParents().update();
}

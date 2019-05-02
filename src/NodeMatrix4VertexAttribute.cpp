/*
 * NodeMatrix4VertexAttribute.cpp
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

#include "NodeMatrix4VertexAttribute.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFMatrix4f.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoMatrix4VertexAttribute::ProtoMatrix4VertexAttribute(Scene *scene)
  : Proto(scene, "Matrix4VertexAttribute")
{
    value.set(
        addExposedField(MFMATRIX4F, "value", new MFMatrix4f()));
    name.set(
        addField(SFSTRING, "name", new SFString("")));
}

Node *
ProtoMatrix4VertexAttribute::create(Scene *scene)
{ 
    return new NodeMatrix4VertexAttribute(scene, this); 
}

NodeMatrix4VertexAttribute::NodeMatrix4VertexAttribute(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

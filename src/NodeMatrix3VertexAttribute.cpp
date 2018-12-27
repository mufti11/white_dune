/*
 * NodeMatrix3VertexAttribute.cpp
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

#include "NodeMatrix3VertexAttribute.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFMatrix3f.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoMatrix3VertexAttribute::ProtoMatrix3VertexAttribute(Scene *scene)
  : Proto(scene, "Matrix3VertexAttribute")
{
    value.set(
        addExposedField(MFMATRIX3F, "value", new MFMatrix3f()));
    name.set(
        addField(SFSTRING, "name", new SFString("")));
}

Node *
ProtoMatrix3VertexAttribute::create(Scene *scene)
{ 
    return new NodeMatrix3VertexAttribute(scene, this); 
}

NodeMatrix3VertexAttribute::NodeMatrix3VertexAttribute(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

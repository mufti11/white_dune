/*
 * NodeFloatVertexAttribute.cpp
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

#include "NodeFloatVertexAttribute.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "SFString.h"
#include "SFInt32.h"
#include "DuneApp.h"

ProtoFloatVertexAttribute::ProtoFloatVertexAttribute(Scene *scene)
  : Proto(scene, "FloatVertexAttribute")
{
    value.set(
        addExposedField(MFFLOAT, "value", new MFFloat()));
    name.set(
        addField(SFSTRING, "name", new SFString("")));
    numComponents.set(
        addField(SFINT32, "numComponents", new SFInt32(4)));
}

Node *
ProtoFloatVertexAttribute::create(Scene *scene)
{ 
    return new NodeFloatVertexAttribute(scene, this); 
}

NodeFloatVertexAttribute::NodeFloatVertexAttribute(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

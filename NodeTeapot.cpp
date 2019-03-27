/*
 * NodeTeapot.cpp
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

#include "NodeTeapot.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFBool.h"
#include "SFBool.h"
#include "SFNode.h"
#include "DuneApp.h"

ProtoTeapot::ProtoTeapot(Scene *scene)
  : Proto(scene, "Teapot")
{
    size.set(
        addField(SFVEC3F, "size", new SFVec3f(3, 3, 3)));
    solid.set(
        addField(SFBOOL, "solid", new SFBool(true)));
    manifold.set(
        addField(SFBOOL, "manifold", new SFBool(false)));
    texCoord.set(
        addExposedField(SFNODE, "texCoord", new SFNode(), TEXTURE_COORDINATE_NODE));
}

Node *
ProtoTeapot::create(Scene *scene)
{ 
    return new NodeTeapot(scene, this); 
}

NodeTeapot::NodeTeapot(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

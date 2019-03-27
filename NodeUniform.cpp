/*
 * NodeUniform.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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

#include "NodeUniform.h"
#include "Scene.h"

ProtoUniform::ProtoUniform(Scene *scene)
  : Proto(scene, "Uniform")
{
    name.set(
          addExposedField(SFSTRING, "name", new SFString("")));
    setFieldFlags(name, FF_X3DOM_ONLY);

    type.set(
          addExposedField(SFSTRING, "type", new SFString("")));
    setFieldFlags(type, FF_X3DOM_ONLY);

    value.set(
          addExposedField(SFSTRING, "value", new SFString("")));
    setFieldFlags(value, FF_X3DOM_ONLY);
}

Node *
ProtoUniform::create(Scene *scene)
{ 
    return new NodeUniform(scene, this); 
}

NodeUniform::NodeUniform(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

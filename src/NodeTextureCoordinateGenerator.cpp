/*
 * NodeTextureCoordinateGenerator.cpp
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

#include "swt.h"
#include "DuneApp.h"

#include "NodeTextureCoordinateGenerator.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFImage.h"
#include "SFBool.h"
#include "Texture.h"

ProtoTextureCoordinateGenerator::ProtoTextureCoordinateGenerator(Scene *scene)
  : Proto(scene, "TextureCoordinateGenerator")
{
    mode.set(
          addExposedField(SFSTRING, "mode", new SFString("SPHERE")));
    parameter.set(
          addExposedField(MFFLOAT, "parameter", new MFFloat()));
}

Node *
ProtoTextureCoordinateGenerator::create(Scene *scene)
{ 
    return new NodeTextureCoordinateGenerator(scene, this); 
}

NodeTextureCoordinateGenerator::NodeTextureCoordinateGenerator(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

NodeTextureCoordinateGenerator::NodeTextureCoordinateGenerator(NodeTextureCoordinateGenerator &node)
  : Node(node)
{
}

NodeTextureCoordinateGenerator::~NodeTextureCoordinateGenerator()
{
}





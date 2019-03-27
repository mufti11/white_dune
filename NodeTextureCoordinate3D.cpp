/*
 * NodeTextureCoordinate3D.cpp
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

#include "NodeTextureCoordinate3D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFVec3f.h"
#include "DuneApp.h"

ProtoTextureCoordinate3D::ProtoTextureCoordinate3D(Scene *scene)
  : Proto(scene, "TextureCoordinate3D")
{
    point.set(
        addExposedField(MFVEC3F, "point", new MFVec3f()));
}

Node *
ProtoTextureCoordinate3D::create(Scene *scene)
{ 
    return new NodeTextureCoordinate3D(scene, this); 
}

NodeTextureCoordinate3D::NodeTextureCoordinate3D(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

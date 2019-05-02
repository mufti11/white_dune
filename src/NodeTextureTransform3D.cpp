/*
 * NodeTextureTransform3D.cpp
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

#include "NodeTextureTransform3D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFRotation.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoTextureTransform3D::ProtoTextureTransform3D(Scene *scene)
  : Proto(scene, "TextureTransform3D")
{
    center.set(
        addExposedField(SFVEC3F, "center", new SFVec3f(0, 0, 0)));
    rotation.set(
        addExposedField(SFROTATION, "rotation", new SFRotation(0, 0, 1, 0)));
    scale.set(
        addExposedField(SFVEC3F, "scale", new SFVec3f(1, 1, 1)));
    translation.set(
        addExposedField(SFVEC3F, "translation", new SFVec3f(0, 0, 0)));
}

Node *
ProtoTextureTransform3D::create(Scene *scene)
{ 
    return new NodeTextureTransform3D(scene, this); 
}

NodeTextureTransform3D::NodeTextureTransform3D(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

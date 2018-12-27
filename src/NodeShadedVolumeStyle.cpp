/*
 * NodeShadedVolumeStyle.cpp
 *
 * Copyright (C) 2011 J. "MUFTI" Scheurich
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

#include "NodeShadedVolumeStyle.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFBool.h"
#include "SFNode.h"
#include "SFBool.h"
#include "SFNode.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoShadedVolumeStyle::ProtoShadedVolumeStyle(Scene *scene)
  : Proto(scene, "ShadedVolumeStyle")
{
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    lighting.set(
        addExposedField(SFBOOL, "lighting", new SFBool(false)));
    material.set(
        addExposedField(SFNODE, "material", new SFNode(), MATERIAL_NODE));
    shadows.set(
        addExposedField(SFBOOL, "shadows", new SFBool(false)));
    surfaceNormals.set(
        addExposedField(SFNODE, "surfaceNormals", new SFNode(), 
                        TEXTURE_3D_NODE));
    phaseFunction.set(
        addField(SFSTRING, "phaseFunction", new SFString("Henyey-Greenstein")));
}

Node *
ProtoShadedVolumeStyle::create(Scene *scene)
{ 
    return new NodeShadedVolumeStyle(scene, this); 
}

NodeShadedVolumeStyle::NodeShadedVolumeStyle(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

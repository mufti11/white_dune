/*
 * NodeToneMappedVolumeStyle.cpp
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

#include "NodeToneMappedVolumeStyle.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFColorRGBA.h"
#include "SFBool.h"
#include "SFNode.h"
#include "SFColorRGBA.h"
#include "DuneApp.h"

ProtoToneMappedVolumeStyle::ProtoToneMappedVolumeStyle(Scene *scene)
  : Proto(scene, "ToneMappedVolumeStyle")
{
    coolColor.set(
        addExposedField(SFCOLORRGBA, "coolColor", new SFColorRGBA(0, 0, 1, 0)));
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    surfaceNormals.set(
        addExposedField(SFNODE, "surfaceNormals", new SFNode(),
                        TEXTURE_3D_NODE));
    warmColor.set(
        addExposedField(SFCOLORRGBA, "warmColor", new SFColorRGBA(1, 1, 0, 0)));
}

Node *
ProtoToneMappedVolumeStyle::create(Scene *scene)
{ 
    return new NodeToneMappedVolumeStyle(scene, this); 
}

NodeToneMappedVolumeStyle::NodeToneMappedVolumeStyle(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

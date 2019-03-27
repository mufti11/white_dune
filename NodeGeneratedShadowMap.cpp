/*
 * NodeGeneratedShadowMap.cpp
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

#include "NodeGeneratedShadowMap.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFString.h"
#include "SFInt32.h"
#include "SFNode.h"
#include "SFFloat.h"
#include "SFFloat.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoGeneratedShadowMap::ProtoGeneratedShadowMap(Scene *scene)
  : Proto(scene, "GeneratedShadowMap")
{
    update.set(
        addExposedField(SFSTRING, "update", new SFString("NONE")));
    size.set(
        addField(SFINT32, "size", new SFInt32(128)));
    light.set(
        addField(SFNODE, "light", new SFNode(), LIGHT_NODE));
    scale.set(
        addExposedField(SFFLOAT, "scale", new SFFloat(1.1)));
    bias.set(
        addExposedField(SFFLOAT, "bias", new SFFloat(4.0)));
    compareMode.set(
        addField(SFSTRING, "compareMode", new SFString("COMPARE_R_LEQUAL")));

    kambiTextureCommonFields()
}

Node *
ProtoGeneratedShadowMap::create(Scene *scene)
{ 
    return new NodeGeneratedShadowMap(scene, this); 
}

NodeGeneratedShadowMap::NodeGeneratedShadowMap(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

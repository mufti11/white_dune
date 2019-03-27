/*
 * NodeShaderTexture.cpp
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

#include "NodeShaderTexture.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoShaderTexture::ProtoShaderTexture(Scene *scene)
  : Proto(scene, "ShaderTexture")
{
    effects.set(
        addField(MFNODE, "effects", new MFNode()));
    defaultTexCoord.set(
        addField(SFSTRING, "defaultTexCoord", new SFString("BOUNDS2D")));
}

Node *
ProtoShaderTexture::create(Scene *scene)
{ 
    return new NodeShaderTexture(scene, this); 
}

NodeShaderTexture::NodeShaderTexture(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

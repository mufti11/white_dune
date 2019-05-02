/*
 * NodeBoundaryEnhancementVolumeStyle.cpp
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

#include "NodeBoundaryEnhancementVolumeStyle.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFFloat.h"
#include "DuneApp.h"

ProtoBoundaryEnhancementVolumeStyle::ProtoBoundaryEnhancementVolumeStyle(Scene *scene)
  : Proto(scene, "BoundaryEnhancementVolumeStyle")
{
    boundaryOpacity.set(
        addExposedField(SFFLOAT, "boundaryOpacity", new SFFloat(0.9)));
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    opacityFactor.set(
        addExposedField(SFFLOAT, "opacityFactor", new SFFloat(2)));
    retainedOpacity.set(
        addExposedField(SFFLOAT, "retainedOpacity", new SFFloat(0.2)));

    surfaceNormals.set(
        addExposedField(SFNODE, "surfaceNormals", new SFNode(), TEXTURE_NODE));
    setFieldFlags(surfaceNormals, FF_X3DOM_ONLY);    
}

Node *
ProtoBoundaryEnhancementVolumeStyle::create(Scene *scene)
{ 
    return new NodeBoundaryEnhancementVolumeStyle(scene, this); 
}

NodeBoundaryEnhancementVolumeStyle::NodeBoundaryEnhancementVolumeStyle(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

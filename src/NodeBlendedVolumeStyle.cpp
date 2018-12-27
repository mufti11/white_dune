/*
 * NodeBlendedVolumeStyle.cpp
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

#include "NodeBlendedVolumeStyle.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFNode.h"
#include "SFFloat.h"
#include "SFString.h"
#include "SFNode.h"
#include "DuneApp.h"

ProtoBlendedVolumeStyle::ProtoBlendedVolumeStyle(Scene *scene)
  : Proto(scene, "BlendedVolumeStyle")
{
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    renderStyle.set(
        addExposedField(SFNODE, "renderStyle", new SFNode(), 
                        COMPOSABLE_VOLUME_RENDER_STYLE_NODE));
    voxels.set(
        addExposedField(SFNODE, "voxels", new SFNode(), TEXTURE_3D_NODE));
    weightConstant1.set(
        addExposedField(SFFLOAT, "weightConstant1", new SFFloat(0.5)));
    weightConstant2.set(
        addExposedField(SFFLOAT, "weightConstant2", new SFFloat(0.5)));
    weightFunction1.set(
        addExposedField(SFSTRING, "weightFunction1", new SFString("CONSTANT")));
    weightFunction2.set(
        addExposedField(SFSTRING, "weightFunction2", new SFString("CONSTANT")));
    weightTransferFunction1.set(
        addExposedField(SFNODE, "weightTransferFunction1", new SFNode(),
                        TEXTURE_NODE));
    weightTransferFunction2.set(
        addExposedField(SFNODE, "weightTransferFunction2", new SFNode(),
                        TEXTURE_NODE));
    surfaceNormals.set(
        addExposedField(SFNODE, "surfaceNormals", new SFNode(), TEXTURE_NODE));
    setFieldFlags(surfaceNormals, FF_X3DOM_ONLY);    
}

Node *
ProtoBlendedVolumeStyle::create(Scene *scene)
{ 
    return new NodeBlendedVolumeStyle(scene, this); 
}

NodeBlendedVolumeStyle::NodeBlendedVolumeStyle(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

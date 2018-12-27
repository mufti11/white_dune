/*
 * NodeOpacityMapVolumeStyle.cpp
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

#include "NodeOpacityMapVolumeStyle.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFNode.h"
#include "DuneApp.h"

ProtoOpacityMapVolumeStyle::ProtoOpacityMapVolumeStyle(Scene *scene)
  : Proto(scene, "OpacityMapVolumeStyle")
{
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    transferFunction.set(
        addExposedField(SFNODE, "transferFunction", new SFNode(),
                        TEXTURE_OR_TEXTURE_3D_NODE));

    lightFactor.set(
          addExposedField(SFFLOAT, "lightFactor", new SFFloat(2)));
    setFieldFlags(lightFactor, FF_X3DOM_ONLY);

    opacityFactor.set(
          addExposedField(SFFLOAT, "opacityFactor", new SFFloat(0)));
    setFieldFlags(opacityFactor, FF_X3DOM_ONLY);

    surfaceNormals.set(
          addExposedField(SFNODE, "surfaceNormals", new SFNode(NULL),
                          TEXTURE_NODE));
    setFieldFlags(surfaceNormals, FF_X3DOM_ONLY);

    type.set(
          addExposedField(SFSTRING, "type", new SFString("simple")));
    setFieldFlags(type, FF_X3DOM_ONLY);
}

Node *
ProtoOpacityMapVolumeStyle::create(Scene *scene)
{ 
    return new NodeOpacityMapVolumeStyle(scene, this); 
}

NodeOpacityMapVolumeStyle::NodeOpacityMapVolumeStyle(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

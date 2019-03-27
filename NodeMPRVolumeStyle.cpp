/*
 * NodeMPRVolumeStyle.cpp
 *
 * Copyright (C) 2018 J. "MUFTI" Scheurich
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

#include "NodeMPRVolumeStyle.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFFloat.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoMPRVolumeStyle::ProtoMPRVolumeStyle(Scene *scene)
  : Proto(scene, "MPRVolumeStyle")
{
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    setFieldFlags(enabled, FF_X3DOM_ONLY);

    finalLine.set(
        addExposedField(SFVEC3F, "finalLine", new SFVec3f(0.0, 0.0, 0.0)));
    setFieldFlags(finalLine, FF_X3DOM_ONLY);

    originLine.set(
        addExposedField(SFVEC3F, "originLine", new SFVec3f(0.0, 0.0, 0.0)));
    setFieldFlags(originLine, FF_X3DOM_ONLY);

    positionLine.set(
        addExposedField(SFFLOAT, "positionLine", new SFFloat(2)));
    setFieldFlags(positionLine, FF_X3DOM_ONLY);

    transferFunction.set(
        addExposedField(SFNODE, "transferFunction", new SFNode(NULL),
                        TEXTURE_NODE));
    setFieldFlags(transferFunction, FF_X3DOM_ONLY);
}

Node *
ProtoMPRVolumeStyle::create(Scene *scene)
{ 
    return new NodeMPRVolumeStyle(scene, this); 
}

NodeMPRVolumeStyle::NodeMPRVolumeStyle(Scene *scene, Proto *proto)
  : Node(scene, proto)
{
}

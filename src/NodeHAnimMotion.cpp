/*
 * NodeHAnimMotion.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2020 J. "MUFTI" Scheurich
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
#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
# include <fcntl.h>
#endif

#include "swt.h"
#include "NodeHAnimMotion.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFRotation.h"
#include "SFFloat.h"
#include "MFNode.h"
#include "FieldCommand.h"
#include "Matrix.h"
#include "Util.h"
#include "Field.h"
#include "Path.h"
#include "NodeViewpoint.h"
#include "NodeNavigationInfo.h"
#include "ExposedField.h"
#include "NodeShape.h"
#include "NodeIndexedFaceSet.h"
#include "NodeMaterial.h"
#include "NodeHAnimJoint.h"

#include "RenderState.h"

void ProtoHAnimMotion::addElements(void) 
{
    channels.set (
           addExposedField(SFSTRING, "channels", new SFString("")));
    channelsEnabled.set(
           addExposedField(MFBOOL, "channelsEnabled", new MFBool()));
    description.set(
           addExposedField(SFSTRING, "description", new SFString("")));
    enabled.set(
           addExposedField(SFBOOL, "enabled", new SFBool(true)));
    frameDuration.set(
           addExposedField(SFTIME, "frameDuration", new SFTime(0.1f), 
                           new SFTime(0.0f)));
    frameIncrement.set(
           addExposedField(SFINT32, "frameIncrement", new SFInt32(1),
                           new SFInt32(0)));
    frameIndex.set(
           addExposedField(SFINT32, "frameIndex", new SFInt32(1),
                           new SFInt32(0)));
    joints.set(
           addExposedField(MFNODE, "joints", new MFNode(), X3D_HANIM_JOINT));
    loa.set(
           addExposedField(SFINT32, "loa", new SFInt32(-1), new SFInt32(-1),
                           new SFInt32(4)));
    loop.set(
           addExposedField(SFBOOL, "loop", new SFBool(false)));
    values.set(
           addExposedField(MFFLOAT, "values", new MFFloat()));
    addEventIn(SFBOOL, "next");
    addEventIn(SFBOOL, "previous");
    addEventOut(SFTIME, "cycleTime");
    addEventOut(SFTIME, "elaspedTime");
    addEventOut(SFINT32, "frameCount");
}

ProtoHAnimMotion::ProtoHAnimMotion(Scene *scene) 
    : Proto(scene, "HAnimMotion")
{                    
     addElements();     
}

Node *
ProtoHAnimMotion::create(Scene *scene)
{ 
    return new NodeHAnimMotion(scene, this); 
}

NodeHAnimMotion::NodeHAnimMotion(Scene *scene, Proto *def)
    : Node(scene, def)
{
}

NodeHAnimMotion::~NodeHAnimMotion()
{
}

int
NodeHAnimMotion::getComponentLevel(void) const
{
    return 3;
}

const char* 
NodeHAnimMotion::getComponentName(void) const
{
    static const char* name = "H-Anim";
    return name;
}

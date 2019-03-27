/*
 * NodeGeoTransform.cpp
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

#include "NodeGeoTransform.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFNode.h"
#include "SFVec3d.h"
#include "SFRotation.h"
#include "SFVec3f.h"
#include "SFRotation.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "SFNode.h"
#include "MFString.h"
#include "DuneApp.h"

ProtoGeoTransform::ProtoGeoTransform(Scene *scene)
  : GeoProto(scene, "GeoTransform")
{
    addEventIn(MFNODE, "addChildren");
    addEventIn(MFNODE, "removeChildren");
    children.set(
        addExposedField(MFNODE, "children", new MFNode()));
    geoCenter.set(
        addExposedField(SFVEC3D, "geoCenter", new SFVec3d(0, 0, 0)));
    rotation.set(
        addExposedField(SFROTATION, "rotation", new SFRotation(0, 0, 1, 0)));
    scale.set(
        addExposedField(SFVEC3F, "scale", new SFVec3f(1, 1, 1)));
    scaleOrientation.set(
        addExposedField(SFROTATION, "scaleOrientation", new SFRotation(0, 0, 1, 0)));
    translation.set(
        addExposedField(SFVEC3F, "translation", new SFVec3f(0, 0, 0)));
    bboxCenter.set(
        addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(
        addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1)));

    globalGeoOrigin.set(
          addExposedField(SFBOOL, "globalGeoOrigin", new SFBool(false)));
    setFieldFlags(globalGeoOrigin, FF_X3DOM_ONLY);

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);
}

Node *
ProtoGeoTransform::create(Scene *scene)
{ 
    return new NodeGeoTransform(scene, this); 
}

NodeGeoTransform::NodeGeoTransform(Scene *scene, Proto *def)
  : GeoNode(scene, def)
{
}

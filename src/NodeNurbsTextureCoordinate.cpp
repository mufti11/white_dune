/*
 * NodeNurbsTextureCoordinate.cpp
 *
 * Copyright (C) 1999 Stephen F. White
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

#include <stdlib.h>
#include <stdio.h>

#ifndef _WIN32
# include "stdlib.h"
#endif

#include "stdafx.h"
#include "NodeNurbsTextureCoordinate.h"
#include "Scene.h"
#include "FieldValue.h"
#include "SFInt32.h"
#include "MFFloat.h"
#include "MFInt32.h"
#include "MFVec2f.h"
#include "MFVec3f.h"
#include "SFNode.h"
#include "SFBool.h"
#include "SFVec3f.h"
#include "Vec2f.h"
#include "Vec3f.h"
#include "RenderState.h"
#include "DuneApp.h"
#include "NodeCoordinate.h"
#include "NodeNormal.h"
#include "NodeTextureCoordinate.h"
#include "NodeIndexedFaceSet.h"
#include "NodeNurbsGroup.h"
#include "Util.h"
#include "Field.h"
#include "ExposedField.h"

ProtoNurbsTextureCoordinate::ProtoNurbsTextureCoordinate(Scene *scene)
  : Proto(scene, "NurbsTextureCoordinate")
{
    uDimension.set(
          addField(SFINT32, "uDimension", new SFInt32(0), new SFInt32(0)));
    vDimension.set(
          addField(SFINT32, "vDimension", new SFInt32(0), new SFInt32(0)));
    uKnot.set(
          addField(MFFLOAT, "uKnot", new MFFloat()));
    vKnot.set(
          addField(MFFLOAT, "vKnot", new MFFloat()));
    uOrder.set(
          addField(SFINT32, "uOrder", new SFInt32(3), new SFInt32(2)));
    vOrder.set(
          addField(SFINT32, "vOrder", new SFInt32(3), new SFInt32(2)));

    ExposedField* cpoint = new ExposedField(MFVEC2F, "controlPoint", 
                                            new MFVec2f());
    controlPoint.set(addExposedField(cpoint));

    weight.set(
          addExposedField(MFFLOAT, "weight", new MFFloat(), new SFFloat(0.0f)));
}

Node *
ProtoNurbsTextureCoordinate::create(Scene *scene)
{
    return new NodeNurbsTextureCoordinate(scene, this); 
}

NodeNurbsTextureCoordinate::NodeNurbsTextureCoordinate(Scene *scene, Proto *proto)
  : Node(scene, proto)
{
}

NodeNurbsTextureCoordinate::~NodeNurbsTextureCoordinate()
{
}

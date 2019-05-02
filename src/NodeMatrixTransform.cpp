/*
 * NodeMatrixTransform.cpp
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

#include "NodeMatrixTransform.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "SFMatrix4f.h"
#include "DuneApp.h"

ProtoMatrixTransform::ProtoMatrixTransform(Scene *scene)
  : Proto(scene, "MatrixTransform")
{
    addEventIn(MFNODE, "addChildren");
    addEventIn(MFNODE, "removeChildren");
    matrix.set(
        addExposedField(SFMATRIX4F, "matrix", new SFMatrix4f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)));
    invert.set(
        addExposedField(SFBOOL, "invert", new SFBool(false)));
    showProxyGeometry.set(
        addExposedField(SFBOOL, "showProxyGeometry", new SFBool(false)));
    children.set(
        addExposedField(MFNODE, "children", new MFNode(), CHILD_NODE));
    bboxCenter.set(
        addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(
        addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1)));
    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    showBBox.set(
        addExposedField(SFBOOL, "showBBox", new SFBool(false)));
    logFeature.set(
        addExposedField(MFSTRING, "logFeature", new MFString()));
}

Node *
ProtoMatrixTransform::create(Scene *scene)
{ 
    return new NodeMatrixTransform(scene, this); 
}

NodeMatrixTransform::NodeMatrixTransform(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

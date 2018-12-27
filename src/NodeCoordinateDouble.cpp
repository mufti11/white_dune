/*
 * NodeCoordinateDouble.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#include "NodeCoordinateDouble.h"
#include "Proto.h"
#include "MFVec3f.h"
#include "ExposedField.h"
#include "Field.h"
#include "RenderState.h"
#include "DuneApp.h"
#include "Util.h"
#include "Vec3f.h"
#include "Scene.h"
#include "NodeIndexedFaceSet.h"
#include "NodeIndexedLineSet.h"
#include "NodePointSet.h"
#include "NodeLineSet.h"
#include "NodeTriangleSet.h"
#include "NodeNurbsGroup.h"

ProtoCoordinateDouble::ProtoCoordinateDouble(Scene *scene)
  : Proto(scene, "CoordinateDouble")
{
    point.set(
          addExposedField(MFVEC3D, "point", new MFVec3d()));
    setFieldFlags(point, EIF_RECOMMENDED);
}

Node *
ProtoCoordinateDouble::create(Scene *scene)
{ 
    return new NodeCoordinateDouble(scene, this); 
}

NodeCoordinateDouble::NodeCoordinateDouble(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeCoordinateDouble::flip(int index)
{
    MFVec3d *coords = point();
    if (coords != NULL)
        coords->flip(index);
    if (hasParent())
        getParent()->update();
}

void
NodeCoordinateDouble::swap(int fromTo)
{
    MFVec3d *coords = point();
    if (coords != NULL)
        coords->swap(fromTo);
    if (hasParent())
        getParent()->update();
}

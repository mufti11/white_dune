/*
 * NodePositionInterpolator2D.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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

#include "NodePositionInterpolator2D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "MFVec2f.h"
#include "Scene.h"
#include "MFieldCommand.h"
#include "FieldCommand.h"
#include "CommandList.h"
#include "SFVec2f.h"

ProtoPositionInterpolator2D::ProtoPositionInterpolator2D(Scene *scene)
  : ProtoInterpolator(scene, "PositionInterpolator2D", SFVEC2F, MFVEC2F, 
                      new MFVec2f())
{
    addURLs(URL_X3D);
}

Node *
ProtoPositionInterpolator2D::create(Scene *scene)
{ 
    return new NodePositionInterpolator2D(scene, this);
}

NodePositionInterpolator2D::NodePositionInterpolator2D(Scene *scene, Proto *def)
  : Interpolator(scene, def)
{
}

FieldValue *
NodePositionInterpolator2D::createKey(float *value) const
{
    FieldValue *r = new SFVec2f(value);
    return r;
}

FieldValue *
NodePositionInterpolator2D::createKeys(float *value, int numKeys) const
{
    return new MFVec2f(value, numKeys * 2);
}

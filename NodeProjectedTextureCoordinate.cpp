/*
 * NodeProjectedTextureCoordinate.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2010 J. "MUFTI" Scheurich
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

#include "NodeProjectedTextureCoordinate.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFFloat.h"
#include "SFVec3f.h"
#include "SFColor.h"
#include "SFFloat.h"
#include "SFBool.h"


ProtoProjectedTextureCoordinate::ProtoProjectedTextureCoordinate(Scene *scene)
  : Proto(scene, "ProjectedTextureCoordinate")
{
    projector.set(
          addExposedField(SFNODE, "projector", new SFNode(), 
                          SPOTLIGHT_OR_DIRECTIONALLIGHT_OR_VIEWPOINT_NODE));
}

Node *
ProtoProjectedTextureCoordinate::create(Scene *scene)
{ 
    return new NodeProjectedTextureCoordinate(scene, this); 
}

NodeProjectedTextureCoordinate::NodeProjectedTextureCoordinate(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

/*
 * NodePickableGroup.cpp
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

#include "NodePickableGroup.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFString.h"
#include "SFBool.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoPickableGroup::ProtoPickableGroup(Scene *scene)
  : StaticGroupProto(scene, "PickableGroup")
{
    objectType.set(
        addExposedField(MFSTRING, "objectType", new MFString("ALL")));
    pickable.set(
        addExposedField(SFBOOL, "pickable", new SFBool(true)));
}

Node *
ProtoPickableGroup::create(Scene *scene)
{ 
    return new NodePickableGroup(scene, this); 
}

NodePickableGroup::NodePickableGroup(Scene *scene, Proto *def)
  : StaticGroupNode(scene, def)
{
}

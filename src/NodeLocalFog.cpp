/*
 * NodeLocalFog.cpp
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

#include "NodeLocalFog.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFColor.h"
#include "SFBool.h"
#include "SFString.h"
#include "SFFloat.h"
#include "DuneApp.h"

ProtoLocalFog::ProtoLocalFog(Scene *scene)
  : Proto(scene, "LocalFog")
{
    color.set(
        addExposedField(SFCOLOR, "color", new SFColor(1, 1, 1)));
    enabled.set(
        addExposedField(SFBOOL, "enabled", new SFBool(true)));
    fogType.set(
        addExposedField(SFSTRING, "fogType", new SFString("LINEAR")));
    visibilityRange.set(
        addExposedField(SFFLOAT, "visibilityRange", new SFFloat(0)));

    FogElements()
}

Node *
ProtoLocalFog::create(Scene *scene)
{ 
    return new NodeLocalFog(scene, this); 
}

NodeLocalFog::NodeLocalFog(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

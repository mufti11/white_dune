/*
 * NodeFillProperties.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "NodeFillProperties.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFInt32.h"
#include "SFFloat.h"

ProtoFillProperties::ProtoFillProperties(Scene *scene)
  : Proto(scene, "FillProperties")
{
    filled.set(
          addExposedField(SFBOOL, "filled", new SFBool(true)));

    hatchColor.set(
          addExposedField(SFCOLOR, "hatchColor", new SFColor(1, 1, 1)));

    hatched.set(
          addExposedField(SFBOOL, "hatched", new SFBool(true)));

    hatchStyle.set(
          addExposedField(SFINT32, "hatchStyle", new SFInt32(1)));

    addURLs(URL_X3D);
}

Node *
ProtoFillProperties::create(Scene *scene)
{ 
    return new NodeFillProperties(scene, this); 
}

NodeFillProperties::NodeFillProperties(Scene *scene, Proto *def)
  : Node(scene, def)
{
}



/*
 * NodeLineProperties.cpp
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

#include "NodeLineProperties.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFInt32.h"
#include "SFFloat.h"

ProtoLineProperties::ProtoLineProperties(Scene *scene)
  : Proto(scene, "LineProperties")
{
    applied.set(
          addExposedField(SFBOOL, "applied", new SFBool(true)));
    linetype.set(
          addExposedField(SFINT32, "linetype", new SFInt32(1)));
    linewidthScaleFactor.set(
          addExposedField(SFFLOAT, "linewidthScaleFactor", new SFFloat(0)));

    addURLs(URL_X3D);
}

Node *
ProtoLineProperties::create(Scene *scene)
{ 
    return new NodeLineProperties(scene, this); 
}

NodeLineProperties::NodeLineProperties(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

 /*
 * NodePointProperties.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2020 J. "MUFTI" Scheurich
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

#include "NodePointProperties.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFInt32.h"
#include "SFFloat.h"

ProtoPointProperties::ProtoPointProperties(Scene *scene)
  : Proto(scene, "PointProperties")
{
    pointSizeScaleFactor.set(
          addExposedField(SFFLOAT, "pointSizeScaleFactor", new SFFloat(1.0f)));
    pointSizeMinValue.set(
          addExposedField(SFFLOAT, "pointSizeMinValue", new SFFloat(1.0f)));
    pointSizeMaxValue.set(
          addExposedField(SFFLOAT, "pointSizeMaxValue", new SFFloat(1.0f)));
    static float att[3] = { 1, 0, 0 };
    pointSizeAttenuation.set(
          addExposedField(MFFLOAT, "pointSizeAttenuation", 
                          new MFFloat(att, 3)));
    colorMode.set(
          addExposedField(SFSTRING, "colorMode", 
                          new SFString("TEXTURE_AND_POINT_COLOR")));
}

Node *
ProtoPointProperties::create(Scene *scene)
{ 
    return new NodePointProperties(scene, this); 
}

NodePointProperties::NodePointProperties(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

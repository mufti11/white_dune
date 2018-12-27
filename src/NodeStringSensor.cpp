/*
 * NodeStringSensor.cpp
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

#include "NodeStringSensor.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFVec2f.h"
#include "SFVec3f.h"
#include "Field.h"


ProtoStringSensor::ProtoStringSensor(Scene *scene)
  : WonderlandExportProto(scene, "StringSensor")
{
    deletionAllowed.set(
          addExposedField(SFBOOL, "deletionAllowed", new SFBool(true)));
    setFieldFlags(deletionAllowed, EIF_WONDERLAND_SUPPORTED);

    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    setFieldFlags(enabled, EIF_WONDERLAND_SUPPORTED);

    addEventOut(SFSTRING, "enteredText", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFSTRING, "finalText", EOF_RECOMMENDED | 
                                       EIF_WONDERLAND_SUPPORTED);
    addEventOut(SFBOOL, "isActive", EOF_WONDERLAND_SUPPORTED);

    addURLs(URL_X3D);
}

Node *
ProtoStringSensor::create(Scene *scene)
{ 
   return new NodeStringSensor(scene, this); 
}

NodeStringSensor::NodeStringSensor(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

const char* 
NodeStringSensor::getComponentName(void) const
{
    static const char* name = "KeyDeviceSensor";
    return name;
}

int         
NodeStringSensor::getComponentLevel(void) const
{
    return 2;
}


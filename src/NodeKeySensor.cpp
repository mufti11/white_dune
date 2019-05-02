/*
 * NodeKeySensor.cpp
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

#include "NodeKeySensor.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFVec2f.h"
#include "SFVec3f.h"
#include "Field.h"


ProtoKeySensor::ProtoKeySensor(Scene *scene)
  : WonderlandExportProto(scene, "KeySensor")
{
    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    setFieldFlags(enabled, EIF_WONDERLAND_SUPPORTED);

    addEventOut(SFINT32, "actionKeyPress", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFINT32, "actionKeyRelease", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFBOOL, "altKey", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFBOOL, "controlKey", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFBOOL, "isActive", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFSTRING, "keyPress", EOF_RECOMMENDED | 
                                      EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFSTRING, "keyRelease", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFBOOL, "shiftKey", EOF_WONDERLAND_SUPPORTED);

    addURLs(URL_X3D);
}

Node *
ProtoKeySensor::create(Scene *scene)
{ 
   return new NodeKeySensor(scene, this); 
}

NodeKeySensor::NodeKeySensor(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

const char* 
NodeKeySensor::getComponentName(void) const
{
    static const char* name = "KeyDeviceSensor";
    return name;
}

int         
NodeKeySensor::getComponentLevel(void) const
{
    return 1;
}


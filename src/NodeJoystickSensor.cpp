/*
 * NodeJoystickSensor.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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

#include "NodeJoystickSensor.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"

ProtoJoystickSensor::ProtoJoystickSensor(Scene *scene)
  : Proto(scene, "JoystickSensor")
{
    enabled.set(
          addField(SFBOOL, "enabled", new SFBool(true)));
    joystickNumber.set(
          addField(SFINT32, "joystickNumber", new SFInt32(-1)));
    // magic numbers still to delete, concept for "sendEvent" is needed
    axes_changed.set(2);
    addEventOut(MFFLOAT, "axes_changed", EOF_RECOMMENDED);
    buttons_changed.set(3);
    addEventOut(MFINT32, "buttons_changed", EOF_RECOMMENDED);

    addURLs(URL_COVER);
}

Node *
ProtoJoystickSensor::create(Scene *scene)
{ 
    return new NodeJoystickSensor(scene, this); 
}

NodeJoystickSensor::NodeJoystickSensor(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
}


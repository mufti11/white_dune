/*
 * NodeSteeringWheel.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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

#include "NodeSteeringWheel.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFBool.h"
#include "SFInt32.h"
#include "MFInt32.h"
#include "MFFloat.h"

ProtoSteeringWheel::ProtoSteeringWheel(Scene *scene)
  : Proto(scene, "SteeringWheel")
{
    addEventIn(SFTIME, "set_time");
    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    joystickNumber.set(
          addExposedField(SFINT32, "joystickNumber", new SFInt32(-1)));
    addEventOut(MFINT32, "buttons_changed");
    addEventOut(MFFLOAT, "axes_changed");

    addURLs(URL_COVER);
}

Node *
ProtoSteeringWheel::create(Scene *scene)
{ 
    return new NodeSteeringWheel(scene, this); 
}

NodeSteeringWheel::NodeSteeringWheel(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
}


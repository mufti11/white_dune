/*
 * NodeVehicle.cpp
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

#include "NodeVehicle.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFInt32.h"
#include "SFString.h"
#include "SFRotation.h"
#include "SFVec3f.h"

ProtoVehicle::ProtoVehicle(Scene *scene)
  : Proto(scene, "Vehicle")
{
    addEventIn(SFBOOL, "reset");
    addEventIn(SFFLOAT, "aMax");
    addEventIn(SFFLOAT, "vMax");
    addEventIn(SFINT32, "gear");

    carRotation.set(
          addExposedField(SFROTATION, "carRotation", 
                          new SFRotation(1, 0, 0, 0)));
    carTranslation.set(
          addExposedField(SFVEC3F, "carTranslation", new SFVec3f(0, 0, 0)));
    inputDevice.set(
          addExposedField(SFSTRING, "inputDevice", new SFString("AUTO")));

    addEventOut(SFROTATION, "breakPedalRotation");
    addEventOut(SFROTATION, "clutchPedalRotation");
    addEventOut(SFROTATION, "gasPedalRotation");
    addEventOut(SFROTATION, "steeringWheelRotation");
    addEventOut(SFROTATION, "wheelRotation");
    addEventOut(SFFLOAT, "speed");
    addEventOut(SFFLOAT, "acceleration");
    addEventOut(SFINT32, "gear");

    addURLs(URL_COVER);
}

Node *
ProtoVehicle::create(Scene *scene)
{ 
    return new NodeVehicle(scene, this); 
}

NodeVehicle::NodeVehicle(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
}


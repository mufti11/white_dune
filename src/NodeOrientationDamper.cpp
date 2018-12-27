/*
 * NodeOrientationDamper.cpp
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

#include "NodeOrientationDamper.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFFloat.h"
#include "SFRotation.h"
#include "SFRotation.h"
#include "SFInt32.h"
#include "DuneApp.h"

ProtoOrientationDamper::ProtoOrientationDamper(Scene *scene)
  : Proto(scene, "OrientationDamper")
{
    addEventIn(SFROTATION, "set_destination");
    addEventIn(SFROTATION, "set_value");
    tau.set(
        addExposedField(SFTIME, "tau", new SFTime(0)));
    tolerance.set(
        addExposedField(SFFLOAT, "tolerance", new SFFloat(-1)));
    initialDestination.set(
        addField(SFROTATION, "initialDestination", new SFRotation(0, 1, 0, 0)));
    initialValue.set(
        addField(SFROTATION, "initialValue", new SFRotation(0, 1, 0, 0)));
    order.set(
        addField(SFINT32, "order", new SFInt32(3),
                 new SFInt32(0), new SFInt32(5)));
    addEventOut(SFBOOL, "isActive");
    addEventOut(SFROTATION, "value_changed");
}

Node *
ProtoOrientationDamper::create(Scene *scene)
{ 
    return new NodeOrientationDamper(scene, this); 
}

NodeOrientationDamper::NodeOrientationDamper(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

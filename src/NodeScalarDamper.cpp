/*
 * NodeScalarDamper.cpp
 *
 * Copyright (C) 2018 J. "MUFTI" Scheurich
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

#include "NodeScalarDamper.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFFloat.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "SFInt32.h"
#include "DuneApp.h"

ProtoScalarDamper::ProtoScalarDamper(Scene *scene)
  : Proto(scene, "ScalarDamper")
{
    addEventIn(SFFLOAT, "set_destination");
    addEventIn(SFFLOAT, "set_value");
    tau.set(
        addExposedField(SFTIME, "tau", new SFTime(0)));
    tolerance.set(
        addExposedField(SFFLOAT, "tolerance", new SFFloat(-1)));
    initialDestination.set(
        addField(SFFLOAT, "initialDestination", new SFFloat(0)));
    initialValue.set(
        addField(SFFLOAT, "initialValue", new SFFloat(0)));
    order.set(
        addField(SFINT32, "order", new SFInt32(3),
                 new SFInt32(0), new SFInt32(5)));
    addEventOut(SFBOOL, "isActive");
    addEventOut(SFFLOAT, "value_changed");
}

Node *
ProtoScalarDamper::create(Scene *scene)
{ 
    return new NodeScalarDamper(scene, this); 
}

NodeScalarDamper::NodeScalarDamper(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

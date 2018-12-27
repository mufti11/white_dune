/*
 * NodeCoordinateDamper.cpp
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

#include "NodeCoordinateDamper.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFFloat.h"
#include "MFVec3f.h"
#include "MFVec3f.h"
#include "SFInt32.h"
#include "DuneApp.h"

ProtoCoordinateDamper::ProtoCoordinateDamper(Scene *scene)
  : Proto(scene, "CoordinateDamper")
{
    addEventIn(MFVEC3F, "set_destination");
    addEventIn(MFVEC3F, "set_value");
    tau.set(
        addExposedField(SFTIME, "tau", new SFTime(0)));
    tolerance.set(
        addExposedField(SFFLOAT, "tolerance", new SFFloat(-1)));
    float *destination = new float[3];
    destination[0] = 0;
    destination[1] = 0;
    destination[2] = 0;
    initialDestination.set(
          addField(MFVEC3F, "initialDestination", new MFVec3f(destination, 1)));
    float *value = new float[3];
    value[0] = 0;
    value[1] = 0;
    value[2] = 0;
    initialValue.set(
        addField(MFVEC3F, "initialValue", new MFVec3f(value, 1)));
    order.set(
        addField(SFINT32, "order", new SFInt32(3),
                 new SFInt32(0), new SFInt32(5)));
    addEventOut(SFBOOL, "isActive");
    addEventOut(MFVEC3F, "value_changed");
}

Node *
ProtoCoordinateDamper::create(Scene *scene)
{ 
    return new NodeCoordinateDamper(scene, this); 
}

NodeCoordinateDamper::NodeCoordinateDamper(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

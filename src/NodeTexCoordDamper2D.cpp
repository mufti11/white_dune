/*
 * NodeTexCoordDamper2D.cpp
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

#include "NodeTexCoordDamper2D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFFloat.h"
#include "MFVec2f.h"
#include "MFVec2f.h"
#include "SFInt32.h"
#include "DuneApp.h"

ProtoTexCoordDamper2D::ProtoTexCoordDamper2D(Scene *scene)
  : Proto(scene, "TexCoordDamper2D")
{
    addEventIn(MFVEC2F, "set_destination");
    addEventIn(MFVEC2F, "set_value");
    tau.set(
        addExposedField(SFTIME, "tau", new SFTime(0.0)));
    tolerance.set(
        addExposedField(SFFLOAT, "tolerance", new SFFloat(-1)));
    initialDestination.set(
        addField(MFVEC2F, "initialDestination", new MFVec2f(0.0, 0.0)));
    initialValue.set(
        addField(MFVEC2F, "initialValue", new MFVec2f(0.0, 0.0)));
    order.set(
        addField(SFINT32, "order", new SFInt32(3),
                 new SFInt32(0), new SFInt32(5)));
    addEventOut(SFBOOL, "isActive");
    addEventOut(MFVEC2F, "value_changed");
}

Node *
ProtoTexCoordDamper2D::create(Scene *scene)
{ 
    return new NodeTexCoordDamper2D(scene, this); 
}

NodeTexCoordDamper2D::NodeTexCoordDamper2D(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

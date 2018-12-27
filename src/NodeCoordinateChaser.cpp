/*
 * NodeCoordinateChaser.cpp
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

#include "NodeCoordinateChaser.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoCoordinateChaser::ProtoCoordinateChaser(Scene *scene)
  : Proto(scene, "CoordinateChaser")
{
    addEventIn(MFVEC3F, "set_destination");
    addEventIn(MFVEC3F, "set_value");
    duration.set(
        addField(SFTIME, "duration", new SFTime(0)));
    initialDestination.set(
        addField(MFVEC3F, "initialDestination", new MFVec3f(0, 0, 0)));
    initialValue.set(
        addField(MFVEC3F, "initialValue", new MFVec3f(0, 0, 0)));
    addEventOut(SFBOOL, "isActive");
    addEventOut(MFVEC3F, "value_changed");
}

Node *
ProtoCoordinateChaser::create(Scene *scene)
{ 
    return new NodeCoordinateChaser(scene, this); 
}

NodeCoordinateChaser::NodeCoordinateChaser(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

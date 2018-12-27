/*
 * NodePositionChaser.cpp
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

#include "NodePositionChaser.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoPositionChaser::ProtoPositionChaser(Scene *scene)
  : Proto(scene, "PositionChaser")
{
    addEventIn(SFVEC3F, "set_destination");
    addEventIn(SFVEC3F, "set_value");
    duration.set(
        addField(SFTIME, "duration", new SFTime(0)));
    initialDestination.set(
        addField(SFVEC3F, "initialDestination", new SFVec3f(0, 0, 0)));
    initialValue.set(
        addField(SFVEC3F, "initialValue", new SFVec3f(0, 0, 0)));
    addEventOut(SFBOOL, "isActive");
    addEventOut(SFVEC3F, "value_changed");
}

Node *
ProtoPositionChaser::create(Scene *scene)
{ 
    return new NodePositionChaser(scene, this); 
}

NodePositionChaser::NodePositionChaser(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

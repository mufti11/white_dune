/*
 * ChaserNode.cpp
 *
 * Copyright (C) 2009, 2019 J. "MUFTI" Scheurich
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

#include "ChaserNode.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFVec3f.h"
#include "DuneApp.h"

#include "swt.h"

ChaserProto::ChaserProto(Scene *scene, const char *name, int type)
  : Proto(scene, name)
{
    addEventIn(type, "set_destination");
    addEventIn(type, "set_value");
    duration.set(
        addField(SFTIME, "duration", new SFTime(1)));
    addEventOut(SFBOOL, "isActive");
    addEventOut(type, "value_changed");
}

ChaserNode::ChaserNode(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_set_value_Field.set(getProto()->lookupEventIn("set_value"));
    m_set_destination_Field.set(getProto()->lookupEventIn("set_destination"));
    m_isActive_Field.set(getProto()->lookupEventOut("isActive"));
    m_value_changed_Field.set(getProto()->lookupEventOut("value_changed"));
}

void
ChaserNode::sendEvents(bool active, FieldValue *value)
{
     double timestamp = swGetCurrentTime();
     sendEvent(m_isActive_Field, timestamp, new SFBool(active));
     sendEvent(m_value_changed_Field, timestamp, value);
}

void
ChaserNode::receiveEvent(int eventIn, double timestamp, FieldValue *value)
{
    Node::receiveEvent(eventIn, timestamp, value);
    sendChasedEvent(eventIn, timestamp, value);
}


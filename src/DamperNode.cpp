/*
 * DamperNode.cpp
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

#include "DamperNode.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFFloat.h"
#include "SFVec3f.h"
#include "SFInt32.h"
#include "DuneApp.h"

#include "swt.h"

DamperProto::DamperProto(Scene *scene, const char *name, int type)
  : Proto(scene, name)
{
    addEventIn(type, "set_destination");
    addEventIn(type, "set_value");

    tau.set(
        addExposedField(SFTIME, "tau", new SFTime(0.3)));
    tolerance.set(
        addExposedField(SFFLOAT, "tolerance", new SFFloat(-1)));
    order.set(
        addField(SFINT32, "order", new SFInt32(3),
                 new SFInt32(0), new SFInt32(5)));

    addEventOut(SFBOOL, "isActive");
    addEventOut(type, "value_changed");
}

DamperNode::DamperNode(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_set_value_Field.set(getProto()->lookupEventIn("set_value"));
    m_set_destination_Field.set(getProto()->lookupEventIn("set_destination"));
    m_tau_Field = getProto()->lookupEventIn("tau");
    m_tolerance_Field = getProto()->lookupEventIn("tolerance");
    m_order_Field = getProto()->lookupEventIn("order");
    m_isActive_Field.set(getProto()->lookupEventOut("isActive"));
    m_value_changed_Field.set(getProto()->lookupEventOut("value_changed"));
}

void
DamperNode::startTimer()
{
     m_lastTick = 0;
     double timestamp = swGetCurrentTime();
     sendEvent(m_isActive_Field, timestamp, new SFBool(true));
}

void
DamperNode::stopTimer()
{
     double timestamp = swGetCurrentTime();
     sendEvent(m_isActive_Field, timestamp, new SFBool(false));
}

void
DamperNode::sendEvents(bool active, FieldValue *value)
{
     double timestamp = swGetCurrentTime();
     sendEvent(m_isActive_Field, timestamp, new SFBool(active));
     sendEvent(m_value_changed_Field, timestamp, value);
}

void
DamperNode::receiveEvent(int eventIn, double timestamp, FieldValue *value)
{
    Node::receiveEvent(eventIn, timestamp, value);
    if (eventIn == m_tau_Field)
        tau((SFTime *)value);
    else if (eventIn == m_tolerance_Field)
        tolerance((SFFloat *)value);
    else if (eventIn == m_order_Field)
        order((SFInt32 *)value);

    sendDampedEvent(eventIn, timestamp, value);
}


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
  : DamperProto(scene, "OrientationDamper", SFROTATION)
{
    initialDestination.set(
        addField(SFROTATION, "initialDestination", new SFRotation(0, 1, 0, 0)));
    initialValue.set(
        addField(SFROTATION, "initialValue", new SFRotation(0, 1, 0, 0)));
}

Node *
ProtoOrientationDamper::create(Scene *scene)
{ 
    return new NodeOrientationDamper(scene, this); 
}

NodeOrientationDamper::NodeOrientationDamper(Scene *scene, Proto *def)
  : DamperNode(scene, def)
{
    m_initialDestination_Field = getProto()->lookupEventIn(
                                     "initialDestination");
    m_initialValue_Field = getProto()->lookupEventIn("initialValue");
}

void   
NodeOrientationDamper::sendDampedEvent(int eventIn, double timestamp, 
                                       FieldValue * value)
{
    double now = timestamp;

    if (eventIn == m_set_value_Field) 
        m_value1 = m_value2 = m_value3 = m_value4 = m_value5 = 
                   ((SFRotation *)value)->getValue();
    else if (eventIn == m_initialDestination_Field)
        initialDestination((SFRotation *)value);
    else if (eventIn == m_initialValue_Field)
        initialValue((SFRotation *)value);
    else if (eventIn == m_set_destination_Field)
        initialDestination((SFRotation *)value);

    if (!m_lastTick) {
        m_lastTick = now;
        return;
    }

    float ftau = tau()->getValue();
    float iorder = order()->getValue();
    SFRotation input = *initialDestination();

    double delta = now - m_lastTick;
    double alpha = exp(-delta / ftau);

    m_value1 = iorder > 0 && ftau ? input.slerp(m_value1, alpha) : input;

    m_value2 = iorder > 1 && ftau ? m_value1.slerp(m_value2, alpha) : m_value1;
    m_value3 = iorder > 1 && ftau ? m_value2.slerp(m_value3, alpha) : m_value2;
    m_value4 = iorder > 1 && ftau ? m_value3.slerp(m_value4, alpha) : m_value3;
    m_value5 = iorder > 1 && ftau ? m_value4.slerp(m_value5, alpha) : m_value4;

    float dist = fabs(m_value1.inverse().multiply(input).getValue()[3]);
    if (iorder > 1) {
       float dist2 = fabs(m_value2.inverse().multiply(m_value1).getValue()[3]);
       if (dist2 > dist)
           dist = dist2;
    }
    if (iorder > 2) {
       float dist3 = fabs(m_value3.inverse().multiply(m_value2).getValue()[3]);
       if (dist3 > dist)
           dist = dist3;
    }
    if (iorder > 3) {
       float dist4 = fabs(m_value4.inverse().multiply(m_value3).getValue()[3]);
       if (dist4 > dist)
           dist = dist4;
    }
    if (iorder > 4) {
       float dist5 = fabs(m_value5.inverse().multiply(m_value4).getValue()[3]);
       if (dist5 > dist)
           dist = dist5;
    }

    float eps = 0.001f;

    if (dist < eps) {
        m_value1 = m_value2 = m_value3 = m_value4 = m_value5 = input;
        sendEvent(m_value_changed_Field, timestamp, new SFRotation(input));
        stopTimer();
        return;
    }

    sendEvent(m_value_changed_Field, timestamp, new SFRotation(m_value5));

    m_lastTick= now;
}


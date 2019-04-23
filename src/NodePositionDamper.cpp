/*
 * NodePositionDamper.cpp
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

#include "NodePositionDamper.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFFloat.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "SFInt32.h"
#include "DuneApp.h"

ProtoPositionDamper::ProtoPositionDamper(Scene *scene)
  : DamperProto(scene, "PositionDamper", SFVEC3F)
{
    initialDestination.set(
        addField(SFVEC3F, "initialDestination", new SFVec3f(0, 0, 0)));
    initialValue.set(
        addField(SFVEC3F, "initialValue", new SFVec3f(0, 0, 0)));
}

Node *
ProtoPositionDamper::create(Scene *scene)
{ 
    return new NodePositionDamper(scene, this); 
}

NodePositionDamper::NodePositionDamper(Scene *scene, Proto *def)
  : DamperNode(scene, def)
{
    m_initialDestination_Field = getProto()->lookupEventIn(
                                     "initialDestination");
    m_initialValue_Field = getProto()->lookupEventIn("initialValue");
}

void   
NodePositionDamper::sendDampedEvent(int eventIn, double timestamp, 
                                    FieldValue * value)
{
    double now = timestamp;

    if (eventIn == m_set_value_Field) 
        m_value1 = m_value2 = m_value3 = m_value4 = m_value5 = 
                   ((SFVec3f *)value)->getValue();
    else if (eventIn == m_initialDestination_Field)
        initialDestination((SFVec3f *)value);
    else if (eventIn == m_initialValue_Field)
        initialValue((SFVec3f *)value);
    else if (eventIn == m_set_destination_Field)
        initialDestination((SFVec3f *)value);

    if (!m_lastTick) {
        m_lastTick = now;
        return;
    }

    float ftau = tau()->getValue();
    float iorder = order()->getValue();
    const float *f = initialDestination()->getValue();
    Vec3f input(f[0], f[1], f[2]);

    double delta = now - m_lastTick;
    double alpha = exp(-delta / ftau);

    m_value1 = iorder > 0 && ftau ? input + (m_value1 - input) * 
                                            alpha : input;
    m_value2 = iorder > 1 && ftau ? m_value1 + (m_value2 - m_value1) * 
                                               alpha : m_value1;
    m_value3 = iorder > 2 && ftau ? m_value2 + (m_value3 - m_value2) *
                                               alpha : m_value2;
    m_value4 = iorder > 3 && ftau ? m_value3 + (m_value4 - m_value3) * 
                                               alpha : m_value3;
    m_value5 = iorder > 4 && ftau ? m_value4 + (m_value5 - m_value4) * 
                                               alpha : m_value4;

    float dist = (m_value1 - input).length();
    if (iorder > 1) {
        float dist2 = (m_value2 - m_value1).length();
        if (dist2 > dist)
            dist = dist2;
    }
    if (iorder > 2) {
        float dist3 = (m_value3 - m_value2).length();
        if (dist3 > dist)
            dist = dist3;
    }
    if (iorder > 3) {
        float dist4 = (m_value4 - m_value3).length();
        if (dist4 > dist)
            dist = dist4;
    }
    if (iorder > 4) {
        float dist5 = (m_value5 - m_value4).length();
        if (dist5 > dist)
            dist = dist5;
    }

    float eps = 0.001f;

    if (dist < eps) {
        m_value1 = m_value2 = m_value3 = m_value4 = m_value5 = input;
        sendEvent(m_value_changed_Field, timestamp, new SFVec3f(input));
        stopTimer();
        return;
    }

    sendEvent(m_value_changed_Field, timestamp, new SFVec3f(m_value5));

    m_lastTick= now;
}


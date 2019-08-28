/*
 * NodeScalarChaser.cpp
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

#include "NodeScalarChaser.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFFloat.h"
#include "SFFloat.h"
#include "DuneApp.h"

ProtoScalarChaser::ProtoScalarChaser(Scene *scene)
  : ChaserProto(scene, "ScalarChaser", SFFLOAT)
{
    initialDestination.set(
        addField(SFFLOAT, "initialDestination", new SFFloat(0)));
    initialValue.set(
        addField(SFFLOAT, "initialValue", new SFFloat(0)));
}

Node *
ProtoScalarChaser::create(Scene *scene)
{ 
    return new NodeScalarChaser(scene, this); 
}

NodeScalarChaser::NodeScalarChaser(Scene *scene, Proto *def)
  : ChaserNode(scene, def)
{
    m_lastTick = 0;
    m_initialDestination_Field = getProto()->lookupEventIn(
                                     "initialDestination");
    m_initialValue_Field = getProto()->lookupEventIn("initialValue");
}

void   
NodeScalarChaser::sendChasedEvent(int eventIn, double timestamp, 
                                  FieldValue* value)
{
    double now = timestamp;

    if (eventIn == m_set_value_Field) 
        m_value = ((SFFloat *)value)->getValue();
    else if (eventIn == m_initialDestination_Field)
        initialDestination((SFFloat *)value);
    else if (eventIn == m_initialValue_Field)
        initialValue((SFFloat *)value);
    else if (eventIn == m_set_destination_Field)
        m_destination = ((SFFloat *)value)->getValue();

    if (!m_lastTick) {
        m_lastTick = now;
        return;
    }

    double dduration = duration()->getValue();
    float initDest = initialDestination()->getValue();
    float initVal = initialValue()->getValue();

    // throw out times and durations too old
    for (int i = m_event_times.size() - 1; i > -1; i--)
        if (m_event_times[i] < (now - dduration)) {
            m_event_times.remove(i);
            m_destinations.remove(i);
        }
    m_event_times.append(now);
    m_destinations.append(m_destination);

    float outputValue = initDest;      
    for (size_t i = 0; i < m_event_times.size(); i++) {
        float dnmin1;
        if (i == 0)
            dnmin1 = initVal;
        else
            dnmin1 = m_destinations[i - 1];
        float dn = m_destinations[i];
        if ((now - m_event_times[i]) > dduration)
            outputValue += dn - dnmin1;
        else
            outputValue += (dn - dnmin1) * 
                           (1 - cos(M_PI * (now - m_event_times[i] / 
                            dduration))) / 2;
    } 

    sendEvent(m_value_changed_Field, timestamp, new SFFloat(outputValue));

    m_lastTick= now;
}


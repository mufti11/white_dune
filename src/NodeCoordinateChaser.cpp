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

#include "swt.h"

ProtoCoordinateChaser::ProtoCoordinateChaser(Scene *scene)
  : ChaserProto(scene, "CoordinateChaser", MFVEC3F)
{
    initialDestination.set(
        addField(MFVEC3F, "initialDestination", new MFVec3f(0, 0, 0)));
    initialValue.set(
        addField(MFVEC3F, "initialValue", new MFVec3f(0, 0, 0)));
}

Node *
ProtoCoordinateChaser::create(Scene *scene)
{ 
    return new NodeCoordinateChaser(scene, this); 
}

NodeCoordinateChaser::NodeCoordinateChaser(Scene *scene, Proto *def)
  : ChaserNode(scene, def)
{
    m_active = true;
    m_lastTick = 0;
    m_initialDestination_Field = getProto()->lookupEventIn(
                                     "initialDestination");
    m_initialValue_Field = getProto()->lookupEventIn("initialValue");
}

void
NodeCoordinateChaser::dynamics(MFVec3f *add, MFVec3f *dest, MFVec3f *val, 
                               float mult)
{
    Vec3f var;
    Vec3f dst;
    Vec3f ad;
    for (int i = 0; i < dest->getSFSize(); i++) {
        var= val->getVec(i);
        dst = dest->getVec(i);
        ad = add->getVec(i);

        ad.x += (dst.x - var.x) * mult;
        ad.y += (dst.y - var.y) * mult;
        ad.z += (dst.z - var.z) * mult;
            
        add->setVec(i, ad);
    }
}

void   
NodeCoordinateChaser::sendChasedEvent(int eventIn, double timestamp, 
                                        FieldValue* value)
{
    if (!m_active)
        return;

    double now = timestamp;

    if (eventIn == m_set_value_Field) 
        m_value = ((MFVec3f *)value);
    else if (eventIn == m_initialDestination_Field)
        initialDestination((MFVec3f *)value);
    else if (eventIn == m_initialValue_Field)
        initialValue((MFVec3f *)value);
    else if (eventIn == m_set_destination_Field)
        m_destination = (MFVec3f *)value;

    if (!m_lastTick) {
        m_lastTick = now;
        return;
    }

    double dduration = duration()->getValue();
    MFVec3f *initDest = initialDestination();
    MFVec3f *initVal = initialValue();
    if (initDest->getSize() != m_destination->getSize()) {
        TheApp->MessageBox(
                    "size of initialDestination do not match destination");
        m_active = false;
        return;
    }
    if (initVal->getSize() != m_destination->getSize()) {
        TheApp->MessageBox("size of initialValue do not match destination");
        m_active = false;
        return;
    }

    // throw out times and durations too old
    for (int i = m_event_times.size() - 1; i > -1; i--)
       if (m_event_times[i] < (now - dduration)) {
            m_event_times.remove(i);
            m_destinations.remove(i);
        }
    m_event_times.append(now);
    m_destinations.append(m_destination);

    MFVec3f *outputValue = (MFVec3f *)initDest->copy();
    outputValue->ref();      
    for (size_t i = 0; i < m_event_times.size(); i++) {
        MFVec3f *dnmin1;
        if (i == 0)
            dnmin1 = initVal;
        else
            dnmin1 = m_destinations[i - 1];
        MFVec3f *dn = m_destinations[i];
        if ((now - m_event_times[i]) > dduration)
            dynamics(outputValue, dn, dnmin1, 1);
        else
            dynamics(outputValue, dn, dnmin1,
                     (1 - cos(M_PI * (now - m_event_times[i]) / dduration)) / 
                     2);
    } 

    sendEvent(m_value_changed_Field, timestamp, new MFVec3f(outputValue));

    m_lastTick= now;
}

void    
NodeCoordinateChaser::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    m_active = true;
}


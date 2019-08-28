/*
 * NodeTexCoordChaser2D.cpp
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

#include "NodeTexCoordChaser2D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "MFVec2f.h"
#include "DuneApp.h"

ProtoTexCoordChaser2D::ProtoTexCoordChaser2D(Scene *scene)
  : ChaserProto(scene, "TexCoordChaser2D", MFVEC2F)
{
    initialDestination.set(
        addField(MFVEC2F, "initialDestination", new MFVec2f(0.0, 0.0)));
    initialValue.set(
        addField(MFVEC2F, "initialValue", new MFVec2f(0.0, 0.0)));
}

Node *
ProtoTexCoordChaser2D::create(Scene *scene)
{ 
    return new NodeTexCoordChaser2D(scene, this); 
}

NodeTexCoordChaser2D::NodeTexCoordChaser2D(Scene *scene, Proto *def)
  : ChaserNode(scene, def)
{
    m_active = true;
    m_lastTick = 0;
    m_initialDestination_Field = getProto()->lookupEventIn(
                                     "initialDestination");
    m_initialValue_Field = getProto()->lookupEventIn("initialValue");
}

void
NodeTexCoordChaser2D::dynamics(MFVec2f *add, MFVec2f *dest, MFVec2f *val, 
                               float mult)
{
    Vec2f var;
    Vec2f dst;
    Vec2f ad;
    for (int i = 0; i < dest->getSFSize(); i++) {
        var= val->getVec(i);
        dst = dest->getVec(i);
        ad = add->getVec(i);
    

        ad.x += (dst.x - var.x) * mult;
        ad.y += (dst.y - var.y) * mult;
            
        add->setVec(i, ad);
    }
}

void   
NodeTexCoordChaser2D::sendChasedEvent(int eventIn, double timestamp, 
                                      FieldValue* value)
{
    if (!m_active)
        return;

    double now = timestamp;

    if (eventIn == m_set_value_Field) 
        m_value = ((MFVec2f *)value);
    else if (eventIn == m_initialDestination_Field)
        initialDestination((MFVec2f *)value);
    else if (eventIn == m_initialValue_Field)
        initialValue((MFVec2f *)value);
    else if (eventIn == m_set_destination_Field)
        m_destination = (MFVec2f *)value;

    if (!m_lastTick) {
        m_lastTick = now;
        return;
    }

    double dduration = duration()->getValue();
    MFVec2f *initDest = initialDestination();
    MFVec2f *initVal = initialValue();
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

    MFVec2f *outputValue = (MFVec2f *)initDest->copy();
    outputValue->ref();      
    for (size_t i = 0; i < m_event_times.size(); i++) {
        MFVec2f *dnmin1;
        if (i == 0)
            dnmin1 = initVal;
        else
            dnmin1 = m_destinations[i - 1];
        MFVec2f *dn = m_destinations[i];
        if ((now - m_event_times[i]) > dduration)
            dynamics(outputValue, dn, dnmin1, 1);
        else
            dynamics(outputValue, dn, dnmin1,
                     (1 - cos(M_PI * (now - m_event_times[i]) / dduration)) / 
                     2);
    } 

    sendEvent(m_value_changed_Field, timestamp, new MFVec2f(outputValue));

    m_lastTick= now;
}

void    
NodeTexCoordChaser2D::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    m_active = true;
}


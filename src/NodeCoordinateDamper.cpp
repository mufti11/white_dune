/*
 * NodeCoordinateDamper.cpp
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

#include "NodeCoordinateDamper.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFFloat.h"
#include "MFVec2f.h"
#include "SFInt32.h"
#include "DuneApp.h"

ProtoCoordinateDamper::ProtoCoordinateDamper(Scene *scene)
  : DamperProto(scene, "CoordinateDamper", MFVEC3F)
{
    float *destination = new float[3];
    destination[0] = 0;
    destination[1] = 0;
    destination[2] = 0;
    initialDestination.set(
          addField(MFVEC3F, "initialDestination", new MFVec3f(destination, 1)));
    float *value = new float[3];
    value[0] = 0;
    value[1] = 0;
    value[2] = 0;
    initialValue.set(
        addField(MFVEC3F, "initialValue", new MFVec3f(value, 1)));
}

Node *
ProtoCoordinateDamper::create(Scene *scene)
{ 
    return new NodeCoordinateDamper(scene, this); 
}

NodeCoordinateDamper::NodeCoordinateDamper(Scene *scene, Proto *def)
  : DamperNode(scene, def)
{
    m_initialDestination_Field = getProto()->lookupEventIn(
                                     "initialDestination");
    m_initialValue_Field = getProto()->lookupEventIn("initialValue");
}

void
NodeCoordinateDamper::dynamics(MFVec3f *dest, MFVec3f *val, float alpha)
{
    Vec3f var;
    Vec3f dst;
    for (int i = 0; i < dest->getSFSize(); i++) {
        var= val->getVec(i);
        dst = dest->getVec(i);
    
        var.x = dst.x + (var.x - dst.x) * alpha;
        var.y = dst.y + (var.y - dst.y) * alpha;
        var.z = dst.z + (var.z - dst.z) * alpha;
            
        val->setVec(i, var);
    }
}

void   
NodeCoordinateDamper::sendDampedEvent(int eventIn, double timestamp, 
                                      FieldValue * value)
{
    double now = timestamp;

    if (eventIn == m_set_value_Field) {
        m_value1 = (MFVec3f *)value;
        m_value1->ref();
        m_value2 = (MFVec3f *)m_value1->copy();
        m_value2->ref();
        m_value3 = (MFVec3f *)m_value2->copy();
        m_value3->ref();
        m_value4 = (MFVec3f *)m_value3->copy();
        m_value4->ref();
        m_value5 = (MFVec3f *)m_value4->copy();
        m_value5->ref();
    } else if (eventIn == m_initialDestination_Field) {
        MFVec3f *val = (MFVec3f *)value->copy();
        val->ref();
        initialDestination(val);
    } else if (eventIn == m_initialValue_Field) {
        MFVec3f *val = (MFVec3f *)value->copy();
        val->ref();
        initialValue(val);
    } else if (eventIn == m_set_destination_Field) {
        MFVec3f *val = (MFVec3f *)value->copy();
        val->ref();
        initialDestination(val);
    }
    MFVec3f *input = (MFVec3f *)initialDestination()->copy();
    input->ref();

    if (!m_lastTick) {
        m_lastTick = now;
        m_value1 = (MFVec3f *)input->copy();
        m_value1->ref();
        m_value2 = (MFVec3f *)m_value1->copy();
        m_value2->ref();
        m_value3 = (MFVec3f *)m_value2->copy();
        m_value3->ref();
        m_value4 = (MFVec3f *)m_value3->copy();
        m_value4->ref();
        m_value5 = (MFVec3f *)m_value4->copy();
        m_value5->ref();
        return;
    }

    float ftau = tau()->getValue();
    float iorder = order()->getValue();

    double delta = now - m_lastTick;
    double alpha = exp(-delta / ftau);

    if (iorder > 0) {
        if (ftau != 0.0f)
            dynamics(input, m_value1, alpha);
         else {
            m_value1 = (MFVec3f *)input->copy();
            m_value1->ref();
        }
    }
    if (iorder > 1) {
        if (ftau != 0.0f)
           dynamics(m_value1, m_value2, alpha);
        else {
            m_value2 = (MFVec3f *)m_value1->copy();
            m_value2->ref();
        }
    }
    if (iorder > 2) {
        if (ftau != 0.0f)
           dynamics(m_value2, m_value3, alpha);
        else {
            m_value3 = (MFVec3f *)m_value2->copy();
            m_value3->ref();
        }
    }
    if (iorder > 3) {
        if (ftau != 0.0f)
           dynamics(m_value3, m_value4, alpha);
        else {
            m_value4 = (MFVec3f *)m_value3->copy();
            m_value4->ref();
        }
    }
    if (iorder > 4) {
        if (ftau != 0.0f)
           dynamics(m_value4, m_value5, alpha);
        else {
            m_value5 = (MFVec3f *)m_value4->copy();
            m_value5->ref();
        }
    }

    MFVec3f *val = iorder <= 1 ? m_value1 : (
                   iorder == 2 ? m_value2 : (
                   iorder == 3 ? m_value3 : (
                   iorder == 4 ? m_value4 : (
                                 m_value5))));

    sendEvent(m_value_changed_Field, timestamp, 
              new MFVec3f((MFVec3f *)val->copy()));

    m_lastTick= now;
}


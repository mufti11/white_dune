/*
 * NodeTexCoordDamper2D.cpp
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

#include "NodeTexCoordDamper2D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFFloat.h"
#include "MFVec2f.h"
#include "MFVec2f.h"
#include "SFInt32.h"
#include "DuneApp.h"

ProtoTexCoordDamper2D::ProtoTexCoordDamper2D(Scene *scene)
  : DamperProto(scene, "TexCoordDamper2D", MFVEC2F)
{
    initialDestination.set(
        addField(MFVEC2F, "initialDestination", new MFVec2f(0.0, 0.0)));
    initialValue.set(
        addField(MFVEC2F, "initialValue", new MFVec2f(0.0, 0.0)));
}

Node *
ProtoTexCoordDamper2D::create(Scene *scene)
{ 
    return new NodeTexCoordDamper2D(scene, this); 
}

NodeTexCoordDamper2D::NodeTexCoordDamper2D(Scene *scene, Proto *def)
  : DamperNode(scene, def)
{
    m_initialDestination_Field = getProto()->lookupEventIn(
                                     "initialDestination");
    m_initialValue_Field = getProto()->lookupEventIn("initialValue");
}

void
NodeTexCoordDamper2D::dynamics(MFVec2f *dest, MFVec2f *val, float alpha)
{
    Vec2f var;
    Vec2f dst;
    for (int i = 0; i < dest->getSFSize(); i++) {
        var= val->getVec(i);
        dst = dest->getVec(i);
    
        var.x = dst.x + (var.x - dst.x) * alpha;
        var.y = dst.y + (var.y - dst.y) * alpha;
            
        val->setVec(i, var);
    }
}

void   
NodeTexCoordDamper2D::sendDampedEvent(int eventIn, double timestamp, 
                                      FieldValue * value)
{
    double now = timestamp;

    if (eventIn == m_set_value_Field) {
        m_value1 = (MFVec2f *)value;
        m_value1->ref();
        m_value2 = (MFVec2f *)m_value1->copy();
        m_value2->ref();
        m_value3 = (MFVec2f *)m_value2->copy();
        m_value3->ref();
        m_value4 = (MFVec2f *)m_value3->copy();
        m_value4->ref();
        m_value5 = (MFVec2f *)m_value4->copy();
        m_value5->ref();
    } else if (eventIn == m_initialDestination_Field) {
        MFVec2f *val = (MFVec2f *)value->copy();
        val->ref();
        initialDestination(val);
    } else if (eventIn == m_initialValue_Field) {
        MFVec2f *val = (MFVec2f *)value->copy();
        val->ref();
        initialValue(val);
    } else if (eventIn == m_set_destination_Field) {
        MFVec2f *val = (MFVec2f *)value->copy();
        val->ref();
        initialDestination(val);
    }
    MFVec2f *input = (MFVec2f *)initialDestination()->copy();
    input->ref();

    if (!m_lastTick) {
        m_lastTick = now;
        m_value1 = (MFVec2f *)input->copy();
        m_value1->ref();
        m_value2 = (MFVec2f *)m_value1->copy();
        m_value2->ref();
        m_value3 = (MFVec2f *)m_value2->copy();
        m_value3->ref();
        m_value4 = (MFVec2f *)m_value3->copy();
        m_value4->ref();
        m_value5 = (MFVec2f *)m_value4->copy();
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
            m_value1 = (MFVec2f *)input->copy();
            m_value1->ref();
        }
    }
    if (iorder > 1) {
        if (ftau != 0.0f)
           dynamics(m_value1, m_value2, alpha);
        else {
            m_value2 = (MFVec2f *)m_value1->copy();
            m_value2->ref();
        }
    }
    if (iorder > 2) {
        if (ftau != 0.0f)
           dynamics(m_value2, m_value3, alpha);
        else {
            m_value3 = (MFVec2f *)m_value2->copy();
            m_value3->ref();
        }
    }
    if (iorder > 3) {
        if (ftau != 0.0f)
           dynamics(m_value3, m_value4, alpha);
        else {
            m_value4 = (MFVec2f *)m_value3->copy();
            m_value4->ref();
        }
    }
    if (iorder > 4) {
        if (ftau != 0.0f)
           dynamics(m_value4, m_value5, alpha);
        else {
            m_value5 = (MFVec2f *)m_value4->copy();
            m_value5->ref();
        }
    }

    MFVec2f *val = iorder <= 1 ? m_value1 : (
                   iorder == 2 ? m_value2 : (
                   iorder == 3 ? m_value3 : (
                   iorder == 4 ? m_value4 : (
                                 m_value5))));

    sendEvent(m_value_changed_Field, timestamp, 
              new MFVec2f((MFVec2f *)val->copy()));

    m_lastTick= now;
}



/*
 * NodeColorDamper.cpp
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

#include "NodeColorDamper.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFFloat.h"
#include "SFColor.h"
#include "SFColor.h"
#include "SFInt32.h"
#include "DuneApp.h"

ProtoColorDamper::ProtoColorDamper(Scene *scene)
  : DamperProto(scene, "ColorDamper", SFCOLOR)
{
    initialDestination.set(
        addField(SFCOLOR, "initialDestination", new SFColor(0.8, 0.8, 0.8)));
    initialValue.set(
        addField(SFCOLOR, "initialValue", new SFColor(0.8, 0.8, 0.8)));
}

Node *
ProtoColorDamper::create(Scene *scene)
{ 
    return new NodeColorDamper(scene, this); 
}

NodeColorDamper::NodeColorDamper(Scene *scene, Proto *def)
  : DamperNode(scene, def)
{
    m_initialDestination_Field = getProto()->lookupEventIn(
                                     "initialDestination");
    m_initialValue_Field = getProto()->lookupEventIn("initialValue");
}

Vec3f 
NodeColorDamper::dynamics(Vec3f dest, Vec3f val, float alpha)
{
    Vec3f diff = val - dest;
    return dest + (diff * alpha);
}

float
NodeColorDamper::distanceHSV(Vec3f a, Vec3f b)
{
    float diffHue= a.x - b.x;
    float diffSat= a.y - b.y;
    float diffVal= a.z - b.z;

    return sqrt(diffHue * diffHue + diffSat * diffSat + diffVal * diffVal);
}

void   
NodeColorDamper::sendDampedEvent(int eventIn, double timestamp, 
                                 FieldValue * value)
{
    double now = timestamp;

    if (eventIn == m_set_value_Field) 
        m_value1 = m_value2 = m_value3 = m_value4 = m_value5 = 
                   ((SFColor *)value)->getValue();
    else if (eventIn == m_initialDestination_Field)
        initialDestination((SFColor *)value);
    else if (eventIn == m_initialValue_Field)
        initialValue((SFColor *)value);
    else if (eventIn == m_set_destination_Field)
        initialDestination((SFColor *)value);

    if (!m_lastTick) {
        m_lastTick = now;
        return;
    }

    float ftau = tau()->getValue();
    float iorder = order()->getValue();
    const float *c = initialDestination()->getValue();
    Vec3f input(c[0], c[1], c[2]);

    double delta = now - m_lastTick;
    double alpha = exp(-delta / ftau);

    m_value1 = iorder > 0 && ftau ? dynamics(input, m_value1, alpha) : input;

    m_value2 = iorder > 1 && ftau ? dynamics(m_value1, m_value2, alpha) : 
                                    m_value1;
    m_value3 = iorder > 2 && ftau ? dynamics(m_value2, m_value3, alpha) : 
                                    m_value2;
    m_value4 = iorder > 3 && ftau ? dynamics(m_value3, m_value4, alpha) : 
                                    m_value3;
    m_value5 = iorder > 4 && ftau ? dynamics(m_value4, m_value5, alpha) : 
                                    m_value4;

    float dist = distanceHSV(m_value1, input);
    if (iorder > 1) {
        float dist2 = distanceHSV(m_value2, m_value1);
        if (dist2 > dist)
           dist = dist2;
    }
    if (iorder > 2) {
        float dist3 = distanceHSV(m_value3, m_value2);
        if (dist3 > dist)
           dist = dist3;
    }
    if (iorder > 3) {
        float dist4 = distanceHSV(m_value4, m_value3);
        if (dist4 > dist)
           dist = dist4;
    }
    if (iorder > 4) {
        float dist5 = distanceHSV(m_value5, m_value4);
        if (dist5 > dist)
           dist = dist5;
    }


    float eps = 0.001f;

    if (dist < eps) {
        m_value1 = m_value2 = m_value3 = m_value4 = m_value5 = input;
        sendEvent(m_value_changed_Field, timestamp, 
                  new SFColor(input.x, input.y, input.z));
        stopTimer();
        return;
    }

    sendEvent(m_value_changed_Field, timestamp, 
              new SFColor(m_value5.x, m_value5.y, m_value5.z));

    m_lastTick= now;
}


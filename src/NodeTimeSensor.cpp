/*
 * NodeTimeSensor.cpp
 *
 * Copyright (C) 1999 Stephen F. White
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

#include "NodeTimeSensor.h"
#include "Proto.h"
#include "Scene.h"
#include "FieldValue.h"
#include "SFTime.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "Field.h"
#include "ExposedField.h"

#ifndef DBL_MIN
# include <float.h>
#endif

ProtoTimeSensor::ProtoTimeSensor(Scene *scene)
  : WonderlandExportProto(scene, "TimeSensor")
{
    cycleInterval.set(
          addExposedField(SFTIME, "cycleInterval", new SFTime(1.0), 
                          new SFTime(DBL_MIN)));
    setFieldFlags(cycleInterval, FF_4KIDS);

    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    setFieldFlags(enabled, EIF_WONDERLAND_SUPPORTED);

    loop.set(
          addExposedField(SFBOOL, "loop", new SFBool(false)));
    setFieldFlags(loop, FF_4KIDS);

    pauseTime.set(
          addExposedField(SFTIME, "pauseTime", new SFTime(0.0)));
    setFieldFlags(pauseTime, FF_X3D_ONLY);
    resumeTime.set(
          addExposedField(SFTIME, "resumeTime", new SFTime(0.0))); 
    setFieldFlags(resumeTime, FF_X3D_ONLY);
    startTime.set(
          addExposedField(SFTIME, "startTime", new SFTime(0.0f)));
    setFieldFlags(startTime, EIF_RECOMMENDED | EIF_WONDERLAND_SUPPORTED);
    stopTime.set(
          addExposedField(SFTIME, "stopTime", new SFTime(0.0f)));
    setFieldFlags(stopTime, EIF_RECOMMENDED | EIF_WONDERLAND_SUPPORTED);

    first.set(
          addExposedField(SFBOOL, "first", new SFBool(true)));
    setFieldFlags(first, FF_X3DOM_ONLY);

    firstCycle.set(
          addExposedField(SFFLOAT, "firstCycle", new SFFloat(0.0f)));
    setFieldFlags(firstCycle, FF_X3DOM_ONLY);

    addEventOut(SFTIME, "cycleTime");
    addEventOut(SFTIME, "elapsedTime", FF_X3D_ONLY);
    fraction_changed.set(addEventOut(SFFLOAT, "fraction_changed", 
                         EOF_RECOMMENDED | EOF_WONDERLAND_SUPPORTED));
    addEventOut(SFBOOL, "isActive", EOF_WONDERLAND_SUPPORTED);
    addEventOut(SFBOOL, "isPaused", FF_X3D_ONLY);
    time.set(addEventOut(SFTIME, "time", EOF_WONDERLAND_SUPPORTED));
}

Node *
ProtoTimeSensor::create(Scene *scene)
{ 
    return new NodeTimeSensor(scene, this); 
}

NodeTimeSensor::NodeTimeSensor(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_active = false;
    m_startTime = 0;
    m_stopTime = 0;
}

void
NodeTimeSensor::preDraw()
{
    if (enabled()->getValue()) m_scene->addTimeSensor(this);
}

void
NodeTimeSensor::setTime(double t)
{
    double dstopTime = m_stopTime;

    if (enabled()->getValue()) {
        if ((t < m_stopTime) || ((m_stopTime <= m_startTime) && 
                                loop()->getValue()))
            m_active = true;
        if (m_active) {
            if ((t >= dstopTime) && 
                (dstopTime > m_startTime) && !loop()->getValue()) {
                m_active = false;
            }
            double temp = (t - m_startTime) / cycleInterval()->getValue();
            double fraction = temp - floor(temp);
            if (fraction == 0.0 && (t > m_startTime)) fraction = 1.0;
            sendEvent(fraction_changed_Field(), t, 
                      new SFFloat((float) fraction));
            sendEvent(time_Field(), t, new SFTime(t));
        }
    }
}

void
NodeTimeSensor::checkStart(bool loop, double startTime, double stopTime, 
                           double t)
{
    if ((t < m_stopTime) || ((stopTime <= startTime) && loop)) {
        m_active = true;
        m_startTime = t;
        m_stopTime = stopTime;
    } else {
        m_active = false;
        m_startTime = startTime;
        m_stopTime = stopTime;
    }
}

void
NodeTimeSensor::start(double t)
{
    checkStart(loop()->getValue(), startTime()->getValue(), 
               stopTime()->getValue(), t);
}

void    
NodeTimeSensor::updateStart(int field, FieldValue *value, double t)
{
    if (m_scene)
        if (m_scene->isParsing())
            return;
    checkStart(loop()->getValue(), startTime()->getValue(), 
               stopTime()->getValue(), t);
}

int
NodeTimeSensor::getProfile(void) const
{ 
    return PROFILE_INTERCHANGE;
}


int
NodeTimeSensor::getComponentLevel(void) const
{
    if (hasOutput("isPaused"))
        return 2;    
    if (!isDefault(pauseTime_Field()))
        return 2;    
    if (!isDefault(resumeTime_Field()))
        return 2;    
    return -1;
}

const char* 
NodeTimeSensor::getComponentName(void) const
{
    static const char* name = "Time";
    return name;
}






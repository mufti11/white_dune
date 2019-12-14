/*
 * NodeTimeSensor.h
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

#ifndef _NODE_TIMESENSOR_H
#define _NODE_TIMESENSOR_H

#ifndef _NODE_H
#include "ExternTheApp.h"
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoTimeSensor : public WonderlandExportProto {
public:
                    ProtoTimeSensor(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_TIME_SENSOR; }
    virtual int     getNodeClass() const { return SENSOR_NODE | CHILD_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    FieldIndex cycleInterval;
    FieldIndex enabled;
    FieldIndex loop;
    FieldIndex pauseTime;
    FieldIndex resumeTime;
    FieldIndex startTime;
    FieldIndex stopTime;
    FieldIndex first;
    FieldIndex firstCycle;

    // not fields but eventIn/eventOut
    FieldIndex fraction_changed;
    FieldIndex time;
};

class NodeTimeSensor : public Node {
public:
                    NodeTimeSensor(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const;
    virtual int     getComponentLevel(void) const;
    virtual const char *getComponentName(void) const;
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeTimeSensor(m_scene, m_proto); }

    virtual void    preDraw();

    virtual bool    hasX3domOnOutputChange(void) { return true; } 

    void            setTime(double t);
    void            start(double t);
    float           getFraction(double t);
    void            updateStart(int field, FieldValue *value, double t);
    void            checkStart(bool loop, double startTime, double stopTime, 
                               double time, double delay = 0);

    fieldMacros(SFTime,  cycleInterval,    ProtoTimeSensor)
    fieldMacros(SFBool,  enabled,          ProtoTimeSensor)
    fieldMacros(SFBool,  loop,             ProtoTimeSensor)
    fieldMacros(SFTime,  pauseTime,        ProtoTimeSensor)
    fieldMacros(SFTime,  resumeTime,       ProtoTimeSensor)
    fieldMacros(SFTime,  startTime,        ProtoTimeSensor)
    fieldMacros(SFTime,  stopTime,         ProtoTimeSensor)
    fieldMacros(SFBool,  first,            ProtoTimeSensor)
    fieldMacros(SFFloat, firstCycle,       ProtoTimeSensor)

    fieldMacros(SFFloat, fraction_changed, ProtoTimeSensor)
    fieldMacros(SFTime,  time,             ProtoTimeSensor)

protected:
    bool            m_active;
    double          m_startTime;
    double          m_stopTime;
    double          m_delay;
};

#endif // _NODE_TIMESENSOR_H

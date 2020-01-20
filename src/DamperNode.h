/*
 * DamperNode.h
 *
 * Copyright (C) 2009, 2019 J. "MUFTI" Scheurich
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"

#include "SFMFTypes.h"

class DamperProto : public Proto {
public:
                    DamperProto(Scene *scene, const char *name, int type);

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex tau;
    FieldIndex tolerance;
    FieldIndex order;
};

class DamperNode : public Node {
public:
                    DamperNode(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Followers"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int         getX3dVersion(void) const { return 2; } 

    void                startTimer();
    void                stopTimer();

    virtual void        sendDampedEvent(int eventIn, double timestamp, 
                                        FieldValue * value) = 0;

    void                sendEvents(bool active, FieldValue *value);

    void                receiveEvent(int eventIn, double timestamp, 
                                     FieldValue *value);

    fieldMacros(SFTime,  tau,                DamperProto);
    fieldMacros(SFFloat, tolerance,          DamperProto);
    fieldMacros(SFInt32, order,              DamperProto);

protected:
    double m_lastTick;
    bool m_takeFirstInput;
    FieldIndex m_set_destination_Field;
    FieldIndex m_set_value_Field;
    int m_tau_Field;
    int m_tolerance_Field;
    int m_order_Field;
    FieldIndex m_isActive_Field;
    FieldIndex m_value_changed_Field;
};


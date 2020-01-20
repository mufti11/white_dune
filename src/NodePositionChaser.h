/*
 * NodePositionChaser.h
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"
#include "ChaserNode.h"
#include "Array.h"

class ProtoPositionChaser : public ChaserProto {
public:
                    ProtoPositionChaser(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_POSITION_CHASER; }

    FieldIndex initialDestination;
    FieldIndex initialValue;
};

class NodePositionChaser : public ChaserNode {
public:
                    NodePositionChaser(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodePositionChaser(*this); }

    virtual void    sendChasedEvent(int eventIn, double timestamp, 
                                    FieldValue * value);

    fieldMacros(SFVec3f, initialDestination, ProtoPositionChaser);
    fieldMacros(SFVec3f, initialValue,       ProtoPositionChaser);

protected:
    SFVec3f m_value;
    Vec3f m_destination;
    MyArray<Vec3f> m_destinations;
    MyArray<double> m_event_times;
    double m_lastTick;

    int m_initialDestination_Field;
    int m_initialValue_Field;
};


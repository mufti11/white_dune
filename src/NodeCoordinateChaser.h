/*
 * NodeCoordinateChaser.h
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

#ifndef _NODE_COORDINATE_CHASER_H
#define _NODE_COORDINATE_CHASER_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"
#include "ChaserNode.h"

class ProtoCoordinateChaser : public ChaserProto {
public:
                    ProtoCoordinateChaser(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_COORDINATE_CHASER; }

    FieldIndex initialDestination;
    FieldIndex initialValue;
};

class NodeCoordinateChaser : public ChaserNode {
public:
                    NodeCoordinateChaser(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeCoordinateChaser(*this); }

    void            dynamics(MFVec3f *add, MFVec3f *dest, MFVec3f *val, 
                             float mult);

    virtual void    sendChasedEvent(int eventIn, double timestamp, 
                                    FieldValue * value);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    fieldMacros(MFVec3f, initialDestination, ProtoCoordinateChaser);
    fieldMacros(MFVec3f, initialValue,       ProtoCoordinateChaser);
protected:
    MFVec3f m_value;
    MFVec3f *m_destination;
    MyArray<MFVec3f *> m_destinations;
    MyArray<double> m_event_times;
    double m_lastTick;
    bool m_active;

    int m_initialDestination_Field;
    int m_initialValue_Field;
};

#endif

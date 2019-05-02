/*
 * NodeScalarChaser.h
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

#ifndef _NODE_SCALAR_CHASER_H
#define _NODE_SCALAR_CHASER_H

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

class ProtoScalarChaser : public ChaserProto {
public:
                    ProtoScalarChaser(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_SCALAR_CHASER; }

    FieldIndex initialDestination;
    FieldIndex initialValue;
};

class NodeScalarChaser : public ChaserNode {
public:
                    NodeScalarChaser(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeScalarChaser(*this); }

    virtual void    sendChasedEvent(int eventIn, double timestamp, 
                                    FieldValue * value);

    fieldMacros(SFFloat, initialDestination, ProtoScalarChaser);
    fieldMacros(SFFloat, initialValue,       ProtoScalarChaser);

protected:
    SFFloat m_value;
    float m_destination;
    MyArray<float> m_destinations;
    MyArray<double> m_event_times;
    double m_lastTick;

    int m_initialDestination_Field;
    int m_initialValue_Field;
};

#endif

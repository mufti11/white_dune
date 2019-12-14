/*
 * NodeTexCoordChaser2D.h
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

#ifndef _NODE_TEX_COORD_CHASER_2D_H
#define _NODE_TEX_COORD_CHASER_2D_H

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
#include "ChaserNode.h"

#include "MFVec2f.h"

class ProtoTexCoordChaser2D : public ChaserProto {
public:
                    ProtoTexCoordChaser2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_TEX_COORD_CHASER_2D; }

    FieldIndex initialDestination;
    FieldIndex initialValue;
};

class NodeTexCoordChaser2D : public ChaserNode {
public:
                    NodeTexCoordChaser2D(Scene *scene, Proto *proto);

    virtual Node *copy() { return new NodeTexCoordChaser2D(m_scene, m_proto); }

    void            dynamics(MFVec2f *add, MFVec2f *dest, MFVec2f *val, 
                             float mult);

    virtual void    sendChasedEvent(int eventIn, double timestamp, 
                                    FieldValue * value);

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    fieldMacros(MFVec2f, initialDestination, ProtoTexCoordChaser2D);
    fieldMacros(MFVec2f, initialValue,       ProtoTexCoordChaser2D);
protected:
    MFVec2f m_value;
    MFVec2f *m_destination;
    MyArray<MFVec2f *> m_destinations;
    MyArray<double> m_event_times;
    double m_lastTick;
    bool m_active;

    int m_initialDestination_Field;
    int m_initialValue_Field;
};

#endif

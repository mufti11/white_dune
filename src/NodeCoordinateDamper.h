/*
 * NodeCoordinateDamper.h
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

#ifndef _NODE_COORDINATE_DAMPER_H
#define _NODE_COORDINATE_DAMPER_H

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
#include "DamperNode.h"

class ProtoCoordinateDamper : public DamperProto {
public:
                    ProtoCoordinateDamper(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_COORDINATE_DAMPER; }

    FieldIndex initialDestination;
    FieldIndex initialValue;
};

class NodeCoordinateDamper : public DamperNode {
public:
                    NodeCoordinateDamper(Scene *scene, Proto *proto);

    virtual Node   *copy() { return new NodeCoordinateDamper(
                                    m_scene, m_proto); }

    void            dynamics(MFVec3f *dest, MFVec3f *val, float alpha);

    virtual void    sendDampedEvent(int eventIn, double timestamp, 
                                    FieldValue * value);

    fieldMacros(MFVec3f, initialDestination, ProtoCoordinateDamper);
    fieldMacros(MFVec3f, initialValue,       ProtoCoordinateDamper);

protected:
    MFVec3f *m_value1;
    MFVec3f *m_value2;
    MFVec3f *m_value3;
    MFVec3f *m_value4;
    MFVec3f *m_value5;

    int m_initialDestination_Field;
    int m_initialValue_Field;
};

#endif

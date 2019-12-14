/*
 * NodeTexCoordDamper2D.h
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

#ifndef _NODE_TEX_COORD_DAMPER_2D_H
#define _NODE_TEX_COORD_DAMPER_2D_H

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

class ProtoTexCoordDamper2D : public DamperProto {
public:
                    ProtoTexCoordDamper2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_TEX_COORD_DAMPER_2D; }

    FieldIndex initialDestination;
    FieldIndex initialValue;
};

class NodeTexCoordDamper2D : public DamperNode {
public:
                    NodeTexCoordDamper2D(Scene *scene, Proto *proto);

    virtual Node   *copy() { return new NodeTexCoordDamper2D(
                             m_scene, m_proto); }


    void            dynamics(MFVec2f *dest, MFVec2f *val, float alpha);

    virtual void    sendDampedEvent(int eventIn, double timestamp, 
                                    FieldValue * value);

    fieldMacros(MFVec2f, initialDestination, ProtoTexCoordDamper2D);
    fieldMacros(MFVec2f, initialValue,       ProtoTexCoordDamper2D);

protected:
    MFVec2f *m_value1;
    MFVec2f *m_value2;
    MFVec2f *m_value3;
    MFVec2f *m_value4;
    MFVec2f *m_value5;

    int m_initialDestination_Field;
    int m_initialValue_Field;
};

#endif

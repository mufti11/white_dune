/*
 * NodePositionDamper2D.h
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

#ifndef _NODE_POSITION_DAMPER_2D_H
#define _NODE_POSITION_DAMPER_2D_H

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
#include "DamperNode.h"
#include "Vec2f.h"

class ProtoPositionDamper2D : public DamperProto {
public:
                    ProtoPositionDamper2D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_POSITION_DAMPER_2D; }

    FieldIndex initialDestination;
    FieldIndex initialValue;
};

class NodePositionDamper2D : public DamperNode {
public:
                    NodePositionDamper2D(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodePositionDamper2D(*this); }

    virtual void    sendDampedEvent(int eventIn, double timestamp, 
                                    FieldValue * value);

    fieldMacros(SFVec2f, initialDestination, ProtoPositionDamper2D);
    fieldMacros(SFVec2f, initialValue,       ProtoPositionDamper2D);

protected:
    Vec2f m_value1;
    Vec2f m_value2;
    Vec2f m_value3;
    Vec2f m_value4;
    Vec2f m_value5;

    int m_initialDestination_Field;
    int m_initialValue_Field;
};

#endif

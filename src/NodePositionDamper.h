/*
 * NodePositionDamper.h
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
#include "DamperNode.h"
#include "SFMFTypes.h"

class ProtoPositionDamper : public DamperProto {
public:
                    ProtoPositionDamper(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_POSITION_DAMPER; }

    FieldIndex initialDestination;
    FieldIndex initialValue;
};

class NodePositionDamper : public DamperNode {
public:
                    NodePositionDamper(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodePositionDamper(*this); }

    virtual void    sendDampedEvent(int eventIn, double timestamp, 
                                    FieldValue * value);

    fieldMacros(SFVec3f, initialDestination, ProtoPositionDamper);
    fieldMacros(SFVec3f, initialValue,       ProtoPositionDamper);

protected:
    Vec3f m_value1;
    Vec3f m_value2;
    Vec3f m_value3;
    Vec3f m_value4;
    Vec3f m_value5;

    int m_initialDestination_Field;
    int m_initialValue_Field;
};


/*
 * NodeOrientationDamper.h
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
#include "DamperNode.h"

class ProtoOrientationDamper : public DamperProto {
public:
                    ProtoOrientationDamper(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_ORIENTATION_DAMPER; }

    FieldIndex initialDestination;
    FieldIndex initialValue;
};

class NodeOrientationDamper : public DamperNode {
public:
                    NodeOrientationDamper(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeOrientationDamper(*this); }

    virtual void    sendDampedEvent(int eventIn, double timestamp, 
                                    FieldValue * value);

    fieldMacros(SFRotation, initialDestination, ProtoOrientationDamper);
    fieldMacros(SFRotation, initialValue,       ProtoOrientationDamper);

protected:
    SFRotation m_value1;
    SFRotation m_value2;
    SFRotation m_value3;
    SFRotation m_value4;
    SFRotation m_value5;

    int m_initialDestination_Field;
    int m_initialValue_Field;
};



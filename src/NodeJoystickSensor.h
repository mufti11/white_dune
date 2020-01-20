/*
 * NodeJoystickSensor.h
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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
#include "CoverNode.h"

class ProtoJoystickSensor : public Proto {
public:
                        ProtoJoystickSensor(Scene *scene);
    virtual Node       *create(Scene *scene);

    virtual int         getType() const { return COVER_JOYSTICK_SENSOR; }
    virtual int         getNodeClass() const 
                           { return SENSOR_NODE | CHILD_NODE; }

    FieldIndex enabled;
    FieldIndex joystickNumber;
 
    // not fields but eventOut
    FieldIndex axes_changed;
    FieldIndex buttons_changed;

    virtual bool          isCoverProto(void) { return true; }
};

class NodeJoystickSensor : public CoverNode {
public:
                        NodeJoystickSensor(Scene *scene, Proto *proto);

    virtual int         getX3dVersion(void) const { return -1; }
    virtual Node       *copy() const { return new NodeJoystickSensor(*this); }

    fieldMacros(SFBool,  enabled,         ProtoJoystickSensor)
    fieldMacros(SFInt32, joystickNumber,  ProtoJoystickSensor)

    fieldMacros(MFFloat, axes_changed,    ProtoJoystickSensor)
    fieldMacros(MFInt32, buttons_changed, ProtoJoystickSensor)
};

/*
 * NodeCylinderSensor.h
 *
 * Copyright (C) 1999 Stephen F. White
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

class ProtoCylinderSensor : public WonderlandExportProto {
public:
                    ProtoCylinderSensor(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_CYLINDER_SENSOR; }
    virtual int     getNodeClass() const { return SENSOR_NODE | CHILD_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    virtual bool    isCRouteSource(void) { return true; }       

    virtual bool    needUpdate(void) { return true; }

    FieldIndex autoOffset;
    FieldIndex axisRotation;
    FieldIndex description;
    FieldIndex diskAngle;
    FieldIndex enabled;
    FieldIndex maxAngle;
    FieldIndex minAngle;
    FieldIndex offset;
};

class NodeCylinderSensor : public Node {
public:
                    NodeCylinderSensor(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeCylinderSensor(*this); }

    virtual bool    hasX3domOnOutputChange(void) { return true; } 

    fieldMacros(SFBool,     autoOffset,   ProtoCylinderSensor)
    fieldMacros(SFRotation, axisRotation, ProtoCylinderSensor)
    fieldMacros(SFFloat,    diskAngle,    ProtoCylinderSensor)
    fieldMacros(SFString,   description,  ProtoCylinderSensor)
    fieldMacros(SFBool,     enabled,      ProtoCylinderSensor)
    fieldMacros(SFFloat,    maxAngle,     ProtoCylinderSensor)
    fieldMacros(SFFloat,    minAngle,     ProtoCylinderSensor)
    fieldMacros(SFFloat,    offset,       ProtoCylinderSensor)
};


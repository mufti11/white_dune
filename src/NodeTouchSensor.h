/*
 * NodeTouchSensor.h
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

#ifndef _NODE_TOUCHSENSOR_H
#define _NODE_TOUCHSENSOR_H

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

class ProtoTouchSensor : public WonderlandExportProto {
public:
                    ProtoTouchSensor(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual bool    isDeclaredInRwd_h() { return true; }      

    virtual bool    isCRouteSource(void) { return true; }       

    virtual int     getType() const { return VRML_TOUCH_SENSOR; }
    virtual int     getNodeClass() const { return SENSOR_NODE | CHILD_NODE; }

    FieldIndex description;
    FieldIndex enabled;
};

class NodeTouchSensor : public Node {
public:
                    NodeTouchSensor(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERACTIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeTouchSensor(*this); }

    fieldMacros(SFString, description, ProtoTouchSensor)
    fieldMacros(SFBool,   enabled,     ProtoTouchSensor)
};

#endif // _NODE_TOUCHSENSOR_H

/*
 * NodeVisibilitySensor.h
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

#ifndef _NODE_VISIBILITYSENSOR_H
#define _NODE_VISIBILITYSENSOR_H

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

class ProtoVisibilitySensor : public Proto {
public:
                    ProtoVisibilitySensor(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_VISIBILITY_SENSOR; }
    virtual int     getNodeClass() const { return SENSOR_NODE | CHILD_NODE; }

    FieldIndex center;
    FieldIndex enabled;
    FieldIndex size;
};

class NodeVisibilitySensor : public Node {
public:
                    NodeVisibilitySensor(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; }    
    virtual Node   *copy() const { return new NodeVisibilitySensor(*this); }

    virtual bool    hasX3domOnOutputChange(void) { return true; } 

    fieldMacros(SFVec3f, center,  ProtoVisibilitySensor)
    fieldMacros(SFBool,  enabled, ProtoVisibilitySensor)
    fieldMacros(SFVec3f, size,    ProtoVisibilitySensor)
};

#endif // _NODE_VISIBILITYSENSOR_H

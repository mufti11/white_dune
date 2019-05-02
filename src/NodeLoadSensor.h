/*
 * NodeLoadSensor.h
 *
 * Copyright (C) 2004 J. "MUFTI" Scheurich, 1999 Stephen F. White
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

#ifndef _NODE_LOADSENSOR_H
#define _NODE_LOADSENSOR_H

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

class ProtoLoadSensor : public Proto {
public:
                    ProtoLoadSensor(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_LOAD_SENSOR; }
    virtual int     getNodeClass() const { return SENSOR_NODE | CHILD_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex enabled;
    FieldIndex timeOut;
    FieldIndex watchList;
};

class NodeLoadSensor : public Node {
public:
                    NodeLoadSensor(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_IMMERSIVE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeLoadSensor(*this); }

    virtual int     writeProto(int f) { return writeX3dProto(f); }

    fieldMacros(SFBool,  enabled,   ProtoLoadSensor)
    fieldMacros(SFTime,  timeOut,   ProtoLoadSensor)
    fieldMacros(MFNode,  watchList, ProtoLoadSensor)
};

#endif // _NODE_LOADSENSOR_H

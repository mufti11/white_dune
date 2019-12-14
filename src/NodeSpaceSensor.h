/*
 * NodeSpaceSensor.h
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
 * 
 * Implements the Cover/Covise SpaceSensor node
 */

#ifndef _NODE_SPACESENSOR_H
#define _NODE_SPACESENSOR_H

#ifndef _COVER_NODE_H
#include "CoverNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoSpaceSensor : public Proto {
public:
                    ProtoSpaceSensor(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return COVER_SPACE_SENSOR; }
    virtual int     getNodeClass() const { return SENSOR_NODE | CHILD_NODE; }

    FieldIndex autoOffset;
    FieldIndex enabled;
    FieldIndex rotationEnabled;
    FieldIndex maxPosition;
    FieldIndex minPosition;
    FieldIndex offset;

    virtual bool          isCoverProto(void) { return true; }
};

class NodeSpaceSensor : public CoverNode {
public:
                    NodeSpaceSensor(Scene *scene, Proto *proto);

    virtual int     getX3dVersion(void) const { return 0; } 
    virtual Node   *copy() { return new NodeSpaceSensor(m_scene, m_proto); }

    fieldMacros(SFBool,  autoOffset,      ProtoSpaceSensor)
    fieldMacros(SFBool,  enabled,         ProtoSpaceSensor)
    fieldMacros(SFBool,  rotationEnabled, ProtoSpaceSensor)
    fieldMacros(SFVec3f, maxPosition,     ProtoSpaceSensor)
    fieldMacros(SFVec3f, minPosition,     ProtoSpaceSensor)
    fieldMacros(SFVec3f, offset,          ProtoSpaceSensor)
};

#endif // _NODE_SPACESENSOR_H

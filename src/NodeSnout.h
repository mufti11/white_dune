/*
 * NodeSnout.h
 *
 * Copyright (C) 1999 Stephen F. White
 *               2003 Th. Rothermel
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

#ifndef _NODE_SNOUT_H
#define _NODE_SNOUT_H

#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "GeometryNode.h"

#include "SFMFTypes.h"

class ProtoSnout : public Proto {
public:
                    ProtoSnout(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_SNOUT; }
    virtual int     getNodeClass() const { return GEOMETRY_NODE; }

    x3domGeometryCommonFieldIndex()
    FieldIndex bottom;
    FieldIndex ccw;
    FieldIndex dbottom;
    FieldIndex dtop;
    FieldIndex height;
    FieldIndex solid;
    FieldIndex subdivision;
    FieldIndex top;
    FieldIndex xoff;
    FieldIndex yoff;
};

class NodeSnout : public Node {
public:
                    NodeSnout(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeSnout(*this); }

    x3domGeometryCommonFieldMacros(ProtoSnout) 
    fieldMacros(SFBool, bottom, ProtoSnout);
    fieldMacros(SFBool, ccw, ProtoSnout);
    fieldMacros(SFFloat, dbottom, ProtoSnout);
    fieldMacros(SFFloat, dtop, ProtoSnout);
    fieldMacros(SFFloat, height, ProtoSnout);
    fieldMacros(SFBool, solid, ProtoSnout);
    fieldMacros(SFFloat, subdivision, ProtoSnout);
    fieldMacros(SFBool, top, ProtoSnout);
    fieldMacros(SFFloat, xoff, ProtoSnout);
    fieldMacros(SFFloat, yoff, ProtoSnout);
};

#endif

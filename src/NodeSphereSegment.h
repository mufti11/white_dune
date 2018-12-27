/*
 * NodeSphereSegment.h
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

#ifndef _NODE_SPHERE_SEGMENT_H
#define _NODE_SPHERE_SEGMENT_H

#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "GeometryNode.h"
#include "SFMFTypes.h"

class ProtoSphereSegment : public Proto {
public:
                    ProtoSphereSegment(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_SPHERE_SEGMENT; }
    virtual int     getNodeClass() const { return GEOMETRY_NODE; }

    x3domGeometryCommonFieldIndex()
    FieldIndex ccw;
    FieldIndex latitude;
    FieldIndex longitude;
    FieldIndex radius;
    FieldIndex stepSize;
    FieldIndex solid;
};

class NodeSphereSegment : public Node {
public:
                    NodeSphereSegment(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return -1; } 
    virtual Node   *copy() const { return new NodeSphereSegment(*this); }

    x3domGeometryCommonFieldMacros(ProtoSphereSegment) 
    fieldMacros(SFBool,  ccw,       ProtoSphereSegment)
    fieldMacros(MFFloat, latitude,  ProtoSphereSegment)
    fieldMacros(MFFloat, longitude, ProtoSphereSegment)
    fieldMacros(SFFloat, radius,    ProtoSphereSegment)
    fieldMacros(SFBool,  solid,     ProtoSphereSegment)
    fieldMacros(SFVec2f, stepSize,  ProtoSphereSegment)

protected:
    void            createMesh(bool cleanDoubleVertices = true,
                               bool triangulate = true);

};

#endif

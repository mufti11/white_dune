/*
 * NodeGeoCoordinate.h
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

#ifndef _NODE_GEO_COORDINATE_H
#define _NODE_GEO_COORDINATE_H

#ifndef _GEO_NODE_H
#include "GeoNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

class ProtoGeoCoordinate : public GeoProto {
public:
                    ProtoGeoCoordinate(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_GEO_COORDINATE; }
    virtual int     getNodeClass() const { return COORDINATE_NODE; }

    FieldIndex point;
    FieldIndex pointX3D;

    virtual int     translateField(int field) const;
};

class NodeGeoCoordinate : public GeoNode {
public:
                    NodeGeoCoordinate(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeGeoCoordinate(*this); }

    void            setField(int index, FieldValue *value, int cf = -1);

    void            drawHandles(void);
    Node           *convert2Vrml(void);

    fieldMacros(MFString, point,    ProtoGeoCoordinate)
    fieldMacros(MFVec3d,  pointX3D, ProtoGeoCoordinate)
};

#endif // _NODE_GEO_COORDINATE_H


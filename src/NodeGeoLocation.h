/*
 * NodeGeoLocation.h
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

#ifndef _NODE_GEO_LOCATION_H
#define _NODE_GEO_LOCATION_H

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

class ProtoGeoLocation : public GeoProto {
public:
                    ProtoGeoLocation(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_GEO_LOCATION; }
    virtual int     getNodeClass() const { return GROUPING_NODE | CHILD_NODE; }

    FieldIndex children;
    FieldIndex geoCoords;
    FieldIndex geoCoordsX3D;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    FieldIndex render;

    virtual int     translateField(int field) const;
};

class NodeGeoLocation : public GeoNode {
public:
                    NodeGeoLocation(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeGeoLocation(*this); }

    void            setField(int index, FieldValue *value, int cf = -1);
    Node           *convert2Vrml(void);

    virtual bool    showFields() { return true; } 

    fieldMacros(MFNode,   children,     ProtoGeoLocation)
    fieldMacros(SFString, geoCoords,    ProtoGeoLocation)
    fieldMacros(SFVec3d,  geoCoordsX3D, ProtoGeoLocation)
    fieldMacros(SFVec3f,  bboxCenter,   ProtoGeoLocation)
    fieldMacros(SFVec3f,  bboxSize,     ProtoGeoLocation)
    fieldMacros(SFBool,   render,       ProtoGeoLocation)
};

#endif // _NODE_GEO_LOCATION_H


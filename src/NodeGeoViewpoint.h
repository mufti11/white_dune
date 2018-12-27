/*
 * NodeGeoViewpoint.h
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

#ifndef _NODE_GEO_VIEWPOINT_H
#define _NODE_GEO_VIEWPOINT_H

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

class ProtoGeoViewpoint : public GeoProto {
public:
                    ProtoGeoViewpoint(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_GEO_VIEWPOINT; }

    FieldIndex centerOfRotation;
    FieldIndex description;
    FieldIndex fieldOfView;
    FieldIndex headlight;
    FieldIndex navType;
    FieldIndex jump;
    FieldIndex orientation;
    FieldIndex position;
    FieldIndex positionX3D;
    FieldIndex retainUserOffsets;
    FieldIndex speedFactor;
    FieldIndex elevationScaling;

    // not fields but eventIn/eventOut
    FieldIndex set_bind;

    virtual int     translateField(int field) const;
};

class NodeGeoViewpoint : public GeoNode {
public:
                    NodeGeoViewpoint(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeGeoViewpoint(*this); }

    void            setField(int index, FieldValue *value, int cf = -1);
    Node           *convert2Vrml(void);

    fieldMacros(SFVec3d,    centerOfRotation,  ProtoGeoViewpoint)
    fieldMacros(SFString,   description,       ProtoGeoViewpoint)
    fieldMacros(SFFloat,    fieldOfView,       ProtoGeoViewpoint)
    fieldMacros(SFBool,     headlight,         ProtoGeoViewpoint)
    fieldMacros(SFBool,     jump,              ProtoGeoViewpoint)
    fieldMacros(MFString,   navType,           ProtoGeoViewpoint)
    fieldMacros(SFRotation, orientation,       ProtoGeoViewpoint)
    fieldMacros(SFString,   position,          ProtoGeoViewpoint)
    fieldMacros(SFVec3d,    positionX3D,       ProtoGeoViewpoint)
    fieldMacros(SFBool,     retainUserOffsets, ProtoGeoViewpoint)
    fieldMacros(SFFloat,    speedFactor,       ProtoGeoViewpoint)
    fieldMacros(SFBool,     elevationScaling,  ProtoGeoViewpoint)
    fieldMacros(SFBool,     set_bind,          ProtoGeoViewpoint)
};

#endif // _NODE_GEO_VIEWPOINT_H


/*
 * NodeGeoOrigin.h
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

#ifndef _NODE_GEO_ORIGIN_H
#define _NODE_GEO_ORIGIN_H

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

class ProtoGeoOrigin : public Proto {
public:
                    ProtoGeoOrigin(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_GEO_ORIGIN; }

    FieldIndex geoCoords;
    FieldIndex geoCoordsX3D;
    FieldIndex geoSystem;
    FieldIndex rotateYUp;

    virtual int     translateField(int field) const;
};

class NodeGeoOrigin : public Node {
public:
                    NodeGeoOrigin(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual const char* getComponentName(void) const { return "Geospatial"; }
    virtual int     getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeGeoOrigin(*this); }

    void            setField(int index, FieldValue *value, int cf = -1);
    Node           *convert2Vrml(void);
    Vec3d           getVec(void);

    fieldMacros(SFString, geoCoords,    ProtoGeoOrigin)
    fieldMacros(SFVec3d,  geoCoordsX3D, ProtoGeoOrigin)
    fieldMacros(MFString, geoSystem,    ProtoGeoOrigin)
    fieldMacros(SFBool,   rotateYUp,    ProtoGeoOrigin)
};

#endif // _NODE_GEO_ORIGIN_H


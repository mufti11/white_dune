/*
 * NodeGeoPositionInterpolator.h
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

#ifndef _NODE_GEO_POSITION_INTERPOLATOR_H
#define _NODE_GEO_POSITION_INTERPOLATOR_H

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

class ProtoGeoPositionInterpolator : public GeoProto {
public:
                    ProtoGeoPositionInterpolator(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_GEO_POSITION_INTERPOLATOR; }

    FieldIndex key;
    FieldIndex keyValue;
    FieldIndex keyValueX3D;
    FieldIndex onGreatCircle;

    virtual int     translateField(int field) const;
};

class NodeGeoPositionInterpolator : public GeoNode {
public:
                    NodeGeoPositionInterpolator(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual Node   *copy() const { return new NodeGeoPositionInterpolator(*this); }

    virtual int     getNumChannels() const { return 3; }

    virtual const char* getComponentName(void) const { return "Geospatial"; }
    virtual int     getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; }

    virtual bool    hasX3domOnoutputchange(void) { return true; } 

/*
    virtual FieldValue *createKey(float *value) const;
    virtual FieldValue *createKeys(float *values, int numKeys) const;

    ADD_FLIP
    ADD_SWAP
*/

    void            setField(int index, FieldValue *value, int cf = -1);

    Node           *convert2Vrml(void);

    fieldMacros(MFFloat,  key,           ProtoGeoPositionInterpolator);
    fieldMacros(MFString, keyValue,      ProtoGeoPositionInterpolator)
    fieldMacros(MFVec3d,  keyValueX3D,   ProtoGeoPositionInterpolator)
    fieldMacros(SFBool,   onGreatCircle, ProtoGeoPositionInterpolator) 
};

#endif // _NODE_GEO_POSITION_INTERPOLATOR_H


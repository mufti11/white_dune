/*
 * NodeGeoPositionInterpolator.h
 *
 * Copyright (C) 1999 Stephen F. White, 2019 J. "MUFTI" Scheurich
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

#include "Interpolator.h"
#include "SFMFTypes.h"

class ProtoGeoPositionInterpolator : public ProtoInterpolator {
public:
                    ProtoGeoPositionInterpolator(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_GEO_POSITION_INTERPOLATOR; }

    FieldIndex keyValueVRML;
    FieldIndex onGreatCircle;

    FieldIndex geoOrigin;
    FieldIndex geoSystem;

    virtual int     translateField(int field) const;

    // not fields but eventIn/eventOut
    FieldIndex geovalue_changed;
};

class NodeGeoPositionInterpolator : public Interpolator {
public:
                    NodeGeoPositionInterpolator(Scene *scene, Proto *proto);

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual Node   *copy() const { return new NodeGeoPositionInterpolator(*this); }

    virtual int     getStride() const { return 3; }
    virtual int     getNumChannels() const { return 3; }

    virtual const char* getComponentName(void) const { return "Geospatial"; }
    virtual int     getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; }

    virtual FieldValue *createKey(void *value) const;
    virtual FieldValue *createKeys(void *value, int numKeys) const;

    virtual float       getKeyValue(int channel, int index) const;

    virtual FieldValue *getInterpolatedFieldValue(float k);

    virtual void        setKeyValue(int channel, int index, float value);

    virtual void        insertKey(int pos, float key, const float *values);

    void                interpolate(float key, double *values); 

    virtual void        sendInterpolatedEvent(double timestamp, float k);

    void                setField(int index, FieldValue *value, int cf = -1);

    Node               *convert2Vrml(void);

    fieldMacros(MFVec3d,  keyValue,         ProtoGeoPositionInterpolator)
    fieldMacros(MFString, keyValueVRML,     ProtoGeoPositionInterpolator)
    fieldMacros(SFBool,   onGreatCircle,    ProtoGeoPositionInterpolator) 
    fieldMacros(SFNode,   geoOrigin,        ProtoGeoPositionInterpolator)
    fieldMacros(MFString, geoSystem,        ProtoGeoPositionInterpolator)

    fieldMacros(SFVec3d,  geovalue_changed, ProtoGeoPositionInterpolator)
};

#endif // _NODE_GEO_POSITION_INTERPOLATOR_H

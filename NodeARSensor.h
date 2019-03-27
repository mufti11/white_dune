/*
 * NodeARSensor.h
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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

#ifndef _NODE_AR_SENSOR_H
#define _NODE_AR_SENSOR_H

#ifndef _COVER_NODE_H
#include "CoverNode.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "DuneApp.h"

#include "SFMFTypes.h"

class ProtoARSensor : public Proto {
public:
                        ProtoARSensor(Scene *scene);
    virtual Node       *create(Scene *scene);

    virtual int         getType() const { return COVER_AR_SENSOR; }
    virtual int         getNodeClass() const 
                           { return SENSOR_NODE | CHILD_NODE; }

    FieldIndex trackObjects;
    FieldIndex freeze;
    FieldIndex enabled;
    FieldIndex currentCamera;
    FieldIndex headingOnly;
    FieldIndex maxPosition;
    FieldIndex minPosition;
    FieldIndex orientationThreshold;
    FieldIndex positionThreshold;
    FieldIndex invisiblePosition;
    FieldIndex cameraPosition;
    FieldIndex cameraOrientation;
    FieldIndex markerName;

    virtual bool          isCoverProto(void) { return true; }
};


class NodeARSensor : public CoverNode {
public:
                        NodeARSensor(Scene *scene, Proto *proto);

    virtual int         getX3dVersion(void) const { return -1; }
    virtual Node       *copy() const { return new NodeARSensor(*this); }

    fieldMacros(SFBool,    trackObjects,         ProtoARSensor)
    fieldMacros(SFBool,    freeze,               ProtoARSensor)
    fieldMacros(SFBool,    enabled,              ProtoARSensor)
    fieldMacros(SFBool,    currentCamera,        ProtoARSensor)
    fieldMacros(SFBool,    headingOnly,          ProtoARSensor)
    fieldMacros(SFVec3f,   maxPosition,          ProtoARSensor)
    fieldMacros(SFVec3f,   minPosition,          ProtoARSensor)
    fieldMacros(SFFloat,   orientationThreshold, ProtoARSensor)
    fieldMacros(SFFloat,   positionThreshold,    ProtoARSensor)
    fieldMacros(SFVec3f,   invisiblePosition,    ProtoARSensor)
    fieldMacros(SFVec3f,   cameraPosition,       ProtoARSensor)
    fieldMacros(SFVec3f,   cameraOrientation,    ProtoARSensor)
    fieldMacros(SFString,  markerName,           ProtoARSensor)
};

#endif // _NODE_ARSensor_H

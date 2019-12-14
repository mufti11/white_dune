/*
 * NodeEspduTransform.h
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#ifndef _NODE_ESPDU_TRANSFORM_H
#define _NODE_ESPDU_TRANSFORM_H

#ifndef _NODE_H
#include "ExternTheApp.h"
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

#include "DISCommonFields.h"

#include "NodeTransform.h"

class ProtoEspduTransform : public ProtoTransform {
public:
                    ProtoEspduTransform(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_ESPDU_TRANSFORM; }

    virtual bool    isDeclaredInRwd_h() { return false; }

    virtual bool    isX3dInternalProto(void) { return true; }

    disCommonBehaviorFieldIndex()
    disCommonIdentificationFieldIndex()
    disCommonCommunicationFieldIndex()
    FieldIndex articulationParameterCount;
    FieldIndex articulationParameterDesignatorArray;
    FieldIndex articulationParameterChangeIndicatorArray;
    FieldIndex articulationParameterIdPartAttachedToArray;
    FieldIndex articulationParameterTypeArray;
    FieldIndex articulationParameterArray;
    FieldIndex collisionType;
    FieldIndex deadReckoning;
    FieldIndex detonationLocation;
    FieldIndex detonationRelativeLocation;
    FieldIndex detonationResult;
    FieldIndex entityCategory;
    FieldIndex entityCountry;
    FieldIndex entityDomain;
    FieldIndex entityExtra;
    FieldIndex entityKind;
    FieldIndex entitySpecific;
    FieldIndex entitySubCategory;
    FieldIndex eventApplicationID;
    FieldIndex eventEntityID;
    FieldIndex eventNumber;
    FieldIndex eventSiteID;
    FieldIndex fired1;
    FieldIndex fired2;
    FieldIndex fireMissionIndex;
    FieldIndex firingRange;
    FieldIndex firingRate;
    FieldIndex forceID;
    FieldIndex fuse;
    FieldIndex linearVelocity;
    FieldIndex linearAcceleration;
    FieldIndex marking;
    FieldIndex munitionApplicationID;
    FieldIndex munitionEndPoint;
    FieldIndex munitionEntityID;
    FieldIndex munitionQuantity;
    FieldIndex munitionSiteID;
    FieldIndex munitionStartPoint;
    FieldIndex warhead;
};

class NodeEspduTransform : public NodeTransform {
public:
                    NodeEspduTransform(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "DIS"; }
    virtual int     getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() { return new NodeEspduTransform(m_scene, m_proto); }

    disCommonBehaviorFieldMacros(ProtoEspduTransform)
    disCommonIdentificationFieldMacros(ProtoEspduTransform)
    disCommonCommunicationFieldMacros(ProtoEspduTransform)
    fieldMacros(SFInt32,    articulationParameterCount, ProtoEspduTransform)
    fieldMacros(MFInt32,    articulationParameterDesignatorArray, ProtoEspduTransform)
    fieldMacros(MFInt32,    articulationParameterChangeIndicatorArray, ProtoEspduTransform)
    fieldMacros(MFInt32,    articulationParameterIdPartAttachedToArray, ProtoEspduTransform)
    fieldMacros(MFInt32,    articulationParameterTypeArray, ProtoEspduTransform)
    fieldMacros(MFFloat,    articulationParameterArray, ProtoEspduTransform)
    fieldMacros(SFInt32,    collisionType,              ProtoEspduTransform)
    fieldMacros(SFInt32,    deadReckoning,              ProtoEspduTransform)
    fieldMacros(SFVec3f,    detonationLocation,         ProtoEspduTransform)
    fieldMacros(SFVec3f,    detonationRelativeLocation, ProtoEspduTransform)
    fieldMacros(SFInt32,    detonationResult,           ProtoEspduTransform)
    fieldMacros(SFInt32,    entityCategory,             ProtoEspduTransform)
    fieldMacros(SFInt32,    entityCountry,              ProtoEspduTransform)
    fieldMacros(SFInt32,    entityDomain,               ProtoEspduTransform)
    fieldMacros(SFInt32,    entityExtra,                ProtoEspduTransform)
    fieldMacros(SFInt32,    entityKind,                 ProtoEspduTransform)
    fieldMacros(SFInt32,    entitySpecific,             ProtoEspduTransform)
    fieldMacros(SFInt32,    entitySubCategory,          ProtoEspduTransform)
    fieldMacros(SFInt32,    eventApplicationID,         ProtoEspduTransform)
    fieldMacros(SFInt32,    eventEntityID,              ProtoEspduTransform)
    fieldMacros(SFInt32,    eventNumber,                ProtoEspduTransform)
    fieldMacros(SFInt32,    eventSiteID,                ProtoEspduTransform)
    fieldMacros(SFBool,     fired1,                     ProtoEspduTransform)
    fieldMacros(SFBool,     fired2,                     ProtoEspduTransform)
    fieldMacros(SFInt32,    fireMissionIndex,           ProtoEspduTransform)
    fieldMacros(SFFloat,    firingRange,                ProtoEspduTransform)
    fieldMacros(SFInt32,    firingRate,                 ProtoEspduTransform)
    fieldMacros(SFInt32,    forceID,                    ProtoEspduTransform)
    fieldMacros(SFInt32,    fuse,                       ProtoEspduTransform)
    fieldMacros(SFVec3f,    linearVelocity,             ProtoEspduTransform)
    fieldMacros(SFVec3f,    linearAcceleration,         ProtoEspduTransform)
    fieldMacros(SFString,   marking,                    ProtoEspduTransform)
    fieldMacros(SFInt32,    munitionApplicationID,      ProtoEspduTransform)
    fieldMacros(SFVec3f,    munitionEndPoint,           ProtoEspduTransform)
    fieldMacros(SFInt32,    munitionEntityID,           ProtoEspduTransform)
    fieldMacros(SFInt32,    munitionQuantity,           ProtoEspduTransform)
    fieldMacros(SFInt32,    munitionSiteID,             ProtoEspduTransform)
    fieldMacros(SFVec3f,    munitionStartPoint,         ProtoEspduTransform)
    fieldMacros(SFInt32,    warhead,                    ProtoEspduTransform)
};

#endif

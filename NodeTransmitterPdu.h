/*
 * NodeTransmitterPdu.h
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

#ifndef _NODE_TRANSMITTER_PDU_H
#define _NODE_TRANSMITTER_PDU_H

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

#include "DISCommonFields.h"

class ProtoTransmitterPdu : public Proto {
public:
                    ProtoTransmitterPdu(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_TRANSMITTER_PDU; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex antennaLocation;
    FieldIndex antennaPatternLength;
    FieldIndex antennaPatternType;
    FieldIndex cryptoKeyID;
    FieldIndex cryptoSystem;
    FieldIndex frequency;
    FieldIndex inputSource;
    FieldIndex lengthOfModulationParameters;
    FieldIndex modulationTypeDetail;
    FieldIndex modulationTypeMajor;
    FieldIndex modulationTypeSpreadSpectrum;
    FieldIndex modulationTypeSystem;
    FieldIndex power;
    FieldIndex radioEntityTypeCategory;
    FieldIndex radioEntityTypeCountry;
    FieldIndex radioEntityTypeDomain;
    FieldIndex radioEntityTypeKind;
    FieldIndex radioEntityTypeNomenclature;
    FieldIndex radioEntityTypeNomenclatureVersion;
    FieldIndex radioID;
    FieldIndex relativeAntennaLocation;
    FieldIndex transmitFrequencyBandwidth ;
    FieldIndex transmitState;
    FieldIndex whichGeometry;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
    disCommonBehaviorFieldIndex()
    disCommonIdentificationFieldIndex()
    disCommonCommunicationFieldIndex()
};

class NodeTransmitterPdu : public Node {
public:
                    NodeTransmitterPdu(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "DIS"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeTransmitterPdu(*this); }

    fieldMacros(SFVec3f, antennaLocation,              ProtoTransmitterPdu)
    fieldMacros(SFInt32, antennaPatternLength,         ProtoTransmitterPdu)
    fieldMacros(SFInt32, antennaPatternType,           ProtoTransmitterPdu)
    fieldMacros(SFInt32, cryptoKeyID,                  ProtoTransmitterPdu)
    fieldMacros(SFInt32, cryptoSystem,                 ProtoTransmitterPdu)
    fieldMacros(SFInt32, frequency,                    ProtoTransmitterPdu)
    fieldMacros(SFInt32, inputSource,                  ProtoTransmitterPdu)
    fieldMacros(SFInt32, lengthOfModulationParameters, ProtoTransmitterPdu)
    fieldMacros(SFInt32, modulationTypeDetail,         ProtoTransmitterPdu)
    fieldMacros(SFInt32, modulationTypeMajor,          ProtoTransmitterPdu)
    fieldMacros(SFInt32, modulationTypeSpreadSpectrum, ProtoTransmitterPdu)
    fieldMacros(SFInt32, modulationTypeSystem,         ProtoTransmitterPdu)
    fieldMacros(SFFloat, power,                        ProtoTransmitterPdu)
    fieldMacros(SFInt32, radioEntityTypeCategory,      ProtoTransmitterPdu)
    fieldMacros(SFInt32, radioEntityTypeCountry,       ProtoTransmitterPdu)
    fieldMacros(SFInt32, radioEntityTypeDomain,        ProtoTransmitterPdu)
    fieldMacros(SFInt32, radioEntityTypeKind,          ProtoTransmitterPdu)
    fieldMacros(SFInt32, radioEntityTypeNomenclature,  ProtoTransmitterPdu)
    fieldMacros(SFInt32, radioEntityTypeNomenclatureVersion, ProtoTransmitterPdu)
    fieldMacros(SFInt32, radioID,                      ProtoTransmitterPdu)
    fieldMacros(SFVec3f, relativeAntennaLocation,    ProtoTransmitterPdu)
    fieldMacros(SFFloat, transmitFrequencyBandwidth, ProtoTransmitterPdu)
    fieldMacros(SFInt32, transmitState,                ProtoTransmitterPdu)
    fieldMacros(SFInt32, whichGeometry,                ProtoTransmitterPdu)
    fieldMacros(SFVec3f, bboxCenter,                   ProtoTransmitterPdu)
    fieldMacros(SFVec3f, bboxSize,                     ProtoTransmitterPdu)
    disCommonBehaviorFieldMacros(ProtoTransmitterPdu)
    disCommonIdentificationFieldMacros(ProtoTransmitterPdu)
    disCommonCommunicationFieldMacros(ProtoTransmitterPdu)
};

#endif 

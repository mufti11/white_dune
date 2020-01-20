/*
 * NodeSignalPdu.h
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"
#include "Proto.h"
#include "SFMFTypes.h"
#include "DISCommonFields.h"

class ProtoSignalPdu : public Proto {
public:
                    ProtoSignalPdu(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_SIGNAL_PDU; }

    virtual bool    isX3dInternalProto(void) { return true; }

    disCommonBehaviorFieldIndex()
    disCommonIdentificationFieldIndex()
    disCommonCommunicationFieldIndex()
    FieldIndex data;
    FieldIndex dataLength;
    FieldIndex encodingScheme;
    FieldIndex radioID;
    FieldIndex sampleRate;
    FieldIndex samples;
    FieldIndex tdlType;
    FieldIndex whichGeometry;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
};

class NodeSignalPdu : public Node {
public:
                    NodeSignalPdu(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "DIS"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; } 
    virtual Node   *copy() const { return new NodeSignalPdu(*this); }

    disCommonBehaviorFieldMacros(ProtoSignalPdu)
    disCommonIdentificationFieldMacros(ProtoSignalPdu)
    disCommonCommunicationFieldMacros(ProtoSignalPdu)
    fieldMacros(MFInt32, data,           ProtoSignalPdu)
    fieldMacros(SFInt32, dataLength,     ProtoSignalPdu)
    fieldMacros(SFInt32, encodingScheme, ProtoSignalPdu)
    fieldMacros(SFInt32, radioID,        ProtoSignalPdu)
    fieldMacros(SFInt32, sampleRate,     ProtoSignalPdu)
    fieldMacros(SFInt32, samples,        ProtoSignalPdu)
    fieldMacros(SFInt32, tdlType,        ProtoSignalPdu)
    fieldMacros(SFInt32, whichGeometry,  ProtoSignalPdu)
    fieldMacros(SFVec3f, bboxCenter,     ProtoSignalPdu)
    fieldMacros(SFVec3f, bboxSize,       ProtoSignalPdu)
};




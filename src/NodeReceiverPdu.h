/*
 * NodeReceiverPdu.h
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

#ifndef _NODE_RECEIVER_PDU_H
#define _NODE_RECEIVER_PDU_H

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

class ProtoReceiverPdu : public Proto {
public:
                    ProtoReceiverPdu(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_RECEIVER_PDU; }

    virtual bool    isX3dInternalProto(void) { return true; }

    disCommonBehaviorFieldIndex()
    disCommonIdentificationFieldIndex()
    disCommonCommunicationFieldIndex()
    FieldIndex radioID;
    FieldIndex receivedPower;
    FieldIndex receiverState;
    FieldIndex transmitterApplicationID;
    FieldIndex transmitterEntityID;
    FieldIndex transmitterRadioID;
    FieldIndex transmitterSiteID;
    FieldIndex whichGeometry;
    FieldIndex bboxCenter;
    FieldIndex bboxSize;
};

class NodeReceiverPdu : public Node {
public:
                    NodeReceiverPdu(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "DIS"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 0; } 
    virtual Node   *copy() const { return new NodeReceiverPdu(*this); }

    disCommonBehaviorFieldMacros(ProtoReceiverPdu)
    disCommonIdentificationFieldMacros(ProtoReceiverPdu)
    disCommonCommunicationFieldMacros(ProtoReceiverPdu)
    fieldMacros(SFInt32, radioID,                  ProtoReceiverPdu)
    fieldMacros(SFFloat, receivedPower,            ProtoReceiverPdu)
    fieldMacros(SFInt32, receiverState,            ProtoReceiverPdu)
    fieldMacros(SFInt32, transmitterApplicationID, ProtoReceiverPdu)
    fieldMacros(SFInt32, transmitterEntityID,      ProtoReceiverPdu)
    fieldMacros(SFInt32, transmitterRadioID,       ProtoReceiverPdu)
    fieldMacros(SFInt32, transmitterSiteID,        ProtoReceiverPdu)
    fieldMacros(SFInt32, whichGeometry,            ProtoReceiverPdu)
    fieldMacros(SFVec3f, bboxCenter,               ProtoReceiverPdu)
    fieldMacros(SFVec3f, bboxSize,                 ProtoReceiverPdu)
};

#endif

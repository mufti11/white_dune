/*
 * NodeReceiverPdu.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "NodeReceiverPdu.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoReceiverPdu::ProtoReceiverPdu(Scene *scene)
  : Proto(scene, "ReceiverPdu")
{
    radioID.set(
          addExposedField(SFINT32, "radioID", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    receivedPower.set(
          addExposedField(SFFLOAT, "receivedPower", new SFFloat(0.0),
                          new SFFloat(0.0f)));
    receiverState.set(
          addExposedField(SFINT32, "receiverState", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    transmitterApplicationID.set(
          addExposedField(SFINT32, "transmitterApplicationID", new SFInt32(1),
                          new SFInt32(0), new SFInt32(65535)));
    transmitterEntityID.set(
          addExposedField(SFINT32, "transmitterEntityID", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    transmitterRadioID.set(
          addExposedField(SFINT32, "transmitterRadioID", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    transmitterSiteID.set(
          addExposedField(SFINT32, "transmitterSiteID", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    whichGeometry.set(
          addExposedField(SFINT32, "whichGeometry", new SFInt32(1),
                          new SFInt32(-1)));
    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1), 
                   new SFFloat(-1.0f)));
    disCommonBehaviorFields()
    disCommonIdentificationFields()
    disCommonCommunicationFields()
}

Node *
ProtoReceiverPdu::create(Scene *scene)
{ 
    return new NodeReceiverPdu(scene, this); 
}

NodeReceiverPdu::NodeReceiverPdu(Scene *scene, Proto *def)
  : Node(scene, def)
{
}


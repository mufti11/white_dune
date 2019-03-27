/*
 * NodeSignalPdu.cpp
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

#include "NodeSignalPdu.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoSignalPdu::ProtoSignalPdu(Scene *scene)
  : Proto(scene, "SignalPdu")
{
    data.set(
          addExposedField(MFINT32, "data", new MFInt32(),
                          new SFInt32(0), new SFInt32(255)));
    dataLength.set(
          addExposedField(SFINT32, "dataLength", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    encodingScheme.set(
          addExposedField(SFINT32, "encodingScheme", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    radioID.set(
          addExposedField(SFINT32, "radioID", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    sampleRate.set(
          addExposedField(SFINT32, "sampleRate", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    samples.set(
          addExposedField(SFINT32, "samples", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    tdlType.set(
          addExposedField(SFINT32, "tdlType", new SFInt32(0),
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
ProtoSignalPdu::create(Scene *scene)
{ 
    return new NodeSignalPdu(scene, this); 
}

NodeSignalPdu::NodeSignalPdu(Scene *scene, Proto *def)
  : Node(scene, def)
{
}


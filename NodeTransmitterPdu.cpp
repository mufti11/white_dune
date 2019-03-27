/*
 * NodeTransmitterPdu.cpp
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

#include "NodeTransmitterPdu.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFString.h"
#include "DuneApp.h"

ProtoTransmitterPdu::ProtoTransmitterPdu(Scene *scene)
  : Proto(scene, "TransmitterPdu")
{
    antennaLocation.set(
          addExposedField(SFVEC3F, "antennaLocation", new SFVec3f(0, 0, 0)));
    antennaPatternLength.set(
          addExposedField(SFINT32, "antennaPatternLength", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    antennaPatternType.set(
          addExposedField(SFINT32, "antennaPatternType", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    cryptoKeyID.set(
          addExposedField(SFINT32, "cryptoKeyID", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    cryptoSystem.set(
          addExposedField(SFINT32, "cryptoSystem", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    frequency.set(
          addExposedField(SFINT32, "frequency", new SFInt32(0),
                          new SFInt32(0)));
    inputSource.set(
          addExposedField(SFINT32, "inputSource", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));
    lengthOfModulationParameters.set(
          addExposedField(SFINT32, "lengthOfModulationParameters", 
                          new SFInt32(0), new SFInt32(0), new SFInt32(255)));
    modulationTypeDetail.set(
          addExposedField(SFINT32, "modulationTypeDetail", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    modulationTypeMajor.set(
          addExposedField(SFINT32, "modulationTypeMajor", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    modulationTypeSpreadSpectrum.set(
          addExposedField(SFINT32, "modulationTypeSpreadSpectrum", 
                          new SFInt32(0), new SFInt32(0), new SFInt32(65535)));
    modulationTypeSystem.set(
          addExposedField(SFINT32, "modulationTypeSystem", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    power.set(
          addExposedField(SFFLOAT, "power", new SFFloat(0),
                          new SFFloat(0)));
    radioEntityTypeCategory.set(
          addExposedField(SFINT32, "radioEntityTypeCategory", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));
    radioEntityTypeCountry.set(
          addExposedField(SFINT32, "radioEntityTypeCountry", new SFInt32(0),
                          new SFInt32(0), new SFInt32(65535)));
    radioEntityTypeDomain.set(
          addExposedField(SFINT32, "radioEntityTypeDomain", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));
    radioEntityTypeKind.set(
          addExposedField(SFINT32, "radioEntityTypeKind", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));
    radioEntityTypeNomenclature.set(
          addExposedField(SFINT32, "radioEntityTypeNomenclature", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));
    radioEntityTypeNomenclatureVersion.set(
          addExposedField(SFINT32, "radioEntityTypeNomenclatureVersion",
                          new SFInt32(0), new SFInt32(0), new SFInt32(65535)));
    radioID.set(
          addExposedField(SFINT32, "radioID", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));
    relativeAntennaLocation.set(
          addExposedField(SFVEC3F, "relativeAntennaLocation", 
                          new SFVec3f(0, 0, 0)));
    transmitFrequencyBandwidth.set(
          addExposedField(SFFLOAT, "transmitFrequencyBandwidth", 
                          new SFFloat(0.0f)));
    transmitState.set(
          addExposedField(SFINT32, "transmitState", new SFInt32(0),
                          new SFInt32(0), new SFInt32(255)));
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
ProtoTransmitterPdu::create(Scene *scene)
{ 
    return new NodeTransmitterPdu(scene, this); 
}

NodeTransmitterPdu::NodeTransmitterPdu(Scene *scene, Proto *def)
  : Node(scene, def)
{
}


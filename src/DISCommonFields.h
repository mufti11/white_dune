/*
 * DISCommonFields.h
 *
 * Copyright (C) 2008 J. "MUFTI" Scheurich
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

#define disCommonBehaviorFieldIndex() \
    FieldIndex enabled; \
    FieldIndex networkMode; \
    FieldIndex readInterval; \
    FieldIndex writeInterval;

#define disCommonBehaviorFieldMacros(proto) \
    fieldMacros(SFBool,   enabled,       proto) \
    fieldMacros(SFString, networkMode,   proto) \
    fieldMacros(SFTime,   readInterval,  proto) \
    fieldMacros(SFTime,   writeInterval, proto)

#define disCommonBehaviorFields() \
    enabled.set( \
          addExposedField(SFBOOL, "enabled", new SFBool(true)));  \
    networkMode.set( \
          addExposedField(SFSTRING, "networkMode",  \
                          new SFString("standAlone")));  \
    readInterval.set( \
          addExposedField(SFTIME, "readInterval", new SFTime(0.1f))); \
    writeInterval.set( \
          addExposedField(SFTIME, "writeInterval", new SFTime(1.0f),  \
                          new SFTime(0.0f))); \
    addEventOut(SFBOOL, "isActive"); \
    addEventOut(SFBOOL, "isNetworkReader"); \
    addEventOut(SFBOOL, "isNetworkWriter"); \
    addEventOut(SFBOOL, "isStandAlone"); \
    addEventOut(SFTIME, "timestamp"); 

#define disCommonIdentificationFieldIndex() \
    FieldIndex siteID; \
    FieldIndex applicationID; \
    FieldIndex entityID;

#define disCommonIdentificationFieldMacros(proto) \
    fieldMacros(SFInt32, siteID,        proto) \
    fieldMacros(SFInt32, applicationID, proto) \
    fieldMacros(SFInt32, entityID,      proto)

#define disCommonIdentificationFields() \
    siteID.set( \
          addExposedField(SFINT32, "siteID", new SFInt32(0), \
                          new SFInt32(0), new SFInt32(65535))); \
    applicationID.set( \
          addExposedField(SFINT32, "applicationID", new SFInt32(1),  \
                          new SFInt32(0), new SFInt32(65535))); \
    entityID.set( \
          addExposedField(SFINT32, "entityID", new SFInt32(0), \
                          new SFInt32(0), new SFInt32(65535)));


#define disCommonCommunicationFieldIndex() \
    FieldIndex address; \
    FieldIndex port; \
    FieldIndex multicastRelayHost; \
    FieldIndex multicastRelayPort; \
    FieldIndex rtpHeaderExpected; 

#define disCommonCommunicationFieldMacros(proto) \
    fieldMacros(SFString, address,            proto) \
    fieldMacros(SFInt32,  port,               proto) \
    fieldMacros(SFString, multicastRelayHost, proto) \
    fieldMacros(SFInt32,  multicastRelayPort, proto) \
    fieldMacros(SFBool,   rtpHeaderExpected,  proto)

#define disCommonCommunicationFields() \
    address.set( \
          addExposedField(SFSTRING, "address", new SFString())); \
    port.set( \
          addExposedField(SFINT32, "port", new SFInt32(0), \
                          new SFInt32(0), new SFInt32(65535))); \
    multicastRelayHost.set( \
          addExposedField(SFSTRING, "multicastRelayHost", new SFString())); \
    multicastRelayPort.set( \
          addExposedField(SFINT32, "multicastRelayPort", new SFInt32())); \
    rtpHeaderExpected.set( \
          addField(SFBOOL, "rtpHeaderExpected", new SFBool(false))); \
    addEventOut(SFBOOL, "isRtpHeaderHeard");

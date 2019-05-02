/*
 * X3domLightCommonFields.h
 *
 * Copyright (C) 2018 J. "MUFTI" Scheurich
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

#ifndef _X3DOM_LIGHT_COMMON_FIELDS_H
#define _X3DOM_LIGHT_COMMON_FIELDS_H

#define x3domLightCommonFieldIndex() \
    FieldIndex shadowFilterSize; \
    FieldIndex shadowIntensity; \
    FieldIndex shadowMapSize; \
    FieldIndex shadowOffset; \
    FieldIndex zFar; \
    FieldIndex zNear;

#define x3domLightCommonFieldMacros(proto) \
    fieldMacros(SFInt32, shadowFilterSize, proto) \
    fieldMacros(SFFloat, shadowIntensity,  proto) \
    fieldMacros(SFInt32, shadowMapSize,    proto) \
    fieldMacros(SFFloat, shadowOffset,     proto) \
    fieldMacros(SFFloat, zFar,             proto) \
    fieldMacros(SFFloat, zNear,            proto)


#define x3domLightCommonFields() \
    shadowFilterSize.set( \
          addField(SFINT32, "shadowFilterSize", new SFInt32(0)));  \
    setFieldFlags(shadowFilterSize, FF_X3DOM_ONLY); \
    shadowIntensity.set( \
          addField(SFFLOAT, "shadowIntensity", new SFFloat(0)));  \
    setFieldFlags(shadowIntensity, FF_X3DOM_ONLY); \
    shadowMapSize.set( \
          addField(SFINT32, "shadowMapSize", new SFInt32(1024)));  \
    setFieldFlags(shadowMapSize, FF_X3DOM_ONLY); \
    shadowOffset.set( \
          addField(SFFLOAT, "shadowOffset", new SFFloat(0)));  \
    setFieldFlags(shadowOffset, FF_X3DOM_ONLY); \
    zFar.set( \
          addField(SFFLOAT, "zFar", new SFFloat(-1)));  \
    setFieldFlags(zFar, FF_X3DOM_ONLY); \
    zNear.set( \
          addField(SFFLOAT, "zNear", new SFFloat(-1)));  \
    setFieldFlags(zNear, FF_X3DOM_ONLY);
#endif

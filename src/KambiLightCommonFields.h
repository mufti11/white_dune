/*
 * KambiLightCommonFields.h
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

#define kambiLightCommonFieldIndex() \
    FieldIndex shadows; \
    FieldIndex kambiShadows; \
    FieldIndex kambiShadowsMain; \
    FieldIndex projectionNear; \
    FieldIndex projectionFar; \
    FieldIndex up; \
    FieldIndex defaultShadowMap; \
    FieldIndex effects; \
    FieldIndex shadowVolumes; \
    FieldIndex shadowVolumesMain;


#define kambiLightCommonFieldMacros(proto) \
    fieldMacros(SFBool,  shadows,           proto) \
    fieldMacros(SFBool,  kambiShadows,      proto) \
    fieldMacros(SFBool,  kambiShadowsMain,  proto) \
    fieldMacros(SFFloat, projectionNear,    proto) \
    fieldMacros(SFFloat, projectionFar,     proto) \
    fieldMacros(SFVec3f, up,                proto) \
    fieldMacros(SFNode,  defaultShadowMap,  proto) \
    fieldMacros(MFNode,  effects,           proto) \
    fieldMacros(SFBool,  shadowVolumes,     proto) \
    fieldMacros(SFBool,  shadowVolumesMain, proto) 

#define kambiLightCommonFields() \
    shadows.set( \
          addField(SFBOOL, "shadows", new SFBool(false)));  \
    setFieldFlags(shadows, FF_KAMBI_ONLY); \
    kambiShadows.set( \
          addExposedField(SFBOOL, "kambiShadows", new SFBool(false)));  \
    setFieldFlags(kambiShadows, FF_KAMBI_ONLY); \
    kambiShadowsMain.set( \
          addExposedField(SFBOOL, "kambiShadowsMain", new SFBool(false))); \
    setFieldFlags(kambiShadowsMain, FF_KAMBI_ONLY); \
    projectionNear.set( \
          addExposedField(SFFLOAT, "projectionNear", new SFFloat(1.0f)));  \
    setFieldFlags(projectionNear, FF_KAMBI_ONLY); \
    projectionFar.set( \
          addExposedField(SFFLOAT, "projectionFar", new SFFloat(100.0f)));  \
    setFieldFlags(projectionFar, FF_KAMBI_ONLY); \
    up.set( \
          addExposedField(SFVEC3F, "up", new SFVec3f(0.0f, 1.0f, 0.0f)));  \
    setFieldFlags(up, FF_KAMBI_ONLY); \
    defaultShadowMap.set( \
          addField(SFNODE, "defaultShadowMap", new SFNode(), \
                   KAMBI_GENERATED_SHADOW_MAP));  \
    setFieldFlags(defaultShadowMap, FF_KAMBI_ONLY); \
    effects.set( \
          addField(MFNODE, "effects", new MFNode(), KAMBI_EFFECT)); \
    setFieldFlags(effects, FF_KAMBI_ONLY); \
    shadowVolumes.set( \
          addField(SFBOOL, "shadowVolumes", new SFBool(false)));  \
    setFieldFlags(shadowVolumes, FF_KAMBI_ONLY); \
    shadowVolumesMain.set( \
          addField(SFBOOL, "shadowVolumesMain", new SFBool(false)));  \
    setFieldFlags(shadowVolumesMain, FF_KAMBI_ONLY);


/*
 * FogMacros.h
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#ifndef _FOG_MACROS_H
#define _FOG_MACROS_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif

#define FogProtoMacro() \
    FieldIndex volumetric; \
    FieldIndex volumetricDirection; \
    FieldIndex volumetricVisibilityStart;


#define FogFieldMacros(proto) \
    fieldMacros(SFBool,   volumetric,                proto) \
    fieldMacros(SFVec3f,  volumetricDirection,       proto) \
    fieldMacros(SFFloat,  volumetricVisibilityStart, proto)

#define FogElements() \
    volumetric.set(                                                  \
          addExposedField(SFBOOL, "volumetric", new SFBool(false))); \
    setFieldFlags(volumetric, FF_KAMBI_ONLY);                        \
    volumetricDirection.set(                                         \
          addExposedField(SFVEC3F, "volumetricDirection",            \
                          new SFVec3f(0.0f, -1.0f, 0.0f)));          \
    setFieldFlags(volumetricDirection, FF_KAMBI_ONLY);               \
    volumetricVisibilityStart.set(                                   \
          addExposedField(SFFLOAT, "volumetricVisibilityStart",      \
                          new SFFloat(0.0f)));                       \
    setFieldFlags(volumetricVisibilityStart, FF_KAMBI_ONLY); 

#endif

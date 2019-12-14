/*
 * ComposedGeometryMacros.h
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

#pragma once

#include "Node.h"
#include "ProtoMacros.h"

#define ComposedGeometryProtoMacro() \
    FieldIndex radianceTransfer; \
    FieldIndex attrib; \
    FieldIndex fogCoord;

#define ComposedGeometryFieldMacros(proto) \
    fieldMacros(MFVec3f, radianceTransfer, proto) \
    fieldMacros(MFNode,  attrib,           proto) \
    fieldMacros(SFNode,  fogCoord,         proto)

#define ComposedGeometryElements() \
    radianceTransfer.set( \
          addExposedField(MFVEC3F, "radianceTransfer", new MFVec3f())); \
    setFieldFlags(radianceTransfer, FF_KAMBI_ONLY); \
    attrib.set( \
          addExposedField(MFNODE, "attrib", new MFNode(), \
                          VERTEX_ATTRIBUTE_NODE)); \
    setFieldFlags(attrib, FF_X3D_ONLY); \
    fogCoord.set( \
          addExposedField(SFNODE, "fogCoord", new SFNode(NULL), \
                          X3D_FOG_COORDINATE)); \
    setFieldFlags(fogCoord, FF_X3D_ONLY); 

/*
 * GeometryNode.h
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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
#include "NodeAppearance.h"
#include "NodeMaterial.h"
#include "NodeImageTexture.h"
#include "NodeTextureTransform.h"
#include "GeometryNodeMacros.h"

class GeometryProto : public WonderlandExportProto {
public:
                    GeometryProto(Scene *scene, const char *name) : 
                          WonderlandExportProto(scene, name) {}

    virtual int     getNodeClass() const { return GEOMETRY_NODE; }
};

class GeometryNode : public Node {
public:
                         GeometryNode(Scene *scene, Proto *proto);

     GeometryNodeDeclarationMacros()
};

#define x3domGeometryCommonFieldIndex() \
    FieldIndex lit; \
    FieldIndex useGeoCache;

#define x3domGeometryCommonFieldMacros(proto) \
    fieldMacros(SFBool,  lit,             proto) \
    fieldMacros(SFBool,  useGeoCache,     proto)

#define x3domGeometryCommonFields() \
    lit.set( \
          addField(SFBOOL, "lit", new SFBool(true)));  \
    setFieldFlags(lit, FF_X3DOM_ONLY); \
    useGeoCache.set( \
          addField(SFBOOL, "useGeoCache", new SFBool(true)));  \
    setFieldFlags(useGeoCache, FF_X3DOM_ONLY);



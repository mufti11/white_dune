/*
 * NodeProjectedTextureCoordinate.h
 *
 * Copyright (C) 1999 Stephen F. White, 2010 J. "MUFTI" Scheurich
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
#include "Proto.h"
#include "KambiCommonFunctions.h"
#include "SFMFTypes.h"

class ProtoProjectedTextureCoordinate : public Proto {
public:
                    ProtoProjectedTextureCoordinate(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const 
                       { return KAMBI_PROJECTED_TEXTURE_COORDINATE; }
    virtual int     getNodeClass() const 
                       { 
                       return TEXTURE_COORDINATE_NODE | 
                              GENERATED_TEXTURE_COORDINATE_NODE; 
                       }

    FieldIndex projector;

    virtual bool          isKambiProto(void) { return true; }
};

class NodeProjectedTextureCoordinate : public Node {
public:
                    NodeProjectedTextureCoordinate(Scene *scene, Proto *proto);

    virtual Node   *copy() const { return new NodeProjectedTextureCoordinate(*this); }

    kambiCommonFunctions()

    fieldMacros(SFNode, projector, ProtoProjectedTextureCoordinate)
};


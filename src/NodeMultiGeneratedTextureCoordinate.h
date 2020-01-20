/*
 * NodeMultiGeneratedTextureCoordinate.h
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
#include "SFMFTypes.h"

class ProtoMultiGeneratedTextureCoordinate : public Proto {
public:
                    ProtoMultiGeneratedTextureCoordinate(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const 
                       { return KAMBI_MULTI_GENERATED_TEXTURE_COORDINATE; }
    virtual int     getNodeClass() const 
                       { return GENERATED_TEXTURE_COORDINATE_NODE; }

    FieldIndex texCoord;

    virtual bool    isKambiProto(void) { return true; }
};

class NodeMultiGeneratedTextureCoordinate : public Node {
public:
                    NodeMultiGeneratedTextureCoordinate(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const 
                           { return "Texturing"; }
    virtual int         getComponentLevel(void) const { return -1; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const 
                       { return new NodeMultiGeneratedTextureCoordinate(*this); }

    virtual bool    isInvalidChildNode(void) { return true; }

    void            setField(int index, FieldValue *value, int cf = -1);

    fieldMacros(MFNode, texCoord, ProtoMultiGeneratedTextureCoordinate);
};



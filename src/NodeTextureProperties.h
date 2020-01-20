/*
 * NodeTextureProperties.h
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
#include "Proto.h"
#include "SFMFTypes.h"

class ProtoTextureProperties : public Proto {
public:
                    ProtoTextureProperties(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_TEXTURE_PROPERTIES; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex anisotropicDegree;
    FieldIndex borderColor;
    FieldIndex borderWidth;
    FieldIndex boundaryModeS;
    FieldIndex boundaryModeT;
    FieldIndex boundaryModeR;
    FieldIndex magnificationFilter;
    FieldIndex minificationFilter;
    FieldIndex textureCompression;
    FieldIndex texturePriority;
    FieldIndex generateMipMaps;
    FieldIndex guiTexture;
};

class NodeTextureProperties : public Node {
public:
                    NodeTextureProperties(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Texturing"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 2; }
    virtual Node   *copy() const { return new NodeTextureProperties(*this); }

    fieldMacros(SFFloat, anisotropicDegree, ProtoTextureProperties);
    fieldMacros(SFColorRGBA, borderColor, ProtoTextureProperties);
    fieldMacros(SFInt32, borderWidth, ProtoTextureProperties);
    fieldMacros(SFString, boundaryModeS, ProtoTextureProperties);
    fieldMacros(SFString, boundaryModeT, ProtoTextureProperties);
    fieldMacros(SFString, boundaryModeR, ProtoTextureProperties);
    fieldMacros(SFString, magnificationFilter, ProtoTextureProperties);
    fieldMacros(SFString, minificationFilter, ProtoTextureProperties);
    fieldMacros(SFString, textureCompression, ProtoTextureProperties);
    fieldMacros(SFFloat, texturePriority, ProtoTextureProperties);
    fieldMacros(SFBool, generateMipMaps, ProtoTextureProperties);
    fieldMacros(SFBool, guiTexture, ProtoTextureProperties);
};


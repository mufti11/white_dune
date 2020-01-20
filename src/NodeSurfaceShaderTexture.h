/*
 * NodeSurfaceShaderTexture.h
 *
 * Copyright (C) 1999 Stephen F. White
 * 
 * Modified by Aaron Cram - Now uses DevIL to load textures
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

class ProtoSurfaceShaderTexture : public Proto {
public:
                    ProtoSurfaceShaderTexture(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_SURFACE_SHADER_TEXTURE; }
    virtual int     getNodeClass() const 
                       { return TEXTURE_NODE | TEXTURE_2D_NODE | URL_NODE; }

    FieldIndex channelMask;
    FieldIndex crossOrigin;
    FieldIndex isSRGB;
    FieldIndex origChannelCount;
    FieldIndex repeatS;
    FieldIndex repeatT;
    FieldIndex scale;
    FieldIndex texture;
    FieldIndex textureCoordinatesId;
    FieldIndex textureProperties;
    FieldIndex textureTransform;
    FieldIndex url;
};

class NodeSurfaceShaderTexture : public Node {
public:
                    NodeSurfaceShaderTexture(Scene *scene, Proto *proto);
                    NodeSurfaceShaderTexture(const NodeSurfaceShaderTexture &node);
    virtual        ~NodeSurfaceShaderTexture();

    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeSurfaceShaderTexture(*this); }

    virtual void    load();

    fieldMacros(SFString, channelMask,          ProtoSurfaceShaderTexture)
    fieldMacros(SFString, crossOrigin,          ProtoSurfaceShaderTexture)
    fieldMacros(SFBool,   isSRGB,               ProtoSurfaceShaderTexture)
    fieldMacros(SFInt32,  origChannelCount,     ProtoSurfaceShaderTexture)
    fieldMacros(SFBool,   repeatS,              ProtoSurfaceShaderTexture)
    fieldMacros(SFBool,   repeatT,              ProtoSurfaceShaderTexture)
    fieldMacros(SFBool,   scale,                ProtoSurfaceShaderTexture)
    fieldMacros(SFNode,   texture,              ProtoSurfaceShaderTexture)
    fieldMacros(SFInt32,  textureCoordinatesId, ProtoSurfaceShaderTexture)
    fieldMacros(SFNode,   textureProperties,    ProtoSurfaceShaderTexture)
    fieldMacros(SFNode,   textureTransform,     ProtoSurfaceShaderTexture)
    fieldMacros(MFString, url,                  ProtoSurfaceShaderTexture)

protected:
    MyString        m_baseURL;
};


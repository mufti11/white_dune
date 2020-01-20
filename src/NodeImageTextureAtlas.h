/*
 * NodeImageTextureAtlas.h
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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

class ProtoImageTextureAtlas : public Proto {
public:
                    ProtoImageTextureAtlas(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3DOM_IMAGE_TEXTURE_ATLAS; }
    virtual int     getNodeClass() const { return TEXTURE_NODE; }

    FieldIndex url;
    FieldIndex repeatS;
    FieldIndex repeatT;
    FieldIndex textureProperties;
    FieldIndex crossOrigin;
    FieldIndex hideChildren;
    FieldIndex numberOfSlices;
    FieldIndex origChannelCount;
    FieldIndex scale;
    FieldIndex slicesOverX;
    FieldIndex slicesOverY;
};

class NodeImageTextureAtlas : public Node {
public:
                    NodeImageTextureAtlas(Scene *scene, Proto *proto);
                    NodeImageTextureAtlas(const NodeImageTextureAtlas &node);
    virtual        ~NodeImageTextureAtlas();

    virtual int     getX3dVersion(void) const { return -1; }
    virtual Node   *copy() const { return new NodeImageTextureAtlas(*this); }

    virtual void    load();

    virtual bool    isInvalidChildNode(void) { return true; }

    fieldMacros(MFString, url,               ProtoImageTextureAtlas)
    fieldMacros(SFBool,   repeatS,           ProtoImageTextureAtlas)
    fieldMacros(SFBool,   repeatT,           ProtoImageTextureAtlas)
    fieldMacros(SFNode,   textureProperties, ProtoImageTextureAtlas);
    fieldMacros(SFString, crossOrigin,       ProtoImageTextureAtlas);
    fieldMacros(SFBool,   hideChildren,      ProtoImageTextureAtlas);
    fieldMacros(SFInt32,  numberOfSlices,    ProtoImageTextureAtlas);
    fieldMacros(SFInt32,  origChannelCount,  ProtoImageTextureAtlas);
    fieldMacros(SFBool,   scale,             ProtoImageTextureAtlas);
    fieldMacros(SFInt32,  slicesOverX,       ProtoImageTextureAtlas);
    fieldMacros(SFInt32,  slicesOverY,       ProtoImageTextureAtlas);

protected:
    MyString        m_baseURL;
};


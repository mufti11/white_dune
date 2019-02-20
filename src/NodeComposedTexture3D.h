/*
 * NodeComposedTexture3D.h
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

#ifndef _NODE_COMPOSED_TEXTURE_3D_H
#define _NODE_COMPOSED_TEXTURE_3D_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "SFMFTypes.h"

#include "KambiTextureCommonFields.h"

class ProtoComposedTexture3D : public Proto {
public:
                    ProtoComposedTexture3D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_COMPOSED_TEXTURE_3D; }
    virtual int     getNodeClass() const 
                       { return TEXTURE_3D_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex texture;
    FieldIndex repeatS;
    FieldIndex repeatR;
    FieldIndex repeatT;
    kambiTextureCommonFieldIndex()
    FieldIndex crossOrigin;
    FieldIndex origChannelCount;
    FieldIndex scale;
    FieldIndex textureProperties;
    FieldIndex url;
};

class NodeComposedTexture3D : public Node {
public:
                    NodeComposedTexture3D(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Texturing3D"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 1; }
    virtual Node   *copy() const { return new NodeComposedTexture3D(*this); }

    fieldMacros(MFNode, texture, ProtoComposedTexture3D);
    fieldMacros(SFBool, repeatS, ProtoComposedTexture3D);
    fieldMacros(SFBool, repeatR, ProtoComposedTexture3D);
    fieldMacros(SFBool, repeatT, ProtoComposedTexture3D);
    kambiTextureCommonFieldMacros(ProtoComposedTexture3D)

    fieldMacros(SFString, crossOrigin, ProtoComposedTexture3D);
    fieldMacros(SFInt32, origChannelCount, ProtoComposedTexture3D);
    fieldMacros(SFBool, scale, ProtoComposedTexture3D);
    fieldMacros(SFNode, textureProperties, ProtoComposedTexture3D);
    fieldMacros(MFString, url, ProtoComposedTexture3D);

public:
    int m_textureTableIndex;
};

#endif

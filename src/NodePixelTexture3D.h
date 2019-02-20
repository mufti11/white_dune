/*
 * NodePixelTexture3D.h
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

#ifndef _NODE_PIXEL_TEXTURE_3D_H
#define _NODE_PIXEL_TEXTURE_3D_H

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

class ProtoPixelTexture3D : public Proto {
public:
                    ProtoPixelTexture3D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_PIXEL_TEXTURE_3D; }
    virtual int     getNodeClass() const 
                       { return TEXTURE_3D_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex image;
    FieldIndex repeatS;
    FieldIndex repeatR;
    FieldIndex repeatT;
    FieldIndex textureProperties;
    kambiTextureCommonFieldIndex()
    FieldIndex crossOrigin;
    FieldIndex origChannelCount;
    FieldIndex scale;
};

class NodePixelTexture3D : public Node {
public:
                    NodePixelTexture3D(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Texturing3D"; }
    virtual int         getComponentLevel(void) const { return 1; }
    virtual int     getX3dVersion(void) const { return 1; } 
    virtual Node   *copy() const { return new NodePixelTexture3D(*this); }

    fieldMacros(MFInt32, image,             ProtoPixelTexture3D);
    fieldMacros(SFBool,  repeatS,           ProtoPixelTexture3D);
    fieldMacros(SFBool,  repeatR,           ProtoPixelTexture3D);
    fieldMacros(SFBool,  repeatT,           ProtoPixelTexture3D);
    fieldMacros(SFNode,  textureProperties, ProtoPixelTexture3D);
    kambiTextureCommonFieldMacros(ProtoPixelTexture3D)
    fieldMacros(SFString, crossOrigin,       ProtoPixelTexture3D);
    fieldMacros(SFInt32,  origChannelCount,  ProtoPixelTexture3D);
    fieldMacros(SFBool,   scale,             ProtoPixelTexture3D);

public:
    int m_textureTableIndex;
};

#endif

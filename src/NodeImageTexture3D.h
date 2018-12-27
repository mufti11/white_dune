/*
 * NodeImageTexture3D.h
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

#ifndef _NODE_IMAGE_TEXTURE_3D_H
#define _NODE_IMAGE_TEXTURE_3D_H

#ifndef _NODE_H
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

#include "KambiTextureCommonFields.h"

#include "SFMFTypes.h"

class ProtoImageTexture3D : public Proto {
public:
                    ProtoImageTexture3D(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_IMAGE_TEXTURE_3D; }
    virtual int     getNodeClass() const 
                       { return TEXTURE_3D_NODE | URL_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex url;
    FieldIndex repeatS;
    FieldIndex repeatT;
    FieldIndex repeatR;
    FieldIndex textureProperties;
    kambiTextureCommonFieldIndex()
    FieldIndex crossOrigin;
    FieldIndex hideChildren;
    FieldIndex origChannelCount;
    FieldIndex scale;
};

class NodeImageTexture3D : public Node {
public:
                    NodeImageTexture3D(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Texturing3D"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 1; }
    virtual Node   *copy() const { return new NodeImageTexture3D(*this); }

    fieldMacros(MFString, url,               ProtoImageTexture3D);
    fieldMacros(SFBool,   repeatS,           ProtoImageTexture3D);
    fieldMacros(SFBool,   repeatT,           ProtoImageTexture3D);
    fieldMacros(SFBool,   repeatR,           ProtoImageTexture3D);
    fieldMacros(SFNode,   textureProperties, ProtoImageTexture3D);
    kambiTextureCommonFieldMacros(ProtoImageTexture3D)
    fieldMacros(SFString, crossOrigin,       ProtoImageTexture3D);
    fieldMacros(SFBool,   hideChildren,      ProtoImageTexture3D);
    fieldMacros(SFInt32,  origChannelCount,  ProtoImageTexture3D);
    fieldMacros(SFBool,   scale,             ProtoImageTexture3D);
};

#endif

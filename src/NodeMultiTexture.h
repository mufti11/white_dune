/*
 * NodeMultitexture.h
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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

#ifndef _NODE_MULTI_TEXTURE_H
#define _NODE_MULTI_TEXTURE_H

#ifndef _NODE_H
#include "ExternTheApp.h"
#include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
#include "Proto.h"
#endif

class MeshBasedNode;

#include "KambiTextureCommonFields.h"

#include "SFMFTypes.h"

class ProtoMultiTexture : public Proto {
public:
                    ProtoMultiTexture(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return X3D_MULTI_TEXTURE; }
    virtual int     getNodeClass() const { return TEXTURE_NODE; }

    virtual bool    isX3dInternalProto(void) { return true; }

    FieldIndex alpha;
    FieldIndex color;
    FieldIndex function;
    FieldIndex mode;
    FieldIndex source;
    FieldIndex texture;
    kambiTextureCommonFieldIndex()
    FieldIndex crossOrigin;
    FieldIndex origChannelCount;
    FieldIndex scale;
    FieldIndex url;
};


class NodeMultiTexture : public Node {
public:
                    NodeMultiTexture(Scene *scene, Proto *proto);

    virtual const char* getComponentName(void) const { return "Texturing"; }
    virtual int         getComponentLevel(void) const { return 2; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node *copy() { return new NodeMultiTexture(m_scene, m_proto); }

    void            bind();
    void            unbind();

    virtual void    setField(int index, FieldValue *value, int cf = -1);

    virtual bool    isInvalidChildNode(void) { return true; }

    virtual bool    isTransparent(void);
   
    virtual int     writeProto(int f) { return writeX3dProto(f); }

    fieldMacros(SFFloat,  alpha,    ProtoMultiTexture)
    fieldMacros(SFColor,  color,    ProtoMultiTexture)
    fieldMacros(MFString, function, ProtoMultiTexture)
    fieldMacros(MFString, mode,     ProtoMultiTexture)
    fieldMacros(MFString, source,   ProtoMultiTexture)
    fieldMacros(MFNode,   texture,  ProtoMultiTexture)
    kambiTextureCommonFieldMacros(ProtoMultiTexture)
    fieldMacros(SFString, crossOrigin,      ProtoMultiTexture);
    fieldMacros(SFInt32,  origChannelCount, ProtoMultiTexture);
    fieldMacros(SFBool,   scale,            ProtoMultiTexture);
    fieldMacros(MFString, url,              ProtoMultiTexture);

protected:
    int m_maxTextureUnits;
    GLuint *m_textureObjects;
    MyArray<int> m_modeInt;
};

#endif // _NODE_MULTI_TEXTURE_H

/*
 * NodePixelTexture.h
 *
 * Copyright (C) 1999 Stephen F. White
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

#ifndef _NODE_PIXELTEXTURE_H
#define _NODE_PIXELTEXTURE_H

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

class ProtoPixelTexture : public Proto {
public:
                    ProtoPixelTexture(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_PIXEL_TEXTURE; }
    virtual int     getNodeClass() const 
                       { return TEXTURE_NODE | TEXTURE_2D_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }      

    FieldIndex image;
    FieldIndex repeatS;
    FieldIndex repeatT;
    FieldIndex blendMode;
    FieldIndex textureProperties;
    kambiTextureCommonFieldIndex()
    kambiVrmlTextureCommonFieldIndex()
    FieldIndex crossOrigin;
    FieldIndex origChannelCount;
    FieldIndex scale;
};

class NodePixelTexture : public Node {
public:
                    NodePixelTexture(Scene *scene, Proto *proto);
                    NodePixelTexture(const NodePixelTexture &node);
    virtual        ~NodePixelTexture();

    virtual int     getProfile(void) const;
    virtual const char* getComponentName(void) const;
    virtual int         getComponentLevel(void) const;
    virtual int     getX3dVersion(void) const { return 0; } 
    virtual Node   *copy() const { return new NodePixelTexture(*this); }
    virtual void    setField(int field, FieldValue *value, int cf = -1);
    void            load();
    int             isLoaded();

    virtual bool    isInvalidChildNode(void) { return true; }

    virtual void    bind(GLuint textureId, GLuint textureName);
    virtual void    bind();
    virtual void    unbind();

    virtual bool    getWasScaleRequired(void) { return m_scaleRequired; }
    virtual int     getNumComponents(void) { return m_components; }

    virtual bool    isTransparent(void) { return m_isTransparent; }

    virtual bool    hasCoverFields(void) { return true; }   

    virtual int     textureGlColorMode(void) { return m_glColorMode; }

    fieldMacros(SFImage,  image,             ProtoPixelTexture)
    fieldMacros(SFBool,   repeatS,           ProtoPixelTexture)
    fieldMacros(SFBool,   repeatT,           ProtoPixelTexture)
    fieldMacros(SFInt32,  blendMode,         ProtoPixelTexture)
    fieldMacros(SFNode,   textureProperties, ProtoPixelTexture);
    kambiTextureCommonFieldMacros(ProtoPixelTexture)
    kambiVrmlTextureCommonFieldMacros(ProtoPixelTexture)
    fieldMacros(SFString, crossOrigin,       ProtoPixelTexture);
    fieldMacros(SFInt32,  origChannelCount,  ProtoPixelTexture);
    fieldMacros(SFBool,   scale,             ProtoPixelTexture);

protected:
    unsigned char  *m_image;
    int             m_imageStatus;
    int             m_textureWidth;
    int             m_textureHeight;
    int             m_components;
    int             m_glColorMode;
    GLuint          m_textureName;
    bool            m_scaleRequired;
    bool            m_isTransparent;
    bool            m_isAlphaNot0Not1;
};

#endif // _NODE_PIXELTEXTURE_H

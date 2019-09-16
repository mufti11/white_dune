/*
 * NodeImageTexture.h
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

#ifndef _NODE_IMAGETEXTURE_H
#define _NODE_IMAGETEXTURE_H

#ifndef _NODE_H
# include "Node.h"
#endif
#ifndef _PROTO_MACROS_H
#include "ProtoMacros.h"
#endif
#ifndef _PROTO_H
# include "Proto.h"
#endif

#include "SFMFTypes.h"

#include "KambiTextureCommonFields.h"

#ifdef HAVE_LIBDEVIL
# include <IL/il.h>
#endif

class ProtoImageTexture : public WonderlandExportProto {
public:
                    ProtoImageTexture(Scene *scene);
    virtual Node   *create(Scene *scene);

    virtual int     getType() const { return VRML_IMAGE_TEXTURE; }
    virtual int     getNodeClass() const 
                       { return TEXTURE_NODE | TEXTURE_2D_NODE | URL_NODE; }

    virtual bool    isDeclaredInRwd_h() { return true; }

    FieldIndex url;
    FieldIndex repeatS;
    FieldIndex repeatT;
    FieldIndex environment;
    FieldIndex blendMode;
    FieldIndex filterMode;
    FieldIndex anisotropy;
#ifdef HAVE_TEXTUREIMAGE_MODE
    FieldIndex mode;
#endif
    FieldIndex textureProperties;
    kambiTextureCommonFieldIndex()
    kambiVrmlTextureCommonFieldIndex()
    FieldIndex crossOrigin;
    FieldIndex hideChildren;
    FieldIndex origChannelCount;
    FieldIndex scale;
};

class NodeImageTexture : public Node {
public:
                    NodeImageTexture(Scene *scene, Proto *proto);
                    NodeImageTexture(const NodeImageTexture &node);
    virtual        ~NodeImageTexture();

    virtual int     getProfile(void) const { return PROFILE_INTERCHANGE; }
    virtual int     getX3dVersion(void) const { return 0; }
    virtual Node   *copy() const { return new NodeImageTexture(*this); }
    virtual void    setField(int field, FieldValue *value, int cf = -1);

    virtual bool    hasNumbers4kids(void) { return true; } 

    virtual void    update();

    virtual void    load();
    virtual void    unload();

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

    Node           *getPixelTexture(void);

    fieldMacros(MFString, url,               ProtoImageTexture)
    fieldMacros(SFBool,   repeatS,           ProtoImageTexture)
    fieldMacros(SFBool,   repeatT,           ProtoImageTexture)
    fieldMacros(SFBool,   environment,       ProtoImageTexture)
    fieldMacros(SFInt32,  blendMode,         ProtoImageTexture)
    fieldMacros(SFInt32,  filterMode,        ProtoImageTexture)
    fieldMacros(SFInt32,  anisotropy,        ProtoImageTexture)
#ifdef HAVE_TEXTUREIMAGE_MODE
    fieldMacros(SFInt32,  mode,              ProtoImageTexture)
#endif
    fieldMacros(SFNode,   textureProperties, ProtoImageTexture);
    kambiTextureCommonFieldMacros(ProtoImageTexture)
    kambiVrmlTextureCommonFieldMacros(       ProtoImageTexture)
    fieldMacros(SFString, crossOrigin,       ProtoImageTexture);
    fieldMacros(SFBool,   hideChildren,      ProtoImageTexture);
    fieldMacros(SFInt32,  origChannelCount,  ProtoImageTexture);
    fieldMacros(SFBool,   scale,             ProtoImageTexture);

#ifdef HAVE_LIBDEVIL
    void            reportLoadError(char* filename, const char *error_msg);
#endif

protected:
    unsigned char  *m_image;
    MyString        m_baseURL;
    int             m_imageStatus;
    int             m_textureWidth;
    int             m_textureHeight;
    int             m_components;
    int             m_glColorMode;
    GLuint          m_textureName;
    bool            m_scaleRequired;
    bool            m_isTransparent;
    MyString        m_path;

#ifdef HAVE_LIBDEVIL
    ILuint          m_imageName;
#endif
};

#endif // _NODE_IMAGETEXTURE_H

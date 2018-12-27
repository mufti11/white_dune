/*
 * NodePixelTexture.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "swt.h"
#include "DuneApp.h"
#include "resource.h"

#include "NodePixelTexture.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFImage.h"
#include "SFBool.h"
#include "Texture.h"

ProtoPixelTexture::ProtoPixelTexture(Scene *scene)
  : Proto(scene, "PixelTexture")
{
    image.set(
          addExposedField(SFIMAGE, "image", new SFImage(0, 0, 0, NULL)));
    repeatS.set(
          addField(SFBOOL, "repeatS", new SFBool(true)));
    repeatT.set(
          addField(SFBOOL, "repeatT", new SFBool(true)));
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        blendMode.set(
          addField(SFINT32, "blendMode", new SFInt32(0), 
                   new SFInt32(0), new SFInt32(5)));
        setFieldFlags(blendMode, FF_COVER_ONLY);
    }

    textureProperties.set(
        addField(SFNODE, "textureProperties", new SFNode(), 
                 X3D_TEXTURE_PROPERTIES));
    setFieldFlags(textureProperties, FF_X3D_ONLY);

    kambiTextureCommonFields()
    kambiVrmlTextureCommonFields()
    crossOrigin.set(
          addExposedField(SFSTRING, "crossOrigin", new SFString("")));
    setFieldFlags(crossOrigin, FF_X3DOM_ONLY);

    origChannelCount.set(
          addExposedField(SFINT32, "origChannelCount", new SFInt32(0),
                   new SFInt32(0)));
    setFieldFlags(origChannelCount, FF_X3DOM_ONLY);

    scale.set(
          addExposedField(SFBOOL, "scale", new SFBool(true)));
    setFieldFlags(scale, FF_X3DOM_ONLY);
}

Node *
ProtoPixelTexture::create(Scene *scene)
{ 
    return new NodePixelTexture(scene, this); 
}

NodePixelTexture::NodePixelTexture(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_image = NULL;
    m_textureWidth = 0;
    m_textureHeight = 0;
    m_imageStatus = IMG_STATUS_UNLOADED;
    m_textureName = 0;
    m_components = 0;
    m_glColorMode = GL_RGB;          // default: no transparency

    m_scaleRequired = false;

    m_isTransparent = false;
    m_isAlphaNot0Not1 = true;
}

NodePixelTexture::NodePixelTexture(const NodePixelTexture &node)
  : Node(node)
{
    m_image = NULL;
    m_textureName = 0;  // must load its own texture though
    m_textureWidth = node.m_textureWidth;
    m_textureHeight = node.m_textureHeight;
    m_components=node.m_components;
    m_glColorMode = node.m_glColorMode ;
    m_imageStatus = node.m_imageStatus;
    m_scaleRequired = node.m_scaleRequired;
    m_isTransparent = node.m_isTransparent;

    int size = m_textureWidth * m_textureHeight * m_components;
    // copy image data, if any
    if (node.m_image)
        if ((size > 0) && (node.image()->getNumPixels() > 0)) {
            m_image = new unsigned char[size];
            memcpy(m_image, node.m_image, size);
        } 
}

NodePixelTexture::~NodePixelTexture()
{
    delete [] m_image;
}

void
NodePixelTexture::setField(int field, FieldValue *value, int cf)
{
    if (field == image_Field()) {
        m_imageStatus = IMG_STATUS_UNLOADED;
    }
    Node::setField(field, value, cf);
}

void
NodePixelTexture::load()
{
    if (m_imageStatus == IMG_STATUS_LOADED)
        return;

    SFImage *sfImage = image();
    int width = 0;
    int height = 0;
    unsigned char *data = NULL;

    m_imageStatus = IMG_STATUS_ERROR;

    if (sfImage != NULL) {
        width = sfImage->getWidth();
        height = sfImage->getHeight();
        m_components = sfImage->getComponents();
        if (m_components==4)
            m_glColorMode=GL_RGBA;
        else if (m_components==3)
            m_glColorMode = GL_RGB;
        else if (m_components==2)
            m_glColorMode = GL_LUMINANCE_ALPHA;
        else if (m_components==1)
            m_glColorMode = GL_LUMINANCE;
        else 
            return;
        if ((width == 0) || (height == 0))
            return;
        data = new unsigned char [width * height * m_components];
        for (int i = 0; i < width; i++)
            for (int j = 0; j < height; j++)
                for (int k = 0; k < m_components; k++) {
                    int numPixel = i + j * width;
                    int numData = numPixel * m_components + k;
                    data[numData] = 0;
                    if (sfImage->getNumPixels() > numPixel) {
                        unsigned int bytes = sfImage->getPixels()[numPixel];
                        int numShifts = (m_components - k - 1) * 8;
                        data[numData] = (bytes >> numShifts) & 0xFF;
                    } 
                }
    } else
        return;

    m_textureWidth = 1;
    m_textureHeight = 1;

    while (m_textureWidth < width)
       m_textureWidth <<= 1;

    while (m_textureHeight < height)
       m_textureHeight <<= 1;

    delete [] m_image;

    if (width == m_textureWidth && height == m_textureHeight) {
        m_scaleRequired = false;
        m_image = data;
    } else {
        m_scaleRequired = true;
        m_image = new unsigned char[m_textureWidth * m_textureHeight * 
                                    m_components];
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        gluScaleImage(m_glColorMode, width, height, GL_UNSIGNED_BYTE, data,
                      m_textureWidth, m_textureHeight, GL_UNSIGNED_BYTE, m_image);
        delete [] data;
    }
    m_imageStatus = IMG_STATUS_LOADED;
    if ((m_components == 2) || (m_components == 4)) {
        int size = m_textureWidth * m_textureHeight * m_components;
        for (int i = 0; i < size; i += m_components)
            if (m_image[i + m_components - 1] < 0xff) {
                m_isTransparent = true;
                break;
            }
        for (int i = 0; i < size; i += m_components)
            if (m_image[i + m_components - 1] != 0xff)
                if (m_image[i + m_components - 1] != 0x0) {
                    m_isAlphaNot0Not1 = true;
                    break;
                }
    }
    if (TheApp->isAnaglyphStereo()) {
        // change colors into grayscale values
        if ((m_components == 3) || (m_components == 4)) {
            int size = m_textureWidth * m_textureHeight * m_components;
            for (int i = 0; i < size; i += m_components) {
                int gray = (m_image[i + 0] + m_image[i + 1] + m_image[i + 2]) / 3;
                m_image[i + 0] = gray;
                m_image[i + 1] = gray;
                m_image[i + 2] = gray;
            }
        }
    }
    m_imageStatus = IMG_STATUS_LOADED;
}

void
NodePixelTexture::bind(GLuint textureId, GLuint textureName)
{
    glEnable(textureId);
    if (m_imageStatus == IMG_STATUS_LOADED) {
        glBindTexture(textureId, m_textureName);
//        glTexParameterf(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameterf(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(textureId, 0, m_components, 
                     m_textureWidth, m_textureHeight, 0,
                     m_glColorMode, GL_UNSIGNED_BYTE, m_image);
        GLenum error=glGetError();
        if (error !=0 ) {
            /* what's wrong here with "invalid value" ? */
            fprintf(stderr,"GL Error: %s\n",gluErrorString(error));
        }
    } else {
        glBindTexture(textureId, m_textureName);
    }
    glTexParameteri(textureId, GL_TEXTURE_WRAP_S, 
                    repeatS()->getValue() ? GL_REPEAT : GL_CLAMP);
    glTexParameteri(textureId, GL_TEXTURE_WRAP_T, 
                    repeatT()->getValue() ? GL_REPEAT : GL_CLAMP);
}

void
NodePixelTexture::bind()
{
    if (m_imageStatus == IMG_STATUS_UNLOADED) {
        load();
        if (m_textureName != 0) glDeleteTextures(1, &m_textureName);
        m_textureName = 0;
    }
    if (m_imageStatus == IMG_STATUS_LOADED) {
        if (m_textureName == 0) {
            glGenTextures(1, &m_textureName);
        }
    }
    bind(GL_TEXTURE_2D, m_textureName);

#ifdef HAVE_TEXTUREIMAGE_MODE
    switch (mode()->getValue()) {
      case 1:
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        break;
      case 2:
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        break;
      case 3:
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        break;
    }
#endif
}

void
NodePixelTexture::unbind()
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
}

int
NodePixelTexture::isLoaded()
{
    return m_imageStatus == IMG_STATUS_LOADED;
}

int NodePixelTexture::getProfile(void) const
{ 
    return PROFILE_INTERCHANGE;
}

int
NodePixelTexture::getComponentLevel(void) const
{
    if (m_textureWidth != 512)
        return 1;
    if (m_textureHeight != 512)
        return 1;
    if (m_isAlphaNot0Not1)
        return 1;
    return -1;
}

const char* 
NodePixelTexture::getComponentName(void) const
{
    static const char* name = "Texturing";
    return name;
}



/*
 * NodeImageTexture.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "swt.h"
#include "DuneApp.h"
#include "resource.h"

#include "NodeImageTexture.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFBool.h"
#include "SFInt32.h"
#include "Field.h"
#include "URL.h"
#ifdef HAVE_LIBDEVIL
# include <IL/ilu.h>
#endif
#include "Image.h"
#include "Util.h"
#include "Texture.h"
#include "MyMesh.h"
#include "NodeShape.h"

ProtoImageTexture::ProtoImageTexture(Scene *scene)
  : WonderlandExportProto(scene, "ImageTexture")
{
    url.set(
          addExposedField(MFSTRING, "url", new MFString(), FF_URL | 
                          FF_WONDERLAND_ART, NULL));
    repeatS.set(
          addField(SFBOOL, "repeatS", new SFBool(true)));
    repeatT.set(
          addField(SFBOOL, "repeatT", new SFBool(true)));
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        environment.set(
          addField(SFBOOL, "environment", new SFBool(false)));
        setFieldFlags(environment, FF_COVER_ONLY);
        blendMode.set(
          addField(SFINT32, "blendMode", new SFInt32(0), 
                   new SFInt32(0), new SFInt32(5)));
        setFieldFlags(blendMode, FF_COVER_ONLY);
        filterMode.set(
          addField(SFINT32, "filterMode", new SFInt32(0), 
                   new SFInt32(0), new SFInt32(5)));
        setFieldFlags(filterMode, FF_COVER_ONLY);
        anisotropy.set(
          addField(SFINT32, "anisotropy", new SFInt32(1), 
                   new SFInt32(0), new SFInt32(2)));
        setFieldFlags(anisotropy, FF_COVER_ONLY);
    }
#ifdef HAVE_TEXTUREIMAGE_MODE
    mode.set(
          addField(SFINT32, "mode", new SFInt32(0), 
                   new SFInt32(0), new SFInt32(3)));
#endif
    textureProperties.set(
        addField(SFNODE, "textureProperties", new SFNode(), 
                 X3D_TEXTURE_PROPERTIES));
    setFieldFlags(textureProperties, FF_X3D_ONLY);

    kambiTextureCommonFields()
    kambiVrmlTextureCommonFields()

    crossOrigin.set(
          addExposedField(SFSTRING, "crossOrigin", new SFString("")));
    setFieldFlags(crossOrigin, FF_X3DOM_ONLY);

    hideChildren.set(
          addExposedField(SFBOOL, "hideChildren", new SFBool(true)));
    setFieldFlags(hideChildren, FF_X3DOM_ONLY);

    origChannelCount.set(
          addExposedField(SFINT32, "origChannelCount", new SFInt32(0),
                          new SFInt32(0)));
    setFieldFlags(origChannelCount, FF_X3DOM_ONLY);

    scale.set(
          addExposedField(SFBOOL, "scale", new SFBool(true)));
    setFieldFlags(scale, FF_X3DOM_ONLY);
}

Node *
ProtoImageTexture::create(Scene *scene)
{ 
    return new NodeImageTexture(scene, this); 
}

NodeImageTexture::NodeImageTexture(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_image = NULL;
    m_baseURL = "";
    m_textureWidth = 0;
    m_textureHeight = 0;
    m_imageStatus = IMG_STATUS_UNLOADED;
    m_textureName = 0;
    m_components = 0;
    m_glColorMode = GL_RGB;          // default: no transparency

    m_scaleRequired = false;
    m_isTransparent = false;

#ifdef HAVE_LIBDEVIL
    static bool il_needs_init = true;
    if (il_needs_init) {
        ilInit();
        iluInit();
        il_needs_init = false;
        ilEnable(IL_ORIGIN_SET);
        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    }

    m_path = "";

    ilGenImages(1, &m_imageName);
    ilBindImage(m_imageName);
#endif
}

NodeImageTexture::NodeImageTexture(const NodeImageTexture &node)
  : Node(node)
{
    m_image = NULL;
    m_textureName = 0;  // must load its own texture though
    m_textureWidth = node.m_textureWidth;
    m_textureHeight = node.m_textureHeight;
    m_components = node.m_components;
    m_glColorMode = node.m_glColorMode ;
    m_imageStatus = node.m_imageStatus;
    m_scaleRequired = node.m_scaleRequired;

#ifdef HAVE_LIBDEVIL
    ilGenImages(1, &m_imageName);
    ilBindImage(m_imageName);

    // copy image data, if needed
    if (node.m_imageStatus == IMG_STATUS_LOADED) {
        if (ilCopyImage(node.m_imageName)) {
            m_image = ilGetData();
            if (m_image) {
                m_imageStatus = IMG_STATUS_LOADED;
            } else {
                m_imageStatus = IMG_STATUS_ERROR;
                reportLoadError(NULL, NULL);
            }
        } else {
            m_imageStatus = IMG_STATUS_ERROR;
            reportLoadError(NULL, NULL);
        }
    }
#else
    int size = m_textureWidth * m_textureHeight * m_components;
    // copy image data, if any
    if (node.m_image) {
        m_image = new unsigned char[size];
        memcpy(m_image, node.m_image, size);
    }
#endif
}

NodeImageTexture::~NodeImageTexture()
{
    if (m_textureName != 0) glDeleteTextures(1, &m_textureName);
#ifdef HAVE_LIBDEVIL
    ilDeleteImages(1, &m_imageName);
#else
    delete [] m_image;
    m_image = NULL;
#endif
}

void
NodeImageTexture::update()
{
    m_imageStatus = IMG_STATUS_UNLOADED;
    if (m_textureName != 0) glDeleteTextures(1, &m_textureName);
#ifdef HAVE_LIBDEVIL
    ilDeleteImages(1, &m_imageName);
#else
    delete [] m_image;
    m_image = NULL;
#endif
}

void
NodeImageTexture::setField(int field, FieldValue *value, int cf)
{
    Node::setField(field, value, cf);
    if (field == url_Field()) {
        m_imageStatus = IMG_STATUS_UNLOADED;
        m_path = "";
        load();
    }
}

void
NodeImageTexture::unload()
{
    m_imageStatus = IMG_STATUS_UNLOADED;
}

void
NodeImageTexture::load()
{
    if (m_imageStatus == IMG_STATUS_LOADED)
        return;
    MFString *urls = url();
    int width = 1;
    int height = 1;
    unsigned char *data = NULL;

    if (urls->getSize() == 0)
        return;
    if (m_imageStatus == IMG_STATUS_UNLOADED) {
        m_baseURL = "";
        m_baseURL += TheApp->getImportURL();
    }
#ifdef HAVE_LIBDEVIL
        int errorflag = false;
        char *lastCheckedPath = NULL;
#endif
    for (int i = 0; i < urls->getSize(); i++) {
        MyString path;
        URL urlI(m_baseURL, urls->getValue(i));
        if (urls->getValue(i).length() == 0) continue;
        m_scene->Download(urlI, &path);
        Image image;
        if (image.Open(path)) {
            bool ret = false;
            if (m_path.length() == 0)
                ret = true;
            m_path = "";
            m_path += path;
            if (ret)
                return;
        }
        if (swHasVisual() == 0)
            return;
#ifdef HAVE_LIBDEVIL
        if (ilLoadImage((char *)(const char *)m_path)) {
            width = ilGetInteger(IL_IMAGE_WIDTH);
            height = ilGetInteger(IL_IMAGE_HEIGHT);
            m_components = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
            if (m_components == 1) {
                m_glColorMode=GL_LUMINANCE;
                ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
            } else if (m_components == 2) {
                m_glColorMode=GL_LUMINANCE_ALPHA;
                ilConvertImage(IL_LUMINANCE_ALPHA, IL_UNSIGNED_BYTE);
            } else if (m_components == 3) {
                m_glColorMode=GL_RGB;
                ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
            } else if (m_components == 4) {
                m_glColorMode=GL_RGBA;
                ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
            } else {
                reportLoadError((char *)(const char *)path, 
                                "Invalid image type: wrong number of components");
                m_imageStatus = IMG_STATUS_ERROR;
                return;
            }
            errorflag=false;
            break;
        } else
            lastCheckedPath = mystrdup(path);

#else
        if (image.Open(m_path)) {
            width = image.GetWidth();
            height = image.GetHeight();
            m_components = image.GetComponents();
            if (m_components==4)
               m_glColorMode = GL_RGBA;
            else if (m_components==3)
               m_glColorMode = GL_RGB;
            else if (m_components==2)
               m_glColorMode = GL_LUMINANCE_ALPHA;
            else
               m_glColorMode = GL_LUMINANCE;
            data = new unsigned char [width * height * m_components];
            image.Read(data);
            break;
        }
#endif        
    }

#ifdef HAVE_LIBDEVIL
    if (errorflag) {
        m_imageStatus = IMG_STATUS_ERROR;
        reportLoadError(lastCheckedPath, NULL);
        return;
    }
#else    
    if (data == NULL) { 
        m_imageStatus = IMG_STATUS_ERROR;
        return;
    }
#endif        
        
    m_textureWidth = 2;
    m_textureHeight = 2;

    while (m_textureWidth < width)
       m_textureWidth <<= 1;

    while (m_textureHeight < height)
       m_textureHeight <<= 1;

#ifdef HAVE_LIBDEVIL
    if (width == m_textureWidth && height == m_textureHeight) {
        m_scaleRequired = false;
    } else {
        m_scaleRequired = true;
        iluImageParameter(ILU_FILTER, ILU_BILINEAR);
        if (!iluScale(m_textureWidth, m_textureHeight, m_components)) {
            m_imageStatus = IMG_STATUS_ERROR;
            reportLoadError(NULL, NULL);
            return;
        }
    }

    m_image = ilGetData();
    if (m_image) {
        m_imageStatus = IMG_STATUS_LOADED;
    } else {
        m_imageStatus = IMG_STATUS_ERROR;
        reportLoadError(NULL, NULL);
        return;
    }
#else
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
                      m_textureWidth, m_textureHeight, GL_UNSIGNED_BYTE, 
                      m_image);
        delete [] data;
    }
#endif
    if ((m_components == 2) || (m_components == 4)) {
        int size = m_textureWidth * m_textureHeight * m_components;
        for (int i = 0; i < size; i += m_components) {
            if (m_image[i + m_components - 1] < 0xff) {
                m_isTransparent = true;
                break;
            }
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

#ifdef HAVE_LIBDEVIL
// If a NULL pointer is passed in, this function will report any errors
// that have happened inside the DevIL image library.  Otherwise, it will
// report whatever string is passed in.
void
NodeImageTexture::reportLoadError(char* filename, const char *error_msg)
{
    // If the user hits the cancel button, all subsequent errors will be ignored.
    // This way, if the VRML file contains 100 broken images, the user won't have
    // to hit the OK button 100 times.
    // FIXME:  This won't be reset until the program is restarted...
    static int mbReturn = 0;
    if (mbReturn == IDCANCEL) return;

    char errorstring[256];
    swLoadString(IDS_IMAGE_TEXTURE_ERROR, errorstring, 255);
    char buf[4096];
    if (error_msg) {                
         if (filename)
             mysnprintf(buf, 4095, "%s: %s\n", filename, error_msg);
         else
             mysnprintf(buf, 4095, "%s\n", error_msg);
    } else {
        ILenum Error;
        while ((Error = ilGetError()) != IL_NO_ERROR) {
            if (filename)
                 mysnprintf(buf, 4095, "%s: ilError #%d: %s\n", filename, Error, iluErrorString(Error));
            else
                 mysnprintf(buf, 4095, "ilError #%d: %s\n", Error, iluErrorString(Error));
            mbReturn = swMessageBox(TheApp->mainWnd(), buf, 
                                    errorstring, SW_MB_OKCANCEL, SW_MB_ERROR);
        }
    }
}
#endif

void
NodeImageTexture::bind(GLuint textureId, GLuint textureName)
{
    glEnable(textureId);
    if (m_imageStatus == IMG_STATUS_LOADED) {
        glBindTexture(textureId, textureName);
        glTexParameterf(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(textureId, 0, m_components, 
                     m_textureWidth, m_textureHeight, 0,
                     m_glColorMode, GL_UNSIGNED_BYTE, m_image);
        GLenum error=glGetError();
        if (error!=0)
           {
           /* what's wrong here with "invalid value" ? */
           fprintf(stderr,"GL Error: %s\n",gluErrorString(error));
           }

    } else {
        glBindTexture(textureId, textureName);
    }
    glTexParameteri(textureId, GL_TEXTURE_WRAP_S, 
                    repeatS()->getValue() ? GL_REPEAT : GL_CLAMP);
    glTexParameteri(textureId, GL_TEXTURE_WRAP_T, 
                    repeatT()->getValue() ? GL_REPEAT : GL_CLAMP);

    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        if (environment()->getValue()) {
            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
//            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
//            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
        }
    }
}

void
NodeImageTexture::bind()
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

    MyMesh *mesh = NULL;
    if (hasParent()) {
        for (int i = 0; i < getNumParents(); i++) {
            Node *parent = getParent(i);
            if (parent && parent->hasParent()) {
                for (int j = 0; j < getNumParents(); j++) {
                    parent = parent->getParent(j);
                    if (parent && parent->getType() == VRML_SHAPE) {
                        NodeShape *shape = (NodeShape *)parent;
                        if (shape->geometry()->getValue()) {
                            if (shape->geometry()->getValue()->isMeshBasedNode()) {
                                MeshBasedNode *meshBased = (MeshBasedNode *)
                                    shape->geometry()->getValue();
                               mesh = meshBased->getMesh();
                           }
                       }
                    }
                }
            }                     
        }
    }
    if (mesh && (mesh->getTexCoordParameter(0) == TEX_GEN_SPHERE)) {
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
    } 
#ifndef _WIN32
      else if (mesh && (mesh->getTexCoordParameter(0) == 
                        TEX_GEN_CAMERA_SPACE_NORMAL)) {
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
    } else if (mesh && (mesh->getTexCoordParameter(0) == 
                        TEX_GEN_SPHERE_REFLECT)) {
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
    }
#endif

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
NodeImageTexture::unbind()
{
    glDisable(GL_TEXTURE_2D);
    Node *parent = getParent();
    if (hasParent() && parent->getType() != X3D_MULTI_TEXTURE) {
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
    }
}

int
NodeImageTexture::isLoaded()
{
    return m_imageStatus == IMG_STATUS_LOADED;
}

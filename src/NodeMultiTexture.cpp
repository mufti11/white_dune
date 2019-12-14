/*
 * NodeMultiTexture.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "NodeMultiTexture.h"
#include "Proto.h"
#include "FieldValue.h"
#include "Node.h"
#include "Util.h"
#include "swt.h"
#include "MyMesh.h"
#include "MeshBasedNode.h"
#include "NodeShape.h"
#include "NodeAppearance.h"
#include "NodeTextureCoordinate.h"
#include "NodeMultiTextureCoordinate.h"

ProtoMultiTexture::ProtoMultiTexture(Scene *scene)
  : Proto(scene, "MultiTexture")
{
    alpha.set(
          addExposedField(SFFLOAT, "alpha", new SFFloat(1),
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    color.set(
          addExposedField(SFCOLOR, "color", new SFColor(1, 1, 1)));
    function.set(
          addExposedField(MFSTRING, "function", new MFString()));
    mode.set(
          addExposedField(MFSTRING, "mode", new MFString()));
    source.set(
          addExposedField(MFSTRING, "source", new MFString()));
    texture.set(
          addExposedField(MFNODE, "texture", new MFNode(), 
                          TEXTURE_NODE | NOT_SELF_NODE));
    kambiTextureCommonFields()

    addURLs(URL_X3D);

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

    url.set(
          addExposedField(MFSTRING, "url", new MFString()));
    setFieldFlags(url, FF_X3DOM_ONLY);
}

Node *
ProtoMultiTexture::create(Scene *scene)
{ 
    return new NodeMultiTexture(scene, this); 
}

NodeMultiTexture::NodeMultiTexture(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_maxTextureUnits = -1;
}

void
NodeMultiTexture::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
#ifdef HAVE_MULTI_TEXTURE
    if (index == mode_Field()) {
        m_modeInt.resize(0);
        for (int i = 0; i < mode()->getSize(); i++) {
            if (strcmp(mode()->getValue(i)->getValue(), "MODULATE") == 0)
                m_modeInt[i] = GL_MODULATE;
            else if (strcmp(mode()->getValue(i)->getValue(), "REPLACE") == 0)
                m_modeInt[i] = GL_REPLACE;
            else if (strcmp(mode()->getValue(i)->getValue(), "ADD") == 0)
                m_modeInt[i] = GL_ADD;
            else if (strcmp(mode()->getValue(i)->getValue(), "ADDSIGNED") == 0)
                m_modeInt[i] = GL_ADD_SIGNED;
            else if (strcmp(mode()->getValue(i)->getValue(), "SUBTRACT") == 0)
                m_modeInt[i] = GL_SUBTRACT;
            else if (strcmp(mode()->getValue(i)->getValue(), "ADDSIGNED") == 0)
                m_modeInt[i] = GL_ADD_SIGNED;
            else if (strcmp(mode()->getValue(i)->getValue(), "OFF") == 0)
                m_modeInt[i] = -1;
            else if (strcmp(mode()->getValue(i)->getValue(),
                            "DOTPRODUCT3") == 0)
                m_modeInt[i] = GL_DOT3_RGBA;
            else {
                // unknown mode
                swDebugf("unknown texture mode %s in MultiTexture: using %s\n", 
                         (const char *)mode()->getValue(i), "MODULATE");
                m_modeInt[i] = GL_MODULATE;
            }
        }
    }
#endif
}

void 
NodeMultiTexture::bind()
{
    MFNode *mfTexture = (MFNode *) getField(texture_Field());
    if (m_maxTextureUnits == -1) {
#ifdef HAVE_MULTI_TEXTURE
        glGetIntegerv(GL_MAX_TEXTURE_UNITS, &m_maxTextureUnits);
#else
        m_maxTextureUnits = 0;
#endif
        if (mfTexture->getSize() > m_maxTextureUnits)
            swDebugf("MultiTexture wants %d texture units but OpenGL has only %d\n",
                      mfTexture->getSize(), m_maxTextureUnits);
    }
#ifdef HAVE_MULTI_TEXTURE
    int textureUnits = MIN(mfTexture->getSize(), m_maxTextureUnits);
#else
    int textureUnits = MIN(mfTexture->getSize(), 1);
#endif
    if (textureUnits <= 0) 
        return;
#ifdef HAVE_MULTI_TEXTURE
    MyMesh *mesh = NULL;
    if (hasParent()) {
        Node *parent = getParent();
        if (parent->hasParent()) {
            parent = parent->getParent();
            if (parent->getType() == VRML_SHAPE) {
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
#endif
    m_textureObjects = new GLuint[textureUnits];
    glGenTextures(textureUnits, m_textureObjects);
    for (int i = 0; i < textureUnits; i++) {
        Node *nTexture = mfTexture->getValue(i);
        if (nTexture) {
            if ((int)m_modeInt.size() > i)
                if (m_modeInt[i] == -1)
                    continue;
            nTexture->load();
#ifdef HAVE_MULTI_TEXTURE
            glActiveTexture(GL_TEXTURE0 + i);
            glClientActiveTexture(GL_TEXTURE0 + i);                    
#endif
            nTexture->bind(GL_TEXTURE_2D, m_textureObjects[i]);
#ifdef HAVE_MULTI_TEXTURE
            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
            if (mesh && (mesh->getTexCoordParameter(i) == TEX_GEN_SPHERE)) {
                glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                glEnable(GL_TEXTURE_GEN_S);
                glEnable(GL_TEXTURE_GEN_T);
            } else if (mesh && (mesh->getTexCoordParameter(i) == 
                                TEX_GEN_CAMERA_SPACE_NORMAL)) {
                glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
                glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
                glEnable(GL_TEXTURE_GEN_S);
                glEnable(GL_TEXTURE_GEN_T);
            } if (mesh && (mesh->getTexCoordParameter(i) == 
                                TEX_GEN_SPHERE_REFLECT)) {
                glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
                glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
                glEnable(GL_TEXTURE_GEN_S);
                glEnable(GL_TEXTURE_GEN_T);
            }   
#endif
        }
    }
#ifdef HAVE_MULTI_TEXTURE
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
#endif
    for (int i = 0; i < textureUnits; i++) {
        Node *nTexture = mfTexture->getValue(i);
        if (nTexture) {
#ifdef HAVE_MULTI_TEXTURE
            if (i == 0) {
                glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
                glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
            } else if (m_modeInt.size() == 0) {
                glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
                glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
            } else {
                glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, m_modeInt[i]);
                glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, m_modeInt[i]);
            }
            if (i > 0) {
                glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
                glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
                glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);
                glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
                glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
                glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
                glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_TEXTURE);
                glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
            }
#endif
        }
    }    
}

void
NodeMultiTexture::unbind()
{
    MFNode *mfTexture = (MFNode *) getField(texture_Field());
    if (mfTexture->getSize() <= 0) 
        return;
#ifdef HAVE_MULTI_TEXTURE
    int textureUnits = MIN(mfTexture->getSize(), m_maxTextureUnits);
#else
    int textureUnits = MIN(mfTexture->getSize(), 1);
#endif
    for (int i = 0; i < textureUnits; i++) {
        Node *nTexture = mfTexture->getValue(i);
        if (nTexture) {
#ifdef HAVE_MULTI_TEXTURE
            glActiveTexture(GL_TEXTURE0 + i);
#endif
            nTexture->unbind();
        }
    }
#ifdef HAVE_MULTI_TEXTURE
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
#endif
    if (textureUnits != 0)
        glDeleteTextures(textureUnits, m_textureObjects);
}

bool 
NodeMultiTexture::isTransparent(void)
{
    MFNode *mfTexture = (MFNode *) getField(texture_Field());
    if (mfTexture->getSize() <= 0) 
        return false;
    Node *nTexture = mfTexture->getValue(0);
    if (nTexture) 
        return nTexture->isTransparent();
    return false; 
}

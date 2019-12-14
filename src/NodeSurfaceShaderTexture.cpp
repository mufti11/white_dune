/*
 * NodeSurfaceShaderTexture.cpp
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

#include "Scene.h"
#include "resource.h"

#include "NodeSurfaceShaderTexture.h"

ProtoSurfaceShaderTexture::ProtoSurfaceShaderTexture(Scene *scene)
  : Proto(scene, "SurfaceShaderTexture")
{
    channelMask.set(
          addExposedField(SFSTRING, "channelMask", new SFString("")));
    setFieldFlags(channelMask, FF_X3DOM_ONLY);

    crossOrigin.set(
          addExposedField(SFSTRING, "crossOrigin", new SFString("DEFAULT")));
    setFieldFlags(crossOrigin, FF_X3DOM_ONLY);

    isSRGB.set(
          addExposedField(SFBOOL, "isSRGB", new SFBool(false)));
    setFieldFlags(isSRGB, FF_X3DOM_ONLY);

    origChannelCount.set(
          addExposedField(SFINT32, "origChannelCount", new SFInt32(0),
                   new SFInt32(0)));
    setFieldFlags(origChannelCount, FF_X3DOM_ONLY);

    repeatS.set(
          addExposedField(SFBOOL, "repeatS", new SFBool(true)));
    setFieldFlags(repeatS, FF_X3DOM_ONLY);

    repeatT.set(
          addExposedField(SFBOOL, "repeatT", new SFBool(true)));
    setFieldFlags(repeatT, FF_X3DOM_ONLY);

    scale.set(
          addExposedField(SFBOOL, "scale", new SFBool(true)));
    setFieldFlags(scale, FF_X3DOM_ONLY);

    texture.set(
        addExposedField(SFNODE, "texture", new SFNode(), 
                 TEXTURE_NODE));
    setFieldFlags(texture, FF_X3DOM_ONLY);

    textureCoordinatesId.set(
        addExposedField(SFINT32, "textureCoordinatesId", new SFInt32(0)));
    setFieldFlags(textureCoordinatesId, FF_X3DOM_ONLY);

    textureProperties.set(
        addExposedField(SFNODE, "textureProperties", new SFNode(), 
                 X3D_TEXTURE_PROPERTIES));
    setFieldFlags(textureProperties, FF_X3DOM_ONLY);

    textureTransform.set(
        addExposedField(SFNODE, "textureTransform", new SFNode(), 
                 VRML_TEXTURE_TRANSFORM));
    setFieldFlags(textureTransform, FF_X3DOM_ONLY);

    url.set(
          addExposedField(MFSTRING, "url", new MFString(), FF_URL, NULL));
    setFieldFlags(url, FF_X3DOM_ONLY);
}

Node *
ProtoSurfaceShaderTexture::create(Scene *scene)
{ 
    return new NodeSurfaceShaderTexture(scene, this); 
}

NodeSurfaceShaderTexture::NodeSurfaceShaderTexture(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_baseURL = "";
}

NodeSurfaceShaderTexture::NodeSurfaceShaderTexture(NodeSurfaceShaderTexture &node)
  : Node(node)
{
}

NodeSurfaceShaderTexture::~NodeSurfaceShaderTexture()
{
}


void
NodeSurfaceShaderTexture::load()
{
    MFString *urls = url();
    if (urls->getSize() == 0)
        return;
    m_baseURL = "";
    m_baseURL += TheApp->getImportURL();
    for (int i = 0; i < urls->getSize(); i++) {
        MyString path;
        URL urlI(m_baseURL, urls->getValue(i)->getValue());
        if (strlen(urls->getValue(i)->getValue()) == 0) continue;
        m_scene->Download(urlI, &path);
    }
}

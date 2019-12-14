/*
 * NodeImageTextureAtlas.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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

#include "NodeImageTextureAtlas.h"
#include "Scene.h"
#include "URL.h"

ProtoImageTextureAtlas::ProtoImageTextureAtlas(Scene *scene)
  : Proto(scene, "ImageTextureAtlas")
{
    url.set(
          addExposedField(MFSTRING, "url", new MFString(), FF_URL | 
                          FF_WONDERLAND_ART, NULL));
    repeatS.set(
          addExposedField(SFBOOL, "repeatS", new SFBool(true)));
    repeatT.set(
          addExposedField(SFBOOL, "repeatT", new SFBool(true)));

    textureProperties.set(
        addExposedField(SFNODE, "textureProperties", new SFNode(), 
                 X3D_TEXTURE_PROPERTIES));
    setFieldFlags(textureProperties, FF_X3D_ONLY);

    crossOrigin.set(
          addExposedField(SFSTRING, "crossOrigin", new SFString("")));
    setFieldFlags(crossOrigin, FF_X3DOM_ONLY);

    hideChildren.set(
          addExposedField(SFBOOL, "hideChildren", new SFBool(true)));
    setFieldFlags(hideChildren, FF_X3DOM_ONLY);

    numberOfSlices.set(
          addExposedField(SFINT32, "numberOfSlices", new SFInt32(0),
                   new SFInt32(0)));
    setFieldFlags(numberOfSlices, FF_X3DOM_ONLY);

    origChannelCount.set(
          addExposedField(SFINT32, "origChannelCount", new SFInt32(0),
                   new SFInt32(0)));
    setFieldFlags(origChannelCount, FF_X3DOM_ONLY);

    scale.set(
          addExposedField(SFBOOL, "scale", new SFBool(true)));
    setFieldFlags(scale, FF_X3DOM_ONLY);

    slicesOverX.set(
          addExposedField(SFINT32, "slicesOverX", new SFInt32(0),
                   new SFInt32(0)));
    setFieldFlags(slicesOverX, FF_X3DOM_ONLY);

    slicesOverY.set(
          addExposedField(SFINT32, "slicesOverY", new SFInt32(0),
                   new SFInt32(0)));
    setFieldFlags(slicesOverY, FF_X3DOM_ONLY);
}

Node *
ProtoImageTextureAtlas::create(Scene *scene)
{ 
    return new NodeImageTextureAtlas(scene, this); 
}

NodeImageTextureAtlas::NodeImageTextureAtlas(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_baseURL = "";
}

NodeImageTextureAtlas::NodeImageTextureAtlas(NodeImageTextureAtlas &node)
  : Node(node)
{
}

NodeImageTextureAtlas::~NodeImageTextureAtlas()
{
}
void
NodeImageTextureAtlas::load()
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

/*
 * NodeRefinementTexture.cpp
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

#include "NodeRefinementTexture.h"
#include "Scene.h"

ProtoRefinementTexture::ProtoRefinementTexture(Scene *myscene)
  : Proto(myscene, "RefinementTexture")
{
    autoRefinement.set(
          addExposedField(SFBOOL, "autoRefinement", new SFBool(true)));
    setFieldFlags(autoRefinement, FF_X3DOM_ONLY);

    background.set(
        addExposedField(SFNODE, "background", new SFNode(NULL), 
                 VRML_BACKGROUND));
    setFieldFlags(background, FF_X3DOM_ONLY);

    crossOrigin.set(
          addExposedField(SFSTRING, "crossOrigin", new SFString("")));
    setFieldFlags(crossOrigin, FF_X3DOM_ONLY);

    depthMap.set(
          addExposedField(SFBOOL, "depthMap", new SFBool(true)));
    setFieldFlags(depthMap, FF_X3DOM_ONLY);

    int *dim = new int[3];
    dim[0] = 128;
    dim[1] = 128;
    dim[2] = 4;
    dimensions.set(
          addExposedField(MFINT32, "dimensions", new MFInt32(dim, 3)));
    setFieldFlags(dimensions, FF_X3DOM_ONLY);

    excludeNodes.set(
          addExposedField(MFNODE, "excludeNodes", new MFNode()));
    setFieldFlags(excludeNodes, FF_X3DOM_ONLY);

    fog.set(
          addExposedField(SFNODE, "fog", new SFNode(NULL), VRML_FOG));
    setFieldFlags(fog, FF_X3DOM_ONLY);

    format.set(
          addExposedField(SFSTRING, "format", new SFString("jpg")));
    setFieldFlags(format, FF_X3DOM_ONLY);

    interpupillaryDistance.set(
          addExposedField(SFFLOAT, "interpupillaryDistance", new SFFloat(064)));
    setFieldFlags(interpupillaryDistance, FF_X3DOM_ONLY);

    iterations.set(
          addExposedField(SFINT32, "iterations", new SFInt32(7)));
    setFieldFlags(iterations, FF_X3DOM_ONLY);

    maxLevel.set(
          addExposedField(SFINT32, "maxLevel", new SFInt32(7)));
    setFieldFlags(maxLevel, FF_X3DOM_ONLY);

    oculusRiftVersion.set(
          addExposedField(SFFLOAT, "oculusRiftVersion", new SFFloat(1)));
    setFieldFlags(oculusRiftVersion, FF_X3DOM_ONLY);

    origChannelCount.set(
          addExposedField(SFINT32, "origChannelCount", new SFInt32(0)));
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

    scene.set(
          addExposedField(SFNODE, "scene", new SFNode(NULL)));
    setFieldFlags(scene, FF_X3DOM_ONLY);

    showNormals.set(
          addExposedField(SFBOOL, "showNormals", new SFBool(false)));
    setFieldFlags(showNormals, FF_X3DOM_ONLY);

    stamp0.set(
          addExposedField(SFSTRING, "stamp0", new SFString("gif")));
    setFieldFlags(stamp0, FF_X3DOM_ONLY);

    stamp1.set(
          addExposedField(SFSTRING, "stamp1", new SFString("gif")));
    setFieldFlags(stamp1, FF_X3DOM_ONLY);

    stereoMode.set(
          addExposedField(SFSTRING, "stereoMode", new SFString("gif")));
    setFieldFlags(stereoMode, FF_X3DOM_ONLY);

    textureProperties.set(
        addExposedField(SFNODE, "textureProperties", new SFNode(), 
                 X3D_TEXTURE_PROPERTIES));
    setFieldFlags(textureProperties, FF_X3DOM_ONLY);

    update.set(
          addExposedField(SFSTRING, "update", new SFString("gif")));
    setFieldFlags(update, FF_X3DOM_ONLY);

    url.set(
          addExposedField(MFSTRING, "url", new MFString(), FF_URL, NULL));
    setFieldFlags(url, FF_X3DOM_ONLY);

    viewpoint.set(
        addExposedField(SFNODE, "viewpoint", new SFNode(), 
                 VIEWPOINT_NODE));
    setFieldFlags(viewpoint, FF_X3DOM_ONLY);
}

Node *
ProtoRefinementTexture::create(Scene *scene)
{ 
    return new NodeRefinementTexture(scene, this); 
}

NodeRefinementTexture::NodeRefinementTexture(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

NodeRefinementTexture::~NodeRefinementTexture()
{
}

void
NodeRefinementTexture::load()
{
    MFString *urls = url();

    if (urls->getSize() == 0)
        return;
    MyString baseURL = "";
    baseURL += TheApp->getImportURL();
    for (int i = 0; i < urls->getSize(); i++) {
        MyString path;
        URL urlI(baseURL, urls->getValue(i));
        if (urls->getValue(i).length() == 0) continue;
        m_scene->Download(urlI, &path);
    }
}

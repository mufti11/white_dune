/*
 * NodeRenderedTexture.cpp
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

#include <stdio.h>
#include "stdafx.h"

#include "NodeRenderedTexture.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFInt32.h"
#include "SFString.h"
#include "SFNode.h"
#include "SFNode.h"
#include "SFBool.h"
#include "SFBool.h"
#include "SFBool.h"
#include "MFBool.h"
#include "DuneApp.h"

ProtoRenderedTexture::ProtoRenderedTexture(Scene *scene2)
  : Proto(scene2, "RenderedTexture")
{

    background.set(
        addExposedField(SFNODE, "background", new SFNode(NULL), 
                 VRML_BACKGROUND));
    setFieldFlags(background, FF_X3DOM_ONLY);

    crossOrigin.set(
          addExposedField(SFSTRING, "crossOrigin", new SFString("")));
    setFieldFlags(crossOrigin, FF_X3DOM_ONLY);

    depthMap1.set(
        addExposedField(SFBOOL, "depthMap", new SFBool(false)));
    setFieldFlags(depthMap1, FF_X3DOM_ONLY);

    depthMap2.set(
        addExposedField(MFBOOL, "depthMap", new MFBool()));
    setFieldFlags(depthMap2, FF_KAMBI_ONLY);

    int values[] = { 128, 128, 4, 1, 1 };
    int *v = new int[5];  
    memcpy(v, values, 5 * sizeof(int));
    dimensions.set(
        addExposedField(MFINT32, "dimensions", new MFInt32(v, 5)));
    setFieldFlags(dimensions, FF_KAMBI_ONLY | FF_X3DOM_ONLY);

    excludeNodes.set(
          addExposedField(MFNODE, "excludeNodes", new MFNode()));
    setFieldFlags(excludeNodes, FF_X3DOM_ONLY);

    fog.set(
          addExposedField(SFNODE, "fog", new SFNode(NULL), VRML_FOG));
    setFieldFlags(fog, FF_X3DOM_ONLY);

    interpupillaryDistance.set(
          addExposedField(SFFLOAT, "interpupillaryDistance", new SFFloat(064)));
    setFieldFlags(interpupillaryDistance, FF_X3DOM_ONLY);

    oculusRiftVersion.set(
          addExposedField(SFFLOAT, "oculusRiftVersion", new SFFloat(1)));
    setFieldFlags(oculusRiftVersion, FF_X3DOM_ONLY);

    origChannelCount.set(
          addExposedField(SFINT32, "origChannelCount", new SFInt32(0)));
    setFieldFlags(origChannelCount, FF_X3DOM_ONLY);

    repeatS.set(
        addExposedField(SFBOOL, "repeatS", new SFBool(true)));
    setFieldFlags(repeatS, FF_KAMBI_ONLY | FF_X3DOM_ONLY);

    repeatT.set(
        addExposedField(SFBOOL, "repeatT", new SFBool(true)));
    setFieldFlags(repeatT, FF_KAMBI_ONLY | FF_X3DOM_ONLY);

    repeatR.set(
        addExposedField(SFBOOL, "repeatR", new SFBool(true)));
    setFieldFlags(repeatR, FF_KAMBI_ONLY);

    scale.set(
          addExposedField(SFBOOL, "scale", new SFBool(true)));
    setFieldFlags(scale, FF_X3DOM_ONLY);

    scene.set(
          addExposedField(SFNODE, "scene", new SFNode(NULL)));
    setFieldFlags(scene, FF_X3DOM_ONLY);

    showNormals.set(
          addExposedField(SFBOOL, "showNormals", new SFBool(false)));
    setFieldFlags(showNormals, FF_X3DOM_ONLY);

    stereoMode.set(
          addExposedField(SFSTRING, "stereoMode", new SFString("gif")));
    setFieldFlags(stereoMode, FF_X3DOM_ONLY);

    textureProperties.set(
        addExposedField(SFNODE, "textureProperties", new SFNode(), 
                 X3D_TEXTURE_PROPERTIES));
    setFieldFlags(textureProperties, FF_KAMBI_ONLY | FF_X3DOM_ONLY);
    
    update.set(
        addExposedField(SFSTRING, "update", new SFString("NONE")));
    setFieldFlags(update, FF_KAMBI_ONLY | FF_X3DOM_ONLY);

    url.set(
        addExposedField(MFSTRING, "url", new MFString()));
    setFieldFlags(url, FF_X3DOM_ONLY);

    viewpoint.set(
        addExposedField(SFNODE, "viewpoint", new SFNode(), VRML_VIEWPOINT));
    setFieldFlags(viewpoint, FF_KAMBI_ONLY | FF_X3DOM_ONLY);

    addEventOut(SFMATRIX4F, "viewing", FF_KAMBI_ONLY);
    addEventOut(SFMATRIX4F, "projection", FF_KAMBI_ONLY);
    addEventOut(SFBOOL, "rendering", FF_KAMBI_ONLY);

    kambiTextureCommonFields()
}

Node *
ProtoRenderedTexture::create(Scene *scene)
{ 
    return new NodeRenderedTexture(scene, this); 
}

NodeRenderedTexture::NodeRenderedTexture(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

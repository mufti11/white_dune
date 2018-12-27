/*
 * NodeComposedCubeMapTexture.cpp
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

#include "NodeComposedCubeMapTexture.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFNode.h"
#include "SFNode.h"
#include "SFNode.h"
#include "SFNode.h"
#include "SFNode.h"
#include "SFNode.h"
#include "DuneApp.h"

ProtoComposedCubeMapTexture::ProtoComposedCubeMapTexture(Scene *scene)
  : Proto(scene, "ComposedCubeMapTexture")
{
    back.set(
        addExposedField(SFNODE, "back", new SFNode(), TEXTURE_NODE));
    bottom.set(
        addExposedField(SFNODE, "bottom", new SFNode(), TEXTURE_NODE));
    front.set(
        addExposedField(SFNODE, "front", new SFNode(), TEXTURE_NODE));
    left.set(
        addExposedField(SFNODE, "left", new SFNode(), TEXTURE_NODE));
    right.set(
        addExposedField(SFNODE, "right", new SFNode(), TEXTURE_NODE));
    top.set(
        addExposedField(SFNODE, "top", new SFNode(), TEXTURE_NODE));

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

    textureProperties.set(
        addExposedField(SFNODE, "textureProperties", new SFNode(NULL),
                        X3D_TEXTURE_PROPERTIES));
    setFieldFlags(textureProperties, FF_X3DOM_ONLY);

    url.set(
        addExposedField(MFSTRING, "url", new MFString(true)));
    setFieldFlags(url, FF_X3DOM_ONLY);
}

Node *
ProtoComposedCubeMapTexture::create(Scene *scene)
{ 
    return new NodeComposedCubeMapTexture(scene, this); 
}

NodeComposedCubeMapTexture::NodeComposedCubeMapTexture(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

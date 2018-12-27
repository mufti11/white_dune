/*
 * NodeComposedTexture3D.cpp
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

#include "NodeComposedTexture3D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFBool.h"
#include "SFBool.h"
#include "SFBool.h"
#include "DuneApp.h"

ProtoComposedTexture3D::ProtoComposedTexture3D(Scene *scene)
  : Proto(scene, "ComposedTexture3D")
{
    texture.set(
        addExposedField(MFNODE, "texture", new MFNode(), TEXTURE_2D_NODE));
    repeatS.set(
        addField(SFBOOL, "repeatS", new SFBool(false)));
    repeatR.set(
        addField(SFBOOL, "repeatR", new SFBool(false)));
    repeatT.set(
        addField(SFBOOL, "repeatT", new SFBool(false)));

    kambiTextureCommonFields()

    crossOrigin.set(
        addExposedField(SFSTRING, "crossOrigin", new SFString("")));
    setFieldFlags(crossOrigin, FF_X3DOM_ONLY);

    origChannelCount.set(
        addExposedField(SFINT32, "origChannelCount", new SFInt32(0)));
    setFieldFlags(origChannelCount, FF_X3DOM_ONLY);

    scale.set(
        addExposedField(SFBOOL, "scale", new SFBool(true)));
    setFieldFlags(scale, FF_X3DOM_ONLY);

    textureProperties.set(
        addExposedField(SFNODE, "textureProperties", new SFNode(NULL),
                        X3D_TEXTURE_PROPERTIES));
    setFieldFlags(textureProperties, FF_X3DOM_ONLY);

    url.set(
        addExposedField(MFSTRING, "url", new MFString()));
    setFieldFlags(url, FF_X3DOM_ONLY);
}

Node *
ProtoComposedTexture3D::create(Scene *scene)
{ 
    return new NodeComposedTexture3D(scene, this); 
}

NodeComposedTexture3D::NodeComposedTexture3D(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

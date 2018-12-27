/*
 * NodeGeneratedCubeMapTexture.cpp
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

#include "NodeGeneratedCubeMapTexture.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFString.h"
#include "SFInt32.h"
#include "SFNode.h"
#include "DuneApp.h"

ProtoGeneratedCubeMapTexture::ProtoGeneratedCubeMapTexture(Scene *scene)
  : Proto(scene, "GeneratedCubeMapTexture")
{
    update.set(
        addExposedField(SFSTRING, "update", new SFString("NONE")));
    size.set(
        addField(SFINT32, "size", new SFInt32(128)));
    textureProperties.set(
        addField(SFNODE, "textureProperties", new SFNode(), 
                 X3D_TEXTURE_PROPERTIES));

    crossOrigin.set(
          addExposedField(SFSTRING, "crossOrigin", new SFString("")));
    setFieldFlags(crossOrigin, FF_X3DOM_ONLY);

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

    url.set(
          addExposedField(MFSTRING, "url", new MFString()));
    setFieldFlags(url, FF_X3DOM_ONLY);
}

Node *
ProtoGeneratedCubeMapTexture::create(Scene *scene)
{ 
    return new NodeGeneratedCubeMapTexture(scene, this); 
}

NodeGeneratedCubeMapTexture::NodeGeneratedCubeMapTexture(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

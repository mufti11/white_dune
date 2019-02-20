/*
 * NodePixelTexture3D.cpp
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

#include "NodePixelTexture3D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFInt32.h"
#include "SFBool.h"
#include "SFBool.h"
#include "SFBool.h"
#include "SFNode.h"
#include "DuneApp.h"

ProtoPixelTexture3D::ProtoPixelTexture3D(Scene *scene)
  : Proto(scene, "PixelTexture3D")
{
    int values[] = { 0, 0, 0, 0 };
    int *i = new int[4];  
    memcpy(i, values, 4 * sizeof(int));
    image.set(
        addExposedField(MFINT32, "image", new MFInt32(i, 4)));
    repeatS.set(
        addField(SFBOOL, "repeatS", new SFBool(false)));
    repeatR.set(
        addField(SFBOOL, "repeatR", new SFBool(false)));
    repeatT.set(
        addField(SFBOOL, "repeatT", new SFBool(false)));
    textureProperties.set(
        addField(SFNODE, "textureProperties", new SFNode(),
                 X3D_TEXTURE_PROPERTIES));

    kambiTextureCommonFields()

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
ProtoPixelTexture3D::create(Scene *scene)
{ 
    return new NodePixelTexture3D(scene, this); 
}

NodePixelTexture3D::NodePixelTexture3D(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_textureTableIndex = 0;
}

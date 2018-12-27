/*
 * NodeTextureProperties.cpp
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

#include "NodeTextureProperties.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFFloat.h"
#include "SFColorRGBA.h"
#include "SFInt32.h"
#include "SFString.h"
#include "SFString.h"
#include "SFString.h"
#include "SFString.h"
#include "SFString.h"
#include "SFString.h"
#include "SFFloat.h"
#include "SFBool.h"
#include "DuneApp.h"

ProtoTextureProperties::ProtoTextureProperties(Scene *scene)
  : Proto(scene, "TextureProperties")
{
    anisotropicDegree.set(
        addExposedField(SFFLOAT, "anisotropicDegree", new SFFloat(1.0)));
    borderColor.set(
        addExposedField(SFCOLORRGBA, "borderColor", new SFColorRGBA(0, 0, 0, 0)));
    borderWidth.set(
        addExposedField(SFINT32, "borderWidth", new SFInt32(0)));
    boundaryModeS.set(
        addExposedField(SFSTRING, "boundaryModeS", new SFString("REPEAT")));
    boundaryModeT.set(
        addExposedField(SFSTRING, "boundaryModeT", new SFString("REPEAT")));
    boundaryModeR.set(
        addExposedField(SFSTRING, "boundaryModeR", new SFString("REPEAT")));
    magnificationFilter.set(
        addExposedField(SFSTRING, "magnificationFilter", new SFString("FASTEST")));
    minificationFilter.set(
        addExposedField(SFSTRING, "minificationFilter", new SFString("FASTEST")));
    textureCompression.set(
        addExposedField(SFSTRING, "textureCompression", new SFString("FASTEST")));
    texturePriority.set(
        addExposedField(SFFLOAT, "texturePriority", new SFFloat(0)));
    generateMipMaps.set(
        addField(SFBOOL, "generateMipMaps", new SFBool(false)));

    guiTexture.set(
        addField(SFBOOL, "guiTexture", new SFBool(false)));
    setFieldFlags(guiTexture, FF_KAMBI_ONLY);
}

Node *
ProtoTextureProperties::create(Scene *scene)
{ 
    return new NodeTextureProperties(scene, this); 
}

NodeTextureProperties::NodeTextureProperties(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

/*
 * NodeCubeTexture.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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

#include "NodeCubeTexture.h"
#include "DuneApp.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFVec2f.h"
#include "SFVec3f.h"
#include "Field.h"


ProtoCubeTexture::ProtoCubeTexture(Scene *scene)
  : Proto(scene, "CubeTexture")
{
    urlXP.set(
          addExposedField(MFSTRING, "urlXP", new MFString(), FF_URL, NULL));

    urlXN.set(
          addExposedField(MFSTRING, "urlXN", new MFString(), FF_URL, NULL));

    urlYP.set(
          addExposedField(MFSTRING, "urlYP", new MFString(), FF_URL, NULL));

    urlYN.set(
          addExposedField(MFSTRING, "urlYN", new MFString(), FF_URL, NULL));

    urlZP.set(
          addExposedField(MFSTRING, "urlZP", new MFString(), FF_URL, NULL));

    urlZN.set(
          addExposedField(MFSTRING, "urlZN", new MFString(), FF_URL, NULL));

    repeatS.set(
          addField(SFBOOL, "repeatS", new SFBool(true)));

    repeatT.set(
          addField(SFBOOL, "repeatT", new SFBool(true)));

    blendMode.set(
          addField(SFINT32, "blendMode", new SFInt32(0), 
                   new SFInt32(0), new SFInt32(5)));

    addURLs(URL_COVER);
}

Node *
ProtoCubeTexture::create(Scene *scene)
{ 
   return new NodeCubeTexture(scene, this); 
}

NodeCubeTexture::NodeCubeTexture(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
}

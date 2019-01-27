/*
 * NodeMovieTexture.cpp
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

#include "NodeMovieTexture.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFTime.h"
#include "MFString.h"
#include "Field.h"  // for FF_URL
#include "ExposedField.h"
#include "DuneApp.h"
#include "Scene.h"

ProtoMovieTexture::ProtoMovieTexture(Scene *scene)
  : Proto(scene, "MovieTexture")
{
    description.set( 
        addExposedField(SFSTRING, "description", new SFString("")));
    setFieldFlags(description, FF_X3D_ONLY);
    loop.set(
          addExposedField(SFBOOL, "loop", new SFBool(false)));
    speed.set(
          addExposedField(SFFLOAT, "speed", new SFFloat(1.0f)));
    pauseTime.set(
          addExposedField(SFTIME, "pauseTime", new SFTime(0.0)));
    setFieldFlags(pauseTime, FF_X3D_ONLY); 
    pitch.set(
          addExposedField(SFFLOAT, "pitch", new SFFloat(1.0f), 
                          new SFFloat(0.0f)));
    setFieldFlags(pitch, FF_X3D_ONLY); 
    resumeTime.set(
          addExposedField(SFTIME, "resumeTime", new SFTime(0.0)));
    setFieldFlags(resumeTime, FF_X3D_ONLY);
    startTime.set(
          addExposedField(SFTIME, "startTime", new SFTime(0.0f)));
    setFieldFlags(startTime, EIF_RECOMMENDED);
    stopTime.set(
          addExposedField(SFTIME, "stopTime", new SFTime(0.0f)));
    setFieldFlags(stopTime, EIF_RECOMMENDED);
    url.set(
          addExposedField(MFSTRING, "url", new MFString(), FF_URL, NULL));
    repeatS.set(
          addField(SFBOOL, "repeatS", new SFBool(true)));
    repeatT.set(
          addField(SFBOOL, "repeatT", new SFBool(true)));
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        blendMode.set(
          addField(SFINT32, "blendMode", new SFInt32(0), 
                   new SFInt32(0), new SFInt32(5)));
        setFieldFlags(blendMode, FF_COVER_ONLY);
    }

    textureProperties.set(
        addField(SFNODE, "textureProperties", new SFNode(), 
                 X3D_TEXTURE_PROPERTIES));
    setFieldFlags(textureProperties, FF_X3D_ONLY);

    kambiTextureCommonFields()
    kambiVrmlTextureCommonFields()

    crossOrigin.set(
        addExposedField(SFSTRING, "crossOrigin", new SFString("")));
    setFieldFlags(crossOrigin, FF_X3DOM_ONLY);

    hideChildren.set(
        addExposedField(SFBOOL, "hideChildren", new SFBool(true)));
    setFieldFlags(hideChildren, FF_X3DOM_ONLY);

    origChannelCount.set(
        addExposedField(SFINT32, "origChannelCount", new SFInt32(0), 
                        new SFInt32(0)));
    setFieldFlags(origChannelCount, FF_X3DOM_ONLY);

    scale.set(
        addExposedField(SFBOOL, "scale", new SFBool(true)));
    setFieldFlags(scale, FF_X3DOM_ONLY);

    addEventOut(SFTIME, "duration_changed");
    addEventOut(SFTIME, "elapsedTime", FF_X3D_ONLY);
    addEventOut(SFBOOL, "isActive");
    addEventOut(SFBOOL, "isPaused", FF_X3D_ONLY);
}

Node *
ProtoMovieTexture::create(Scene *scene)
{ 
    return new NodeMovieTexture(scene, this);
}

NodeMovieTexture::NodeMovieTexture(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeMovieTexture::load()
{
    MFString *urls = url();

    if (urls->getSize() == 0)
        return;
    for (int i = 0; i < urls->getSize(); i++) {
        MyString path;
        URL urlI(TheApp->getImportURL(), urls->getValue(i));
        if (urls->getValue(i).length() == 0) continue;
        m_scene->Download(urlI, &path);
        // download only
    }
}

/*
 * NodeAudioClip.cpp
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

#include "NodeAudioClip.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFString.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "SFTime.h"
#include "MFString.h"
#include "Field.h"  // for FF_URL
#include "ExposedField.h"
#include "Scene.h"

ProtoAudioClip::ProtoAudioClip(Scene *scene)
  : Proto(scene, "AudioClip")
{
    description.set(
          addExposedField(SFSTRING, "description", new SFString("")));

    loop.set(
          addExposedField(SFBOOL, "loop", new SFBool(false)));

    pauseTime.set(
          addExposedField(SFTIME, "pauseTime", new SFTime(0.0f)));
    setFieldFlags(pauseTime, FF_X3D_ONLY);

    pitch.set(
          addExposedField(SFFLOAT, "pitch", new SFFloat(1.0f), 
                          new SFFloat(0.0f)));

    resumeTime.set(
          addExposedField(SFTIME, "resumeTime", new SFTime(0.0f)));
    setFieldFlags(resumeTime, FF_X3D_ONLY);

    startTime.set(
          addExposedField(SFTIME, "startTime", new SFTime(0.0f)));
    setFieldFlags(startTime, EIF_RECOMMENDED);

    stopTime.set(
          addExposedField(SFTIME, "stopTime", new SFTime(0.0f)));
    setFieldFlags(stopTime, EIF_RECOMMENDED);

    url.set(
          addExposedField(MFSTRING, "url", new MFString(), FF_URL, NULL));

    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    setFieldFlags(enabled, FF_X3DOM_ONLY);

    addEventOut(SFTIME, "duration_changed");
    addEventOut(SFTIME, "elapsedTime", FF_X3D_ONLY);
    addEventOut(SFBOOL, "isActive");
    addEventOut(SFBOOL, "isPaused", FF_X3D_ONLY);
}

Node *
ProtoAudioClip::create(Scene *scene)
{ 
    return new NodeAudioClip(scene, this); 
}

NodeAudioClip::NodeAudioClip(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeAudioClip::load()
{
    MFString *urls = url();
    
    if (urls->getSize() == 0)
        return;
    for (int i = 0; i < urls->getSize(); i++) {
        MyString path;
        URL urlI(TheApp->getImportURL(), urls->getValue(i)->getValue());
        if (strlen(urls->getValue(i)->getValue()) == 0) continue;
        m_scene->Download(urlI, &path);
        // download only
    }
}

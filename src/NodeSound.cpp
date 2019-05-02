/*
 * NodeSound.cpp
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

#include "NodeSound.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "SFNode.h"
#include "SFBool.h"
#include "DuneApp.h"
#include "resource.h"

ProtoSound::ProtoSound(Scene *scene)
  : Proto(scene, "Sound")
{

    direction.set(
          addExposedField(SFVEC3F, "direction", new SFVec3f(0.0f, 0.0f, 1.0f)));
    intensity.set(
          addExposedField(SFFLOAT, "intensity", new SFFloat(1.0f),
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    location.set(
          addExposedField(SFVEC3F, "location", new SFVec3f(0.0f, 0.0f, 0.0f)));
    maxBack.set(
          addExposedField(SFFLOAT, "maxBack", new SFFloat(10.0f),
                          new SFFloat(0.0f)));
    maxFront.set(
          addExposedField(SFFLOAT, "maxFront", new SFFloat(10.0f), 
                          new SFFloat(0.0f)));
    minBack.set(
          addExposedField(SFFLOAT, "minBack", new SFFloat(1.0f), 
                          new SFFloat(0.0f)));
    minFront.set(
          addExposedField(SFFLOAT, "minFront", new SFFloat(1.0f), 
                          new SFFloat(0.0f)));
    priority.set(
          addExposedField(SFFLOAT, "priority", new SFFloat(0.0f),
                          new SFFloat(0.0f), new SFFloat(1.0f)));
    source.set(
          addExposedField(SFNODE, "source", new SFNode(NULL), 
                          AUDIO_CLIP_OR_MOVIE_TEXTURE_NODE));
    spatialize.set(
          addField(SFBOOL, "spatialize", new SFBool(true)));
    if (TheApp->getCoverMode()) {
        // non standard Covise/COVER extensions
        doppler.set(
          addExposedField(SFBOOL, "doppler", new SFBool(false)));
        setFieldFlags(doppler, FF_COVER_ONLY);
    }
}

Node *
ProtoSound::create(Scene *scene)
{ 
    return new NodeSound(scene, this); 
}

NodeSound::NodeSound(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

int
NodeSound::getAnimationCommentID(void) 
{ 
    return IDS_ANIMATION_HELP_SOUND + swGetLang(); 
}


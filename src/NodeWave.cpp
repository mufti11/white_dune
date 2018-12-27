/*
 * NodeWave.cpp
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
#include <float.h>
#include "stdafx.h"

#include "NodeWave.h"
#include "DuneApp.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFVec2f.h"
#include "SFVec3f.h"
#include "Field.h"


ProtoWave::ProtoWave(Scene *scene)
  : Proto(scene, "Wave")
{
    fraction.set(
          addExposedField(SFFLOAT, "fraction", new SFFloat(0)));
    freq1.set(
          addExposedField(SFFLOAT, "freq1", new SFFloat(0.18f)));
    height1.set(
          addExposedField(SFFLOAT, "height1", new SFFloat(0.1f)));
    damping1.set(
          addExposedField(SFFLOAT, "damping1", new SFFloat(1.0f)));
    dir1.set(
          addExposedField(SFVEC3F, "dir1", new SFVec3f(1,1,0)));
    freq2.set(
          addExposedField(SFFLOAT, "freq2", new SFFloat(3.0f)));
    height2.set(
          addExposedField(SFFLOAT, "height2", new SFFloat(0.1f)));
    damping2.set(
          addExposedField(SFFLOAT, "damping2", new SFFloat(1.0f)));
    dir2.set(
          addExposedField(SFVEC3F, "dir2", new SFVec3f(1,0,0)));
    speed1.set(
          addExposedField(SFFLOAT, "speed1", new SFFloat(1.0f)));
    speed2.set(
          addExposedField(SFFLOAT, "speed2", new SFFloat(1.0f)));
    coeffSin.set(
          addExposedField(SFROTATION, "coeffSin", 
                          new SFRotation(1.0,-1.0f / 6.0f,1.0f / 120.0f,
                                         -1.0f / 5040.0f 
                                         /* * M_PI / 180.0f ???? */)));
    coeffCos.set(
          addExposedField(SFROTATION, "coeffCos", 
                          new SFRotation(1.0,-1.0f / 2.0f,1.0f / 24.0f,
                                         -1.0f / 720.0f 
                                         /* * M_PI / 180.0f ???? */)));
    fileName.set(
          addExposedField(SFSTRING, "fileName", new SFString("cg_water.cg")));

    addURLs(URL_COVER);
}

Node *
ProtoWave::create(Scene *scene)
{ 
   return new NodeWave(scene, this); 
}

NodeWave::NodeWave(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
}

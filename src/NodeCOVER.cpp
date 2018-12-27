/*
 * NodeCOVER.cpp
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

#include "NodeCOVER.h"
#include "DuneApp.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFVec2f.h"
#include "SFVec3f.h"
#include "Field.h"


ProtoCOVER::ProtoCOVER(Scene *scene)
  : Proto(scene, "COVER")
{
    position1.set(
          addExposedField(SFVEC3F, "position1", new SFVec3f(0, 0, 0)));

    position2.set(
          addExposedField(SFVEC3F, "position2", new SFVec3f(0, 0, 0)));

    position3.set(
          addExposedField(SFVEC3F, "position3", new SFVec3f(0, 0, 0)));

    position4.set(
          addExposedField(SFVEC3F, "position4", new SFVec3f(0, 0, 0)));

    position5.set(
          addExposedField(SFVEC3F, "position5", new SFVec3f(0, 0, 0)));

    position6.set(
          addExposedField(SFVEC3F, "position6", new SFVec3f(0, 0, 0)));

    position7.set(
          addExposedField(SFVEC3F, "position7", new SFVec3f(0, 0, 0)));

    position8.set(
          addExposedField(SFVEC3F, "position8", new SFVec3f(0, 0, 0)));

    orientation1.set(
          addExposedField(SFROTATION, "orientation1", 
                          new SFRotation(0, 0, 1, 0)));

    orientation2.set(
          addExposedField(SFROTATION, "orientation2", 
                          new SFRotation(0, 0, 1, 0)));

    orientation3.set(
          addExposedField(SFROTATION, "orientation3", 
                          new SFRotation(0, 0, 1, 0)));

    orientation4.set(
          addExposedField(SFROTATION, "orientation4", 
                          new SFRotation(0, 0, 1, 0)));

    orientation5.set(
          addExposedField(SFROTATION, "orientation5", 
                          new SFRotation(0, 0, 1, 0)));

    orientation6.set(
          addExposedField(SFROTATION, "orientation6", 
                          new SFRotation(0, 0, 1, 0)));

    orientation7.set(
          addExposedField(SFROTATION, "orientation7", 
                          new SFRotation(0, 0, 1, 0)));

    orientation8.set(
          addExposedField(SFROTATION, "orientation8", 
                          new SFRotation(0, 0, 1, 0)));

    soundEnvironment.set(
          addExposedField(SFINT32, "soundEnvironment", 
                          new SFInt32(26)));

    addEventOut(SFSTRING, "localKeyPressed", EOF_RECOMMENDED);
    addEventOut(SFSTRING, "localKeyReleased");

    addEventOut(SFSTRING, "keyPressed");
    addEventOut(SFSTRING, "keyReleased");
    
    addEventOut(SFVEC3F, "avatar1Position");
    addEventOut(SFROTATION, "avatar1Orientation");

    addEventOut(SFVEC3F, "localPosition", EOF_RECOMMENDED);
    addEventOut(SFROTATION, "localOrientation", EOF_RECOMMENDED);

    addURLs(URL_COVER);
}

Node *
ProtoCOVER::create(Scene *scene)
{ 
   return new NodeCOVER(scene, this); 
}

NodeCOVER::NodeCOVER(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
}

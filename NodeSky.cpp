/*
 * NodeSky.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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

#include "NodeSky.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFInt32.h"
#include "SFFloat.h"

#define COVER_FAR_CLIPPING_PLANE 1000.0f

ProtoSky::ProtoSky(Scene *scene)
  : Proto(scene, "Sky")
{
    addEventIn(SFTIME, "set_time");

    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    timeLapse.set(
          addExposedField(SFBOOL, "timeLapse", new SFBool(false)));
    currentTime.set(
          addExposedField(SFBOOL, "currentTime", new SFBool(true)));
    year.set(
          addExposedField(SFINT32, "year", new SFInt32(2006)));
    month.set(
          addExposedField(SFINT32, "month", new SFInt32(4),
                          new SFInt32(1), new SFInt32(12)));
    day.set(
          addExposedField(SFINT32, "day", new SFInt32(7),
                          new SFInt32(1), new SFInt32(31)));
    hour.set(
          addExposedField(SFINT32, "hour", new SFInt32(13),  
                          new SFInt32(0), new SFInt32(23)));
    minute.set(
          addExposedField(SFINT32, "minute", new SFInt32(12),  
                          new SFInt32(0), new SFInt32(59)));
    radius.set(
          addExposedField(SFFLOAT, "radius",
                          new SFFloat(COVER_FAR_CLIPPING_PLANE * 0.8f)));
    latitude.set(
          addExposedField(SFFLOAT, "latitude", new SFFloat(48.6f)));
    longitude.set(
          addExposedField(SFFLOAT, "longitude", new SFFloat(9.0008f)));
    altitude.set(
          addExposedField(SFFLOAT, "altitude", new SFFloat(400.0f)));

    addURLs(URL_COVER);
}

Node *
ProtoSky::create(Scene *scene)
{ 
    return new NodeSky(scene, this); 
}

NodeSky::NodeSky(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
}


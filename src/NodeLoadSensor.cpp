/*
 * NodeLoadSensor.cpp
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

#include "NodeLoadSensor.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFBool.h"
#include "SFNode.h"
#include "SFTime.h"
#include "SFFloat.h"
#include "MFNode.h"
#include "DuneApp.h"
#include "Field.h"

ProtoLoadSensor::ProtoLoadSensor(Scene *scene)
  : Proto(scene, "LoadSensor")
{
    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    timeOut.set(
          addExposedField(SFTIME, "timeOut", new SFTime(0.0)));
    watchList.set(
          addExposedField(MFNODE, "watchList", new MFNode(), URL_NODE));
    addEventOut(SFBOOL, "isActive"); 
    addEventOut(SFBOOL, "isLoaded"); 
    addEventOut(SFTIME, "loadTime", EOF_RECOMMENDED);
    addEventOut(SFFLOAT, "progress");

    addURLs(URL_X3D);
}

Node *
ProtoLoadSensor::create(Scene *scene)
{ 
   return new NodeLoadSensor(scene, this); 
}

NodeLoadSensor::NodeLoadSensor(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

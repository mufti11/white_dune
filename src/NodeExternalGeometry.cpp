/*
 * NodeExternalGeometry.cpp
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

#include "NodeExternalGeometry.h"
#include "Scene.h"

ProtoExternalGeometry::ProtoExternalGeometry(Scene *scene)
  : Proto(scene, "ExternalGeometry")
{
    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);

    x3domGeometryCommonFields()

    url.set(
          addExposedField(MFSTRING, "url", new MFString()));
    setFieldFlags(url, FF_X3DOM_ONLY);
}

Node *
ProtoExternalGeometry::create(Scene *scene)
{ 
    return new NodeExternalGeometry(scene, this); 
}

NodeExternalGeometry::NodeExternalGeometry(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeExternalGeometry::load()
{
    MyString baseURL = "";
    baseURL += TheApp->getImportURL();
    MyString path;
    for (int i = 0; i < url()->getSize(); i++) {
        URL url1(baseURL, url()->getValue(i)->getValue());
        if (strlen(url()->getValue(i)->getValue()) > 0) 
            m_scene->Download(url1, &path);
    }
}

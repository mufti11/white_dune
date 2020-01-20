/*
 * NodeMultiPart.cpp
 *
 * Copyright (C) 2018 J. "MUFTI" Scheurich
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

#include "NodeMultiPart.h"
#include "Proto.h"
#include "DuneApp.h"
#include "Scene.h"

ProtoMultiPart::ProtoMultiPart(Scene *scene)
  : ProtoGroup(scene, "MultiPart")
{
    initialVisibility.set(
        addExposedField(SFSTRING, "initialVisibility", new SFString("auto")));
    setFieldFlags(initialVisibility, FF_X3DOM_ONLY);

    isPickable.set(
        addExposedField(SFBOOL, "isPickable", new SFBool(true)));
    setFieldFlags(isPickable, FF_X3DOM_ONLY);

    load.set(
        addExposedField(SFBOOL, "load", new SFBool(true)));
    setFieldFlags(load, FF_X3DOM_ONLY);

    mapDEFToID.set(
        addExposedField(SFBOOL, "mapDEFToID", new SFBool(false)));
    setFieldFlags(mapDEFToID, FF_X3DOM_ONLY);

    nameSpaceName.set(
        addExposedField(MFSTRING, "nameSpaceName", new MFString()));
    setFieldFlags(nameSpaceName, FF_X3DOM_ONLY);

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);

    solid.set(
        addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);

    sortKey.set(
        addExposedField(SFINT32, "sortKey", new SFInt32(0)));
    setFieldFlags(sortKey, FF_X3DOM_ONLY);

    sortType.set(
        addExposedField(SFSTRING, "sortType", new SFString("auto")));
    setFieldFlags(sortType, FF_X3DOM_ONLY);

    url.set(
        addExposedField(MFSTRING, "url", new MFString()));
    setFieldFlags(url, FF_X3DOM_ONLY);

    urlIDMap.set(
        addExposedField(MFSTRING, "urlIDMap", new MFString()));
    setFieldFlags(urlIDMap, FF_X3DOM_ONLY);
}

Node *
ProtoMultiPart::create(Scene *scene)
{ 
    return new NodeMultiPart(scene, this); 
}

NodeMultiPart::NodeMultiPart(Scene *scene, Proto *def)
  : NodeGroup(scene, def)
{
}
 

void
NodeMultiPart::load()
{
    MyString baseURL = "";
    baseURL += TheApp->getImportURL();
    MyString path;
    for (int i = 0; i < url()->getSize(); i++) {
        URL url1(baseURL, url()->getValue(i));
        if (strlen(url()->getValue(i)) > 0) 
            m_scene->Download(url1, &path);
    }
}


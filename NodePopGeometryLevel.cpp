/*
 * NodePopGeometryLevel.cpp
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

#include "NodePopGeometryLevel.h"
#include "Scene.h"

ProtoPopGeometryLevel::ProtoPopGeometryLevel(Scene *scene)
  : Proto(scene, "PopGeometryLevel")
{
    numIndices.set(
          addExposedField(SFINT32, "numIndices", new SFInt32(0)));
    setFieldFlags(numIndices, FF_X3DOM_ONLY);

    src.set(
          addExposedField(SFSTRING, "src", new SFString("")));
    setFieldFlags(src, FF_X3DOM_ONLY);

    vertexDataBufferOffset.set(
          addExposedField(SFINT32, "vertexDataBufferOffset", new SFInt32(0)));
    setFieldFlags(vertexDataBufferOffset, FF_X3DOM_ONLY);
}

Node *
ProtoPopGeometryLevel::create(Scene *scene)
{ 
    return new NodePopGeometryLevel(scene, this); 
}

NodePopGeometryLevel::NodePopGeometryLevel(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodePopGeometryLevel::load()
{
    MyString baseURL = "";
    baseURL += TheApp->getImportURL();
    MyString path;
    URL url1(baseURL, src()->getValue());
    if (strlen(src()->getValue()) > 0) 
        m_scene->Download(url1, &path);
}

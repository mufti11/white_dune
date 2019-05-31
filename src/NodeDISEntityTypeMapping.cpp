/*
 * NodeDISEntityTypeMapping.cpp
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#include "NodeDISEntityTypeMapping.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFString.h"
#include "SFInt32.h"
#include "Scene.h"
#include "DuneApp.h"

ProtoDISEntityTypeMapping::ProtoDISEntityTypeMapping(Scene *scene)
  : Proto(scene, "DISEntityTypeMapping")
{
    url.set(
        addExposedField(MFSTRING, "url", new MFString()));
    category.set(
        addField(SFINT32, "category", new SFInt32(0)));
    country.set(
        addField(SFINT32, "country", new SFInt32(0)));
    domain.set(
        addField(SFINT32, "domain", new SFInt32(0)));
    extra.set(
        addField(SFINT32, "extra", new SFInt32(0)));
    kind.set(
        addField(SFINT32, "kind", new SFInt32(0)));
    specific.set(
        addField(SFINT32, "specific", new SFInt32(0)));
    subcategory.set(
        addField(SFINT32, "subcategory", new SFInt32(0)));
}

Node *
ProtoDISEntityTypeMapping::create(Scene *scene)
{ 
    return new NodeDISEntityTypeMapping(scene, this); 
}

NodeDISEntityTypeMapping::NodeDISEntityTypeMapping(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeDISEntityTypeMapping::load()
{
    MFString *urls = url();
    if (urls->getSize() == 0)
        return;
    m_baseURL = "";
    m_baseURL += TheApp->getImportURL();
    for (int i = 0; i < urls->getSize(); i++) {
        MyString path;
        URL urlI(m_baseURL, urls->getValue(i));
        if (urls->getValue(i).length() == 0) continue;
        m_scene->Download(urlI, &path);
    }
}

/*
 * NodeLabView.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2008 J. "MUFTI" Scheurich
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

#include "swt.h"
#include "DuneApp.h"
#include "resource.h"

#include "NodeLabView.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFImage.h"
#include "SFBool.h"
#include "Texture.h"

ProtoLabView::ProtoLabView(Scene *scene)
  : Proto(scene, "LabView")
{
    enabled.set(
          addExposedField(SFBOOL, "enabled", new SFBool(true)));
    addEventOut(MFINT32, "ints_changed");
    addEventOut(MFFLOAT, "floats_changed");

    addURLs(URL_COVER);
}

Node *
ProtoLabView::create(Scene *scene)
{ 
    return new NodeLabView(scene, this); 
}

NodeLabView::NodeLabView(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
}

NodeLabView::NodeLabView(NodeLabView &node)
  : CoverNode(node)
{
}

NodeLabView::~NodeLabView()
{
}

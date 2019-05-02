/*
 * TUIElement.cpp 
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

#include "TUIElement.h"
#include "DuneApp.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFString.h"
#include "SFVec2f.h"
#include "Field.h"

void ProtoTUIElement::addElements(void)
{
    elementName.set(
          addExposedField(SFSTRING, "elementName", new SFString("")));
    parent.set(
          addExposedField(SFSTRING, "parent", new SFString("")));
    pos.set(
          addExposedField(SFVEC2F, "pos", new SFVec2f(0.0f, 0.0f)));
}

ProtoTUIElement::ProtoTUIElement(Scene *scene, const char* nodeName)
  : Proto(scene, nodeName)
{
    addElements();
}

ProtoTUIElement::ProtoTUIElement(Scene *scene)
  : Proto(scene, "TUIElement")
{
    addElements();
}

Node *
ProtoTUIElement::create(Scene *scene)
{ 
   return new TUIElement(scene, this); 
}

TUIElement::TUIElement(Scene *scene, Proto *def)
  : CoverNode(scene, def)
{
}



/*
 * NodeTUIComboBox.cpp 
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

#include "NodeTUIComboBox.h"
#include "DuneApp.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFString.h"
#include "SFVec2f.h"
#include "Field.h"


ProtoTUIComboBox::ProtoTUIComboBox(Scene *scene)
  : ProtoTUIElement(scene, "TUIComboBox")
{
    items.set(
          addExposedField(MFSTRING, "items", new MFString()));
    withNone.set(
          addExposedField(SFBOOL, "withNone", new SFBool(true)));
    defaultChoice.set(
          addExposedField(SFINT32, "defaultChoice", new SFInt32(0)));
    addEventOut(SFINT32, "choice");

    addURLs(URL_COVER);
}

Node *
ProtoTUIComboBox::create(Scene *scene)
{ 
   return new NodeTUIComboBox(scene, this); 
}

NodeTUIComboBox::NodeTUIComboBox(Scene *scene, Proto *def)
  : TUIElement(scene, def)
{
}


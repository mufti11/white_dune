/*
 * NodeExample.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
 *
 * Example of a minimal VRML97 node implementation
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
 * not, write to the Free Software Foundation, Inc., 
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdio.h>
#include "stdafx.h"

#include "NodeExample.h"
#include "Proto.h"
#include "FieldValue.h"

//replace SFExampleTyp or MFExampleTyp with something like SFBool or MFVec3f
#include "SFExampleTyp.h"
#include "MFExampleTyp.h"

ProtoExample::ProtoExample(Scene *scene)
  : Proto(scene, "Example")
{
    addEventIn(SFEXAMPLETYP, "eventIn1");

    field1.set(
          addField(SFEXAMPLETYP, "field1", new SFExampleTyp(arguments)));

    field2.set(
          addExposedField(MFEXAMPLETYP, "field2", new MFExampleTyp(arguments)));

    addEventOut(MFEXAMPLETYP, "eventOut1");
}

Node *
ProtoExample::create(Scene *scene)
{ 
    return new NodeExample(scene, this); 
}

NodeExample::NodeExample(Scene *scene, Proto *def)
  : Node(scene, def)
{
}


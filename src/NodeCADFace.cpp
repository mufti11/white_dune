/*
 * NodeCADFace.cpp
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

#include "NodeCADFace.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFString.h"
#include "SFNode.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "DuneApp.h"

ProtoCADFace::ProtoCADFace(Scene *scene)
  : WonderlandExportProto(scene, "CADFace")
{
    name.set(
          addExposedField(SFSTRING, "name", new SFString("")));
    shape.set(
          addExposedField(SFNODE, "shape", new SFNode(), SHAPE_OR_LOD_NODE));
    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1)));

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);
}

Node *
ProtoCADFace::create(Scene *scene)
{ 
    return new NodeCADFace(scene, this); 
}

NodeCADFace::NodeCADFace(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeCADFace::preDraw() 
{
    if (shape()->getValue())
        shape()->getValue()->preDraw(); 
} 

void
NodeCADFace::draw(int pass) 
{
    if (shape()->getValue())
        shape()->getValue()->draw(pass);
} 

void
NodeCADFace::flip(int index) 
{
    if (shape()->getValue())
        shape()->getValue()->flip(index);
} 

void
NodeCADFace::swap(int fromTo) 
{
    if (shape()->getValue())
        shape()->getValue()->swap(fromTo);
} 

int
NodeCADFace::countPolygons(void)
{
    if (shape()->getValue())
        return shape()->getValue()->countPolygons(); 
    return 0;
} 

int
NodeCADFace::countPrimitives(void) 
{
    if (shape()->getValue())
        return shape()->getValue()->countPrimitives();
    return 0;
}

int
NodeCADFace::countPolygons1Sided(void)
{
    if (shape()->getValue())
        return shape()->getValue()->countPolygons1Sided();
    return 0;
} 

int
NodeCADFace::countPolygons2Sided(void)
{
    if (shape()->getValue())
        return shape()->getValue()->countPolygons2Sided(); 
    return 0;
} 



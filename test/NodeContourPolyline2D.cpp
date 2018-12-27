/*
 * NodeContourPolyline2D.cpp
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

#include "NodeContourPolyline2D.h"
#include "Proto.h"
#include "MFVec3f.h"
#include "ExposedField.h"
#include "Field.h"
#include "DuneApp.h"
#include "Scene.h"

ProtoContourPolyline2D::ProtoContourPolyline2D(Scene *scene)
  : Proto(scene, "ContourPolyline2D")
{
    static bool init = true;
    ExposedField* l;
    if (init || (scene->getParsedX3dVersion() != 0))
        l = new ExposedField(MFVEC2D, "lineSegments", new MFVec2d(), NULL, 
                             NULL, ANY_NODE, 0, NULL, "controlPoint");
    else
        l = new ExposedField(MFVEC2F, "lineSegments", new MFVec2f(), NULL, 
                             NULL, ANY_NODE, 0, NULL, "point");
    init = false;
    lineSegments.set(addExposedField(l));
}

Node *
ProtoContourPolyline2D::create(Scene *scene)
{ 
    return new NodeContourPolyline2D(scene, this); 
}

NodeContourPolyline2D::NodeContourPolyline2D(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeContourPolyline2D::setField(int index, FieldValue *value)
{
    Node::setField(index, value);
    if (hasParent())
        getParent()->update();
}

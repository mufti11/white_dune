/*
 * NodeColorRGBA.cpp
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

#include "NodeColorRGBA.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFColorRGBA.h"
#include "NodeShape.h"
#include "NodeAppearance.h"
#include "NodeMaterial.h"
#include "NodeCoordinate.h"
#include "NodeElevationGrid.h"
#include "NodeGeoElevationGrid.h"
#include "Util.h"

ProtoColorRGBA::ProtoColorRGBA(Scene *scene)
  : WonderlandExportProto(scene, "ColorRGBA")
{
    color.set(
          addExposedField(MFCOLORRGBA, "color", new MFColorRGBA()));
}

Node *
ProtoColorRGBA::create(Scene *scene)
{
    return new NodeColorRGBA(scene, this); 
}

NodeColorRGBA::NodeColorRGBA(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeColorRGBA::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    if (hasParent())
        getParents().update();
}

int
NodeColorRGBA::getComponentLevel(void) const
{
    const float *rgba = color()->getValues();
    for (int i = 3; i < color()->getSize(); i += 4)
        if (rgba[i] != 0)
            return 4;
    return 1;
}

void
NodeColorRGBA::drawHandles(void)
{
    if (getParent() && getParent()->getColored()) {
        NodeCoordinate *coord = (NodeCoordinate *)
                                getParent()->getCoordinateNode();
        if (coord)
            coord->drawHandles();
    }
    if (getParent() && (getParent()->getType() == VRML_ELEVATION_GRID)) {
        NodeElevationGrid *grid = (NodeElevationGrid *)getParent();
        if (grid)
            grid->drawHandles();
    }
    if (getParent() && (getParent()->getType() == VRML_GEO_ELEVATION_GRID)) {
        NodeGeoElevationGrid *grid = (NodeGeoElevationGrid *)getParent();
        if (grid)
            grid->drawHandles();
    }
}

Vec3f
NodeColorRGBA::getHandle(int handle, int *constraint, int *field)
{
    if (getParent() && getParent()->getColored()) {
        NodeCoordinate *coord = (NodeCoordinate *)
                                getParent()->getCoordinateNode();
        if (coord) {
            *field = coord->point_Field();
            return coord->getHandle(handle, constraint, field);
        }
    }
    if (getParent() && (getParent()->getType() == VRML_ELEVATION_GRID)) {
        NodeElevationGrid *elevation = (NodeElevationGrid *)getParent();
        *field = elevation->height_Field();
    }
    if (getParent() && (getParent()->getType() == VRML_GEO_ELEVATION_GRID)) {
        NodeGeoElevationGrid *elevation = (NodeGeoElevationGrid *)getParent();
        *field = elevation->height_Field();
    }
    Vec3f zeros(0, 0, 0);
    *field = getProto()->metadata_Field();
    return zeros;
}

void
NodeColorRGBA::setHandle(int handle, const Vec3f &v)
{
    if (getParent() && getParent()->getColored()) {
        NodeCoordinate *coord = (NodeCoordinate *)
                                getParent()->getCoordinateNode();
        if (coord)
            coord->setHandle(handle, v);
    }
}


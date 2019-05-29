/*
 * NodeColor.cpp
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

#include "NodeColor.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFColor.h"
#include "NodeShape.h"
#include "NodeAppearance.h"
#include "NodeMaterial.h"
#include "NodeIndexedFaceSet.h"
#include "NodeCoordinate.h"
#include "NodeElevationGrid.h"
#include "NodeGeoElevationGrid.h"
#include "Util.h"

ProtoColor::ProtoColor(Scene *scene)
  : WonderlandExportProto(scene, "Color")
{
    color.set(
          addExposedField(MFCOLOR, "color", new MFColor()));
}

Node *
ProtoColor::create(Scene *scene)
{
    return new NodeColor(scene, this); 
}

NodeColor::NodeColor(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeColor::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    if (hasParent())
        getParents().update();
}

void
NodeColor::drawHandles(void)
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
NodeColor::getHandle(int handle, int *constraint, int *field)
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
NodeColor::setHandle(int handle, const Vec3f &v)
{
    if (getParent() && getParent()->getColored()) {
        NodeCoordinate *coord = (NodeCoordinate *)
                                getParent()->getCoordinateNode();
        if (coord)
            coord->setHandle(handle, v);
    }
}

char *
NodeColor::buildExportMaterialName(const char *name)
{
    char defaultName[1024];
    mysnprintf(defaultName, 1023, "%s__", TheApp->GetDefaultAc3dMaterialName());
    return mystrdup(defaultName);
}

int 
NodeColor::writeAc3dMaterial(int f, int indent, const char *name)
{
    // search for Material node for missing field values like emissiveColor, etc
    Node *parent = NULL;
    if (hasParent())
        parent = getParent(); // geometry node

    if (hasParent())
        parent = getParent(); // shape node

    NodeShape *nshape = NULL;
    if (parent)
        if (parent->getType() == VRML_SHAPE)
            nshape = (NodeShape *) parent;

    NodeAppearance *nappearance = NULL;
    if (nshape)
        nappearance = (NodeAppearance *)(nshape->appearance()->getValue());

    NodeMaterial *nmaterial = NULL;
    if (nappearance)
        nmaterial = (NodeMaterial *)nappearance->material()->getValue();

    // VRML default Material settings
    float ambientIntensity = 0.2f;
    Vec3f emissiveColor(0, 0, 0);
    Vec3f specularColor(0, 0, 0);
    int shininess = 0.2f * 128;
    float transparency = 0;
    if (nmaterial) {
        ambientIntensity = nmaterial->ambientIntensity()->getValue();

        emissiveColor[0] = nmaterial->emissiveColor()->getValue(0);
        emissiveColor[1] = nmaterial->emissiveColor()->getValue(1);
        emissiveColor[2] = nmaterial->emissiveColor()->getValue(2);

        specularColor[0] = nmaterial->specularColor()->getValue(0);
        specularColor[1] = nmaterial->specularColor()->getValue(1);
        specularColor[2] = nmaterial->specularColor()->getValue(2);
        
        shininess = nmaterial->shininess()->getValue() * 128.0f;

    }    

    for (int i = 0; i < color()->getSFSize(); i++) {
        RET_ONERROR( mywritestr(f, "MATERIAL \"") )
        RET_ONERROR( mywritef(f, "%s%d", name, i) )
        RET_ONERROR( mywritestr(f, "\" ") )
        RET_ONERROR( mywritef(f, "rgb %f %f %f  ", 
                              color()->getValue(i)[0],
                              color()->getValue(i)[1],
                              color()->getValue(i)[2]) )
        RET_ONERROR( mywritef(f, "amb %f %f %f  ",
                              ambientIntensity, 
                              ambientIntensity,
                              ambientIntensity) )
        RET_ONERROR( mywritef(f, "emis %f %f %f  ", 
                              emissiveColor[0],
                              emissiveColor[1],
                              emissiveColor[2]) )
        RET_ONERROR( mywritef(f, "spec %f %f %f  ", 
                              specularColor[0],
                              specularColor[1],
                              specularColor[2]) )
        RET_ONERROR( mywritef(f, "shi %d  ", shininess ) )
        RET_ONERROR( mywritef(f, "trans %f\n", transparency) )
    }
    return 0;
}

void
NodeColor::handleAc3dMaterial(ac3dMaterialCallback callback, Scene* scene)
{
    callback(scene, getExportMaterialName(TheApp->GetDefaultAc3dMaterialName()),
             this);
}

int 
NodeColor::getLdrawColor(int index)
{
    return Util::getLdrawColorFromRGBA(color()->getValue(index)[0],
                                       color()->getValue(index)[1],
                                       color()->getValue(index)[2], 1, true);
}

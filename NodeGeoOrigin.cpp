/*
 * NodeGeoOrigin.cpp
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

#include "NodeGeoOrigin.h"
#include "GeoMacros.h"
#include "Proto.h"
#include "ExposedField.h"
#include "Field.h"
#include "RenderState.h"
#include "DuneApp.h"
#include "Util.h"
#include "Vec3f.h"
#include "Scene.h"
#include "NodeIndexedFaceSet.h"
#include "NodeIndexedLineSet.h"
#include "NodePointSet.h"

ProtoGeoOrigin::ProtoGeoOrigin(Scene *scene)
  : Proto(scene, "GeoOrigin")
{
    geoCoords.set(
          addExposedField(SFSTRING, "geoCoords", new SFString()));
    setFieldFlags(geoCoords, FF_VRML_ONLY);
    geoCoordsX3D.set(
          addExposedField(SFVEC3D, "geoCoords", new SFVec3d()));
    setFieldFlags(geoCoordsX3D, FF_X3D_ONLY);

    addGeoSystem();
    rotateYUp.set(
          addField(SFBOOL, "rotateYUp", new SFBool(false)));
}

Node *
ProtoGeoOrigin::create(Scene *scene)
{ 
    return new NodeGeoOrigin(scene, this); 
}

int 
ProtoGeoOrigin::translateField(int field) const
{
    bool x3d = m_scene->isX3d();
    if (x3d && (field == geoCoords))
        return geoCoordsX3D;
    else if (!x3d && (field == geoCoordsX3D))
        return geoCoords;
    return field;
}


NodeGeoOrigin::NodeGeoOrigin(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeGeoOrigin::setField(int index, FieldValue *value, int cf)
{
    if (index == geoCoords_Field()) {
        SFVec3d *value3d = new SFVec3d((SFString *)value);
        Node::setField(geoCoordsX3D_Field(), value3d);
    }
    Node::setField(index, value, cf);
    update();
}

Node *
NodeGeoOrigin::convert2Vrml(void) 
{
    const double *values = geoCoordsX3D()->getValue();
    char buf[4096];
    mysnprintf(buf, 4095, "%g %g %g", values[0], values[1], values[2]);
    SFString *string = new SFString(buf);
    geoCoords(string);    
    return NULL;
}


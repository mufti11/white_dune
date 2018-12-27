/*
 * NodeGeoCoordinate.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2006/2007 BOBMAX
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

#include "NodeGeoCoordinate.h"
#include "Proto.h"
#include "MFVec3d.h"
#include "MFString.h"
#include "ExposedField.h"
#include "Field.h"
#include "RenderState.h"
#include "DuneApp.h"
#include "Util.h"
#include "Vec3d.h"
#include "Scene.h"
#include "NodeIndexedFaceSet.h"
#include "NodeIndexedLineSet.h"
#include "NodePointSet.h"
#include "GpsMathLib.h"

ProtoGeoCoordinate::ProtoGeoCoordinate(Scene *scene)
  : GeoProto(scene, "GeoCoordinate")
{
    point.set(
          addExposedField(MFSTRING, "point", new MFString()));
    setFieldFlags(point, FF_VRML_ONLY | EIF_RECOMMENDED);
    pointX3D.set(
          addExposedField(MFVEC3D, "point", new MFVec3d()));
    setFieldFlags(pointX3D, FF_X3D_ONLY | EIF_RECOMMENDED);
}

Node *
ProtoGeoCoordinate::create(Scene *scene)
{ 
    return new NodeGeoCoordinate(scene, this); 
}

int 
ProtoGeoCoordinate::translateField(int field) const
{
    bool x3d = m_scene->isX3d();
    if (x3d && (field == point))
        return pointX3D;
    else if (!x3d && (field == pointX3D))
        return point;
    return field;
}

NodeGeoCoordinate::NodeGeoCoordinate(Scene *scene, Proto *def)
  : GeoNode(scene, def)
{
}

void
NodeGeoCoordinate::setField(int index, FieldValue *value, int cf)
{
    if (index == point_Field()) {
        MFVec3d *value3d = new MFVec3d((MFString *)value);
        Node::setField(pointX3D_Field(), value3d);
    }
    Node::setField(index, value, cf);
    if (hasParent())
        getParent()->update();
}


Node *
NodeGeoCoordinate::convert2Vrml(void) 
{
    const double *values = pointX3D()->getValues();
    MFString *strings = new MFString();
    for (int i = 0; i < pointX3D()->getSFSize(); i++) {
        char string[4096];
        mysnprintf(string, 4095, "%g %g %g",
                   values[i * 3], values[i * 3 + 1], values[i * 3 + 2]);
        strings->insertSFValue(i, strdup(string));
    }
    point(strings);    
    return NULL;
}

void
NodeGeoCoordinate::drawHandles(void)
{
    RenderState state;

    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glPushName(0);
    state.startDrawHandles();
    for (int ci = 0; ci < point()->getSFSize(); ci++) {
        state.setHandleColor(m_scene, ci);
        glLoadName(ci);
        const double *values = pointX3D()->getValues();      
        double latN, longE, H, xxx, yyy, zzz;

        latN = values[ci * 3 + 0];
        longE = values[ci * 3 + 1]; 
        H = values[ci * 3 + 2];      
        Wgs84LatLonHToWgs84XYZ(latN, longE, H, &xxx, &yyy, &zzz ); 
        state.drawHandle(Vec3d(xxx, yyy, zzz));
    }
    state.endDrawHandles();
    glPopName();
    glPopAttrib();
}


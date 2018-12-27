/*
 * NodeGeoElevationGrid.cpp
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

#include "NodeGeoElevationGrid.h"
#include "Proto.h"
#include "MFVec3f.h"
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
#include "NodeShape.h"

ProtoGeoElevationGrid::ProtoGeoElevationGrid(Scene *scene)
  : GeoProto(scene, "GeoElevationGrid")
{
    color.set(
          addExposedField(SFNODE, "color", new SFNode(NULL), COLOR_NODE));
    normal.set(
          addExposedField(SFNODE, "normal", new SFNode(NULL), VRML_NORMAL));
    texCoord.set(
          addExposedField(SFNODE, "texCoord", 
                          new SFNode(NULL), TEXTURE_COORDINATE_NODE));
    yScale.set(
          addField(SFFLOAT, "yScale", new SFFloat(1.0f), new SFFloat(1.0f)));
    setFieldFlags(yScale, FF_VRML_ONLY);
    yScaleX3D.set(
          addExposedField(SFFLOAT, "yScale", new SFFloat(1.0f), 
                          new SFFloat(0.0f)));
    setFieldFlags(yScaleX3D, FF_X3D_ONLY);

    ccw.set(
          addField(SFBOOL, "ccw", new SFBool(true)));
    colorPerVertex.set(
          addField(SFBOOL, "colorPerVertex", new SFBool(true)));

    creaseAngle.set(
          addField(SFFLOAT, "creaseAngle", new SFFloat(0.0f), 
                   new SFFloat(0.0f)));
    setFieldFlags(creaseAngle, FF_VRML_ONLY);
    creaseAngleX3D.set(
          addField(SFDOUBLE, "creaseAngle", new SFDouble(0.0), 
                   new SFDouble(0.0)));
    setFieldFlags(creaseAngleX3D, FF_X3D_ONLY);

    geoGridOrigin.set(
          addField(SFSTRING, "geoGridOrigin", new SFString("0.0 0.0 0.0")));
    setFieldFlags(geoGridOrigin, FF_VRML_ONLY);
    geoGridOriginX3D.set(
          addField(SFVEC3D, "geoGridOrigin", new SFVec3d(0.0, 0.0, 0.0)));
    setFieldFlags(geoGridOriginX3D, FF_X3D_ONLY);

    height.set(
          addField(MFFLOAT, "height", new MFFloat()));
    setFieldFlags(height, FF_VRML_ONLY);
    heightX3D.set(
          addField(MFDOUBLE, "height", new MFDouble()));
    setFieldFlags(heightX3D, FF_X3D_ONLY);

    normalPerVertex.set(
          addField(SFBOOL, "normalPerVertex", new SFBool(true)));
    solid.set(
          addField(SFBOOL, "solid", new SFBool(true)));
    xDimension.set(
          addField(SFINT32, "xDimension", new SFInt32(0), new SFInt32(0)));

    xSpacing.set(
          addField(SFSTRING, "xSpacing", new SFString("1.0")));
    setFieldFlags(xSpacing, FF_VRML_ONLY);
    xSpacingX3D.set(
          addField(SFDOUBLE, "xSpacing", new SFDouble(1.0), new SFDouble(0.0)));
    setFieldFlags(xSpacingX3D, FF_X3D_ONLY);

    zDimension.set(
          addField(SFINT32, "zDimension", new SFInt32(0), new SFInt32(0)));

    zSpacing.set(
          addField(SFSTRING, "zSpacing", new SFString("1.0")));
    setFieldFlags(zSpacing, FF_VRML_ONLY);
    zSpacingX3D.set(
          addField(SFDOUBLE, "zSpacing", new SFDouble(1.0), new SFDouble(0.0)));
    setFieldFlags(zSpacingX3D, FF_X3D_ONLY);

    addEventIn(MFFLOAT,  "set_yScale", EIF_RECOMMENDED | FF_VRML_ONLY, yScale);

    addEventIn(MFFLOAT,  "set_height", EIF_RECOMMENDED | FF_VRML_ONLY, height);
    addEventIn(MFDOUBLE, "set_height", EIF_RECOMMENDED | FF_X3D_ONLY, 
               heightX3D);
}

Node *
ProtoGeoElevationGrid::create(Scene *scene)
{ 
    return new NodeGeoElevationGrid(scene, this); 
}

int 
ProtoGeoElevationGrid::translateField(int field) const
{
    bool x3d = m_scene->isX3d();
    if (x3d) {
        if (field == yScale)
            field = yScaleX3D;
        else if (field == creaseAngle)
            field = creaseAngleX3D;
        else if (field == geoGridOrigin)
            field = geoGridOriginX3D;
        else if (field == height)
            field = heightX3D;
        else if (field == xSpacing)
            field = xSpacingX3D;
        else if (field == zSpacing)
            field = zSpacingX3D;
    } else {
        if (field == yScaleX3D)
            field = yScale;
        else if (field == creaseAngleX3D)
            field = creaseAngle;
        else if (field == geoGridOriginX3D)
            field = geoGridOrigin;
        else if (field == heightX3D)
            field = height;
        else if (field == xSpacingX3D)
            field = xSpacing;
        else if (field == zSpacingX3D)
            field = zSpacing;
    }
    return field;
}


NodeGeoElevationGrid::NodeGeoElevationGrid(Scene *scene, Proto *def)
  : GeoNode(scene, def)
{
}

void
NodeGeoElevationGrid::setField(int index, FieldValue *value, int cf)
{
    if (index == yScale_Field()) {
        SFFloat *fvalue = new SFFloat(((SFFloat *)value)->getValue());
        Node::setField(yScaleX3D_Field(), fvalue);
    }

    if (index == height_Field()) {
        MFFloat *oldValue = (MFFloat *)value;
        MFDouble *dvalue = new MFDouble(oldValue->getValues(), 
                                        oldValue->getSize());
        Node::setField(heightX3D_Field(), dvalue, cf);
    }

    if (index == creaseAngle_Field()) {
        SFDouble *dvalue = new SFDouble(((SFFloat *)value)->getValue());
        Node::setField(creaseAngleX3D_Field(), dvalue, cf);
    }

    if (index == geoGridOrigin_Field()) {
        SFVec3d *value3d = new SFVec3d((SFString *)value);
        Node::setField(geoGridOriginX3D_Field(), value3d, cf);
    }

    if (index == xSpacing_Field()) {
        SFDouble *dvalue = new SFDouble((SFString *)value);
        Node::setField(xSpacingX3D_Field(), dvalue, cf);
    }

    if (index == zSpacing_Field()) {
        SFDouble *dvalue = new SFDouble((SFString *)value);
        Node::setField(zSpacingX3D_Field(), dvalue);
    }

    Node::setField(index, value, cf);
    update();
}

Node *
NodeGeoElevationGrid::convert2Vrml(void) 
{
    const double fyScale = yScaleX3D()->getValue();

    yScale(new SFFloat(fyScale));

    const double *dheight = heightX3D()->getValues();
    height(new MFFloat(dheight, heightX3D()->getSize()));

    const double fcreaseAngle = creaseAngleX3D()->getValue();
    creaseAngle(new SFFloat(fcreaseAngle));

    const double *values = geoGridOriginX3D()->getValue();
    char buf[4096];
    mysnprintf(buf, 4095, "%g %g %g", values[0], values[1], values[2]);
    SFString *string = new SFString(buf);
    geoGridOrigin(string);    

    double fxSpacing = xSpacingX3D()->getValue();
    mysnprintf(buf, 4095, "%g", fxSpacing);
    string = new SFString(buf);
    xSpacing(string);    

    double fzSpacing = zSpacingX3D()->getValue();
    mysnprintf(buf, 4095, "%g", fzSpacing);
    string = new SFString(buf);
    zSpacing(string);    

    return NULL;
}


bool    
NodeGeoElevationGrid::validHandle(int handle)
{
    if ((handle < 0) || (handle > getMaxHandle()))
        return false;
    return true;
}

bool    
NodeGeoElevationGrid::checkHandle(int handle)
{
    for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
        int selectedHandle = m_scene->getSelectedHandle(i);
        if (!validHandle(selectedHandle))
            continue;
        int temph = handle % xDimension()->getValue();
        int xh = xDimension()->getValue() - temph - 1;
        int zh = (handle - temph);
        int tempi = selectedHandle % xDimension()->getValue();
        int xi = tempi;
        int zi = (selectedHandle - tempi); 
        if ((xh == xi) && (zh == zi) &&
            ((height()->getValue(xh + zh * xDimension()->getValue())) ==
             (height()->getValue(xi + zh * xDimension()->getValue()))))
            return true;
        if (selectedHandle == handle)
            return true;           
    }
    return false;
}

int     
NodeGeoElevationGrid::getMaxHandle(void)
{
    return xDimension()->getValue() * zDimension()->getValue() - 1;
}

GeometryNodeMacros(NodeGeoElevationGrid)

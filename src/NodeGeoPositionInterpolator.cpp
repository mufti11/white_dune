/*
 * NodeGeoPositionInterpolator.cpp
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

#include "NodeGeoPositionInterpolator.h"
#include "GeoMacros.h"
#include "Proto.h"
#include "MFVec3d.h"
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

ProtoGeoPositionInterpolator::ProtoGeoPositionInterpolator(Scene *scene)
  : GeoProto(scene, "GeoPositionInterpolator")
{
    addEventIn(SFFLOAT, "set_fraction", EIF_RECOMMENDED);
    key.set(addExposedField(MFFLOAT, "key", new MFFloat()));

    keyValue.set(
          addExposedField(MFSTRING, "keyValue", new MFString()));
    setFieldFlags(keyValue, FF_VRML_ONLY | EIF_RECOMMENDED);
    keyValueX3D.set(
          addExposedField(MFVEC3D, "keyValue", new MFVec3d()));
    setFieldFlags(keyValueX3D, FF_X3D_ONLY | EIF_RECOMMENDED);

    onGreatCircle.set(
          addExposedField(SFBOOL, "onGreatCircle", new SFBool(false)));
    setFieldFlags(onGreatCircle, FF_X3DOM_ONLY);

    addEventOut(SFSTRING, "geovalue_changed", FF_VRML_ONLY | EOF_RECOMMENDED);
    addEventOut(SFVEC3D , "geovalue_changed", FF_X3D_ONLY | EOF_RECOMMENDED);

    addEventOut(SFVEC3F, "value_changed", EOF_RECOMMENDED);
}

Node *
ProtoGeoPositionInterpolator::create(Scene *scene)
{ 
    return new NodeGeoPositionInterpolator(scene, this); 
}

int 
ProtoGeoPositionInterpolator::translateField(int field) const
{
    bool x3d = m_scene->isX3d();
    if (x3d && (field == keyValue))
        return keyValueX3D;
    else if (!x3d && (field == keyValueX3D))
        return keyValue;
    return field;
}


NodeGeoPositionInterpolator::NodeGeoPositionInterpolator(Scene *scene, 
                                                         Proto *def)
  : GeoNode(scene, def)
{
}

void
NodeGeoPositionInterpolator::setField(int index, FieldValue *value, int cf)
{
    if (index == keyValue_Field()) {
        MFVec3d *value3d = new MFVec3d((MFString *)value);
        Node::setField(keyValueX3D_Field(), value3d);
    }
    Node::setField(index, value, cf);
    update();
}

Node *
NodeGeoPositionInterpolator::convert2Vrml(void) 
{
    const double *values = keyValueX3D()->getValues();
    MFString *strings = new MFString();
    for (int i = 0; i < keyValueX3D()->getSFSize(); i++) {
        char string[4096];
        mysnprintf(string, 4095, "%g %g %g",
                   values[i * 3], values[i * 3 + 1], values[i * 3 + 2]);
        strings->insertSFValue(i, strdup(string));
    }
    keyValue(strings);    

    return NULL;
}

/*

/ / upgrade for real double support 8-(
FieldValue *
NodeGeoPositionInterpolator::createKey(float *value) const
{
    FieldValue *r = new SFVec3d(value);
    return r;
}

/ / upgrade for real double support 8-(
FieldValue *
NodeGeoPositionInterpolator::createKeys(float *value, int numKeys) const
{
    return new MFVec3d(value, numKeys * 3);
}

*/

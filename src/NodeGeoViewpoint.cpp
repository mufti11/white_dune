/*
 * NodeGeoViewpoint.cpp
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

#include "NodeGeoViewpoint.h"
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

ProtoGeoViewpoint::ProtoGeoViewpoint(Scene *scene)
  : GeoProto(scene, "GeoViewpoint")
{
    set_bind.set(addEventIn(SFBOOL, "set_bind"));

    centerOfRotation.set(
          addExposedField(SFVEC3F, "centerOfRotation", 
                          new SFVec3d(0, 0, 0)));
    setFieldFlags(centerOfRotation, FF_X3D_ONLY);

    // problem: Field for VRML97, ExposedField for X3D 8-(
    description.set(
          addField(SFSTRING, "description", new SFString("")));
    fieldOfView.set(
          addExposedField(SFFLOAT, "fieldOfView", new SFFloat(0.785398f), 
                          new SFFloat(0.0f), new SFFloat(M_PI)));
    headlight.set(
          addExposedField(SFBOOL, "headlight", new SFBool(true)));
    jump.set(
         addExposedField(SFBOOL, "jump", new SFBool(true)));
    static const char *types[] = { "ANY", "WALK", "EXAMINE", "FLY", "NONE", 
                                   NULL};
    StringArray *defaulttypes = new StringArray();
    defaulttypes->append("WALK");
    defaulttypes->append("ANY");
    navType.set(
          addExposedField(MFSTRING, "navType", new MFString(defaulttypes), 0, 
                          types));
    setFieldFlags(navType, FF_VRML_ONLY);
    // why is orientation a field in VRML97 and not a exposedField ?
    orientation.set(
         addExposedField(SFROTATION, "orientation", 
                         new SFRotation(0, 0, 1, 0)));

    position.set(
          addField(SFSTRING, "position", new SFString("0 0 100000")));
    setFieldFlags(position, FF_VRML_ONLY);
    positionX3D.set(
         addExposedField(SFVEC3D, "position", new SFVec3d(0, 0, 100000)));
    setFieldFlags(positionX3D, FF_X3D_ONLY);

    retainUserOffsets.set(
          addExposedField(SFBOOL, "retainUserOffsets", 
                          new SFBool(false)));
    setFieldFlags(retainUserOffsets, FF_X3D_ONLY);

    speedFactor.set(
          addField(SFFLOAT, "speedFactor", new SFFloat(1.0f), 
                   new SFFloat(0.0f)));
    elevationScaling.set(
        addExposedField(SFBOOL, "elevationScaling", new SFBool(true)));
    setFieldFlags(elevationScaling, FF_X3DOM_ONLY);

    addEventOut(SFTIME, "bindTime");
    addEventOut(SFBOOL, "isBound");
}

Node *
ProtoGeoViewpoint::create(Scene *scene)
{ 
    return new NodeGeoViewpoint(scene, this); 
}

int 
ProtoGeoViewpoint::translateField(int field) const
{
    bool x3d = m_scene->isX3d();
    if (x3d) {
        if (field == position)
            return positionX3D;
    } else {
        if (field == positionX3D)
            return position;
    }
    return field;
}


NodeGeoViewpoint::NodeGeoViewpoint(Scene *scene, Proto *def)
  : GeoNode(scene, def)
{
}

void
NodeGeoViewpoint::setField(int index, FieldValue *value, int cf)
{
    if (index == position_Field()) {
        SFVec3d *value3d = new SFVec3d((SFString *)value);
        Node::setField(positionX3D_Field(), value3d);
    }
    Node::setField(index, value, cf);
    update();
}

Node *
NodeGeoViewpoint::convert2Vrml(void) 
{
    const double *values = positionX3D()->getValue();
    char buf[4096];
    mysnprintf(buf, 4095, "%g %g %g", values[0], values[1], values[2]);
    SFString *string = new SFString(buf);
    position(string);    
    return NULL;
}



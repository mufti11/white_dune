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
#include "NodeGeoOrigin.h"
#include "Scene.h"

#include "GeoMacros.h"

ProtoGeoViewpoint::ProtoGeoViewpoint(Scene *scene)
  : ViewpointProto(scene, "GeoViewpoint")
{
    position.set(
          addField(SFSTRING, "position", new SFString("0 0 100000")));
    setFieldFlags(position, FF_VRML_ONLY);
    positionX3D.set(
         addExposedField(SFVEC3D, "position", new SFVec3d(0, 0, 100000)));
    setFieldFlags(positionX3D, FF_X3D_ONLY);

    addGeoFields()
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
  : ViewpointNode(scene, def)
{
}

void
NodeGeoViewpoint::setField(int index, FieldValue *value, int cf)
{
    if (index == position_Field()) {
        SFVec3d *value3d = new SFVec3d((SFString *)value);
        Node::setField(positionX3D_Field(), value3d);
        update();
        return;
    }
    if (index == positionX3D_Field()) {
        Node::setField(positionX3D_Field(), value);
        update();
        return;
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

void
NodeGeoViewpoint::apply(bool useStereo)
{
    Vec3d vec;
    SFRotation rot;

    NodeGeoOrigin *origin = (NodeGeoOrigin *)geoOrigin()->getValue();

    if (origin) {
        vec = origin->getVec();
        Vec3d vecNorm(vec);
        vecNorm.normalize();
        Vec3f norm(vecNorm.x, vecNorm.y, vecNorm.z);
        Quaternion quat(Vec3f(0, 1, 0), norm);
        rot = SFRotation(quat);
    }

    ViewpointNode::apply(useStereo, vec, rot);
}

Vec3d  
NodeGeoViewpoint::getPosition() const
{
    return Vec3d(positionX3D()->getValue()[0],
                 positionX3D()->getValue()[1],
                 -positionX3D()->getValue()[2] / (2 * 40000));  
}

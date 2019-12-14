/*
 * NodeGeoLocation.cpp
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

#include "NodeGeoLocation.h"
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

#include "NodeGeoOrigin.h"

ProtoGeoLocation::ProtoGeoLocation(Scene *scene)
  : GeoProto(scene, "GeoLocation")
{
    addEventIn(MFNODE, "addChildren");
    addEventIn(MFNODE, "removeChildren");
    children.set(
          addExposedField(MFNODE, "children", new MFNode(), CHILD_NODE));

    geoCoords.set(
          addExposedField(SFSTRING, "geoCoords", new SFString()));
    setFieldFlags(geoCoords, FF_VRML_ONLY);
    geoCoordsX3D.set(
          addExposedField(SFVEC3D, "geoCoords", new SFVec3d()));
    setFieldFlags(geoCoordsX3D, FF_X3D_ONLY);

    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0.0f, 0.0f, 0.0f)));
    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1.0f, -1.0f, -1.0f), 
                   new SFFloat(-1.0f)));

    render.set(
          addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);
    }

Node *
ProtoGeoLocation::create(Scene *scene)
{ 
    return new NodeGeoLocation(scene, this); 
}

int 
ProtoGeoLocation::translateField(int field)
{
    bool x3d = m_scene->isX3d();
    if (x3d && (field == geoCoords))
        return geoCoordsX3D;
    else if (!x3d && (field == geoCoordsX3D))
        return geoCoords;
    return field;
}


NodeGeoLocation::NodeGeoLocation(Scene *scene, Proto *def)
  : GeoNode(scene, def)
{
}

void
NodeGeoLocation::setField(int index, FieldValue *value, int cf)
{
    if (index == geoCoords_Field()) {
        SFVec3d *value3d = new SFVec3d((SFString *)value);
        Node::setField(geoCoordsX3D_Field(), value3d);
    }
    Node::setField(index, value, cf);
    update();
}

Node *
NodeGeoLocation::convert2Vrml(void) 
{
    const double *values = geoCoordsX3D()->getValue();
    char buf[4096];
    mysnprintf(buf, 4095, "%g %g %g", values[0], values[1], values[2]);
    SFString *string = new SFString(buf);
    geoCoords(string);    
    return NULL;
}

void
NodeGeoLocation::preDraw()
{
    NodeList *childList = children()->getValues();

    glPushMatrix();
    transform();

    for (long i = 0; i < childList->size(); i++)
        if (childList->get(i) != this)
            childList->get(i)->preDraw();

    glPopMatrix();
}

void
NodeGeoLocation::draw(int pass)
{
    int i;
    NodeList *childList = children()->getValues();
    int n = childList->size();

    glPushMatrix();

    NodeGeoOrigin *origin = (NodeGeoOrigin *)geoOrigin()->getValue();

    if (origin) {
        Vec3d vec = origin->getVec();
        glTranslated(vec.x, vec.y, vec.z);
    }

    for (i = 0; i < n; i++)
        childList->get(i)->bind();

    glPushName(children_Field());  // field
    glPushName(0);                 // index
    for (i = 0; i < n; i++) {
        glLoadName(i);
        if (childList->get(i) != this)
            childList->get(i)->draw(pass);
    }
    glPopName();
    glPopName();

    for (i = 0; i < n; i++)
        childList->get(i)->unbind();

    glPopMatrix();
}




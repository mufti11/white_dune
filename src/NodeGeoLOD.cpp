/*
 * NodeGeoLOD.cpp
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

#include "NodeGeoLOD.h"
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

ProtoGeoLOD::ProtoGeoLOD(Scene *scene)
  : GeoProto(scene, "GeoLOD")
{
    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0.0f, 0.0f, 0.0f)));
    setFieldFlags(bboxCenter, FF_X3D_ONLY);
    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1.0f, -1.0f, -1.0f), 
                   new SFFloat(-1.0f)));
    setFieldFlags(bboxSize, FF_X3D_ONLY);

    center.set(
          addField(SFSTRING, "center", new SFString("0.0 0.0 0.0")));
    setFieldFlags(center, FF_VRML_ONLY);
    centerX3D.set(
          addField(SFVEC3D, "center", new SFVec3d(0.0, 0.0, 0.0)));
    setFieldFlags(centerX3D, FF_X3D_ONLY);

    child1Url.set(
          addField(MFSTRING, "child1Url", new MFString()));
    child2Url.set(
          addField(MFSTRING, "child2Url", new MFString()));
    child3Url.set(
          addField(MFSTRING, "child3Url", new MFString()));
    child4Url.set(
          addField(MFSTRING, "child4Url", new MFString()));
    range.set(
          addField(MFFLOAT, "range", new MFFloat(), new SFFloat(0.0f)));
    rootUrl.set(
          addField(MFSTRING, "rootUrl", new MFString()));
    rootNode.set(
          addField(MFNODE, "rootNode", new MFNode(), CHILD_NODE));

    referenceBindableDescription.set(
          addExposedField(SFSTRING, "referenceBindableDescription", 
                          new SFString("")));
    setFieldFlags(referenceBindableDescription, FF_X3DOM_ONLY);

    render.set(
          addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);
    
    addEventOut(MFNODE, "children");
    addEventOut(SFINT32, "level_changed", FF_X3D_ONLY);
}

Node *
ProtoGeoLOD::create(Scene *scene)
{ 
    return new NodeGeoLOD(scene, this); 
}

int 
ProtoGeoLOD::translateField(int field) const
{
    bool x3d = m_scene->isX3d();
    if (x3d && (field == center))
        return centerX3D;
    else if (!x3d && (field == centerX3D))
        return center;
    return field;
}


NodeGeoLOD::NodeGeoLOD(Scene *scene, Proto *def)
  : GeoNode(scene, def)
{
}

void
NodeGeoLOD::setField(int index, FieldValue *value, int cf)
{
    if (index == center_Field()) {
        SFVec3d *value3d = new SFVec3d((SFString *)value);
        Node::setField(centerX3D_Field(), value3d, cf);
    } 
    Node::setField(index, value, cf);
    update();
}

Node *
NodeGeoLOD::convert2Vrml(void) 
{
    const double *values = centerX3D()->getValue();
    char buf[4096];
    mysnprintf(buf, 4095, "%g %g %g", values[0], values[1], values[2]);
    SFString *string = new SFString(buf);
    center(string);    
    return NULL;
}




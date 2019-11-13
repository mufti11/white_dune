/*
 * NodeBillboard.cpp
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

#include "NodeBillboard.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFVec3f.h"
#include "MFNode.h"
#include "SFFloat.h"
#include "Matrix.h"
#include "Scene.h"
#include "NodeViewpoint.h"
#include "Util.h"
#include "resource.h"

ProtoBillboard::ProtoBillboard(Scene *scene)
  : WonderlandExportProto(scene, "Billboard")
{
    addEventIn(MFNODE, "addChildren");
    addEventIn(MFNODE, "removeChildren");

    axisOfRotation.set(
          addExposedField(SFVEC3F, "axisOfRotation", new SFVec3f(0, 1, 0)));

    children.set(
          addExposedField(MFNODE, "children", new MFNode(), CHILD_NODE));

    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));

    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1), 
                   new SFFloat(-1.0f)));

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);
}

Node *
ProtoBillboard::create(Scene *scene)
{ 
    return new NodeBillboard(scene, this); 
}

NodeBillboard::NodeBillboard(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeBillboard::transform()
{
    Matrix mat;

    glGetFloatv(GL_MODELVIEW_MATRIX, mat);

    // compute the axis of rotation in eye coordinates
    const float *a = axisOfRotation()->getValue();

    if (a[0] == 0.0f && a[1] == 0.0f && a[2] == 0.0f) {
        Matrix mat;
        Vec3d cam(m_scene->getCamera()->getPosition());

        // rotate local Z axis into billboard->viewer vector

        Vec3d bvec(cam);
        bvec.normalize();
        static const Vec3d zvec(0.0f, 0.0f, 1.0f);

        Vec3d axis1(bvec.cross(zvec));
        axis1.normalize();

        float phi = (float) acos(bvec.dot(zvec));

        glRotatef(-RAD2DEG(phi), axis1.x, axis1.y, axis1.z);
            
        glGetFloatv(GL_MODELVIEW_MATRIX, mat);
        Vec3f origin(mat * Vec3f(0.0f, 0.0f, 0.0f));
        Vec3f yvec(0.0f, 1.0f, 0.0f);
        Vec3f localy((mat * yvec - origin));
        localy.normalize();

        // rotate local Y axis into up vector
        Vec3f axis2(localy.cross(yvec));
        axis2.normalize();

        float theta = (float) acos(localy.dot(yvec));
        glRotatef(RAD2DEG(theta), axis2.x, axis2.y, axis2.z);
    } else {
        // regular billboarding
        // compute the billboard->viewer vector
        Vec3f origin(mat * Vec3f(0.0f, 0.0f, 0.0f));
        Vec3f bvec(-origin);
        bvec.normalize();

        // compute the local z axis in eye coordinates
        Vec3f localz(((mat * Vec3f(0.0f, 0.0f, 1.0f)) - origin));
        localz.normalize();

        Vec3f axis(((mat * Vec3f(a)) - origin));
        axis.normalize();

        Vec3f normal(bvec.cross(axis));
        normal.normalize();

        float theta = (float) asin(localz.dot(normal));

        if (localz.z < 0.0f) theta = M_PI - theta;

        glRotatef(RAD2DEG(theta), a[0], a[1], a[2]);
    }
}

void
NodeBillboard::preDraw()
{
    NodeList *childList = children()->getValues();

    glPushMatrix();
    transform();

    for (long i = 0; i < childList->size(); i++)
        childList->get(i)->preDraw();

    glPopMatrix();
}

void
NodeBillboard::draw(int pass)
{
    NodeList *childList = children()->getValues();
    int n = childList->size();

    glPushMatrix();
    transform();

    for (int i = 0; i < n; i++)
        childList->get(i)->bind();

    glPushName(children_Field()); // field
    glPushName(0);                // index
    for (int i = 0; i < n; i++) {
        glLoadName(i);
        childList->get(i)->draw(pass);
    }
    glPopName();
    glPopName();

    for (int i = 0; i < n; i++)
        childList->get(i)->unbind();

    glPopMatrix();
}

void
NodeBillboard::writeCWarning(void)
{
    bool needWarning = false;
    if (TheApp->isWonderlandModuleExport()) {
        needWarning = true;
        const float *faxisOfRotation = axisOfRotation()->getValue();
        if (faxisOfRotation[0] == 0) {
            if (faxisOfRotation[1] == 0)
                needWarning = false;
            else
                if (faxisOfRotation[2] == 0)
                    needWarning = false;
        }
    }
    if (needWarning)
        TheApp->PrintMessageWindowsId(IDS_WONDERLAND_BILLBOARD_CAN_000_001_010);
}

void
NodeBillboard::flip(int index)
{
    bboxCenter()->flip(index);
    children()->flip(index);
}

void
NodeBillboard::swap(int fromTo)
{
    bboxCenter()->swap(fromTo);
    bboxSize()->swap(fromTo);
    children()->swap(fromTo);
}

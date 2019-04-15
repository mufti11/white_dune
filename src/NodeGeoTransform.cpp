/*
 * NodeGeoTransform.cpp
 *
 * Copyright (C) 2009, 2019 J. "MUFTI" Scheurich
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

#include "NodeGeoTransform.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFNode.h"
#include "MFNode.h"
#include "SFVec3d.h"
#include "SFRotation.h"
#include "SFVec3f.h"
#include "SFRotation.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "SFVec3f.h"
#include "SFNode.h"
#include "MFString.h"
#include "Scene.h"
#include "DuneApp.h"

#include "NodeGeoOrigin.h"

ProtoGeoTransform::ProtoGeoTransform(Scene *scene)
  : GeoProto(scene, "GeoTransform")
{
    children.set(
        addExposedField(MFNODE, "children", new MFNode()));
    geoCenter.set(
        addExposedField(SFVEC3D, "geoCenter", new SFVec3d(0, 0, 0)));
    rotation.set(
        addExposedField(SFROTATION, "rotation", new SFRotation(0, 0, 1, 0)));
    scale.set(
        addExposedField(SFVEC3F, "scale", new SFVec3f(1, 1, 1)));
    scaleOrientation.set(
        addExposedField(SFROTATION, "scaleOrientation", new SFRotation(0, 0, 1, 0)));
    translation.set(
        addExposedField(SFVEC3F, "translation", new SFVec3f(0, 0, 0)));
    bboxCenter.set(
        addField(SFVEC3F, "bboxCenter", new SFVec3f(0, 0, 0)));
    bboxSize.set(
        addField(SFVEC3F, "bboxSize", new SFVec3f(-1, -1, -1)));

    globalGeoOrigin.set(
          addExposedField(SFBOOL, "globalGeoOrigin", new SFBool(false)));
    setFieldFlags(globalGeoOrigin, FF_X3DOM_ONLY);

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);

    addEventIn(MFNODE, "addChildren");
    addEventIn(MFNODE, "removeChildren");
}

Node *
ProtoGeoTransform::create(Scene *scene)
{ 
    return new NodeGeoTransform(scene, this); 
}

NodeGeoTransform::NodeGeoTransform(Scene *scene, Proto *def)
  : GeoNode(scene, def)
{
    m_matrixDirty = true;
}

void
NodeGeoTransform::transform()
{
    const double *fcenter = geoCenter()->getValue();
    const float *frotation = rotation()->getValue();
    const float *fscale = scale()->getValue();
    const float *fscaleOrientation = scaleOrientation()->getValue();
    const float *ftranslation = translation()->getValue();

    if (m_matrixDirty) {
        double rotAngle = frotation[3];
        double oriAngle = fscaleOrientation[3];
        if (m_scene) {
            double angleUnit = m_scene->getUnitAngle();
            if (angleUnit != 0) {
               rotAngle *= angleUnit;
               oriAngle *= angleUnit;
            }
        }
        double identity[16] = { 1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1 };
        glPushMatrix();
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixd(identity);
        glTranslatef(ftranslation[0], ftranslation[1], ftranslation[2]);
        glTranslated(fcenter[0], fcenter[1], fcenter[2]);
        glRotatef(RAD2DEG(rotAngle), 
              frotation[0], frotation[1], frotation[2]);
        glRotatef(RAD2DEG(oriAngle), 
              fscaleOrientation[0], fscaleOrientation[1], fscaleOrientation[2]);
        glScalef(fscale[0], fscale[1], fscale[2]);
        glRotatef(-RAD2DEG(oriAngle), 
              fscaleOrientation[0], fscaleOrientation[1], fscaleOrientation[2]);
        glTranslated(-fcenter[0], -fcenter[1], -fcenter[2]);

        glGetDoublev(GL_MODELVIEW_MATRIX, m_matrix);

        m_matrixDirty = false;
        glPopMatrix();
    }
//for (int i = 0; i < 16; i++)
//printf("%lf ", m_matrix[i]);
//printf("\n");
    glMultMatrixd(m_matrix);
}

void
NodeGeoTransform::preDraw()
{
    NodeList *childList = children()->getValues();

    glPushMatrix();
    transform();

    for (int i = 0; i < childList->size(); i++)
        if (childList->get(i) != this)
            childList->get(i)->preDraw();

    glPopMatrix();
}

void
NodeGeoTransform::draw(int pass)
{
    int i;
    NodeList *childList = children()->getValues();
    int n = childList->size();

    glPushMatrix();

    NodeGeoOrigin *origin = (NodeGeoOrigin *)geoOrigin()->getValue();

    transform();

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



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

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
# include <unistd.h>
#endif

#include "NodeGeoLOD.h"
#include "NodeGeoOrigin.h"
#include "Proto.h"
#include "MFVec3f.h"
#include "ExposedField.h"
#include "Field.h"
#include "RenderState.h"
#include "DuneApp.h"
#include "resource.h"
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

void
NodeGeoLOD::draw(int pass)
{
    Node *node = NULL;
    switch (m_nodeToDrawIndex) {
      case 0:
        node = m_child1;
        break;
      case 1:   
        node = m_child2;
        break;
      case 2:   
        node = m_child3;
        break;
      case 3:   
        node = m_child4;
        break;
      default:
        node = m_root;
    }

    if (node != NULL) {
        glPushName(-1);  // field offset

        node->bind();

        glPushName(0);

        glLoadName(m_nodeToDrawIndex);
        node->draw(pass);

        glPopName();

        node->unbind();

        glPopName();
    }
}

void
NodeGeoLOD::preDraw()
{
    accountNodeToDrawIndex();

    Node *node = NULL;
    switch (m_nodeToDrawIndex) {
      case 0:
        node = m_child1;
        break;
      case 1:   
        node = m_child2;
        break;
      case 2:   
        node = m_child3;
        break;
      case 3:   
        node = m_child4;
        break;
      default:
        node = m_root;
    }

    if (node != NULL)
        node->preDraw();
}

void
NodeGeoLOD::accountNodeToDrawIndex()
{
    int tmpNodeToDrawIndex = -1;

    tmpNodeToDrawIndex = 4 - 1;
    Matrixd matrix;
    const double *fcenter = centerX3D()->getValue();

    glPushMatrix();
    glTranslated(fcenter[0], fcenter[1], fcenter[2]);
    glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
    
    NodeGeoOrigin *origin = (NodeGeoOrigin *)geoOrigin()->getValue();

    Vec3d vec;
    if (origin)
        vec = origin->getVec();

    Vec3d v(matrix[12], matrix[13], matrix[14]);
    MFFloat *mfrange = range();
    if (tmpNodeToDrawIndex > mfrange->getSize())
        tmpNodeToDrawIndex = mfrange->getSize();
    // vrml97/part1/nodesRef.html#LOD
    // | An empty range field ... a hint to the browser that it may
    // | choose a level automatically to maintain a constant display rate.
    // we choose the first level 8-(
    if (mfrange->getSize() > 0)
        for (int i = tmpNodeToDrawIndex; i >= 0; i--)
            if (i < mfrange->getSize()) 
                if ((v - vec).length() < mfrange->getValues()[i])
                    tmpNodeToDrawIndex = i;

    m_nodeToDrawIndex = -1;
    if (tmpNodeToDrawIndex >= 0)
        m_nodeToDrawIndex = tmpNodeToDrawIndex;
}

void
NodeGeoLOD::loadChild(Node *node, MFString *urls)
{
    if (urls->getSize() == 0)
        return;
    MyString m_baseURL = "";
    m_baseURL += TheApp->getImportURL();
    for (int i = 0; i < urls->getSize(); i++) {
        MyString path;
        URL urlI(m_baseURL, urls->getValue(i));
        if (urls->getValue(i).length() == 0) continue;
        if (m_scene->Download(urlI, &path)) {
            TheApp->setImportURL(urlI.GetPath());
            struct stat fileStat;
            MyString filename = "";
            filename += path;
            if (stat(filename, &fileStat) == 0) {
                if (S_ISREG(fileStat.st_mode)) {
                    bool oldX3d = m_scene->isX3d();
                    FILE *file = fopen(filename, "rb");
                    if (file == NULL) {
                        TheApp->MessageBoxPerror(IDS_URL_FILE_FAILED, filename);
                        continue;
                    }
//                    double oldUnitLength = getUnitLength();
//                    pushUnitLength();
                    TheApp->setImportFile(filename);
                    m_scene->parse(file, m_child1, -1, SCAN_FOR_BOTH);
//                    node->setUnitLength(oldUnitLength / getUnitLength());
//                    popUnitLength();
                    fclose(file);
                    if (oldX3d && (!m_scene->isX3d()))
                        m_scene->setX3d();
                    else if ((!oldX3d) && m_scene->isX3d())             
                        m_scene->setVrml();
                    break;
                }
            }
        }
    }
}

void
NodeGeoLOD::load()
{
    m_root = NULL;
    loadChild(m_root, rootUrl());
    m_child1 = NULL;
    loadChild(m_child1, child1Url());
    m_child2 = NULL;
    loadChild(m_child2, child2Url());
    m_child3 = NULL;
    loadChild(m_child3, child3Url());
    m_child4 = NULL;
    loadChild(m_child4, child4Url());
}

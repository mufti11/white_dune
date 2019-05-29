/*
 * NodeContourPolyline2D.cpp
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

#include "NodeContourPolyline2D.h"
#include "Proto.h"
#include "MFVec3f.h"
#include "ExposedField.h"
#include "Field.h"
#include "DuneApp.h"
#include "Scene.h"
#include "RenderState.h"
#include "Util.h"
#include "resource.h"

ProtoContourPolyline2D::ProtoContourPolyline2D(Scene *scene)
  : Proto(scene, "ContourPolyline2D")
{
    static bool init = true;
    ExposedField* l;
    if (init || (scene->getParsedX3dVersion() != 0))
        l = new ExposedField(MFVEC2D, "lineSegments", new MFVec2d(), NULL, 
                             NULL, ANY_NODE, 0, NULL, "controlPoint");
    else
        l = new ExposedField(MFVEC2F, "lineSegments", new MFVec2f(), NULL, 
                             NULL, ANY_NODE, 0, NULL, "point");
    init = false;
    lineSegments.set(addExposedField(l));
}

Node *
ProtoContourPolyline2D::create(Scene *scene)
{ 
    return new NodeContourPolyline2D(scene, this); 
}

NodeContourPolyline2D::NodeContourPolyline2D(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeContourPolyline2D::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    if (hasParent())
        getParents().update();
}

void
NodeContourPolyline2D::drawHandles()
{
    int iSize = lineSegments()->getSize() / 2;
    RenderState state;

    glPushName(iSize + 1);
    glDisable(GL_LIGHTING);
    Util::myGlColor3f(1.0f, 1.0f, 1.0f);
    glLoadName(NO_HANDLE);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < iSize; i++) {
        const double *v = lineSegments()->getValue(i);
        glVertex3d(v[0], v[1], 0);
    }
    glEnd();
    
    int ci;

    state.startDrawHandles();
    for (ci = 0; ci < iSize; ci++) {
        state.setHandleColor(m_scene, ci);
        glLoadName(ci);
        const double *v = lineSegments()->getValue(ci);

        state.drawHandle(Vec3f((float)v[0], (float)v[1], 0));
    }
    state.endDrawHandles();
    glPopName();
    glEnable(GL_LIGHTING);
}


Vec3f
NodeContourPolyline2D::getHandle(int handle, int *constraint, int *field)
{
    *field = lineSegments_Field() ;

    if (handle >= 0 && handle < lineSegments()->getSize() / 2) {
        const double *p = lineSegments()->getValue(handle);

        Vec3f ret(p[0], p[1], 0);
        TheApp->PrintMessageWindowsVertex(IDS_VERTEX_SELECTED, 
                                          "lineSegments", handle, ret);
        return ret;
    } 
    *field = -1;
    return Vec3f(0.0f, 0.0f, 0.0f);
}

void
NodeContourPolyline2D::setHandle(int handle, const Vec3f &v)
{
    MFVec2d *newValue = new MFVec2d(*lineSegments());

    int numPoints = lineSegments()->getSize() / 2; 
    if (handle >= 0 && handle < numPoints) {
        newValue->setValue(handle * 2    , v.x);
        newValue->setValue(handle * 2 + 1, v.y);
    }
    m_scene->setField(this, lineSegments_Field(), newValue);
    if (hasParent())
        getParents().update();
}



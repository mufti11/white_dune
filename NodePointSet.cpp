/*
 * NodePointSet.cpp
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

#include "NodePointSet.h"
#include "Proto.h"
#include "FieldValue.h"
#include "SFNode.h"
#include "NodeColor.h"
#include "NodeColorRGBA.h"
#include "NodeCoordinate.h"
#include "DuneApp.h"
#include "Util.h"

ProtoPointSet::ProtoPointSet(Scene *scene)
  : GeometryProto(scene, "PointSet")
{
    attrib.set(
          addExposedField(MFNODE, "attrib", new MFNode(),
                          VERTEX_ATTRIBUTE_NODE));
    setFieldFlags(attrib, FF_X3D_ONLY); 
    color.set(
          addExposedField(SFNODE, "color", new SFNode(NULL), COLOR_NODE));
    coord.set(
          addExposedField(SFNODE, "coord", new SFNode(NULL), COORDINATE_NODE));
    fogCoord.set(
          addExposedField(SFNODE, "fogCoord", new SFNode(NULL), 
                          X3D_FOG_COORDINATE));
    setFieldFlags(fogCoord, FF_X3D_ONLY);
}

Node *
ProtoPointSet::create(Scene *scene)
{ 
    return new NodePointSet(scene, this); 
}

NodePointSet::NodePointSet(Scene *scene, Proto *def)
  : GeometryNode(scene, def)
{
}

void 
NodePointSet::draw()
{
    Node *ncoord = coord()->getValue();
    if (ncoord != NULL) {
        glPushName(coord_Field());       // field coord
        glPushName(0);                   // index 0
        ((NodeCoordinate *)ncoord)->draw(this);
        glPopName();
        glPopName();
    }
}

void
NodePointSet::pointDraw()
{
    Node *coord = ((SFNode *) getField(coord_Field()))->getValue();
    MFFloat *colors = NULL;
    int colorSize = 0;
    float pointSize = TheApp->GetPointSetSize();

    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    if (pointSize == 0.0) {
        glEnable(GL_POINT_SMOOTH);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glPointSize(1.0);
    } else {
        glDisable(GL_POINT_SMOOTH);
        glPointSize(pointSize);
    }
    int colorInc = 3;
    if (color()->getValue()) {
        if (color()->getValue()->getType() == VRML_COLOR) {
            colors = ((NodeColor *)(color()->getValue()))->color();
            colorSize = colors->getSFSize();
        } else if (color()->getValue()->getType() == X3D_COLOR_RGBA) {
            colors = ((NodeColorRGBA *)(color()->getValue()))->color();
            colorSize = colors->getSFSize();
            colorInc = 4;
        }
    }
    if (colors == NULL) {
        float c[4];
        glGetMaterialfv(GL_FRONT, GL_EMISSION, c);
        Util::myGlColor4fv(c);
    }

    if (!coord || ((NodeCoordinate *) coord)->point()->getType() != MFVEC3F)
        return;

    MFVec3f *coords = ((NodeCoordinate *) coord)->point();
    int coordSize = coords->getSFSize();

    glBegin(GL_POINTS);
    for (int i = 0; i < coordSize; i++) {
        if (i < colorSize)
            Util::myGlColor3fv(colors->getValues() + i * colorInc);
        glVertex3fv(coords->getValue(i));
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopAttrib();
}


Vec3f
NodePointSet::getMinBoundingBox(void)
{
    Vec3f ret(0, 0, 0);
    Node *coord = ((SFNode *) getField(coord_Field()))->getValue();
    if (coord != NULL) {
        MFVec3f *coords = ((NodeCoordinate *)coord)->point();
        if (coords != NULL)
            ret = coords->getMinBoundingBox();
   }
   return ret;
}

Vec3f
NodePointSet::getMaxBoundingBox(void)
{
    Vec3f ret(0, 0, 0);
    Node *coord = ((SFNode *) getField(coord_Field()))->getValue();
    if (coord != NULL) {
        MFVec3f *coords = ((NodeCoordinate *)coord)->point();
        if (coords != NULL)
            ret = coords->getMaxBoundingBox();
   }
   return ret;
}

void
NodePointSet::flip(int index)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord)
        if (ncoord->getType() == VRML_COORDINATE)
            ncoord->flip(index);
}

void
NodePointSet::swap(int fromTo)
{
    NodeCoordinate *ncoord = (NodeCoordinate *)coord()->getValue();
    if (ncoord)
        if (ncoord->getType() == VRML_COORDINATE) 
             ncoord->swap(fromTo);
}


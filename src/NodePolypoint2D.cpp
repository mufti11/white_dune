/*
 * NodePolypoint2D.cpp
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
#include "Scene.h"

#include "NodePolypoint2D.h"
#include "Proto.h"
#include "ExposedField.h"
#include "FieldValue.h"
#include "SFNode.h"
#include "NodePointSet.h"
#include "NodeCoordinate.h"
#include "DuneApp.h"
#include "Util.h"

ProtoPolypoint2D::ProtoPolypoint2D(Scene *scene)
  : GeometryProto(scene, "Polypoint2D")
{
    point.set(addExposedField(new ExposedField(MFVEC2F, "point", 
                                               new MFVec2f())));
}

Node *
ProtoPolypoint2D::create(Scene *scene)
{ 
    return new NodePolypoint2D(scene, this); 
}

NodePolypoint2D::NodePolypoint2D(Scene *scene, Proto *def)
  : GeometryNode(scene, def)
{
}

void 
NodePolypoint2D::draw()
{
    pointDraw();
}

void
NodePolypoint2D::pointDraw()
{
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
    float c[4];
    glGetMaterialfv(GL_FRONT, GL_EMISSION, c);
    Util::myGlColor4fv(c);

    MFVec3f coords(point());
    int coordSize = coords.getSFSize();

    glBegin(GL_POINTS);
    for (int i = 0; i < coordSize; i++) {
        glVertex3fv(coords.getValue(i));
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopAttrib();
}

Vec3f
NodePolypoint2D::getMinBoundingBox(void)
{
    MFVec3f mfvec3f(point());
    return mfvec3f.getMinBoundingBox();
}

Vec3f
NodePolypoint2D::getMaxBoundingBox(void)
{
    MFVec3f mfvec3f(point());
    return mfvec3f.getMaxBoundingBox();
}

NodePointSet *
NodePolypoint2D::convertToPointSet(void)
{
    MFVec3f *mfvec3f = new MFVec3f(point());
    NodeCoordinate *ncoord = (NodeCoordinate *)m_scene->createNode("Coordinate");
    ncoord->point(mfvec3f);
    NodePointSet *node = (NodePointSet *) m_scene->createNode("PointSet");
    node->coord(new SFNode(ncoord));

    return node;
}

void
NodePolypoint2D::flip(int index)
{
    NodePointSet *pointSet = convertToPointSet();
    pointSet->flip(index);
}

void
NodePolypoint2D::swap(int fromTo)
{
    NodePointSet *pointSet = convertToPointSet();
    pointSet->swap(fromTo);
}


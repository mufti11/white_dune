/*
 * NodeArcClose2D.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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

#include "NodeArcClose2D.h"
#include "MyMesh.h"
#include "Proto.h"
#include "Scene.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "RenderState.h"
#include "Util.h"
#include "stdafx.h"

ProtoArcClose2D::ProtoArcClose2D(Scene *scene)
  : Proto(scene, "ArcClose2D")
{
    closureType.set(
          addExposedField(SFSTRING, "closureType", new SFString("PIE")));

    endAngle.set(
          addField(SFFLOAT, "endAngle", new SFFloat(M_PI / 2.0f), 
                   new SFFloat(-2.0f * M_PI), new SFFloat(2.0f * M_PI)));

    radius.set(
          addField(SFFLOAT, "radius", new SFFloat(1.0F), new SFFloat(0.0f)));

    solid.set(
          addField(SFBOOL, "solid", new SFBool(false)));

    startAngle.set(
          addField(SFFLOAT, "startAngle", new SFFloat(0.0F), 
                   new SFFloat(-2.0f * M_PI), new SFFloat(2.0f * M_PI)));

    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);
}

Node *
ProtoArcClose2D::create(Scene *scene)
{ 
    return new NodeArcClose2D(scene, this); 
}

NodeArcClose2D::NodeArcClose2D(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
}

void
NodeArcClose2D::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    bool isPie = (strcmp(closureType()->getValue(), "PIE") == 0); 
    int tess = TheApp->getTessellation() + 1;
    int numPoints = tess * 3 + (isPie ? 3 : 0);
    float *fpoint = new float[numPoints];

    float fradius = radius()->getValue();
    float fstartAngle = startAngle()->getValue();
    float fendAngle = endAngle()->getValue();
    if (fstartAngle == fendAngle) {
        fstartAngle = 0;
        fendAngle = 2 * M_PI;
    }
    float incAngle = (fendAngle - fstartAngle) / (tess - 1);
  
    if (isPie) {
        fpoint[0] = 0.0f;
        fpoint[1] = 0.0f;
        fpoint[2] = 0.0f;
    }

    int loopSize = tess + (isPie ? 1 : 0);
    int offset = isPie ? 1 : 0;
    int i;
    for (i = offset; i < loopSize; i++) {
        float angle = fstartAngle + (i - offset) * incAngle;
        fpoint[i * 3 + 0] = fradius * cos(angle);
        fpoint[i * 3 + 1] = fradius * sin(angle);
        fpoint[i * 3 + 2] = 0;
    }

    MyArray<int> icoordIndex;
    for (i = 0; i < loopSize; i++)
        icoordIndex.append(i);
    icoordIndex.append(0);
    icoordIndex.append(-1);

    MFVec3f* mfpoint = new MFVec3f(fpoint, numPoints);
    MFVec3f *coords = (MFVec3f *)mfpoint->copy();

    MFInt32 *mfcoordIndex = new MFInt32((int *)icoordIndex.getData(), 
                                        icoordIndex.size());
    MFInt32 *coordIndex = (MFInt32 *)mfcoordIndex->copy();

    MyArray<MFVec2f *> texCoords;

    int meshFlags = MESH_NORMAL_PER_VERTEX;
    if (solid()->getValue())
        meshFlags |= MESH_SOLID;
    meshFlags |= MESH_CCW;

    float transparency = 0;
    if (hasParent())
        transparency = getParent()->getTransparency();

    m_mesh = new MyMesh(this, coords, coordIndex, NULL, NULL, NULL, NULL, 
                        texCoords, NULL, 0, meshFlags, transparency);
}



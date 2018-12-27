/*
 * NodeDisk2D.cpp
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

#include "NodeDisk2D.h"
#include "MyMesh.h"
#include "Proto.h"
#include "Scene.h"
#include "SFBool.h"
#include "SFFloat.h"
#include "RenderState.h"
#include "Util.h"
#include "stdafx.h"

ProtoDisk2D::ProtoDisk2D(Scene *scene)
  : Proto(scene, "Disk2D")
{
    innerRadius.set(
          addField(SFFLOAT, "innerRadius", new SFFloat(0.0F), 
                   new SFFloat(0.0f)));

    outerRadius.set(
          addField(SFFLOAT, "outerRadius", new SFFloat(1.0F), 
                   new SFFloat(0.0f)));

    solid.set(
          addField(SFBOOL, "solid", new SFBool(false)));

    addURLs(URL_X3D);

    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);
}

Node *
ProtoDisk2D::create(Scene *scene)
{ 
    return new NodeDisk2D(scene, this); 
}

NodeDisk2D::NodeDisk2D(Scene *scene, Proto *def)
  : MeshBasedNode(scene, def)
{
}

void
NodeDisk2D::createMesh(bool cleanDoubleVertices, bool triangulate)
{
    float finnerRadius = innerRadius()->getValue();
    float fouterRadius = outerRadius()->getValue();

    float *fpoint = NULL;
    int numPoints = 0;
    Array<int> icoordIndex;

    int tess = TheApp->getTessellation() + 1;
    
    if (finnerRadius == 0) {
        numPoints = tess * 3;
        fpoint = new float[numPoints];
    
        float fstartAngle = 0; 
        float fendAngle = 2 * M_PI;
        float incAngle = (fendAngle - fstartAngle) / (tess - 1);
      
        for (int i = 0; i < tess; i++) {
            float angle = i * incAngle;
            fpoint[i * 3 + 0] = fouterRadius * cos(angle);
            fpoint[i * 3 + 1] = fouterRadius * sin(angle);
            fpoint[i * 3 + 2] = 0;
        }
    
        for (int i = 0; i < tess; i++)
            icoordIndex.append(i);
        icoordIndex.append(0);
        icoordIndex.append(-1);
    } else {
        numPoints = tess * 3 * 2;
        fpoint = new float[numPoints];
    
        float fstartAngle = 0; 
        float fendAngle = 2 * M_PI;
        float incAngle = (fendAngle - fstartAngle) / (tess - 1);
      
        for (int i = 0; i < tess; i++) {
            float angle = i * incAngle;
            fpoint[i * 3 * 2 + 0] = fouterRadius * cos(angle);
            fpoint[i * 3 * 2 + 1] = fouterRadius * sin(angle);
            fpoint[i * 3 * 2 + 2] = 0;
            fpoint[i * 3 * 2 + 3 + 0] = finnerRadius * cos(angle);
            fpoint[i * 3 * 2 + 3 + 1] = finnerRadius * sin(angle);
            fpoint[i * 3 * 2 + 3 + 2] = 0;
        }
    
        for (int i = 0; i < (tess - 1); i++) {
            icoordIndex.append(2 * i + 0);
            icoordIndex.append(2 * i + 1);
            icoordIndex.append(2 * i + 3);
            icoordIndex.append(2 * i + 2);
            icoordIndex.append(-1);
        }
    }

    MFVec3f* mfpoint = new MFVec3f(fpoint, numPoints);
    MFVec3f *coords = (MFVec3f *)mfpoint->copy();

    MFInt32 *mfcoordIndex = new MFInt32((int *)icoordIndex.getData(), 
                                        icoordIndex.size());
    MFInt32 *coordIndex = (MFInt32 *)mfcoordIndex->copy();

    int meshFlags = MESH_NORMAL_PER_VERTEX;
    if (solid()->getValue())
        meshFlags |= MESH_SOLID;
    meshFlags |= MESH_CCW;

    float transparency = 0;
    if (hasParent())
        transparency = getParent()->getTransparency();

    Array<MFVec2f *>texCoords;
    m_mesh = new MyMesh(this,
                        coords, coordIndex, NULL, NULL, NULL, NULL, texCoords, 
                        NULL, 0, meshFlags, transparency);
}



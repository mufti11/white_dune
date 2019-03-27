/*
 * NodeArc2D.cpp
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

#include "NodeArc2D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "Scene.h"
#include "SFFloat.h"
#include "RenderState.h"
#include "Util.h"
#include "math.h"
#include "DuneApp.h"

ProtoArc2D::ProtoArc2D(Scene *scene)
  : Proto(scene, "Arc2D")
{
    double unitAngle = scene->getUnitAngle();

    endAngle.set(
          addField(SFFLOAT, "endAngle", new SFFloat(M_PI / 2.0f / unitAngle), 
                   new SFFloat(-2.0f * M_PI / unitAngle), 
                   new SFFloat(2.0f * M_PI / unitAngle)));

    radius.set(
          addField(SFFLOAT, "radius", new SFFloat(1.0F), new SFFloat(0.0f)));

    startAngle.set(
          addField(SFFLOAT, "startAngle", new SFFloat(0.0F), 
                   new SFFloat(-2.0f * M_PI / unitAngle), 
                   new SFFloat(2.0f * M_PI / unitAngle)));

    addURLs(URL_X3D);

    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    subdivision.set(
          addExposedField(SFFLOAT, "subdivision", new SFFloat(32)));
    setFieldFlags(subdivision, FF_X3DOM_ONLY);
}

Node *
ProtoArc2D::create(Scene *scene)
{ 
    return new NodeArc2D(scene, this); 
}

NodeArc2D::NodeArc2D(Scene *scene, Proto *def)
  : ChainBasedGeometryNode(scene, def)
{
    m_chainDirty = true;
}

void
NodeArc2D::createChain(void)
{
    int tess = TheApp->getTessellation() + 1;
    m_chain.resize(tess);  
    float fradius = radius()->getValue();
    float fstartAngle = startAngle()->getFixedAngle(m_scene->getUnitAngle());
    float fendAngle = endAngle()->getFixedAngle(m_scene->getUnitAngle());
    if (fstartAngle == fendAngle) {
        fstartAngle = 0;
        fendAngle = 2 * M_PI;
    }
    float incAngle = (fendAngle - fstartAngle) / (tess - 1);
  

    for (int i = 0; i < tess; i++) {
        float angle = fstartAngle + i * incAngle;
        m_chain[i].x = fradius * cos(angle);
        m_chain[i].y = fradius * sin(angle);
        m_chain[i].z = 0;
    }
}



/*
 * NodeCircle2D.cpp
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

#include "NodeCircle2D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "Scene.h"
#include "SFFloat.h"
#include "RenderState.h"
#include "Util.h"
#include "math.h"
#include "DuneApp.h"

ProtoCircle2D::ProtoCircle2D(Scene *scene)
  : Proto(scene, "Circle2D")
{
    radius.set(
          addField(SFFLOAT, "radius", new SFFloat(1.0F), new SFFloat(0.0f)));

    addURLs(URL_X3D);
}

Node *
ProtoCircle2D::create(Scene *scene)
{ 
    return new NodeCircle2D(scene, this); 
}

NodeCircle2D::NodeCircle2D(Scene *scene, Proto *def)
  : ChainBasedGeometryNode(scene, def)
{
    m_chainDirty = true;
}

void
NodeCircle2D::createChain(void)
{
    int tess = TheApp->getTessellation() + 1;
    m_chain.resize(tess);  
    float fradius = radius()->getValue();
    float fstartAngle = 0;
    float fendAngle = 2 * M_PI; 

    float incAngle = (fendAngle - fstartAngle) / (tess - 1);
  
    for (int i = 0; i < tess; i++) {
        float angle = fstartAngle + i * incAngle;
        m_chain[i].x = fradius * cos(angle);
        m_chain[i].y = fradius * sin(angle);
        m_chain[i].z = 0;
    }
}



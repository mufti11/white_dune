/*
 * NodePolyline2D.cpp
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

#include "NodePolyline2D.h"
#include "Proto.h"
#include "MFVec3f.h"
#include "ExposedField.h"
#include "Field.h"
#include "DuneApp.h"
#include "Scene.h"

ProtoPolyline2D::ProtoPolyline2D(Scene *scene)
  : Proto(scene, "Polyline2D")
{
    ExposedField* p = new ExposedField(MFVEC2F, "point", new MFVec2f(0), 
                                       NULL, NULL, ANY_NODE, 0, NULL,
                                       "lineSegments");
    point.set(addExposedField(p));

    addURLs(URL_X3D);

    x3domGeometryCommonFields()

    ccw.set(
          addExposedField(SFBOOL, "ccw", new SFBool(true)));
    setFieldFlags(ccw, FF_X3DOM_ONLY);

    solid.set(
          addExposedField(SFBOOL, "solid", new SFBool(true)));
    setFieldFlags(solid, FF_X3DOM_ONLY);
}

Node *
ProtoPolyline2D::create(Scene *scene)
{ 
    return new NodePolyline2D(scene, this); 
}

int     
ProtoPolyline2D::getNodeClass() const
{ 
    if (m_scene->isX3d())
        return GEOMETRY_NODE; 
    else
        return NURBS_CONTROL_CURVE_NODE;
}

NodePolyline2D::NodePolyline2D(Scene *scene, Proto *def)
  : ChainBasedNode(scene, def)
{
}

void
NodePolyline2D::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    if (hasParent())
        getParents().update();
}

int
NodePolyline2D::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f ,"EXTERNPROTO Polyline2D[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f ," ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:web3d:vrml97:node:Polyline2D\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:inet:blaxxun.com:node:Polyline2D\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:ParaGraph:Polyline2D\",\n") )
    TheApp->incSelectionLinenumber();
#ifdef HAVE_VRML97_AMENDMENT1_PROTO_URL
    RET_ONERROR( mywritestr(f ," \"") )
    RET_ONERROR( mywritestr(f ,HAVE_VRML97_AMENDMENT1_PROTO_URL) )
    RET_ONERROR( mywritestr(f ,"/Polyline2DPROTO.wrl") )
    RET_ONERROR( mywritestr(f ,"\"\n") )
    TheApp->incSelectionLinenumber();
#else
    RET_ONERROR( mywritestr(f ," \"Polyline2DPROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#endif
    RET_ONERROR( mywritestr(f ," \"http://wdune.ourproject.org/docs/vrml97Amendment1/Polyline2DPROTO.wrl\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int             
NodePolyline2D::write(int filedes, int indent)
{
    if (!m_scene->isPureVRML())
        RET_ONERROR( Node::write(filedes, indent) )
    return 0;
}

void            
NodePolyline2D::createChain(void)
{
    if (!m_scene->isX3d())
        return;
    int size = point()->getSFSize();
    const float *flineSegments = point()->getValues();
    for (int i = 0; i < size; i++) {
        m_chain[i].x = flineSegments[i * 2 + 0];
        m_chain[i].y = flineSegments[i * 2 + 1];
        m_chain[i].z = 0;
    }
}


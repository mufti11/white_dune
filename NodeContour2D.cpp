/*
 * NodeContour2D.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2004 J. "MUFTI" Scheurich
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

#include "NodeContour2D.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "Scene.h"
#include "NodePolyline2D.h"
#include "NodeContourPolyline2D.h"
#include "MoveCommand.h"

ProtoContour2D::ProtoContour2D(Scene *scene)
  : Proto(scene, "Contour2D")
{                    
    addEventIn(MFNODE, "addChildren");
    addEventIn(MFNODE, "removeChildren");

    children.set(
          addExposedField(MFNODE, "children", new MFNode(), 
                          NURBS_CURVE_2D_OR_CONTOUR_POLYLINE_2D_NODE));

    addURLs(URL_X3D);
}

Node *
ProtoContour2D::create(Scene *scene)
{ 
    return new NodeContour2D(scene, this); 
}

NodeContour2D::NodeContour2D(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

int
NodeContour2D::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f, "EXTERNPROTO Contour2D[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f, " ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:web3d:vrml97:node:Contour2D\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:inet:blaxxun.com:node:Contour2D\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:ParaGraph:Contour2D\",\n") )
    TheApp->incSelectionLinenumber();
#ifdef HAVE_VRML97_AMENDMENT1_PROTO_URL
    RET_ONERROR( mywritestr(f, " \"") )
    RET_ONERROR( mywritestr(f, HAVE_VRML97_AMENDMENT1_PROTO_URL) )
    RET_ONERROR( mywritestr(f, "/Contour2DPROTO.wrl") )
    RET_ONERROR( mywritestr(f, "\"\n") )
    TheApp->incSelectionLinenumber();
#else
    RET_ONERROR( mywritestr(f, " \"Contour2DPROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#endif
    RET_ONERROR( mywritestr(f, " \"http://wdune.ourproject.org/docs/vrml97Amendment1/Contour2DPROTO.wrl\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int             
NodeContour2D::write(int filedes, int indent)
{
    if (!m_scene->isPureVRML())
        RET_ONERROR( Node::write(filedes, indent) )
    return 0;
}

int
NodeContour2D::getComponentLevel(void) const
{
    return 4;
}

const char* 
NodeContour2D::getComponentName(void) const
{
    static const char* name = "NURBS";
    return name;
}

Node *
NodeContour2D::convert2X3d(void)
{
    NodeList *child = children()->getValues();
    for (int i = 0; i < child->size(); i++) {
        Node *node = child->get(i);
        if (node->getType() == VRML_POLYLINE_2D) {
            m_scene->execute(new MoveCommand(node, this, children_Field(),
                                             NULL, -1));
            NodeContourPolyline2D *polyline = (NodeContourPolyline2D *)
                  m_scene->createNode("ContourPolyline2D");
            MFVec2d *points = new MFVec2d();
            for (int j = 0; j < 
                 ((NodeContourPolyline2D *)node)->lineSegments()->getSize(); 
                 j++) {
                double p = *((NodeContourPolyline2D *)node)->lineSegments()->
                            getValue(j);
                points->setValue(j, p);
            }    
            polyline->lineSegments(points);
            node = polyline;
        } else
            node = node->convert2X3d();
        m_scene->execute(new MoveCommand(node, NULL, -1, this, 
                                         children_Field()));
    }
    return this;
}

Node *
NodeContour2D::convert2Vrml(void)
{
    NodeList *child = children()->getValues();
    for (int i = 0; i < child->size(); i++) {
        Node *node = child->get(i);
        if (node->getType() == X3D_CONTOUR_POLYLINE_2D) {
            m_scene->execute(new MoveCommand(node, this, children_Field(),
                                             NULL, -1));
            NodePolyline2D *polyline = (NodePolyline2D *)
                                       m_scene->createNode("Polyline2D");
            MFVec2f *points = new MFVec2f();
            for (int j = 0; j < 
                 ((NodeContourPolyline2D *)node)->lineSegments()->getSize(); 
                 j++) {
                float p = *((NodeContourPolyline2D *)node)->lineSegments()->
                           getValue(j);
                points->setValue(j, p);
            }    
            polyline->point(points);
            node = polyline;
        } else
            node = node->convert2Vrml();
        m_scene->execute(new MoveCommand(node, NULL, -1, this, 
                                         children_Field()));
    }
    return this;
}


void
NodeContour2D::setField(int index, FieldValue *value, int cf)
{
    if (hasParent()) {
        Node *parent = getParent();
        parent->update();
    }
    Node::setField(index, value, cf);
}

void
NodeContour2D::update()
{
    if (hasParent())
        getParent()->update();
}



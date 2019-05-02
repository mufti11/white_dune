/*
 * NodeTrimmedSurface.cpp
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

#include "NodeTrimmedSurface.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "Scene.h"
#include "NodeNurbsTrimmedSurface.h"
#include "NodeIndexedFaceSet.h"
#include "NodeCoordinate.h"
#include "MoveCommand.h"

ProtoTrimmedSurface::ProtoTrimmedSurface(Scene *scene)
  : GeometryProto(scene, "TrimmedSurface")
{                    
    addEventIn(MFNODE, "addTrimmingContour");
    addEventIn(MFNODE, "removeTrimmingContour");
    trimmingContour.set(
          addExposedField(MFNODE, "trimmingContour", new MFNode(), 
                          VRML_CONTOUR_2D));
    surface.set(
          addExposedField(SFNODE, "surface", new SFNode(NULL), 
                          VRML_NURBS_SURFACE));

    addURLs(URL_VRML97_AMENDMENT1);
}

Node *
ProtoTrimmedSurface::create(Scene *scene)
{ 
    return new NodeTrimmedSurface(scene, this); 
}

NodeTrimmedSurface::NodeTrimmedSurface(Scene *scene, Proto *def)
  : GeometryNode(scene, def)
{
}

int
NodeTrimmedSurface::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f ,"EXTERNPROTO TrimmedSurface[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f ," ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:web3d:vrml97:node:TrimmedSurface\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:inet:blaxxun.com:node:TrimmedSurface\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:ParaGraph:TrimmedSurface\",\n") )
    TheApp->incSelectionLinenumber();
#ifdef HAVE_VRML97_AMENDMENT1_PROTO_URL
    RET_ONERROR( mywritestr(f ," \"") )
    RET_ONERROR( mywritestr(f ,HAVE_VRML97_AMENDMENT1_PROTO_URL) )
    RET_ONERROR( mywritestr(f ,"/TrimmedSurfacePROTO.wrl") )
    RET_ONERROR( mywritestr(f ,"\"\n") )
    TheApp->incSelectionLinenumber();
#else
    RET_ONERROR( mywritestr(f ," \"TrimmedSurfacePROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#endif
    RET_ONERROR( mywritestr(f ," \"http://wdune.ourproject.org/docs/vrml97Amendment1/TrimmedSurfacePROTO.wrl\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int             
NodeTrimmedSurface::write(int filedes, int indent)
{
    if (m_scene->isPureVRML()) {
        if (surface()->getValue())
            return surface()->getValue()->write(filedes, indent);
        NodeIndexedFaceSet *faceSet = (NodeIndexedFaceSet *)
                                      m_scene->createNode("IndexedFaceSet");
        int ret = faceSet->write(filedes, indent);
        m_scene->removeNode(faceSet);
        return ret;
    }
    return Node::write(filedes, indent);
}

void    
NodeTrimmedSurface::draw() 
{ 
    if (surface()->getValue())
        surface()->getValue()->draw(); 
}

Vec3f   
NodeTrimmedSurface::getMinBoundingBox(void)
{
    if (surface()->getValue())
        return surface()->getValue()->getMinBoundingBox();
    else {
        static Vec3f ret(0,0,0);
        return ret;
    }
}

Vec3f   
NodeTrimmedSurface::getMaxBoundingBox(void)
{
    if (surface()->getValue())
        return surface()->getValue()->getMaxBoundingBox();
    else {
        static Vec3f ret(0,0,0);
        return ret;
    }
}

void    
NodeTrimmedSurface::update(void) 
{ 
    if (surface()->getValue())
        surface()->getValue()->update(); 
}

void
NodeTrimmedSurface::reInit(void) 
{ 
    if (surface()->getValue())
        surface()->getValue()->reInit(); 
    Node::reInit();
}

int
NodeTrimmedSurface::countPolygons(void) 
{
     if (surface()->getValue())          
         return surface()->getValue()->countPolygons();
     else 
         return 0;
}

int   
NodeTrimmedSurface:: countPrimitives(void)
{
     if (surface()->getValue())
         return surface()->getValue()->countPrimitives();
     else 
         return 0;
}


int
NodeTrimmedSurface::countPolygons1Sided(void) 
{
     if (surface()->getValue())          
         return surface()->getValue()->countPolygons1Sided();
     else 
         return 0;
}

int
NodeTrimmedSurface::countPolygons2Sided(void) 
{
     if (surface()->getValue())          
         return surface()->getValue()->countPolygons2Sided();
     else 
         return 0;
}


void
NodeTrimmedSurface::flip(int index) 
{ 
     if (surface()->getValue())
         surface()->getValue()->flip(index); 
}


void
NodeTrimmedSurface::swap(int fromTo) 
{ 
     if (surface()->getValue())
         surface()->getValue()->swap(fromTo); 
}

Node *
NodeTrimmedSurface::convert2X3d(void)
{
    NodeNurbsTrimmedSurface *ret = (NodeNurbsTrimmedSurface *)
                                   m_scene->createNode("NurbsTrimmedSurface");
    NodeNurbsSurface *nsurface = (NodeNurbsSurface *)surface()->getValue();
    if (nsurface) {
        m_scene->removeNode(ret);
        ret->unref();
        ret = (NodeNurbsTrimmedSurface *)nsurface->toNurbsTrimmedSurface();
    }    

    NodeList *trimm = trimmingContour()->getValues();
    for (int i = 0; i < trimm->size(); i++) {
        Node *node = trimm->get(i)->convert2X3d();
        m_scene->execute(new MoveCommand(node, NULL, -1, ret, 
                                         ret->trimmingContour_Field()));
    }
    return ret;
}



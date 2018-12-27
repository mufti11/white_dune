/*
 * NodeCoordinateDeformer.cpp
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

#include "NodeCoordinateDeformer.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "Scene.h"

ProtoCoordinateDeformer::ProtoCoordinateDeformer(Scene *scene)
  : Proto(scene, "CoordinateDeformer")
{                    
    addEventIn(MFNODE, "addChildren");
    addEventIn(MFNODE, "removeChildren");

    children.set(
          addExposedField(MFNODE, "children", new MFNode(), CHILD_NODE));

    outputCoord.set(
          addExposedField(MFNODE, "outputCoord", new MFNode(), 
                          COORDINATE_NODE));

    inputCoord.set(
          addExposedField(MFNODE, "inputCoord", new MFNode(), COORDINATE_NODE));

    controlPoint.set(
          addExposedField(MFVEC3F, "controlPoint", new MFVec3f()));

    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0.0f, 0.0f, 0.0f)));

    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1.0f, -1.0f, -1.0f), 
                   new SFFloat(-1.0f)));

    weight.set(
          addExposedField(MFFLOAT, "weight", new MFFloat(), new SFFloat(0.0f)));

    uDimension.set(
          addField(SFINT32, "uDimension", new SFInt32(0), new SFInt32(0)));

    vDimension.set(
          addField(SFINT32, "vDimension", new SFInt32(0), new SFInt32(0)));

    wDimension.set(
          addField(SFINT32, "wDimension", new SFInt32(0), new SFInt32(0)));

    uKnot.set(
          addField(MFFLOAT, "uKnot", new MFFloat()));

    vKnot.set(
          addField(MFFLOAT, "vKnot", new MFFloat()));

    wKnot.set(
          addField(MFFLOAT, "wKnot", new MFFloat()));

    uOrder.set(
          addField(SFINT32, "uOrder", new SFInt32(3), new SFInt32(2)));

    vOrder.set(
          addField(SFINT32, "vOrder", new SFInt32(3), new SFInt32(2)));

    wOrder.set(
          addField(SFINT32, "wOrder", new SFInt32(3), new SFInt32(2)));

    addURLs(URL_VRML97_AMENDMENT1);
}

NodeCoordinateDeformer::NodeCoordinateDeformer(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

Node *
ProtoCoordinateDeformer::create(Scene *scene)
{ 
    return new NodeCoordinateDeformer(scene, this); 
}

void
NodeCoordinateDeformer::flip(int index)
{
    bboxCenter()->flip(index);
    children()->flip(index);
}

void
NodeCoordinateDeformer::swap(int fromTo)
{
    bboxCenter()->swap(fromTo);
    bboxSize()->swap(fromTo);
    children()->swap(fromTo);
}

int
NodeCoordinateDeformer::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f, "EXTERNPROTO CoordinateDeformer[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f, " ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:web3d:vrml97:node:CoordinateDeformer\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:inet:blaxxun.com:node:CoordinateDeformer\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:ParaGraph:CoordinateDeformer\",\n") )
    TheApp->incSelectionLinenumber();
#ifdef HAVE_VRML97_AMENDMENT1_PROTO_URL
    RET_ONERROR( mywritestr(f, " \"") )
    RET_ONERROR( mywritestr(f, HAVE_VRML97_AMENDMENT1_PROTO_URL) )
    RET_ONERROR( mywritestr(f, "/CoordinateDeformerPROTO.wrl") )
    RET_ONERROR( mywritestr(f, "\"\n") )
    TheApp->incSelectionLinenumber();
#else
    RET_ONERROR( mywritestr(f, " \"CoordinateDeformerPROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#endif
    RET_ONERROR( mywritestr(f, " \"http://wdune.ourproject.org/docs/vrml97Amendment1/CoordinateDeformerPROTO.wrl\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int             
NodeCoordinateDeformer::write(int filedes, int indent)
{
    if (!m_scene->isPureVRML())
        RET_ONERROR( Node::write(filedes, indent) )
    return 0;
}



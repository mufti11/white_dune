/*
 * NodeNurbsSet.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2003 J. "MUFTI" Scheurich
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
#ifndef FLT_MAX
# include <float.h>
#endif
#include "stdafx.h"

#include "NodeNurbsSet.h"
#include "Proto.h"
#include "Field.h"
#include "FieldValue.h"
#include "MoveCommand.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "SFFloat.h"
#include "DuneApp.h"
#include "NodeNurbsSurface.h"
#include "NodeShape.h"
#include "Scene.h"
#include "NodeNurbsGroup.h"

ProtoNurbsSet::ProtoNurbsSet(Scene *scene)
  : Proto(scene, "NurbsSet")
{
    geometry.set(    
          addExposedField(MFNODE, "geometry", new MFNode(), 
                          PARAMETRIC_GEOMETRY_NODE));
    tessellationScale.set(
          addExposedField(SFFLOAT, "tessellationScale", new SFFloat(1.0f), 
                          new SFFloat(0.0f), new SFFloat(FLT_MAX)));
    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0.0f, 0.0f, 0.0f)));
    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1.0f, -1.0f, -1.0f), 
                   new SFFloat(-1.0f)));
    addEventIn(MFNODE, "addGeometry");
    addEventIn(MFNODE, "removeGeometry");
}

Node *
ProtoNurbsSet::create(Scene *scene)
{ 
    return new NodeNurbsSet(scene, this); 
}

NodeNurbsSet::NodeNurbsSet(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

void
NodeNurbsSet::draw(void)
{
    NodeList   *childList = geometry()->getValues();

    glPushName(geometry_Field());  // field offset

    for (int i = 0; i < childList->size(); i++)
        childList->get(i)->bind();

    glPushName(0);
    for (int i = 0; i < childList->size(); i++) {
        glLoadName(i);
        childList->get(i)->draw();
    }
    glPopName();

    for (int i = 0; i < childList->size(); i++)
        childList->get(i)->unbind();

    glPopName();
}

int
NodeNurbsSet::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f ,"EXTERNPROTO NurbsSet[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f ," ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:web3d:vrml97:node:NurbsSet\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:inet:blaxxun.com:node:NurbsSet\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ," \"urn:ParaGraph:NurbsSet\",\n") )
    TheApp->incSelectionLinenumber();
#ifdef HAVE_X3D_PROTO_URL
    RET_ONERROR( mywritestr(f ," \"") )
    RET_ONERROR( mywritestr(f ,HAVE_X3D_PROTO_URL) )
    RET_ONERROR( mywritestr(f ,"/NurbsSetPROTO.wrl") )
    RET_ONERROR( mywritestr(f ,"\"\n") )
    TheApp->incSelectionLinenumber();
#else
    RET_ONERROR( mywritestr(f ," \"NurbsSetPROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#endif
    RET_ONERROR( mywritestr(f ," \"http://wdune.ourproject.org/docs/x3dDraft/NurbsSetPROTO.wrl\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f ,"]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

   
int             
NodeNurbsSet::write(int filedes, int indent) 
{
    if (m_scene->isPureVRML()) {
        NodeGroup group(this);
        RET_ONERROR( group.write(filedes, indent) )
    } else
        RET_ONERROR( Node::write(filedes, indent) )
    return 0;
}

void
NodeNurbsSet::setHandle(Node* caller, float newWeight,
                          const Vec3f &newV, const Vec3f &oldV)
{
    NodeList   *childList = geometry()->getValues();
    for (int i = 0; i < childList->size(); i++) {
        Node *child = childList->get(i);
        if (child->getType() == VRML_NURBS_SURFACE) {
            if (child != caller)
                ((NodeNurbsSurface *)child)->setHandle(newWeight, newV, oldV);
        } else if (child->getType() == VRML_SHAPE) {
            Node *node = ((NodeShape *)child)->geometry()->getValue();
            if ((node != caller) && (node->getType() == VRML_NURBS_SURFACE))
                ((NodeNurbsSurface *)node)->setHandle(newWeight, newV, oldV);
        }
    }
}

void
NodeNurbsSet::backupFieldsAppend(Node* caller, int field)
{
    NodeList   *childList = geometry()->getValues();
    for (int i = 0; i < childList->size(); i++) {
        Node *child = childList->get(i);
        if (child->getType() == VRML_NURBS_SURFACE) {
            if (child != caller)
                m_scene->backupFieldsAppend(child, field);
        } else if (child->getType() == VRML_SHAPE) {
            Node *node = ((NodeShape *)child)->geometry()->getValue();
            if ((node != caller) && (node->getType() == VRML_NURBS_SURFACE))
                m_scene->backupFieldsAppend(node, field);
        }
    }
}

Node *
NodeNurbsSet::convert2X3d(void)
{
    return NULL;
}

Node *
NodeNurbsSet::convert2Vrml(void) 
{
    NodeList *childs = geometry()->getValues();
    NodeNurbsGroup *nurbsGroup = (NodeNurbsGroup *)m_scene->createNode(
                                 "NurbsGroup");
    
    nurbsGroup->tessellationScale(new SFFloat(*tessellationScale()));
    m_scene->addNodes(nurbsGroup, nurbsGroup->children_Field(), childs,
                      SCAN_FOR_BOTH);
    m_scene->changeRoutes(nurbsGroup, nurbsGroup->children_Field(),
                         this, this->geometry_Field());
    m_scene->changeRoutes(nurbsGroup, nurbsGroup->tessellationScale_Field(),
                         this, this->tessellationScale_Field());
    Node *parent = getParent();
    int parentField = getParentField();
    MoveCommand *command = new MoveCommand(this, parent, parentField, 
                                           NULL, -1);
    command->execute();
    command = new MoveCommand(nurbsGroup, NULL, -1, parent, parentField);       
    command->execute();

    m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
    geometry()->convert2Vrml();
    return nurbsGroup;
}


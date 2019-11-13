/*
 * NodeNurbsGroup.cpp
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

#include "NodeNurbsGroup.h"
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
#include "NodeNurbsSet.h"

ProtoNurbsGroup::ProtoNurbsGroup(Scene *scene)
  : ProtoGroup(scene, "NurbsGroup")
{
    tessellationScale.set(
          addExposedField(SFFLOAT, "tessellationScale", new SFFloat(1.0f), 
                          new SFFloat(0.0f), new SFFloat(FLT_MAX)));
    getField(children)->addToNodeType(PARAMETRIC_GEOMETRY_NODE);

    addURLs(URL_X3D);
}

Node *
ProtoNurbsGroup::create(Scene *scene)
{ 
    return new NodeNurbsGroup(scene, this); 
}

NodeNurbsGroup::NodeNurbsGroup(Scene *scene, Proto *def)
  : NodeGroup(scene, def)
{
}

void
NodeNurbsGroup::draw(void)
{
    NodeList *childList = children()->getValues();

    glPushName(children_Field());  // field offset

    for (long i = 0; i < childList->size(); i++)
        childList->get(i)->bind();

    glPushName(0);
    for (long i = 0; i < childList->size(); i++) {
        glLoadName(i);
        childList->get(i)->draw();
    }
    glPopName();

    for (long i = 0; i < childList->size(); i++)
        childList->get(i)->unbind();

    glPopName();
}

int
NodeNurbsGroup::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeX3dProto(f);

    RET_ONERROR( mywritestr(f, "EXTERNPROTO NurbsGroup[\n") )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f, " ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "[\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:web3d:vrml97:node:NurbsGroup\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:inet:blaxxun.com:node:NurbsGroup\",\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " \"urn:ParaGraph:NurbsGROUP\",\n") )
    TheApp->incSelectionLinenumber();
#ifdef HAVE_VRML97_AMENDMENT1_PROTO_URL
    RET_ONERROR( mywritestr(f, " \"") )
    RET_ONERROR( mywritestr(f, HAVE_VRML97_AMENDMENT1_PROTO_URL) )
    RET_ONERROR( mywritestr(f, "/NurbsGroupPROTO.wrl") )
    RET_ONERROR( mywritestr(f, "\"\n") )
    TheApp->incSelectionLinenumber();
#else
    RET_ONERROR( mywritestr(f, " \"NurbsGroupPROTO.wrl\",\n") )
    TheApp->incSelectionLinenumber();
#endif
    RET_ONERROR( mywritestr(f, " \"http://wdune.ourproject.org/docs/vrml97Amendment1/NurbsGroupPROTO.wrl\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "]\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

   
int             
NodeNurbsGroup::write(int filedes, int indent, bool avoidUse) 
{
    if (m_scene->isPureVRML()) {
        NodeGroup group(this);
        RET_ONERROR( group.write(filedes, indent, avoidUse) )
    } else
        RET_ONERROR( Node::write(filedes, indent, avoidUse) )
    return 0;
}

void
NodeNurbsGroup::setHandle(Node* caller, float newWeight,
                          const Vec3f &newV, const Vec3f &oldV)
{
    NodeList   *childList = children()->getValues();
    for (long i = 0; i < childList->size(); i++) {
        Node *child = childList->get(i);
        if (child->getType() == VRML_NURBS_SURFACE) {
            if (child != caller)
                ((NodeNurbsSurface *)child)->setHandle(newWeight, newV, oldV, 
                                                       true);
        } else if (child->getType() == VRML_SHAPE) {
            Node *node = ((NodeShape *)child)->geometry()->getValue();
            if ((node != caller) && (node->getType() == VRML_NURBS_SURFACE))
                ((NodeNurbsSurface *)node)->setHandle(newWeight, newV, oldV, 
                                                      true);
        }
    }
}

void
NodeNurbsGroup::backupFieldsAppend(Node* caller, int field)
{
    m_scene->addNextCommand();
    NodeList *childList = children()->getValues();
    for (long i = 0; i < childList->size(); i++) {
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
NodeNurbsGroup::convert2X3d(void)
{
    Node *ret = NULL;
    NodeList *childs = children()->getValues();
    NodeList *groupChilds = new NodeList();
    NodeList *nurbsSetChilds = new NodeList();
    for (long i = 0; i < childs->size(); i++) {
        Node *node = childs->get(i);
        if (node != NULL)
            node = node->convert2X3d();
        if (node == NULL) {
            node = childs->get(i)->copy();
            node->ref();
        }
        if (node->getNodeClass() & CHILD_NODE)
            groupChilds->append(node);
        else
            nurbsSetChilds->append(node);
    }
    SFFloat *tScale = new SFFloat(tessellationScale()->getValue());

    Node *parent = getParent();
    int parentField = getParentField();

    Node *newGroup = NULL;
    NodeNurbsSet *nurbsSet = NULL;
    NodeGroup *group = NULL;

    Node *newParent = parent;
    int newParentField = parentField;
    if ((nurbsSetChilds->size() > 0) && (groupChilds->size() > 0)) {
        swDebugf("Warning: NurbsGroup should be either NurbsSet style or Group style, not both\n");
        newGroup = m_scene->createNode("Group");
        ret = newGroup;
        newParent = newGroup;
        newParentField = ((NodeGroup *)newGroup)->children_Field();
    }

    if (nurbsSetChilds->size() > 0) {
        nurbsSet = (NodeNurbsSet *) m_scene->createNode("NurbsSet");
        nurbsSet->tessellationScale(new SFFloat(*tScale));
        m_scene->changeRoutes(nurbsSet, nurbsSet->geometry_Field(),
                              this, this->children_Field());
        m_scene->changeRoutes(nurbsSet, nurbsSet->tessellationScale_Field(),
                              this, this->tessellationScale_Field());

        if (ret == NULL)
            ret = nurbsSet;
    }
    
    if (groupChilds->size() > 0) {
        group = (NodeGroup *) m_scene->createNode("Group");
    
        m_scene->changeRoutes(group, group->children_Field(),
                              this, this->children_Field());

        if (ret == NULL)
            ret = group;
    }

    MoveCommand *command = new MoveCommand(this, parent, parentField, NULL, -1);
    command->execute();

    if (newGroup) {
        command = new MoveCommand(newGroup, NULL, -1, parent, parentField);
        command->execute();
    }
    if (nurbsSet) {
        command = new MoveCommand(nurbsSet, NULL, -1, 
                                            newParent, newParentField);
        command->execute();
        m_scene->addNodes(nurbsSet, nurbsSet->geometry_Field(), nurbsSetChilds,
                          SCAN_FOR_BOTH);
    }
    if (group) {
        command = new MoveCommand(group, NULL, -1, newParent, newParentField);
        command->execute();
        m_scene->addNodes(group, group->children_Field(), groupChilds,
                          SCAN_FOR_BOTH);
    }
    m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
    return ret;
}

Node *
NodeNurbsGroup::convert2Vrml(void) 
{
    NodeGroup *group = (NodeGroup *)m_scene->createNode("Group");
    NodeList *newChildren = buildVrml97Children(children());

    m_scene->addNodes(group, group->children_Field(), newChildren, 
                      SCAN_FOR_BOTH);
    m_scene->changeRoutes(group, group->children_Field(),
                         this, this->children_Field());
    Node *parent = getParent();
    int parentField = getParentField();
    MoveCommand *command = new MoveCommand(this, parent, parentField, 
                                           NULL, -1);
    command->execute();
    command = new MoveCommand(group, NULL, -1, parent, parentField);       
    command->execute();

    m_scene->UpdateViews(NULL, UPDATE_REDRAW, NULL);
    return group;
}

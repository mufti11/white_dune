/*
 * NodeVrmlScene.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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

#include "NodeVrmlScene.h"
#include "Scene.h"
#include "Proto.h"
#include "MFNode.h"
#include "RouteCommand.h"
#include "NodeTimeSensor.h"
#include "NodeViewpoint.h"
#include "NodeVrmlCut.h"

ProtoVrmlScene::ProtoVrmlScene(Scene *scene)
  : Proto(scene, "VrmlScene")
{
    children.set(
          addExposedField(MFNODE, "children", new MFNode(), CHILD_NODE));
    timeIn.set(addEventIn(SFTIME, "timeIn"));
    timeOut.set(addEventOut(SFTIME, "timeOut"));
    bindOut.set(addEventOut(SFBOOL, "bindOut"));
}

Node *
ProtoVrmlScene::create(Scene *scene)
{ 
    return new NodeVrmlScene(scene, this); 
}

NodeVrmlScene::NodeVrmlScene(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

int
NodeVrmlScene::writeXmlProto(int f)
{
    RET_ONERROR( mywritestr(f, "<ProtoDeclare name='VrmlScene' >\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  <ProtoInterface>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='timeIn' type='SFTime' accessType='inputOnly'/>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='timeOut' type='SFTime' accessType='outputOnly'/>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='bindOut' type='SFBool' accessType='outputOnly'/>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='children' type='MFNode' accessType='inputOutput'>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    </field>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  </ProtoInterface>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  <ProtoBody>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <Group>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <IS>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='children' protoField='children' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    </IS>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    </Group>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <Script >\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='timeIn' type='SFTime' accessType='inputOnly'/>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='timeOut' type='SFTime' accessType='outputOnly'/>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='bindOut' type='SFBool' accessType='outputOnly'/>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <IS>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='timeIn' protoField='timeIn' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='timeOut' protoField='timeOut' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='bindOut' protoField='bindOut' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    </IS>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <![CDATA[ecmascript:\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " // eventOut SFTime timeOut //\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " // eventOut SFBool bindOut //\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " function timeIn(value)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    // value  SFTime\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    timeOut=value;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    bindOut=true;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    \n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    ]]>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  </Script>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  </ProtoBody>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "</ProtoDeclare>\n") )
    TheApp->incSelectionLinenumber();
     return 0;
}

int
NodeVrmlScene::writeProto(int f)
{
    if (m_scene->isX3dom())
        return 0;
    if (m_scene->isX3dXml())
        return writeXmlProto(f);

    RET_ONERROR( mywritestr(f, "PROTO VrmlScene [\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeEventInStr(f) )
    RET_ONERROR( mywritestr(f, "  SFTime timeIn\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeEventOutStr(f) )
    RET_ONERROR( mywritestr(f, " SFTime timeOut\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeEventOutStr(f) )
    RET_ONERROR( mywritestr(f, " SFBool bindOut\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeExposedFieldStr(f) )
    RET_ONERROR( mywritestr(f, " MFNode children \n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    [\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "{\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "Group \n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  children IS children\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "Script \n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeEventInStr(f) )
    RET_ONERROR( mywritestr(f, "  SFTime timeIn IS timeIn\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeEventOutStr(f) )
    RET_ONERROR( mywritestr(f, " SFTime timeOut IS timeOut\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeEventOutStr(f) )
    RET_ONERROR( mywritestr(f, " SFBool bindOut IS bindOut\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  url     \"javascript:\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " // eventOut SFTime timeOut //\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " // eventOut SFBool bindOut //\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " function timeIn(value)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    // value  SFTime\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    timeOut=value;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    bindOut=true;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    \"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "}\n") )
    TheApp->incSelectionLinenumber();
    return(0);
}

static bool routeToTimeSensor(Node *node, void *data)
{
    if (node->getType() == VRML_TIME_SENSOR) {
        NodeTimeSensor *timer = (NodeTimeSensor *)node;
        if (TheApp->isImportingDataFile() || (!node->getScene()->isParsing()))
            timer->loop(new SFBool(false));
        NodeVrmlScene *vrmlScene = (NodeVrmlScene *)data;
        node->getScene()->execute(
              new RouteCommand(vrmlScene, vrmlScene->timeOut_Field(),
                               timer, timer->startTime_Field()));
        if (vrmlScene->hasParent()) {
            SFTime *cycleInterval = timer->cycleInterval();
            Node *parent = vrmlScene->getParent();
            if (parent->getType() == DUNE_VRML_CUT) {
                NodeVrmlCut *vrmlCut = (NodeVrmlCut *)parent;
                vrmlCut->updateCycleInterval(vrmlScene, cycleInterval);
            }
        }
    }
    return true;     
}

static bool routeToViewpoint(Node *node, void *data)
{
    NodeVrmlScene *vrmlScene = (NodeVrmlScene *)data;
    if (vrmlScene->getOutput(vrmlScene->bindOut_Field()).size() > 0) 
        return false;
    if (node->getType() == VRML_VIEWPOINT) {
        NodeViewpoint *viewpoint = (NodeViewpoint *)node;
        node->getScene()->execute(
              new RouteCommand(vrmlScene, vrmlScene->bindOut_Field(),
                               viewpoint, viewpoint->set_bind_Field()));
        return false;
    }
    return true;     
}

void
NodeVrmlScene::setField(int index, FieldValue *value, int cf)
{
    if (value == NULL)
        return;
    if (index == children_Field()) {
        IntArray newValues;
        IntArray deleteValues;
        ((MFieldValue *)value)->getDiff(&newValues, &deleteValues, children());
        for (long i = 0; i < newValues.size(); i++) {
            Node *node = ((MFNode *)value)->getValue(newValues[i]);
            node->doWithBranch(routeToTimeSensor, (void *)this);
            node->doWithBranch(routeToViewpoint, (void *)this);
        }
    }
    update();
    Node::setField(index, value, cf);
}

// The VrmlScene node creates routes by itself, but it is important,
// that only one Route to the bind field of a viewpoint may be created
// In this case, the old route is deleted
void
NodeVrmlScene::onAddRoute(Node *s, int out, Node *d, int in)
{
}


/*
 * NodeVrmlCut.cpp
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
#include <ctype.h>

#include "NodeVrmlCut.h"
#include "swt.h"
#include "Scene.h"
#include "Proto.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "MFFloat.h"
#include "SFBool.h"
#include "ExposedField.h"
#include "Field.h"
#include "EventIn.h"
#include "EventOut.h"
#include "DuneApp.h"
#include "RouteCommand.h"
#include "UnRouteCommand.h"
#include "NodeTimeSensor.h"
#include "NodeViewpoint.h"
#include "NodeVrmlScene.h"

ProtoVrmlCut::ProtoVrmlCut(Scene *scene)
  : Proto(scene, "VrmlCut")
{
    sceneLengths.set(
          addField(MFTIME, "sceneLengths", new MFTime(), new SFTime(0)));
    setFieldFlags(sceneLengths, FF_STATIC);
    sceneNumber.set(
          addField(SFINT32, "sceneNumber", new SFInt32(-1), new SFInt32(-1)));
    setFieldFlags(sceneNumber, FF_STATIC);
    scenes.set(
          addExposedField(MFNODE, "scenes", new MFNode(), DUNE_VRML_SCENE));
    setFieldFlags(scenes, FF_STATIC);
    numberPreviousScenes.set(
          addField(SFINT32, "numberPreviousScenes", new SFInt32(0), 
                   new SFInt32(0)));
    setFieldFlags(numberPreviousScenes, FF_STATIC);
    numberNextScenes.set(
          addField(SFINT32, "numberNextScenes", new SFInt32(0), 
                   new SFInt32(0)));
    setFieldFlags(numberNextScenes, FF_STATIC);                          

    m_firstDynamicEventOut = getNumEventOuts();
}

Node *
ProtoVrmlCut::create(Scene *scene)
{ 
    return new NodeVrmlCut(scene, this); 
}

NodeVrmlCut::NodeVrmlCut(Scene *scene, Proto *proto)
  : DynamicFieldsNode(scene, proto)
{
    initialize();
}

void 
NodeVrmlCut::initialize(void)
{
    m_currentScene = -1;
    accountAllSceneLengths();
}

NodeVrmlCut::~NodeVrmlCut()
{
    delete m_proto;
}

char *
NodeVrmlCut::newStartTimeName(void)
{
    int number = 0;
    while (1) {
        char name[1024]; 
        snprintf(name, 1023, "startTime%d", number);
        bool eventOutFound = false;
        for (int i = 0; i < getProto()->getNumEventOuts(); i++)
            if (strcmp(name, getProto()->getEventOut(i)->getName(false)) == 0)
                eventOutFound = true;
        if (!eventOutFound)
            return strdup(name);
        number++; 
    }
}

static bool searchGreatestCycleInterval(Node *node, void *data)
{
    if (node->getType() == VRML_TIME_SENSOR) {
        NodeTimeSensor *timer = (NodeTimeSensor *)node;
        double *cycleInterval = (double *)data;
        if (timer->cycleInterval()->getValue() > *cycleInterval)
            *cycleInterval = timer->cycleInterval()->getValue();
    }
    return true;     
}


void
NodeVrmlCut::setField(int index, FieldValue *value, int cf)
{
    if (index == scenes_Field()) {
        IntArray newValues;
        IntArray deleteValues;
        ((MFieldValue *)value)->getDiff(&newValues, &deleteValues, scenes());
        ProtoVrmlCut *protoVrmlCut = (ProtoVrmlCut *)m_proto;
        int firstDynamicEventOut = protoVrmlCut->getFirstDynamicEventOut();
        for (size_t i = 0; i < newValues.size(); i++) {
            Node *node = ((MFNode *)value)->getValue(newValues[i]);
            if (node->getType() == DUNE_VRML_SCENE) {
                MyString name = "";
                    int event =  newValues[i] + firstDynamicEventOut;
                    if ((event < getProto()->getNumEventOuts()) &&
                        m_scene->isParsing())
                        name += getProto()->getEventOut(event)->getName(false);
                    else {
                        name += newStartTimeName();
                        addEventOut(SFTIME, name);
                        update();
                    }
                int field = getProto()->lookupEventOut(name);
                bool alreadyRouted = false;
                for (SocketList::Iterator *j = this->getOutput(field).first(); 
                     j != NULL; j = j->next()) {
                    Node *outputNode = j->item().getNode();
                    if (outputNode == node) {
                        alreadyRouted = true;
                        break;
                    }
                }    
                if ((!alreadyRouted) && (!m_scene->isParsing())) {
                    NodeVrmlScene *vrmlScene = (NodeVrmlScene *)node;
                    m_scene->execute(new RouteCommand(
                         this, getProto()->lookupEventOut(name),
                         vrmlScene, vrmlScene->timeIn_Field()));
                }
                if (m_scene->isParsing() && (!m_scene->getImportIntoVrmlScene()))
                    continue;
                double sceneLength = 0;
                node->doWithBranch(searchGreatestCycleInterval, 
                                   (void *)(&sceneLength));
                if (sceneLengths()->getSize() < ((MFNode *)value)->getSize())
                    sceneLengths()->insertSFValue(newValues[i], sceneLength);
                else
                    sceneLengths()->setValue(newValues[i], sceneLength);
            }            
        }

        for (size_t i = 0; i < deleteValues.size(); i++) {
            int index = deleteValues[i];
            int eventOutIndex = firstDynamicEventOut + index;
            sceneLengths()->removeSFValue(index);
            if (eventOutIndex >= getProto()->getNumEventOuts())
                continue;
            deleteEventOut(eventOutIndex);
            update();
        }
    } else if (index == sceneLengths_Field())
        accountAllSceneLengths();
    Node::setField(index, value, cf);
    update();
}
 
// since dynamic nodes can have dynamically-added fields, we need to update
// the node's fields after the node is defined.
//
// It is important, that only one item (field, eventIn or eventOut) can
// be added or deleted during this call of update.

void
NodeVrmlCut::update()
{
    updateDynamicFields();
}

void
NodeVrmlCut::updateTime(void)
{
    int oldScene = m_currentScene;
    accountWhich();
    if (m_currentScene != oldScene)
        m_scene->UpdateViews(NULL, UPDATE_REDRAW);
}

void
NodeVrmlCut::updateCycleInterval(Node *vrmlScene, SFTime *interval)
{
    if ((!m_scene->isParsing()) || m_scene->getImportIntoVrmlScene())
        for (int i = 0; i < scenes()->getSize(); i++)
            if (scenes()->getValue(i) == vrmlScene)
                if (interval->getValue() > sceneLengths()->getValue(i))
                    sceneLengths()->setValue(i, interval->getValue());
}

void 
NodeVrmlCut::startNextScene(int currentScene, double time)
{
     m_currentScene = currentScene;
     if (currentScene >= scenes()->getSize())
         return;
     Node *node = scenes()->getValue(currentScene);
     if (node->getType() == DUNE_VRML_SCENE) {
         NodeVrmlScene *vrmlScene = (NodeVrmlScene *)node;
         int field = vrmlScene->timeOut_Field();
         for (SocketList::Iterator *j = vrmlScene->getOutput(field).first(); 
              j != NULL; j = j->next()) {
             Node *outputNode = j->item().getNode();
             if (outputNode == NULL)
                 break;
             if (outputNode->getType() == VRML_TIME_SENSOR) {
                 NodeTimeSensor *timer = (NodeTimeSensor *)outputNode;
                 timer->checkStart(true, time, 
                                   time + timer->cycleInterval()->getValue(),
                                   time);
             }
         }             

         field = vrmlScene->bindOut_Field();
         SocketList::Iterator *iter = vrmlScene->getOutput(field).first(); 
         if (iter != NULL) {
             Node *outputNode = iter->item().getNode();
             if (outputNode != NULL) {
                 if (outputNode->getType() == VRML_VIEWPOINT)
                     m_scene->setCamera((NodeViewpoint *)outputNode);
                 if (outputNode->getType() == VRML_FOG)
                     m_scene->setFog(outputNode);
                 if (outputNode->getType() == VRML_BACKGROUND)
                     m_scene->setBackground(outputNode);
             }
         }
     }    
}

int 
NodeVrmlCut::accountWhich()
{
    static int currentScene = -1;
    static double firstTime = 0;
    static bool running = false;
    NodeList *choiceList = scenes()->getValues();
    static int firstScene = 0;
    static int lastScene = 0;
 
    int which = sceneNumber()->getValue();
    if (m_scene->isRunning()) {
        // start Scenes
        firstScene = sceneNumber()->getValue() - 
                     numberPreviousScenes()->getValue();
        if (firstScene < 0)
            firstScene = 0;
        lastScene = sceneNumber()->getValue() + numberNextScenes()->getValue();
        if (lastScene >= sceneLengths()->getSize())
            lastScene = sceneLengths()->getSize() - 1;
        double t = m_scene->getCurrentTime();
        if (sceneNumber()->getValue() < 0) {
            firstScene = 0;
            lastScene = sceneLengths()->getSize() - 1;
        }            
        if (!running) {
            currentScene = firstScene;
            firstTime = t;
            running = true;
            startNextScene(currentScene, t);
        }

        if (currentScene > lastScene) {
            firstTime = t;
            currentScene = firstScene;
            startNextScene(currentScene, t);
        }
        if ((t) >= 
            (firstTime + sceneLengths()->getValue(currentScene))) {
            firstTime = t;
            currentScene++;
            if (currentScene > lastScene)
                currentScene = firstScene;
            startNextScene(currentScene, t);
        }
        which = currentScene;
    } else 
        running = false;
  
    int whichCount = 0;
    for (size_t i = 0; i < choiceList->size(); i++)
        if (choiceList->get(i)->getType() != VRML_COMMENT) {
            if (whichCount == which)
                return i;
            else
                whichCount++;
        }
    return which;
}

void
NodeVrmlCut::accountAllSceneLengths(void)
{
    m_allSceneLengths = 0;
    for (int i = 0; i < sceneLengths()->getSize(); i++)
        m_allSceneLengths += sceneLengths()->getValue(i);
}

void
NodeVrmlCut::preDraw()
{
    NodeList *choiceList = scenes()->getValues();

    int which = accountWhich();

    if (which < 0 || which >= (int)choiceList->size()) return;

    choiceList->get(which)->preDraw();
}

int
NodeVrmlCut::writeRib(int filedes, int indent)
{
    NodeList *choiceList = scenes()->getValues();

    int which = accountWhich();

    if (which < 0 || which >= (int)choiceList->size()) return 0;

    choiceList->get(which)->writeRib(filedes, indent);

    return 0; 
}

void
NodeVrmlCut::draw(int pass)
{
    NodeList *choiceList = scenes()->getValues();

    int which = accountWhich();

    if (which < 0 || which >= (int)choiceList->size()) return;

    glPushName(scenes_Field());
    glPushName(which);
    choiceList->get(which)->draw(pass);
    glPopName();
    glPopName();
}

int 
NodeVrmlCut::writeJavaScript(int f) 
{ 
    RET_ONERROR( mywritestr(f, "      function initialize()\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         nextScene=-1;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         firstScene=sceneNumber-numberPreviousScenes;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         if (firstScene<0)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "             firstScene=0;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         currentScene=firstScene;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         lastScene=sceneNumber+numberNextScenes;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         if (lastScene>=sceneLengths.length)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            lastScene=sceneLengths.length-1;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         if (sceneNumber<0)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            firstScene=0;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            lastScene=sceneLengths.length-1;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            }            \n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         bind_out = true\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      function changeScene(value)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         whichScene_out=currentScene;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         nextScene=currentScene;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         nextTime=value;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      function startScene(value)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         {\n") )
    TheApp->incSelectionLinenumber();
    int numScenes = 0;
    for (int i = 0; i < getProto()->getNumEventOuts(); i++) {
        const char *name = getProto()->getEventOut(i)->getName(false); 
        if (stringncmp(name, "startTime") == 0) {
            RET_ONERROR( mywritef(f, "         if (nextScene==%d)\n", 
                                  numScenes) )
            TheApp->incSelectionLinenumber();
            RET_ONERROR( mywritef(f, "            %s=value;\n", name) )
            TheApp->incSelectionLinenumber();
            numScenes++;
        }
    } 
    RET_ONERROR( mywritestr(f, "         }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      function time_in(value, time)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            if (nextScene!=-1)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            startScene(nextTime);\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            nextScene=-1;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         if ((firstTime==0) || (currentScene>lastScene))\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            firstTime=value;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            currentScene=firstScene;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            changeScene(value);\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         if (value>=firstTime+sceneLengths[currentScene])\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            firstTime=value;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            currentScene++;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            changeScene(value);\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "            }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         }\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int
NodeVrmlCut::writeXmlProto(int f)
{
    RET_ONERROR( mywritestr(f, "<ProtoDeclare name='VrmlCut' >\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  <ProtoInterface>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='sceneLengths' type='MFFloat' accessType='initializeOnly' value='' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='sceneNumber' type='SFInt32' accessType='initializeOnly' value='-1' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='numberPreviousScenes' type='SFInt32' accessType='initializeOnly' value='0' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='numberNextScenes' type='SFInt32' accessType='initializeOnly' value='0' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='time_in' type='SFTime' accessType='inputOnly'/>\n") )
    TheApp->incSelectionLinenumber();
    for (int i = 0;
         i < m_numEventOuts - ((ProtoVrmlCut *)m_proto)->getFirstDynamicEventOut(); i++) {
        RET_ONERROR( mywritef(f, "    <field name='startTime%d' type='SFTime' accessType='outputOnly'/>\n", i) )
        TheApp->incSelectionLinenumber();
    }
    RET_ONERROR( mywritestr(f, "    <field name='scenes' type='MFNode' accessType='inputOutput'>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    </field>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  </ProtoInterface>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  <ProtoBody>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <Switch DEF='VrmlCutSwitch'>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <IS>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='children' protoField='scenes' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    </IS>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    </Switch>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <Script DEF='VrmlCutScript'  >\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='sceneLengths' type='MFFloat' accessType='initializeOnly' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='sceneNumber' type='SFInt32' accessType='initializeOnly' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='numberPreviousScenes' type='SFInt32' accessType='initializeOnly' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='numberNextScenes' type='SFInt32' accessType='initializeOnly' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='firstTime' type='SFTime' accessType='initializeOnly' value='0' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='currentScene' type='SFInt32' accessType='initializeOnly' value='0' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='firstScene' type='SFInt32' accessType='initializeOnly' value='0' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='lastScene' type='SFInt32' accessType='initializeOnly' value='0' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='nextScene' type='SFInt32' accessType='initializeOnly' value='-1' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='nextTime' type='SFTime' accessType='initializeOnly' value='0' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='time_in' type='SFTime' accessType='inputOnly'/>\n") )
    TheApp->incSelectionLinenumber();
    for (int i = 0; i < getProto()->getNumEventOuts(); i++) {
        const char *name = getProto()->getEventOut(i)->getName(false); 
        if (stringncmp(name, "startTime") == 0) {
            RET_ONERROR( mywritef(f, "    <field name='%s' type='SFTime' ",
                                  name) )
            RET_ONERROR( mywritestr(f, "accessType='outputOnly'/>\n") )
            TheApp->incSelectionLinenumber();
        }
    } 
    RET_ONERROR( mywritestr(f, "    <field name='whichScene_out' type='SFInt32' accessType='outputOnly'/>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='bind_out' type='SFBool' accessType='outputOnly'/>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <IS>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='sceneLengths' protoField='sceneLengths' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='sceneNumber' protoField='sceneNumber' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='numberPreviousScenes' protoField='numberPreviousScenes' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='numberNextScenes' protoField='numberNextScenes' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='time_in' protoField='time_in' />\n") )
    TheApp->incSelectionLinenumber();
    for (int i = 0; i < getProto()->getNumEventOuts(); i++) {
        const char *name = getProto()->getEventOut(i)->getName(false); 
        if (stringncmp(name, "startTime") == 0) {
            RET_ONERROR( mywritef(f, "      <connect nodeField='%s'", name) )
            RET_ONERROR( mywritef(f, " protoField='%s' />\n", name) )
            TheApp->incSelectionLinenumber();
        }
    } 
    RET_ONERROR( mywritestr(f, "    </IS>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <![CDATA[ecmascript:\n") )
    TheApp->incSelectionLinenumber();

    RET_ONERROR( writeJavaScript(f) )

    RET_ONERROR( mywritestr(f, "      \n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    ]]>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  </Script>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  <TimeSensor DEF='VrmlCutTimeSensor'  loop='true'/>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  <ROUTE fromNode='VrmlCutScript' fromField='whichScene_out' toNode='VrmlCutSwitch' toField='whichChoice'></ROUTE>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  <ROUTE fromNode='VrmlCutScript' fromField='bind_out' toNode='VrmlCutNavigationInfo' toField='set_bind'></ROUTE>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  <ROUTE fromNode='VrmlCutTimeSensor' fromField='time' toNode='VrmlCutScript' toField='time_in'></ROUTE>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  </ProtoBody>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "</ProtoDeclare>\n\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  <NavigationInfo DEF='VrmlCutNavigationInfo' transitionType='\"TELEPORT\"'/>\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}

int
NodeVrmlCut::writeProto(int f)
{
    if (m_scene->isX3dXml())
        return writeXmlProto(f);

    const char* nodeName = m_proto->getName(m_scene->isX3d());
    RET_ONERROR( mywritef(f, "PROTO %s [\n", nodeName) )    
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeProtoArguments(f) )
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " MFFloat sceneLengths \n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    [\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " SFInt32 sceneNumber -1\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " SFInt32 numberPreviousScenes 0\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " SFInt32 numberNextScenes 0\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeEventInStr(f) )
    RET_ONERROR( mywritestr(f, " SFTime time_in\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeExposedFieldStr(f) )
    RET_ONERROR( mywritestr(f, " MFNode scenes \n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    [\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "{\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "DEF VrmlCutSwitch Switch \n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    if (m_scene->isX3d())
        RET_ONERROR( mywritestr(f, "children") )
    else
        RET_ONERROR( mywritestr(f, "choice") )
    RET_ONERROR( mywritestr(f, " IS scenes\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "DEF VrmlCutScript Script \n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  {\n") )
    TheApp->incSelectionLinenumber();
    for (int i = 0; i < getProto()->getNumEventOuts(); i++) {
        const char *name = getProto()->getEventOut(i)->getName(false); 
        if (stringncmp(name, "startTime") == 0) {
            RET_ONERROR( mywritef(f, "  ") )
            RET_ONERROR( writeEventOutStr(f) )
            RET_ONERROR( mywritef(f, " SFTime %s IS %s\n", name, name) )
            TheApp->incSelectionLinenumber();
        }
    } 
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeEventInStr(f) )
    RET_ONERROR( mywritestr(f, " SFTime time_in IS time_in\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeEventOutStr(f) )
    RET_ONERROR( mywritestr(f, " SFInt32 whichScene_out\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeEventOutStr(f) )
    RET_ONERROR( mywritestr(f, " SFBool bind_out\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " MFFloat sceneLengths IS sceneLengths\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " SFInt32 sceneNumber IS sceneNumber\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " SFInt32 numberPreviousScenes IS numberPreviousScenes\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " SFInt32 numberNextScenes IS numberNextScenes\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " SFTime firstTime 0\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " SFInt32 currentScene 0\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " SFInt32 firstScene 0\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " SFInt32 lastScene 0\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " SFInt32 nextScene -1\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " SFTime nextTime 0\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  url     \"javascript:\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();

    RET_ONERROR( writeJavaScript(f) )

    RET_ONERROR( mywritestr(f, "      \"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "DEF VrmlCutTimeSensor TimeSensor \n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  loop TRUE\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "ROUTE VrmlCutTimeSensor.time TO VrmlCutScript.time_in\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "ROUTE VrmlCutScript.whichScene_out TO VrmlCutSwitch.set_whichChoice\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "}\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "NavigationInfo {\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, " transitionType \"TELEPORT\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "}\n") )    
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    return(0);
}

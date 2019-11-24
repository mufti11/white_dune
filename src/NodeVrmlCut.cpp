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
#include "Path.h"
#include "ExposedField.h"
#include "Field.h"
#include "EventIn.h"
#include "EventOut.h"
#include "DuneApp.h"
#include "RouteCommand.h"
#include "UnRouteCommand.h"
#include "NodeTimeSensor.h"
#include "NodeViewpoint.h"
#include "NodeBackground.h"
#include "NodeCurveAnimation.h"
#include "NodePositionInterpolator.h"
#include "NodeOrientationInterpolator.h"
#include "NodeVrmlScene.h"

ProtoVrmlCut::ProtoVrmlCut(Scene *scene)
  : Proto(scene, "VrmlCut")
{
    sceneLengths.set(
          addField(MFTIME, "sceneLengths", new MFTime(), new SFTime(0)));
    setFieldFlags(sceneLengths, FF_STATIC);
    sceneDelays.set(
          addField(MFTIME, "sceneDelays", new MFTime(), new SFTime(0)));
    setFieldFlags(sceneDelays, FF_STATIC);
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
    m_firstScene = 0;
    m_lastScene = 0;
    m_currentScene = -1;
    m_selectionWhich = -1;
    m_eventOutsInitialised = true;
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
        for (long i = 0; i < ((MFNode *)value)->getSize(); i++) {
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
                for (SocketList::Iterator *j = this->getOutput(field).first(); 
                     j != NULL; j = j->next()) {
                    Node *outputNode = j->item().getNode();
                    if (outputNode == node) {
                        break;
                    }
                }    
                NodeVrmlScene *vrmlScene = (NodeVrmlScene *)node;
                m_scene->execute(new RouteCommand(
                     this, getProto()->lookupEventOut(name),
                     vrmlScene, vrmlScene->timeIn_Field()));
                if (m_scene->isParsing() && 
                    (!m_scene->getImportIntoVrmlScene()))
                    continue;
                if (sceneLengths()->getValue(i) == 0) {
                    double sceneLength = 0;
                    node->doWithBranch(searchGreatestCycleInterval, 
                                       (void *)(&sceneLength));
                    if (sceneLength == 0)
                        sceneLength = TheApp->getDefaultSceneLength();
                    sceneLengths()->setValue(i, sceneLength);
                }
            }            
            m_eventOutsInitialised = false;
            Proto *vrmlCutProto = getProto();;
            int numEventOuts = vrmlCutProto->getNumEventOuts();
            for (int i = numEventOuts - 1; i > -1; i--) {
                 EventOut *eventOut = vrmlCutProto->getEventOut(i);
                 if (stringncmp(eventOut->getName(false), "startTime") == 0)
                     vrmlCutProto->deleteEventOut(i);
        }

        }

        for (long i = 0; i < deleteValues.size(); i++) {
            int index = deleteValues[i];
            int eventOutIndex = firstDynamicEventOut + index;
            sceneLengths()->removeSFValue(index);
            if (eventOutIndex >= getProto()->getNumEventOuts())
                continue;
            deleteEventOut(eventOutIndex);
            update();
        }
    }
    Node::setField(index, value, cf);
    if (index == sceneLengths_Field())
        accountAllSceneLengths();
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
                                   time + timer->cycleInterval()->getValue() +
                                   sceneDelays()->getValue(currentScene),
                                   time, 
                                   sceneDelays()->getValue(currentScene));
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

static Node *vrmlScene = NULL;

static bool searchVrmlScene(Node *node, void *data)
{
    Node *selection = (Node *)data; 
    if ((node != NULL) && node->getType() == DUNE_VRML_SCENE) {
        vrmlScene = node;
    }
    if (node == selection)
        return false;
    return true;     
}

int 
NodeVrmlCut::getVrmlSceneWhich(void)
{
    int which = -1;
    Node *selection = m_scene->getSelection()->getNode();
    vrmlScene = NULL;
    getScene()->getRoot()->doWithBranch(searchVrmlScene, selection, false); 
    if (vrmlScene && vrmlScene->getType() == DUNE_VRML_SCENE) {
        for (int i = 0; i < scenes()->getSize(); i++)
             if (vrmlScene == scenes()->getValue(i)) {
                 which = i;
                 break;
              }
    }
    return which;
}

void
NodeVrmlCut::updateSelection(void)
{
    m_selectionWhich = getVrmlSceneWhich();
}

int 
NodeVrmlCut::accountWhich(void)
{
    static int currentScene = -1;
    static double firstTime = 0;
    static bool running = false;
    NodeList *choiceList = scenes()->getValues();
    static int firstScene = 0;
    static int lastScene = 0;

    int vrmlSceneWhich = m_selectionWhich; 

    int which = sceneNumber()->getValue();
    if (m_scene->isRunning()) {
        // start Scenes
        firstScene = vrmlSceneWhich == -1 ? sceneNumber()->getValue() - 
                     numberPreviousScenes()->getValue() : vrmlSceneWhich;
        if (firstScene < 0)
            firstScene = 0;
        lastScene = vrmlSceneWhich == -1 ? sceneNumber()->getValue() + 
                    numberNextScenes()->getValue() : vrmlSceneWhich;
        if (lastScene >= sceneLengths()->getSize())
            lastScene = sceneLengths()->getSize() - 1;
        double t = m_scene->getCurrentTime();
        if ((vrmlSceneWhich == -1) && (sceneNumber()->getValue() < 0)) {
            firstScene = 0;
            lastScene = vrmlSceneWhich == -1 ? sceneLengths()->getSize() - 1 :
                        vrmlSceneWhich;
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
            startNextScene(currentScene, 
                           t + sceneLengths()->getValue(currentScene) +
                           2 * sceneDelays()->getValue(currentScene));
        }
        if ((t) >= (firstTime + sceneLengths()->getValue(currentScene))) {
            firstTime = t;
            currentScene++;
            if (currentScene > lastScene)
                currentScene = firstScene;
            startNextScene(currentScene, t);
        }
        which = currentScene;
    } else
        which = vrmlSceneWhich;

    int whichCount = 0;
    for (long i = 0; i < choiceList->size(); i++)
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

    if (which == -1)
        which = 0; 

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

    if (which == -1)
        which = 0; 

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
    RET_ONERROR( mywritestr(f, "            }\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "         bind_out = true;\n") )
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
    Proto *vrmlCutProto = getProto();
    for (int i = 0; i < vrmlCutProto->getNumEventOuts(); i++) {
        const char *name = vrmlCutProto->getEventOut(i)->getName(false); 
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
    RET_ONERROR( mywritestr(f, "         if (value>=firstTime+") )
    RET_ONERROR( mywritestr(f, "sceneLengths[currentScene])\n") )
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
    RET_ONERROR( mywritestr(f, "    <field name='sceneDelays' type='MFFloat' accessType='initializeOnly' value='' />\n") )
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
    RET_ONERROR( mywritestr(f, "      <connect nodeField='choice' protoField='scenes' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    </IS>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    </Switch>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <Script DEF='VrmlCutScript'  >\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='sceneLengths' type='MFFloat' accessType='initializeOnly' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    <field name='sceneDelays' type='MFFloat' accessType='initializeOnly' />\n") )
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
    Proto *vrmlCutProto = getProto();
    for (int i = 0; i < vrmlCutProto->getNumEventOuts(); i++) {
        const char *name = vrmlCutProto->getEventOut(i)->getName(false); 
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
    RET_ONERROR( mywritestr(f, "      <connect nodeField='sceneDelays' protoField='sceneDelays' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='sceneNumber' protoField='sceneNumber' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='numberPreviousScenes' protoField='numberPreviousScenes' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='numberNextScenes' protoField='numberNextScenes' />\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "      <connect nodeField='time_in' protoField='time_in' />\n") )
    TheApp->incSelectionLinenumber();
    for (int i = 0; i < vrmlCutProto->getNumEventOuts(); i++) {
        const char *name = vrmlCutProto->getEventOut(i)->getName(false); 
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
//    RET_ONERROR( mywritestr(f, "  <ROUTE fromNode='VrmlCutScript' fromField='bind_out' toNode='VrmlCutNavigationInfo' toField='set_bind'></ROUTE>\n") )
//    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  <ROUTE fromNode='VrmlCutTimeSensor' fromField='time' toNode='VrmlCutScript' toField='time_in'></ROUTE>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  </ProtoBody>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "</ProtoDeclare>\n\n") )
    TheApp->incSelectionLinenumber();
//    RET_ONERROR( mywritestr(f, "  <NavigationInfo DEF='VrmlCutNavigationInfo' transitionType='\"TELEPORT\"'/>\n") )
//    TheApp->incSelectionLinenumber();
    return 0;
}

int
NodeVrmlCut::writeProto(int f)
{
//    if (m_scene->isX3dom())
//        return 0;
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
    TheApp->incSelectionLinenumber();
    RET_ONERROR( writeFieldStr(f) )
    RET_ONERROR( mywritestr(f, " MFFloat sceneDelays \n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    [\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "    ]\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "  ") )
    TheApp->incSelectionLinenumber();
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
    RET_ONERROR( mywritestr(f, "]\n") )
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
    Proto *vrmlCutProto = getProto();
    for (int i = 0; i < vrmlCutProto->getNumEventOuts(); i++) {
        const char *name = vrmlCutProto->getEventOut(i)->getName(false); 
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
    RET_ONERROR( mywritestr(f, " MFFloat sceneDelays IS sceneDelays\n") )
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
    RET_ONERROR( mywritestr(f, " transitionType \"TELEPORT\"\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "}\n") )    
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    return(0);
}

class X3domSendEventData {
public:
    int f;
    int indent;
    MyArray<Node *>curveAnimations;
};

static X3domSendEventData data;

static bool writeX3domSendEvent(Node *node, void *data)
{
    X3domSendEventData *sendData = (X3domSendEventData *)data;
    int f = sendData->f;
    if (node->getType() == VRML_VIEWPOINT) {
        indentf(f, sendData->indent + TheApp->GetIndent());
        mywritef(f, "document.getElementById(\"%s\")", node->getX3domId());
        mywritestr(f, ".setAttribute('bind', true);\n");
    }
    if (node->getType() == VRML_BACKGROUND) {
        if (!node->hasName())
            node->getScene()->generateUniqueNodeName(node);
        indentf(f, sendData->indent + TheApp->GetIndent());
        mywritef(f, "document.getElementById(\"%s\")", node->getX3domId());
        mywritestr(f, ".setAttribute('bind', true);\n");
    }
    if (node->getType() == VRML_FOG) {
        if (!node->hasName())
            node->getScene()->generateUniqueNodeName(node);
        indentf(f, sendData->indent + TheApp->GetIndent());
        mywritef(f, "document.getElementById(\"%s\")", node->getX3domId());
        mywritestr(f, ".setAttribute('bind', true);\n");
    }
    if (node->getType() == VRML_TIME_SENSOR) {
        indentf(f, sendData->indent + TheApp->GetIndent());
        mywritef(f, "document.getElementById(\"%s\")", node->getX3domId());
        mywritestr(f, ".setAttribute('startTime', value.toString());\n");
    }
    if (node->getType() == DUNE_CURVE_ANIMATION) {
        sendData->curveAnimations.append(node);
    }
    return true;
}

int
NodeVrmlCut::writeX3domScript(int f, int indent)
{
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "<Script>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "var firstTime;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritef(f, "var numberScenes = %d;\n", 
                          scenes()->getSize()) )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "var firstScene = 0;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritef(f, "var lastScene = %d;\n", 
                          scenes()->getSize() - 1) )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "var currentScene = firstScene;\n") )
    TheApp->incSelectionLinenumber();

    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "var sceneLengths = [") )
    for (long i = 0; i < sceneLengths()->getSize(); i++) { 
         RET_ONERROR( mywritef(f, "%f", sceneLengths()->getValue(i)) )
         if (i < sceneLengths()->getSize() - 1)
             RET_ONERROR( mywritestr(f, ", ") )
    }
    RET_ONERROR( mywritestr(f, "];\n") )
    TheApp->incSelectionLinenumber();

    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "var sceneDelays = [") )
    for (long i = 0; i < sceneLengths()->getSize(); i++) { 
         RET_ONERROR( mywritef(f, "%f", sceneDelays()->getValue(i)) )
         if (i < sceneLengths()->getSize() - 1)
             RET_ONERROR( mywritestr(f, ", ") )
    }
    RET_ONERROR( mywritestr(f, "];\n") )
    TheApp->incSelectionLinenumber();

    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "function vrmlCutInit(eventObject)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "{\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "if (eventObject.type != \"outputchange\"") )
    RET_ONERROR( mywritestr(f, " || eventObject.fieldName != ") )
    RET_ONERROR( mywritestr(f, "\"time\")\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + 2 * TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "return;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "var value = eventObject.value;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "firstTime = 0;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "document.getElementById(") )
    RET_ONERROR( mywritestr(f, "\"VrmlCutInitTimeSensor\")") )
    RET_ONERROR( mywritestr(f, ".setAttribute('stoptime', value);\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "}\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "function changeScene(value)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "{\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "document.getElementById(") )
    RET_ONERROR( mywritestr(f, "\"VrmlCutSwitch\").") )
    RET_ONERROR( mywritestr(f, "setAttribute('whichChoice',") )
    RET_ONERROR( mywritestr(f, " currentScene);\n") )
    TheApp->incSelectionLinenumber();

    data.curveAnimations.resize(0);
    for (long i = 0; i < scenes()->getSize(); i++) {
        indentf(f, indent + TheApp->GetIndent());
        mywritef(f, "if (currentScene == %d) {\n", i);
        data.f = f;
        data.indent = indent + TheApp->GetIndent();
        scenes()->getValue(i)->doWithBranch(writeX3domSendEvent, &data, false);
        indentf(f, indent + TheApp->GetIndent());
        mywritestr(f, "}\n");
    }

    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "}\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "function time_in(value)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "{\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "if ((firstTime == 0) || ") )
    RET_ONERROR( mywritestr(f, "(currentScene > lastScene))\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "{\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + 2 * TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "firstTime = value;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + 2 * TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "currentScene = firstScene;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + 2 * TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "changeScene(value);\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "}\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "if (value >=firstTime + ") )
    RET_ONERROR( mywritestr(f, "sceneLengths[currentScene])\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "{\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + 2 * TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "firstTime = value;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + 2 * TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "currentScene++;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + 2 * TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "changeScene(value);\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "}\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "}\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "function getTime(eventObject)\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "{\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "if (eventObject.type != \"outputchange\"") )
    RET_ONERROR( mywritestr(f, "|| eventObject.fieldName != \"time\")\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + 2 * TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "return;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "var value = eventObject.value;\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
    RET_ONERROR( mywritestr(f, "time_in(value);\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "}\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( indentf(f, indent) )
    RET_ONERROR( mywritestr(f, "</Script>\n") )
    TheApp->incSelectionLinenumber();
    RET_ONERROR( mywritestr(f, "\n") )
    TheApp->incSelectionLinenumber();
    return 0;
}


int
NodeVrmlCut::writeXml(int f, int indent, int containerField, bool avoidUse)
{
    writeXmlProto(f);
    if (m_scene->getWriteFlags() && X3DOM) {
        RET_ONERROR( indentf(f, indent ) )
        RET_ONERROR( mywritestr(f, "<TimeSensor DEF='VrmlCutTimeSensor' ") )
        RET_ONERROR( mywritestr(f, "id='VrmlCutTimeSensor' ") )
        RET_ONERROR( mywritestr(f, "OnOutputChange='getTime(event)'") )
        RET_ONERROR( mywritestr(f, " loop='true' >\n") )
        TheApp->incSelectionLinenumber();
        RET_ONERROR( indentf(f, indent ) )
        RET_ONERROR( mywritestr(f, "</TimeSensor>\n") )
        TheApp->incSelectionLinenumber();
        RET_ONERROR( indentf(f, indent ) )
        RET_ONERROR( mywritestr(f, "<TimeSensor DEF='VrmlCutInitTimeSensor' ") )
        RET_ONERROR( mywritestr(f, "id='VrmlCutInitTimeSensor' ") )
        RET_ONERROR( mywritestr(f, "OnOutputChange='vrmlCutInit(event)' ") )
        RET_ONERROR( mywritestr(f, "loop='true' >\n") )
        TheApp->incSelectionLinenumber();
        RET_ONERROR( indentf(f, indent ) )
        RET_ONERROR( mywritestr(f, "</TimeSensor>\n") )
        TheApp->incSelectionLinenumber();
        RET_ONERROR( indentf(f, indent ) )
        RET_ONERROR( mywritestr(f, "<NavigationInfo ") )
        RET_ONERROR( mywritestr(f, "DEF='VrmlCutNavigationInfo' ") ) 
        RET_ONERROR( mywritestr(f, "id='VrmlCutNavigationInfo' ") )
        RET_ONERROR( mywritestr(f, "transitionType='TELEPORT' >\n") )
        TheApp->incSelectionLinenumber();
        RET_ONERROR( indentf(f, indent ) )
        RET_ONERROR( mywritestr(f, "</NavigationInfo>\n") )
        TheApp->incSelectionLinenumber();
        RET_ONERROR( indentf(f, indent ) )
        RET_ONERROR( mywritestr(f, "\n") )
        TheApp->incSelectionLinenumber();
        RET_ONERROR( indentf(f, indent ) )
        RET_ONERROR( mywritestr(f, "<Switch DEF='VrmlCutSwitch' ") )
        RET_ONERROR( mywritestr(f, "id='VrmlCutSwitch'  whichChoice='-1' >\n") )
        TheApp->incSelectionLinenumber();

        for (long i = 0; i < scenes()->getSize(); i++) { 
             RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
             RET_ONERROR( mywritestr(f, "<Group >\n") )
             TheApp->incSelectionLinenumber();

             NodeVrmlScene *vrmlScene = (NodeVrmlScene *)scenes()->getValue(i);
             for (long j = 0; j < vrmlScene->children()->getSize(); j++)
                  vrmlScene->children()->getValue(j)->writeXml(f,
                      indent + 2 * TheApp->GetIndent());        

             RET_ONERROR( indentf(f, indent + TheApp->GetIndent()) )
             RET_ONERROR( mywritestr(f, "</Group>\n") )
             TheApp->incSelectionLinenumber();
        }
        RET_ONERROR( indentf(f, indent) )
        RET_ONERROR( mywritestr(f, "</Switch>\n") )
        TheApp->incSelectionLinenumber();
        for (int n = 0; n < data.curveAnimations.size(); n++) {
            NodeCurveAnimation *anim = (NodeCurveAnimation *)
                                       data.curveAnimations[n];
            NodePositionInterpolator *posInter = 
                (NodePositionInterpolator *)
                anim->getPositionInterpolator();

            Proto *outProto = posInter->getProto();
            int eventOuts = outProto->getNumEventOuts();
            for (int i = 0; i < eventOuts; i++) {
                for (SocketList::Iterator *j = posInter->getOutput(i).first();
                     j != NULL; j = j->next()) {
                    RouteSocket s = j->item();
                    RET_ONERROR( indentf(f, indent) )
                    RET_ONERROR( mywritestr(f, "<ROUTE ") )
                    RET_ONERROR( mywritef(f, " fromNode='%s' ", 
                                          (const char *)
                                          posInter->getName()) )
                    EventOut *outEvent = outProto->getEventOut(i);
                    RET_ONERROR( mywritef(f, " fromField='%s' ", 
                                          (const char *)
                                          outEvent->getName(true)) )
                    RET_ONERROR( mywritef(f, " toNode='%s' ", 
                                          (const char *)
                                          s.getNode()->getName()) )
                    Field *outField = s.getNode()->getProto()->getField(
                                           s.getField());
                    RET_ONERROR( mywritef(f, " toField='%s' ", 
                                          (const char *)
                                          outField->getName(true)) )
                    RET_ONERROR( mywritestr(f, "></ROUTE>") )
                }
            }
            NodeOrientationInterpolator *orientInter = 
                (NodeOrientationInterpolator *) 
                anim->getOrientationInterpolator();
            outProto = orientInter->getProto();
            eventOuts = outProto->getNumEventOuts();
            for (int i = 0; i < eventOuts; i++) {
                for (SocketList::Iterator *j = orientInter->getOutput(i).first();
                     j != NULL; j = j->next()) {
                    RouteSocket s = j->item();
                    RET_ONERROR( indentf(f, indent) )
                    RET_ONERROR( mywritestr(f, "<ROUTE ") )
                    RET_ONERROR( mywritef(f, " fromNode='%s' ", 
                                          (const char *)
                                          orientInter->getName()) )
                    EventOut *outEvent = outProto->getEventOut(i);
                    RET_ONERROR( mywritef(f, " fromField='%s' ", 
                                          (const char *)
                                          outEvent->getName(true)) )
                    RET_ONERROR( mywritef(f, " toNode='%s' ", 
                                          (const char *)
                                          s.getNode()->getName()) )
                    Field *outField = s.getNode()->getProto()->getField(
                                           s.getField());
                    RET_ONERROR( mywritef(f, " toField='%s' ", 
                                          (const char *)
                                          outField->getName(true)) )
                    RET_ONERROR( mywritestr(f, "></ROUTE>") )
                }
            }
        }
    } else
        RET_ONERROR( Node::writeXml(f, indent, containerField, avoidUse) )
    return 0;
}
    


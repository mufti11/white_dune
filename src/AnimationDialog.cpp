/*
 * AnimationDialog.cpp
 *
 * Copyright (C) 2003 J. "MUFTI" Scheurich
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
 
#include "AnimationDialog.h"
#include "resource.h"
#include "MyString.h"
#include "FieldValue.h"
#include "SFMFTypes.h"
#include "Field.h"
#include "EventIn.h"
#include "EventOut.h"
#include "ExposedField.h"
#include "Element.h"
#include "DuneApp.h"
#include "NodeTransform.h"
#include "NodeTimeSensor.h"

AnimationDialog::AnimationDialog(SWND parent, Node* oldNode)
  : Dialog(parent, IDD_ANIMATION)
{
    m_window.initCheckBoxWindow(parent, m_dlg);
    m_animationNode = oldNode;
    m_newTimeSensorSeconds = 5;
    buildInterfaceData();
    LoadData();
    m_window.accountYmax();
    m_window.invalidateWindow();
}

void
AnimationDialog::buildInterfaceData(void)
{
    if (!m_window.isValid())
        return;
    Proto* proto = m_animationNode->getProto();
    Scene *scene = m_animationNode->getScene();
    bool x3d = scene->isX3d();
    if (proto == NULL)
        return;
    int index = 0;
    for (int i = 0; i < proto->getNumEventIns(); i++) {
        if (scene->isInvalidElement(proto->getEventIn(i)))
            continue;
        int type = proto->getEventIn(i)->getType();
        if (typeDefaultValue(type)->supportAnimation(x3d)) {
            m_eventInTypes[index] = type;
            m_eventInNames[index] = proto->getEventIn(i)->getName(x3d);
            m_window.setString(index, m_eventInNames[index]);
            m_eventInFields[index] = i;
            index++; 
        }
    }
}

bool
AnimationDialog::Validate()
{
    bool valid = true;

    Proto* proto = m_animationNode->getProto();
    if (!m_window.isValid())
        return false;
    Scene *scene = m_animationNode->getScene();
    bool x3d = scene->isX3d();
    for (int i = 0 ; i < numEventInNames() ; i++) {
        m_eventInIsAnimated[i] = m_window.getChecked(i);
        if (TheApp->is4Kids() && m_eventInIsAnimated[i]) {
            bool eventHandled = false;
            SocketList::Iterator *j;
            int field = m_eventInFields[i];
            for (j = m_animationNode->getInput(field).first(); 
                j != NULL; j = j->next()) {
                Node *inputNode = j->item().getNode();
                if (inputNode->isInterpolator()) {
                    if (eventHandled)
                        break;
                    char str[256], message[256], title[256];
                    swLoadString(IDS_DUNE, title, 256);
                    swLoadString(IDS_EVENT_ALREADY_ANIMATED, str, 256);
                    const char *name = proto->getEventIn(field)->getName(x3d);
                    mysnprintf(message, 255, str, name);
                    swMessageBox(TheApp->mainWnd(), message, title, SW_MB_OK, 
                                 SW_MB_ERROR);
                    valid = false;
                    eventHandled = true;
                }
            }
        }
    }

    m_window.accountYmax();
    m_window.invalidateWindow();

    float loopTime = 0.0f;
    char buf[128];
    swGetText(swGetDialogItem(m_dlg, IDC_TIMESENSOR_SECONDS), buf, 128);
    loopTime = atof(buf);
    if (loopTime <= 0) {
        TheApp->MessageBoxId(IDS_ANIMATE_TIME_LESS_0);        
        return false;
    }

    for (int i = 0 ; i < numEventInNames() ; i++)
        if (m_eventInIsAnimated[i] == true)
            return valid;
    int commentID = m_animationNode->getAnimationCommentID();
    if (commentID != -1)
        TheApp->MessageBoxId(IDS_ANIMATE_WHAT);

    return false;
}

void
AnimationDialog::LoadData()
{
    if (!m_window.isValid())
        return;

    SWND comboTimeSensors = swGetDialogItem(m_dlg, IDC_TIMESENSORS);
    swComboBoxDeleteAll(comboTimeSensors);

    MyArray<MyString> timeSensors;
    timeSensors[0] = "new TimeSensor"; 

    const NodeList *nodes = m_animationNode->getScene()->getNodes();
    for (int i = 0; i < nodes->size(); i++) {
        Node *node = nodes->get(i);
        if (node->isInScene(m_animationNode->getScene()))
            if (node->getType() == VRML_TIME_SENSOR)
                if (node->hasName()) {
                    MyString text = "";
                    text += node->getName();
                    text += " (";
                    NodeTimeSensor *timeSensor = (NodeTimeSensor *)node;
                    text.catFloat(timeSensor->cycleInterval()->getValue());
                    text += " sec)";
                    timeSensors.append(text.copy()); 
                }
    }
    
    for (int i = 0;i < timeSensors.size(); i++)
        swComboBoxAppendItem(comboTimeSensors, timeSensors[i]);

    char buf[128];

    mysnprintf(buf, 128, "%g", m_newTimeSensorSeconds);
    swSetText(swGetDialogItem(m_dlg, IDC_TIMESENSOR_SECONDS), buf);

    for (int i = 0; i < m_eventInNames.size(); i++) {
        m_window.setInitButtonsPressed(i,false);
        if (TheApp->is4Kids())
            if (m_animationNode->getType() == VRML_TRANSFORM) {
                if (strcmp((const char *)m_eventInNames[i], "set_rotation") == 0)
                    m_window.setInitButtonsPressed(i, true);
                if (strcmp((const char *)m_eventInNames[i], "set_translation") 
                    == 0)
                    m_window.setInitButtonsPressed(i, true);
            }
            if (m_animationNode->getType() == VRML_VIEWPOINT) {
                if (strcmp((const char *)m_eventInNames[i], "set_orientation") 
                    == 0)
                    m_window.setInitButtonsPressed(i, true);
                if (strcmp((const char *)m_eventInNames[i], "set_position") 
                    == 0)
                    m_window.setInitButtonsPressed(i, true);
            }
            if (m_animationNode->getType() == VRML_NURBS_SURFACE) {
                if (strcmp((const char *)m_eventInNames[i], "set_controlPoint") 
                    == 0)
                    m_window.setInitButtonsPressed(i, true);
            }
            if (m_animationNode->getType() == VRML_COORDINATE) {
                if (strcmp((const char *)m_eventInNames[i], "set_point") == 0)
                    m_window.setInitButtonsPressed(i, true);
            }
            if (m_animationNode->getType() == VRML_MATERIAL) {
                if (strcmp((const char *)m_eventInNames[i], "set_diffuseColor") 
                    == 0)
                    m_window.setInitButtonsPressed(i, true);
            }
            if (m_animationNode->getType() == DUNE_SUPER_SHAPE) {
                if (strcmp((const char *)m_eventInNames[i], "set_size") == 0)
                    m_window.setInitButtonsPressed(i, true);
            }
       }            
}

void  AnimationDialog::SaveData()
{
    if (!m_window.isValid())
        return;

    m_timeSensor = NULL;
    int sensor = swComboBoxGetSelection(swGetDialogItem(m_dlg, IDC_TIMESENSORS));
    if (sensor > 0) {
        int index = 1;
        const NodeList *nodes = m_animationNode->getScene()->getNodes();
        for (int i = 0; i < nodes->size() && (m_timeSensor == NULL); i++) {
            Node *node = nodes->get(i);
            if (node->isInScene(m_animationNode->getScene()))
                if (node->getType() == VRML_TIME_SENSOR)
                    if (node->hasName())
                        if (index++ == sensor) {
                            m_timeSensor = (NodeTimeSensor *)node;
                            break;
                        }
        }
    }

    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_TIMESENSOR_SECONDS), buf, 128);
    m_newTimeSensorSeconds = atof(buf);

    for (int i = 0 ; i < numEventInNames() ; i++)
        m_eventInIsAnimated[i] = m_window.getChecked(i);
}




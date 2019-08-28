/*
 * InteractionDialog2.cpp
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
 
#include "InteractionDialog2.h"
#include "resource.h"
#include "FieldValue.h"
#include "SFMFTypes.h"
#include "Field.h"
#include "EventIn.h"
#include "EventOut.h"
#include "ExposedField.h"
#include "Element.h"
#include "DuneApp.h"
#include "NodeTransform.h"

InteractionDialog2::InteractionDialog2(SWND parent, Scene *scene,
                                       InteractionRouteData *routeData)
  : Dialog(parent, IDD_INTERACTION2)
{
    m_window.initCheckBoxWindow(parent, m_dlg);
    m_scene = scene;
    m_routeData = routeData;
    buildInterfaceData();
    LoadData();
    m_window.accountYmax();
    m_window.invalidateWindow();
}

void
InteractionDialog2::buildInterfaceData(void)
{
    m_protos.resize(0);
    m_window.resize0();
    int index = 0;
    int type = m_routeData->type;
    bool x3d = m_scene->isX3d();
    if (typeDefaultValue(type)->supportInteraction()) {
        int j;
        for (j = 0; j < m_scene->getNumInteractiveProtos(type); j++) {
            Proto *outProto = m_scene->getInteractiveProto(type, j);
            bool already = false;
            for (int k = 0; k < outProto->getNumEventOuts(); k++) {
                // when m_level == 0 filter out not recommended fields
                int outFlags = outProto->getEventOut(k)->getFlags();
                if (!(outFlags & EOF_RECOMMENDED))
                    continue;
                if (m_scene->isInvalidElement(outProto->getEventOut(k)))
                    continue;
                if (already)
                    continue;
                if (outProto->getEventOut(k)->getType() == type) {
                    already = true;
                    MyString string = "";
                    string += outProto->getName(x3d);
                    m_window.setString(index, string);
                    m_protos.append(string);
                    index++; 
                }
            }
        }
        for (int i = 0; i < index; i++)
            m_window.setInitButtonsPressed(i, false);
        m_sensors.resize(0);
        m_nodes.resize(0);
        char newSensor[256];
        swLoadString(IDS_INTERACTION_NEW_SENSOR, newSensor, 255); 
        m_sensors.append(newSensor);

        for (j = 0; j < m_scene->getNumInteractiveProtos(type); j++) {
            Proto *outProto = m_scene->getInteractiveProto(type, j);
            const NodeList *nodes = m_scene->getNodes();
            for (size_t k = 0; k < nodes->size(); k++) {
                Node *node = nodes->get(k);
                if (node->isInScene(m_scene))
                      if (strcmp(outProto->getName(x3d), 
                          node->getProto()->getName(x3d)) == 0) {
                          bool already = false;
                          for (int n = 0; n < outProto->getNumEventOuts(); 
                               n++) {
                              // when m_level == 0 filter out not recommended fields
                              int outFlags = outProto->getEventOut(n)->getFlags();
                              if (!(outFlags & EOF_RECOMMENDED))
                                  continue;
                              if (m_scene->isInvalidElement(outProto->getEventOut(n)))
                                  continue;
                              if (already)
                                  continue;
                              if (outProto->getEventOut(n)->getType() == type) {
                                  already = true;
                                  MyString string = "";
                                  string += node->getNameOrNewName();
                                  m_sensors.append(string);
                                  m_nodes.append(node);
                              }
                          }
                      }
            }
        }
    }
}

void
InteractionDialog2::OnCommand(int id)
{
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
#ifndef _WIN32
    } else if (id == IDC_INTERACTION_SENSORS) {
        buildInterfaceData();
        m_window.accountYmax();
        m_window.invalidateWindow();
#endif
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
}

bool
InteractionDialog2::Validate()
{
    int numSensor = swComboBoxGetSelection(swGetDialogItem(m_dlg,
          IDC_INTERACTION_SENSORS));
    if (numSensor <= 0) {
        bool checked = false;
        for (size_t i = 0; i < m_protos.size(); i++)
            if (m_window.getChecked(i)) {
                if (checked) {
                    TheApp->MessageBoxId(IDS_NOT_2_INTERACTIVE);
                    return false;
                }
                checked = true;
            }    
        for (size_t i = 0 ; i < m_protos.size(); i++)
            if (m_window.getChecked(i))
                return true;
        TheApp->MessageBoxId(IDS_MAKE_WHAT_INTERACTIVE);
        return false;
    }
    return true;
}

void
InteractionDialog2::LoadData()
{
    buildInterfaceData();

    SWND comboSensors = swGetDialogItem(m_dlg, IDC_INTERACTION_SENSORS);
    swComboBoxDeleteAll(comboSensors);

    for (size_t i = 0;i < m_sensors.size(); i++)
        swComboBoxAppendItem(comboSensors, m_sensors[i]);
}

void 
InteractionDialog2::SaveData()
{
    int numSensor = swComboBoxGetSelection(swGetDialogItem(m_dlg,
          IDC_INTERACTION_SENSORS));
    if (numSensor <= 0) {
        for (size_t i = 0 ; i < m_protos.size(); i++)
            if (m_window.getChecked(i))
                m_routeData->nodeProtoName = m_protos[i];
    } else
       m_routeData->node = m_nodes[numSensor - 1];
}

void 
InteractionDialog2::drawInterface(SDC dc)
{
    m_window.drawInterface(dc);
}



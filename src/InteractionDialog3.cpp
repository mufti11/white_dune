/*
 * InteractionDialog3.cpp
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
 
#include "InteractionDialog3.h"
#include "InteractionDialog.h"
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

InteractionDialog3::InteractionDialog3(SWND parent, Scene *scene,
                                       InteractionRouteData *routeData)
  : Dialog(parent, IDD_INTERACTION3)
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
InteractionDialog3::buildInterfaceData(void)
{
    Scene *scene = m_scene;
    bool x3d = scene->isX3d();

    m_window.resize0();
    m_eventOuts.resize(0);
    int index = 0;
    if (typeDefaultValue(m_routeData->type)->supportInteraction() &&
        (m_routeData->node != NULL)) {
        Proto *outProto = m_routeData->node->getProto();
        for (int k = 0; k < outProto->getNumEventOuts(); k++) {
            // when level == 0 filter out not recommended fields
            int outFlags = outProto->getEventOut(k)->getFlags();
            if ((m_routeData->level == 0) && !(outFlags & EOF_RECOMMENDED))
                continue;
            if (scene->isInvalidElement(outProto->getEventOut(k)))
                continue;
            if (outProto->getEventOut(k)->getType() == m_routeData->type) {
                MyString string = "";
                string += outProto->getEventOut(k)->getName(x3d);
                m_eventOuts.append(k);
                m_window.setString(index, string);
                m_window.setInitButtonsPressed(index, false);
                index++; 
            }
        }
    }
}

void
InteractionDialog3::OnCommand(int id)
{
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
}

bool
InteractionDialog3::Validate()
{
    bool checked = false;
    for (size_t i = 0 ;i < m_eventOuts.size() ;i++)
        if (m_window.getChecked(i)) {
            if (checked) {
                TheApp->MessageBoxId(IDS_NOT_2_INTERACTIVE);
                return false;
            }
            checked = true;
        }
    for (size_t i = 0 ;i < m_eventOuts.size() ;i++)
        if (m_window.getChecked(i))
            return true;
    if (m_routeData->node == NULL)
         return false;
    int commentID = m_routeData->node->getInteractionCommentID();
    if (commentID != -1) {
        TheApp->MessageBoxId(commentID);
        return false;
    } else
        TheApp->MessageBoxId(IDS_MAKE_WHAT_INTERACTIVE);
    return false;
}

void
InteractionDialog3::LoadData()
{
    buildInterfaceData();
}

void 
InteractionDialog3::SaveData()
{
    for (size_t i = 0; i < m_eventOuts.size() ;i++)
        if (m_window.getChecked(i)) {
            m_routeData->eventOutField = m_eventOuts[i];
            return;
        }
}

void 
InteractionDialog3::drawInterface(SDC dc)
{
    m_window.drawInterface(dc);
}


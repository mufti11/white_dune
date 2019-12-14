/*
 * InteractionDialog.cpp
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

InteractionDialog::InteractionDialog(SWND parent, Node* oldNode, 
                                     InteractionRouteData* routeData)
  : Dialog(parent, IDD_INTERACTION)
{
    m_interactionNode = oldNode;
    m_routeData = routeData;
    buildInterfaceData();
    LoadData();
}

void
InteractionDialog::buildInterfaceData(void)
{
    Scene *scene = m_interactionNode->getScene();
    bool x3d = scene->isX3d();
    Proto* inProto = m_interactionNode->getProto();
    if (inProto == NULL)
        return;

    bool m_eventInHasRecommendedFields = false;
    for (int i = 0; i < inProto->getNumEventIns(); i++)
        if (inProto->getEventIn(i)->getFlags() &
            EIF_RECOMMENDED)
            m_eventInHasRecommendedFields = true;

    int index = 0;
    for (int i = 0; i < inProto->getNumEventIns(); i++) {
        int type = inProto->getEventIn(i)->getType();
        // when level == 0 filter out not recommended fields
        int inFlags = inProto->getEventIn(i)->getFlags();
        if ((m_routeData->level == 0) && m_eventInHasRecommendedFields &&
            !(inFlags & EIF_RECOMMENDED))
            continue;
        if (scene->isInvalidElement(inProto->getEventIn(i)))
            continue;
        bool hasInteractiveEventIn = false;
        if (typeDefaultValue(type)->supportInteraction()) {
            for (int j = 0; j < scene->getNumInteractiveProtos(type); j++) {
                Proto *outProto = scene->getInteractiveProto(type, j);
                for (int k = 0; k < outProto->getNumEventOuts(); k++) {
                    // when m_level == 0 filter out not recommended fields
                    int outFlags = outProto->getEventOut(k)->getFlags();
                    if ((m_routeData->level == 0) && 
                        !(outFlags & EOF_RECOMMENDED))
                        continue;
                    if (scene->isInvalidElement(outProto->getEventOut(k)))
                        continue;
                    if (inProto->getEventIn(i)->getType() == type)
                        hasInteractiveEventIn = true;
                }
            }
        }
        if (hasInteractiveEventIn) {
            m_eventIns.append(i);
            MyString string = "";
            string += inProto->getEventIn(i)->getName(x3d);
            index++; 
        }
    }
}

void
InteractionDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == IDC_INTERACTION_LEVEL) {
        m_routeData->level = swComboBoxGetSelection(swGetDialogItem(m_dlg, 
              IDC_INTERACTION_LEVEL));
        if (m_routeData->level < 0)
            m_routeData->level = 0;
        buildInterfaceData();
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
}

bool
InteractionDialog::Validate()
{
    int commentID = m_interactionNode->getInteractionCommentID();
    if (commentID != -1) {
        TheApp->MessageBoxId(commentID);
        return false;
    } else
        TheApp->MessageBoxId(IDS_MAKE_WHAT_INTERACTIVE);
    return true;
}

void
InteractionDialog::LoadData()
{
    SWND comboLevels = swGetDialogItem(m_dlg, IDC_INTERACTION_LEVEL);
    swComboBoxDeleteAll(comboLevels);

    char recommended[256];
    swLoadString(IDS_INTERACTION_LEVEL_RECOMMENDED, recommended, 255); 
    swComboBoxAppendItem(comboLevels, recommended);

    char all[256];
    swLoadString(IDS_INTERACTION_LEVEL_ALL, all, 255); 
    swComboBoxAppendItem(comboLevels, all);
}

void 
InteractionDialog::SaveData()
{
    m_routeData->level = swComboBoxGetSelection(swGetDialogItem(m_dlg,
                                                IDC_INTERACTION_LEVEL));
}


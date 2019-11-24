/*
 * SceneDialog.cpp
 *
 * Copyright (C) 2003, 2019 J. "MUFTI" Scheurich
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
 
#include "SceneDialog.h"
#include "resource.h"
#include "swt.h"
#include "NodeVrmlScene.h"

SceneDialog::SceneDialog(SWND parent, Node* node, bool add, int sceneNumber, 
                         NodeVrmlCut *vrmlCut)
  : Dialog(parent, IDD_SCENES_MANAGE)
{
    m_node = node;
    m_add = add;
    m_vrmlCut = vrmlCut;
    m_sceneNumber = sceneNumber;
    m_oldSceneNumber = sceneNumber;
    LoadData();
}

void
SceneDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    } else if (id == IDC_SCENES_FIRST) {
        LoadData();
    } else if (id == IDC_SCENES_LAST) {
        LoadData();
    } else if (id == IDC_SCENE_NUMBER) {
        loadComboxData();
        LoadData();
    }
}

bool
SceneDialog::Validate()
{
    char buf[256];
    if (swGetCheck(swGetDialogItem(m_dlg, IDC_SCENES_FIRST)) &&
        swGetCheck(swGetDialogItem(m_dlg, IDC_SCENES_LAST))) {
        swLoadString(IDS_SCENES_NUMBER_NOT_FIRST_AND_LAST + swGetLang(), buf, 
                     255);
        TheApp->MessageBox(buf);
        return false;
    }
    swGetText(swGetDialogItem(m_dlg, IDC_SCENES_NUMBER), buf, 255);
    int sceneNumber = atoi(buf);
    if ((sceneNumber < 0) || (sceneNumber > m_vrmlCut->scenes()->getSize() - 1)) {
        swLoadString(IDS_SCENES_NUMBER_NOT_VALID + swGetLang(), buf, 
                     255);
        TheApp->MessageBox(buf);
        return false;
    }
    swGetText(swGetDialogItem(m_dlg, IDC_SCENE_NUMBER), buf, 255);
    sceneNumber = atoi(buf);
    if ((sceneNumber < 0) || (sceneNumber > m_vrmlCut->scenes()->getSize() - 1)) {
        swLoadString(IDS_SCENES_NUMBER_NOT_VALID + swGetLang(), buf, 
                     255);
        TheApp->MessageBox(buf);
        return false;
    }
    return true;
}

void
SceneDialog::loadComboxData(void)
{
    int number = swComboBoxGetSelection(swGetDialogItem(m_dlg,
                                            IDC_SCENE_NUMBER));    
    if (number != 0)
        m_sceneNumber = number - 1;
    if (m_sceneNumber > m_vrmlCut->sceneLengths()->getSize() - 1)
        m_sceneNumber = m_vrmlCut->sceneLengths()->getSize() - 1;
}

void
SceneDialog::LoadData()
{
    SWND comboNumbers = swGetDialogItem(m_dlg, IDC_SCENE_NUMBER);
    swComboBoxDeleteAll(comboNumbers);

    swComboBoxAppendItem(comboNumbers, " ");

    char buf[256];
    for (int i = 1; i <= m_vrmlCut->scenes()->getSize(); i++) {
         snprintf(buf, 256, "%d", i);
         swComboBoxAppendItem(comboNumbers, buf);
    }
    swLoadString(IDS_SCENES_LAST, buf, 255);
    swComboBoxAppendItem(comboNumbers, buf);

    snprintf(buf, 256, "%d", m_sceneNumber);
    swSetText(swGetDialogItem(m_dlg, IDC_SCENES_NUMBER), buf);

    snprintf(buf, 256, "%lf", getSceneLength());
    swSetText(swGetDialogItem(m_dlg, IDC_SCENES_LENGTH), buf);

    snprintf(buf, 256, "%lf", TheApp->getDefaultSceneLength());
    swSetText(swGetDialogItem(m_dlg, IDC_DEFAULT_SCENE_LENGTH), buf);
}

void 
SceneDialog::SaveData()
{
    char buf[256];

    m_sceneNumber = swComboBoxGetSelection(
        swGetDialogItem(m_dlg, IDC_SCENE_NUMBER)) - 1; 
    if (m_sceneNumber == -1) {
        swGetText(swGetDialogItem(m_dlg, IDC_SCENES_NUMBER), buf, 255);
        m_sceneNumber = atoi(buf);
    }
    if (swGetCheck(swGetDialogItem(m_dlg, IDC_SCENES_FIRST)))
        m_sceneNumber = 0;
    if (swGetCheck(swGetDialogItem(m_dlg, IDC_SCENES_LAST)))
        m_sceneNumber = m_vrmlCut->scenes()->getSize();
    swGetText(swGetDialogItem(m_dlg, IDC_DEFAULT_SCENE_LENGTH), buf, 255);
    TheApp->setDefaultSceneLength(atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_SCENES_LENGTH), buf, 255);
    setSceneLength(atof(buf)); 
}

double              
SceneDialog::getSceneLength(void) 
{
     return m_vrmlCut->sceneLengths()->getValue(m_oldSceneNumber);
}

void              
SceneDialog::setSceneLength(double length) 
{
     m_vrmlCut->sceneLengths()->setValue(m_oldSceneNumber, length);
}



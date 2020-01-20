/*
 * ModellingSettingsDialog.cpp
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

#include "stdafx.h"
#include "ModellingSettingsDialog.h"
#include "DuneApp.h"
#include "resource.h"
#include "swt.h"
#include "EulerAngles.h"

static int rot[24] = {
    EulOrdXYZs, EulOrdXYXs, EulOrdXZYs, EulOrdXZXs, 
    EulOrdYZXs, EulOrdYZYs, EulOrdYXZs, EulOrdYXYs,
    EulOrdZXYs, EulOrdZXZs, EulOrdZYXs, EulOrdZYZs,

    EulOrdZYXr, EulOrdXYXr, EulOrdYZXr, EulOrdXZXr,
    EulOrdXZYr, EulOrdYZYr, EulOrdZXYr, EulOrdYXYr,
    EulOrdYXZr, EulOrdZXZr, EulOrdXYZr, EulOrdZYZr 
};

static const char *rotTitles[24] = { 
    "XYZs", "XYXs", "XZYs", "XZXs",
    "YZXs", "YZYs", "YXZs", "YXYs",
    "ZXYs", "ZXZs", "ZYXs", "ZYZs",

    "ZYXr", "XYXr", "YZXr", "XZXr",
    "XZYr", "YZYr", "ZXYr", "YXYr",
    "YXZr", "ZXZr", "XYZr", "ZYZr"
};

void
ModellingSettingsDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == IDC_BUTTON_DEFAULT) {
        TheApp->ModellingDefaults();
        LoadData();
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
}

ModellingSettingsDialog::ModellingSettingsDialog(SWND parent)
  : Dialog(parent, IDD_MODELLING_SETTINGS)
{
    LoadData();
}

void ModellingSettingsDialog::LoadData()
{
    char buf[128];
    int handleMode = TheApp->GetHandleMode();
    swSetCheck(swGetDialogItem(m_dlg, IDC_HANDLES_NONE),
               handleMode == HM_NONE);
    swSetCheck(swGetDialogItem(m_dlg, IDC_HANDLES_SELECTED),
               handleMode == HM_SELECTED);
    swSetCheck(swGetDialogItem(m_dlg, IDC_HANDLES_TREE),
               handleMode == HM_TREE);
    swSetCheck(swGetDialogItem(m_dlg, IDC_HANDLES_ALL),
               handleMode == HM_ALL);
    swSetCheck(swGetDialogItem(m_dlg, IDC_HANDLE_MESH_ALWAYS),
               TheApp->GetHandleMeshAlways());

    mysnprintf(buf, 128, "%g", TheApp->GetPreferenceHandleSize());
    swSetText(swGetDialogItem(m_dlg, IDC_HANDLE_SIZE), buf);
    mysnprintf(buf, 128, "%g", TheApp->GetHandleScale());
    swSetText(swGetDialogItem(m_dlg, IDC_HANDLE_SCALE), buf);
    mysnprintf(buf, 128, "%g", TheApp->getHumanoidScale());
    swSetText(swGetDialogItem(m_dlg, IDC_HUMANOID_JOINT_SCALE), buf);
    mysnprintf(buf, 128, "%g", TheApp->getExtrusionAmount());
    swSetText(swGetDialogItem(m_dlg, IDC_EXTRUSION_AMOUNT), buf);
    SWND combo = swGetDialogItem(m_dlg, IDC_ROTATION_ORDER);
    swComboBoxDeleteAll(combo);
    for (int i = 0; i < 24; i++) {
        swComboBoxAppendItem(combo, rotTitles[i]);
        if (rot[i] == TheApp->GetRotationOrder()) {
            swComboBoxSetSelection(combo, i);
        }
    }
    mysnprintf(buf, 128, "%g", TheApp->GetHandleEpsilon());
    swSetText(swGetDialogItem(m_dlg, IDC_HANDLE_EPSILON), buf);
    swSetCheck(swGetDialogItem(m_dlg, IDC_WEIGHTS_1), TheApp->getWeights1());
}

void
ModellingSettingsDialog::shouldBeMoreThanZero(const char *name)
{
    char title[256];
    swLoadString(IDS_DUNE, title, 256);
    char str[256];
    swLoadString(IDS_MORE_THAN_ZERO, str, 256);
    char message[256];
    mysnprintf(message, 255, str, name);
    swMessageBox(TheApp->mainWnd(), message, title, SW_MB_OK, SW_MB_ERROR);
}

bool
ModellingSettingsDialog::Validate()
{
    char buf[128];
    swGetText(swGetDialogItem(m_dlg, IDC_HANDLE_SIZE), buf, 128);
    if (atof(buf) <= 0) {
        char str[256];
        swLoadString(IDS_HANDLE_SIZE, str, 256);
        shouldBeMoreThanZero(str);
        return false;
    }    
    swGetText(swGetDialogItem(m_dlg, IDC_HANDLE_SCALE), buf, 128);
    if (atof(buf) <= 0) {
        char str[256];
        swLoadString(IDS_HANDLE_SCALE, str, 256);
        shouldBeMoreThanZero(str);
        return false;
    }    
    swGetText(swGetDialogItem(m_dlg, IDC_HUMANOID_JOINT_SCALE), buf, 128);
    if (atof(buf) <= 0) {
        char str[256];
        swLoadString(IDS_HUMANOID_JOINT_SCALE, str, 256);
        shouldBeMoreThanZero(str);
        return false;
    }    
    swGetText(swGetDialogItem(m_dlg, IDC_HANDLE_EPSILON), buf, 128);
    if (atof(buf) <= 0) {
        char str[256];
        swLoadString(IDS_HANDLE_EPSILON, str, 256);
        shouldBeMoreThanZero(str);
        return false;
    }    

    return true;
}


void
ModellingSettingsDialog::SaveData() 
{
    HandleMode handleMode = HM_NONE;
    char buf[128];

    if (swGetCheck(swGetDialogItem(m_dlg, IDC_HANDLES_NONE))) {
        handleMode = HM_NONE;
    } else if (swGetCheck(swGetDialogItem(m_dlg, IDC_HANDLES_SELECTED))) {
        handleMode = HM_SELECTED;
    } else if (swGetCheck(swGetDialogItem(m_dlg, IDC_HANDLES_TREE))) {
        handleMode = HM_TREE;
    } else if (swGetCheck(swGetDialogItem(m_dlg, IDC_HANDLES_ALL))) {
        handleMode = HM_ALL;
    }
    TheApp->SetHandleMeshAlways(swGetCheck(swGetDialogItem(m_dlg, 
          IDC_HANDLE_MESH_ALWAYS)));
    TheApp->SetHandleMode(handleMode);
    swGetText(swGetDialogItem(m_dlg, IDC_HANDLE_SIZE), buf, 128);
    TheApp->SetPreferenceHandleSize(atof(buf));    
    swGetText(swGetDialogItem(m_dlg, IDC_HANDLE_SCALE), buf, 128);
    TheApp->SetPreferenceHandleScale(atof(buf));    
    swGetText(swGetDialogItem(m_dlg, IDC_HUMANOID_JOINT_SCALE), buf, 128);
    TheApp->setHumanoidScale(atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_EXTRUSION_AMOUNT), buf, 128);
    TheApp->setExtrusionAmount(atof(buf));

    int sel = swComboBoxGetSelection(swGetDialogItem(m_dlg, IDC_ROTATION_ORDER));
    TheApp->SetRotationOrder(rot[sel]);
    TheApp->SetRotationTitle(rotTitles[sel]);

    swGetText(swGetDialogItem(m_dlg, IDC_HANDLE_EPSILON), buf, 128);
    TheApp->SetHandleEpsilon(atof(buf));

    TheApp->setWeights1(swGetCheck(swGetDialogItem(m_dlg, IDC_WEIGHTS_1)));
}

/*
 * StereoViewSettingsDialog.cpp
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
#include "StereoViewSettingsDialog.h"
#include "DuneApp.h"
#include "ExternTheApp.h"
#include "resource.h"
#include "swt.h"

StereoViewSettingsDialog::StereoViewSettingsDialog(SWND parent)
  : Dialog(parent, IDD_STEREO_VIEW)
{
    LoadData();
}

//StereoViewSettingsDialog::~StereoViewSettingsDialog()
//{
//}

void
StereoViewSettingsDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == IDC_STEREO_VIEW_SET_DEFAULTS) {
        TheApp->StereoViewSetDefaults();
        LoadData();
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
}

void StereoViewSettingsDialog::LoadData()
{
    char buf[128];

    int cursor3dMode = TheApp->Get3dCursorMode();
    swSetCheck(swGetDialogItem(m_dlg, IDC_3DCURSOR_NONE),
               cursor3dMode == CM_3DCURSOR_NONE);
    swSetCheck(swGetDialogItem(m_dlg, IDC_3DCURSOR_RECORDING),
               cursor3dMode == CM_3DCURSOR_RECORDING);
    swSetCheck(swGetDialogItem(m_dlg, IDC_3DCURSOR_NOT_RUN),
               cursor3dMode == CM_3DCURSOR_NOT_RUN);
    swSetCheck(swGetDialogItem(m_dlg, IDC_3DCURSOR_ALWAYS),
               cursor3dMode == CM_3DCURSOR_ALWAYS);

    swSetCheck(swGetDialogItem(m_dlg, IDC_USE_STEREO), TheApp->useStereo());

    mysnprintf(buf, 128, "%g", TheApp->getEyeHalfDist()*2);
    swSetText(swGetDialogItem(m_dlg, IDC_EYE_DIST), buf);
    mysnprintf(buf, 128, "%g", TheApp->getEyeScreenDist());
    swSetText(swGetDialogItem(m_dlg, IDC_SCREEN_DIST), buf);

    mysnprintf(buf, 128, "%g", TheApp->getFixFieldOfView());
    swSetText(swGetDialogItem(m_dlg, IDC_FIX_FOV), buf);

    mysnprintf(buf, 128, "%g", TheApp->GetStereoHandleSizeMult());
    swSetText(swGetDialogItem(m_dlg, IDC_STEREO_HANDLE_MULT), buf);

    mysnprintf(buf, 128, "%g", TheApp->Get3dCursorWidth());
    swSetText(swGetDialogItem(m_dlg, IDC_3DCURSOR_WIDTH), buf);
    mysnprintf(buf, 128, "%g", TheApp->Get3dCursorLength());
    swSetText(swGetDialogItem(m_dlg, IDC_3DCURSOR_LENGTH), buf);

    mysnprintf(buf, 128, "%g", TheApp->GetEyeAngleFactor());
    swSetText(swGetDialogItem(m_dlg, IDC_EYE_ANGLE_FACTOR), buf);

}


void
StereoViewSettingsDialog::SaveData() 
{
    Cursor3dMode cursor3dMode = CM_3DCURSOR_NONE;
    char buf[128];

    if (swGetCheck(swGetDialogItem(m_dlg, IDC_3DCURSOR_NONE))) {
        cursor3dMode = CM_3DCURSOR_NONE;
    } else if (swGetCheck(swGetDialogItem(m_dlg, IDC_3DCURSOR_RECORDING))) {
        cursor3dMode = CM_3DCURSOR_RECORDING;
    } else if (swGetCheck(swGetDialogItem(m_dlg, IDC_3DCURSOR_NOT_RUN))) {
        cursor3dMode = CM_3DCURSOR_NOT_RUN;
    } else if (swGetCheck(swGetDialogItem(m_dlg, IDC_3DCURSOR_ALWAYS))) {
        cursor3dMode = CM_3DCURSOR_ALWAYS;
    }
    TheApp->Set3dCursorMode(cursor3dMode);

    TheApp->setWantStereo(swGetCheck(swGetDialogItem(m_dlg, IDC_USE_STEREO))!=0);

    swGetText(swGetDialogItem(m_dlg, IDC_EYE_DIST), buf, 128);
    TheApp->setEyeDist(atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_SCREEN_DIST), buf, 128);
    TheApp->setEyeScreenDist(atof(buf));

    swGetText(swGetDialogItem(m_dlg, IDC_FIX_FOV), buf, 128);
    TheApp->setFixFieldOfView(atof(buf));

    swGetText(swGetDialogItem(m_dlg, IDC_STEREO_HANDLE_MULT), buf, 128);
    TheApp->SetStereoHandleSizeMult(atof(buf));

    swGetText(swGetDialogItem(m_dlg, IDC_3DCURSOR_WIDTH), buf, 128);
    TheApp->Set3dCursorWidth(atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_3DCURSOR_LENGTH), buf, 128);
    TheApp->Set3dCursorLength(atof(buf));

    swGetText(swGetDialogItem(m_dlg, IDC_EYE_ANGLE_FACTOR), buf, 128);
    TheApp->SetEyeAngleFactor(atof(buf));
    
}

/*
 * RenderSettingsDialog.cpp
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
#include "RenderSettingsDialog.h"
#include "DuneApp.h"
#include "resource.h"
#include "swt.h"

void
RenderSettingsDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == IDC_BUTTON_DEFAULT) {
        TheApp->RenderDefaults();
        LoadData();
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
}

RenderSettingsDialog::RenderSettingsDialog(SWND parent)
  : Dialog(parent, IDD_RENDER_SETTINGS)
{
    LoadData();
}

void RenderSettingsDialog::LoadData()
{
    char buf[128];
    swSetCheck(swGetDialogItem(m_dlg, IDC_SHOW_ALL_FIELDS),
               TheApp->GetShowAllFields());

    mysnprintf(buf, 128, "%g", TheApp->GetPointSetSize());
    swSetText(swGetDialogItem(m_dlg, IDC_POINTSET_SIZE), buf);

    mysnprintf(buf, 128, "%g", TheApp->GetNearClippingPlaneDist());
    swSetText(swGetDialogItem(m_dlg, IDC_NEAR_CLIPPING_PLANE_DIST), buf);

    mysnprintf(buf, 128, "%g", TheApp->GetFarClippingPlaneDist());
    swSetText(swGetDialogItem(m_dlg, IDC_FAR_CLIPPING_PLANE_DIST), buf);

    mysnprintf(buf, 128, "%d", TheApp->GetMaxInlinesToLoad());
    swSetText(swGetDialogItem(m_dlg, IDC_MAX_INLINES_TO_LOAD), buf);

    mysnprintf(buf, 128, "%d", TheApp->GetMaxKeysInChannelView());
    swSetText(swGetDialogItem(m_dlg, IDC_MAX_KEYS_IN_CHANNELVIEW), buf);
}

bool
RenderSettingsDialog::Validate()
{
    char buf[128];
    swGetText(swGetDialogItem(m_dlg, IDC_NEAR_CLIPPING_PLANE_DIST), buf, 128);
    float nearClippingPlane = atof(buf);
    if (nearClippingPlane <= 0) {
        TheApp->MessageBoxId(IDS_NEAR_CLIPPING_TOO_SMALL);
        return false;
    }
    swGetText(swGetDialogItem(m_dlg, IDC_FAR_CLIPPING_PLANE_DIST), buf, 128);
    float FarClippingPlane = atof(buf);
    if (nearClippingPlane >= FarClippingPlane) {
        TheApp->MessageBoxId(IDS_NEAR_BIGGER_FAR);
        return false;
    }
    swGetText(swGetDialogItem(m_dlg, IDC_POINTSET_SIZE), buf, 128);
    if (atof(buf) < 0) {
        char str[256];
        swLoadString(IDS_POINTSET_SIZE, str, 256);
        shouldBeMoreThanZero(str);
        return false;
    }    
    return true;
}

void
RenderSettingsDialog::shouldBeMoreThanZero(const char *name)
{
    char title[256];
    swLoadString(IDS_DUNE, title, 256);
    char str[256];
    swLoadString(IDS_MORE_THAN_ZERO, str, 256);
    char message[256];
    mysnprintf(message, 255, str, name);
    swMessageBox(TheApp->mainWnd(), message, title, SW_MB_OK, SW_MB_ERROR);
}

void
RenderSettingsDialog::SaveData() 
{
    SWND saf;
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_POINTSET_SIZE), buf, 128);
    TheApp->SetPointSetSize(atof(buf));

    saf = swGetDialogItem(m_dlg, IDC_SHOW_ALL_FIELDS);
    TheApp->SetShowAllFields(swGetCheck(saf) != FALSE);

    swGetText(swGetDialogItem(m_dlg, IDC_NEAR_CLIPPING_PLANE_DIST), buf, 128);
    TheApp->SetNearClippingPlaneDist(atof(buf));

    swGetText(swGetDialogItem(m_dlg, IDC_FAR_CLIPPING_PLANE_DIST), buf, 128);
    TheApp->SetFarClippingPlaneDist(atof(buf));

    swGetText(swGetDialogItem(m_dlg, IDC_MAX_INLINES_TO_LOAD), buf, 128);
    TheApp->SetMaxInlinesToLoad(atoi(buf));

    swGetText(swGetDialogItem(m_dlg, IDC_MAX_KEYS_IN_CHANNELVIEW), buf, 128);
    TheApp->SetMaxKeysInChannelView(atoi(buf));
}

/*
 * OutputSettingsDialog.cpp
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
#include "OutputSettingsDialog.h"
#include "DuneApp.h"
#include "resource.h"
#include "swt.h"

OutputSettingsDialog::OutputSettingsDialog(SWND parent)
  : Dialog(parent, IDD_OUTPUT)
{
    LoadData();
}

void
OutputSettingsDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == IDC_OUTPUT_DEFAULTS) {
        TheApp->OutputSetDefaults();
        LoadData();
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
}

void OutputSettingsDialog::LoadData()
{
    char buf[128];
    swSetCheck(swGetDialogItem(m_dlg, IDC_KEEP_URLS), 
               TheApp->GetKeepURLs());
    mysnprintf(buf, 128, "%d", TheApp->GetIndent());
    swSetText(swGetDialogItem(m_dlg, IDC_INDENT), buf);
    mysnprintf(buf, 128, "%d", TheApp->GetFloatDigits());
    swSetText(swGetDialogItem(m_dlg, IDC_FLOAT_DIGITS), buf);  
    swSetCheck(swGetDialogItem(m_dlg, IDC_INCLUDE_PROTOS), 
                               TheApp->GetIncludeProtos() ? 1 : 0);
    swSetCheck(swGetDialogItem(m_dlg, IDC_K_AND_R), 
               TheApp->GetkrFormating());
    swSetCheck(swGetDialogItem(m_dlg, IDC_COMPRESS), 
               TheApp->GetCompress() ? 1 : 0);
    swSetCheck(swGetDialogItem(m_dlg, IDC_NORMALS_ON_MESH_CREATION), 
       TheApp->OutputApp::GetNormalsOnMeshCreation());

    swSetCheck(swGetDialogItem(m_dlg, IDC_REVISION_CONTROL_ENABLED), 
                               TheApp->GetRevisionControlCheckinFlag() ? 
                               1 : 0);
    swSetText(swGetDialogItem(m_dlg, IDC_REVISION_CONTROL_COMMAND), 
                              TheApp->GetRevisionControlCheckinCommand());

    swSetText(swGetDialogItem(m_dlg, IDC_X3DOM_PATH), 
                              TheApp->GetX3domPath());
    swSetText(swGetDialogItem(m_dlg, IDC_X3DOM_PARAMETER), 
                              TheApp->GetX3domParameter());

    swSetText(swGetDialogItem(m_dlg, IDC_XITE_PATH), 
                              TheApp->GetXitePath());

    swSetCheck(swGetDialogItem(m_dlg, 
          IDC_SKIP_MATERIAL_NAME_BEFORE_FIRST_UNDERSCORE), 
          TheApp->SkipMaterialNameBeforeFirstUnderscore() ? 1 : 0);
    swSetCheck(swGetDialogItem(m_dlg, 
          IDC_SKIP_MATERIAL_NAME_AFTER_LAST_UNDERSCORE),
          TheApp->SkipMaterialNameAfterLastUnderscore() ? 1 : 0);

    swSetText(swGetDialogItem(m_dlg, IDC_DOWNLOAD_DIR), 
                              TheApp->getDownloadDirectory());

    swSetText(swGetDialogItem(m_dlg, IDC_DEFAULT_AC3D_MATERIAL_NAME), 
                              TheApp->GetDefaultAc3dMaterialName()); 
    swSetCheck(swGetDialogItem(m_dlg, IDC_AC3D_EXPORT_4_RAVEN), 
                               TheApp->GetAc3dExport4Raven());
    swSetText(swGetDialogItem(m_dlg, IDC_REVISION_CONTROL_COMMAND),
                              TheApp->GetRevisionControlCheckinCommand());

#ifdef HAVE_IMAGE_CONVERTER
    swSetCheck(swGetDialogItem(m_dlg, IDC_AC3D_EXPORT_CONVERT_2_GIF), 
                               TheApp->GetAc3dExportConvert2Gif());
#else
    swSetCheck(swGetDialogItem(m_dlg, IDC_AC3D_EXPORT_CONVERT_2_GIF), 
                               false);
#endif
    swSetText(swGetDialogItem(m_dlg, IDC_DEFAULT_CATT_MATERIAL_NAME), 
                              TheApp->GetDefaultCattMaterialName());

    swSetText(swGetDialogItem(m_dlg, IDC_RIB_TEXTURER),
                              TheApp->GetRibTexturer());
    swSetText(swGetDialogItem(m_dlg, IDC_IMAGE_CONVERTER),
                              TheApp->GetImageConverter());
}

bool
OutputSettingsDialog::Validate()
{
    char buf[128];
    swGetText(swGetDialogItem(m_dlg, IDC_FLOAT_DIGITS), buf, 128);
    if (atoi(buf) > 9)
        TheApp->MessageBoxId(IDS_FLOAT_ONLY_9_CHARACTERS, SW_MB_WARNING);
#ifdef _WIN32
    if (swGetCheck(swGetDialogItem(m_dlg, IDC_KEEP_URLS)) == 0)
        TheApp->MessageBoxId(IDS_URL_RECOMMENDED, SW_MB_WARNING);
#endif
    if (atoi(buf) < 1)
        return false;
#ifndef HAVE_IMAGE_CONVERTER
    if (swGetCheck(swGetDialogItem(m_dlg, IDC_AC3D_EXPORT_CONVERT_2_GIF))) {
        TheApp->MessageBoxId(IDS_NO_GIF_CONVERTTER, SW_MB_WARNING);
        return false;
    }
#endif
    char temp[1024];
    swGetText(swGetDialogItem(m_dlg, IDC_DOWNLOAD_DIR), temp, 1023); 
    MyString dir = "";
    dir += temp;
    dir += '/';
    if (!mkdir_parents4file(dir))
        return false;
    return true;
}


void
OutputSettingsDialog::SaveData() 
{
    char buf[128];

    TheApp->SetKeepURLs(swGetCheck(swGetDialogItem(m_dlg, IDC_KEEP_URLS))!=0);

    swGetText(swGetDialogItem(m_dlg, IDC_INDENT), buf, 128);
    TheApp->SetIndent(atoi(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_FLOAT_DIGITS), buf, 128);
    TheApp->SetFloatDigits(atoi(buf));
    TheApp->SetIncludeProtos(swGetCheck(swGetDialogItem(m_dlg, 
                                        IDC_INCLUDE_PROTOS))!=0);
    TheApp->SetkrFormating(swGetCheck(swGetDialogItem(m_dlg, IDC_K_AND_R))!=0);
    TheApp->SetCompress(swGetCheck(swGetDialogItem(m_dlg, IDC_COMPRESS))!=0);
    TheApp->SetNormalsOnMeshCreation(swGetCheck(swGetDialogItem(m_dlg, 
                                   IDC_NORMALS_ON_MESH_CREATION))!=0);

    char temp[1024];
    swGetText(swGetDialogItem(m_dlg, IDC_XITE_PATH), temp, 1023);
    TheApp->SetXitePath(temp);

    swGetText(swGetDialogItem(m_dlg, IDC_X3DOM_PATH), temp, 1023);
    TheApp->SetX3domPath(temp);
    swGetText(swGetDialogItem(m_dlg, IDC_X3DOM_PARAMETER), temp, 1023);
    TheApp->SetX3domParameter(temp);

    TheApp->SetRevisionControlCheckinFlag(swGetCheck(
          swGetDialogItem(m_dlg, IDC_REVISION_CONTROL_ENABLED))!=0);

    swGetText(swGetDialogItem(m_dlg, IDC_REVISION_CONTROL_COMMAND), temp, 1023);
    TheApp->SetRevisionControlCheckinCommand(temp);

    TheApp->SetSkipMaterialNameBeforeFirstUnderscore(
           swGetCheck(swGetDialogItem(m_dlg, 
                      IDC_SKIP_MATERIAL_NAME_BEFORE_FIRST_UNDERSCORE)));
    TheApp->SetSkipMaterialNameAfterLastUnderscore(
           swGetCheck(swGetDialogItem(m_dlg, 
                      IDC_SKIP_MATERIAL_NAME_AFTER_LAST_UNDERSCORE)));

    swGetText(swGetDialogItem(m_dlg, IDC_DOWNLOAD_DIR), temp, 1023); 
    TheApp->setDownloadDirectory(temp);

    swGetText(swGetDialogItem(m_dlg, IDC_DEFAULT_AC3D_MATERIAL_NAME), temp, 
                              1023);
    TheApp->SetDefaultAc3dMaterialName(temp);

    TheApp->SetAc3dExport4Raven(swGetCheck(
          swGetDialogItem(m_dlg, IDC_AC3D_EXPORT_4_RAVEN))!=0);
    TheApp->SetAc3dExportConvert2Gif(swGetCheck(
          swGetDialogItem(m_dlg, IDC_AC3D_EXPORT_CONVERT_2_GIF))!=0);


    swGetText(swGetDialogItem(m_dlg, IDC_DEFAULT_CATT_MATERIAL_NAME), temp, 
                              1023);
    TheApp->SetDefaultCattMaterialName(temp);

    swGetText(swGetDialogItem(m_dlg, IDC_RIB_TEXTURER), temp, 1023);
    TheApp->SetRibTexturer(temp);
    swGetText(swGetDialogItem(m_dlg, IDC_IMAGE_CONVERTER), temp, 1023);
    TheApp->SetImageConverter(temp);
}

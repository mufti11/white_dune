/*
 * CattExportDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White/2010 J. "MUFTI" Scheurich
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

#include <ctype.h>
#include <sys/stat.h>
#include "stdafx.h"
#include "CattExportDialog.h"
#include "DuneApp.h"
#include "resource.h"
#include "swt.h"

CattExportDialog::CattExportDialog(SWND parent)
  : Dialog(parent, IDD_CATT_EXPORT)
{
    LoadData();
}

void
CattExportDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == IDC_BROWSE_CATT_EXPORT_PATH) {
        char buf[1024];
        buf[0] = 0;
        const char *choice = "export directory containing material.geo(*)\0;*\0\0";
        if (swOpenDirDialog(TheApp->mainWnd(), "Enter export directory", 
                            choice, buf, 1023)) {
            TheApp->SetCattExportPath(buf);
            LoadData();
        }
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
}

void CattExportDialog::LoadData()
{
    swSetText(swGetDialogItem(m_dlg, IDC_CATT_EXPORT_PATH), 
                              TheApp->GetCattExportPath());

    swSetCheck(swGetDialogItem(m_dlg, 
          IDC_SKIP_MATERIAL_NAME_BEFORE_FIRST_UNDERSCORE), 
          TheApp->SkipMaterialNameBeforeFirstUnderscore() ? 1 : 0);
    swSetCheck(swGetDialogItem(m_dlg, 
          IDC_SKIP_MATERIAL_NAME_AFTER_LAST_UNDERSCORE), 
          TheApp->SkipMaterialNameAfterLastUnderscore() ? 1 : 0);
}

bool
CattExportDialog::Validate()
{
    MyString path = "";
    path += TheApp->GetCattExportPath();
    struct stat inodeInfo;
#ifdef WIN32
    if ((path[path.length() - 1] == swGetPathSeperator()) ||
        (path[path.length() - 1] == '/')) {
        // skip last pathseperator, windows fstat is too silly for that
        MyString oldPath = "";
        oldPath += path;
        path = "";
        for (int i = 0; i < oldPath.length() - 1; i++)
            path += oldPath[i];
    }
#endif
    if (stat((const char *)path, &inodeInfo) != 0) {
        TheApp->MessageBoxPerror(TheApp->GetCattExportPath());
        return false;
    } else
        if ((inodeInfo.st_mode & S_IFMT) != S_IFDIR) {
            TheApp->MessageBox(IDS_NOT_A_DIRECTORY,
                               TheApp->GetCattExportPath());
            return false;
        }
#ifdef WIN32
    path += swGetPathSeperator();
#endif
    path += "material.geo";
    if (stat((const char *)path, &inodeInfo) == 0)
        if (S_ISREG(inodeInfo.st_mode))
            return true;
    TheApp->MessageBox(IDS_SHOULD_HAVE_MATERIAL_GEO, 
                       TheApp->GetCattExportPath());
    return true;
}


void
CattExportDialog::SaveData() 
{
    char temp[1024];

    swGetText(swGetDialogItem(m_dlg, IDC_CATT_EXPORT_PATH), temp, 1023);
    TheApp->SetCattExportPath(temp);

    TheApp->SetSkipMaterialNameBeforeFirstUnderscore(swGetCheck(
          swGetDialogItem(m_dlg, IDC_SKIP_MATERIAL_NAME_BEFORE_FIRST_UNDERSCORE))
          );
    TheApp->SetSkipMaterialNameAfterLastUnderscore(swGetCheck(
          swGetDialogItem(m_dlg, IDC_SKIP_MATERIAL_NAME_AFTER_LAST_UNDERSCORE)));
}

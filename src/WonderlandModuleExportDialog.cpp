/*
 * WonderlandModuleExportDialog.cpp
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

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>

#include "stdafx.h"
#include "resource.h"
#include "swt.h"
#include "WonderlandModuleExportDialog.h"
#include "DuneApp.h"
#include "Scene.h"

WonderlandModuleExportDialog::WonderlandModuleExportDialog(SWND parent, 
                                                           Scene *scene)
  : Dialog(parent, IDD_WONDERLAND_MODULE_EXPORT)
{
    m_scene = scene;
    m_success = false;
    LoadData();
    
}

void
WonderlandModuleExportDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
            m_success = m_scene->write(1, TheApp->GetWonderlandModuleExportPath(),
                         TEMP_SAVE | TRIANGULATE | JAVA_SOURCE |
                         (TheApp->GetWonderlandModuleExportManyClasses() ?
                          MANY_JAVA_CLASSES : 0));
            if (TheApp->GetRunAnt())
                TheApp->runAnt(m_scene);
        }
    } else if (id == IDC_WONDERLAND_MODULE_EXPORT_MANY_CLASSES) {
        TheApp->SetWonderlandModuleExportManyClasses(
              !TheApp->GetWonderlandModuleExportManyClasses());
        LoadData();
    } else if (id == IDC_WONDERLAND_MODULE_EXPORT_ALL_X3D_CLASSES) {
        TheApp->SetWonderlandModuleExportAllX3dClasses(
              !TheApp->GetWonderlandModuleExportAllX3dClasses());
        LoadData();
    } else if (id == IDC_BROWSE_WONDERLAND_EXPORT_PATH) {
        char buf[1024];
        strcpy(buf, "modules");
        const char *choice = "modules directory (modules)\0modules;\0\0";
        if (swOpenDirDialog(TheApp->mainWnd(), "Enter modules directory", 
                            choice, buf, 1023)) {
            TheApp->SetWonderlandModuleExportPath(buf);
            LoadData();
        }
    } else if (id == IDC_BROWSE_ANT_PATH) {
        char buf[1024];
        strcpy(buf, "ant");
        if (TheApp->browseCommand(buf, 1023, IDC_BROWSE_ANT_PATH)) {
            TheApp->SetAntPath(buf);
            LoadData();
        }
    } else if (id == IDC_WONDERLAND_MODULE_EXPORT_RUN_ANT) {
        TheApp->SetRunAnt(!TheApp->GetRunAnt());
        LoadData();
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
}

void WonderlandModuleExportDialog::LoadData()
{
    swSetText(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_PATH), 
                              TheApp->GetWonderlandModuleExportPath());
    swSetText(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_PREFIX), 
                              TheApp->GetWonderlandModuleExportPrefix());
    swSetCheck(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_MANY_CLASSES),
                               TheApp->GetWonderlandModuleExportManyClasses());
    swSetCheck(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_ALL_X3D_CLASSES),
                               TheApp->GetWonderlandModuleExportAllX3dClasses());

    swSetCheck(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_RUN_ANT),
                               TheApp->GetRunAnt());
    swSetText(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_ANT_PATH), 
                              TheApp->GetAntPath());
    swSetText(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_ANT_COMMAND),
                              TheApp->GetAntCommand());
    swSetText(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_ANT_OPTIONS),
                              TheApp->GetAntOptions());
}

bool
WonderlandModuleExportDialog::Validate()
{
    const char *prefix = TheApp->GetWonderlandModuleExportPrefix();
    bool invalidPrefix = false;
    if ((prefix == NULL) || (strlen(prefix) == 0))
        invalidPrefix = true;
    else
       for (unsigned int i = 0; i < strlen(prefix); i++)
           if ((!isascii(prefix[i])) || (!isalnum(prefix[i])))
               invalidPrefix = true;
    if (invalidPrefix) {
        TheApp->MessageBoxId(IDS_NEED_WONDERLAND_MODULE_PREFIX);
        return false;
    }
    if (TheApp->GetWonderlandModuleExportPath() == NULL)
        return false;
    if (strlen(TheApp->GetWonderlandModuleExportPath()) == 0)
        return false;
    const char *lastDir = strrchr(TheApp->GetWonderlandModuleExportPath(), '/');
    if (lastDir) {
        lastDir++;
        if (strlen(lastDir) == 0) {
            lastDir = strrchr(lastDir - 1, '/');
            if (lastDir) {
                lastDir++;
                if (strlen(lastDir) == 0)
                    return false;
            }
        }
        if (stringncmp(lastDir, "modules") != 0) {
            char message[256];
            swLoadString(IDS_LAST_PART_NOT_MODULES, message, 256);
            if (swMessageBox(m_dlg, message, "Are your sure ?", 
                             SW_MB_YESNO, SW_MB_WARNING) == IDNO) 
                return false;
        }
    }
    struct stat inodeInfo;
    if (stat(TheApp->GetWonderlandModuleExportPath(), &inodeInfo) == 0) {
        if ((inodeInfo.st_mode & S_IFMT) != S_IFDIR) {
            TheApp->MessageBoxId(IDS_NOT_A_DIRECTORY);
            return false;
        }
    } else {
        TheApp->MessageBoxPerror(TheApp->GetWonderlandModuleExportPath());
        return false;
    }
    if (TheApp->GetRunAnt()) {
        if ((TheApp->GetAntPath() == NULL) || 
            (strlen(TheApp->GetAntPath()) == 0))
            return false;
    } 
    return true;
}


void
WonderlandModuleExportDialog::SaveData() 
{
    char temp[1024];

    swGetText(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_PATH), temp, 
                              1023);
    TheApp->SetWonderlandModuleExportPath(temp);
    swGetText(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_PREFIX), temp, 
                              1023);
    TheApp->SetWonderlandModuleExportPrefix(temp);
    TheApp->SetWonderlandModuleExportManyClasses(swGetCheck(swGetDialogItem(
           m_dlg, IDC_WONDERLAND_MODULE_EXPORT_MANY_CLASSES)));
    TheApp->SetWonderlandModuleExportAllX3dClasses(swGetCheck(swGetDialogItem(
           m_dlg, IDC_WONDERLAND_MODULE_EXPORT_ALL_X3D_CLASSES)));

    TheApp->SetRunAnt(swGetCheck(swGetDialogItem(m_dlg, 
           IDC_WONDERLAND_MODULE_EXPORT_RUN_ANT)));
    swGetText(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_ANT_PATH), 
                              temp, 1023);
    TheApp->SetAntPath(temp);
    swGetText(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_ANT_COMMAND), 
                              temp, 1023);
    TheApp->SetAntCommand(temp);
    swGetText(swGetDialogItem(m_dlg, IDC_WONDERLAND_MODULE_EXPORT_ANT_OPTIONS), 
                              temp, 1023);
    TheApp->SetAntOptions(temp);
}

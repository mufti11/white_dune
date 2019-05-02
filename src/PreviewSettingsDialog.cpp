/*
 * PreviewSettingsDialog.cpp
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
#include "PreviewSettingsDialog.h"
#include "DuneApp.h"
#include "resource.h"
#include "swt.h"
#include "WriteFlags.h"

PreviewSettingsDialog::PreviewSettingsDialog(SWND parent)
  : Dialog(parent, IDD_PREVIEW_SETTINGS)
{
    LoadData();
}

void
PreviewSettingsDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    switch(id) {
      case IDC_BROWSE:
        OnBrowse();
        break;
      case IDC_USE_REMOTE:
        OnUseRemote();
        break;
      case IDC_BROWSER_USE_DEFAULT:
        OnSetDefault();
        break;
      default:
        Dialog::OnCommand(swToPtr(id));
        break;
    }
}

static MyString
GetText(SWND item)
{
    char buf[1024];
    swGetText(item, buf, 1024);
    return buf;
}

static const char *vrmlLevelStrings[] = { 
    "X_ITE html",
    "pure VRML97",
    "VRML97 Amendment 1",
    "VRML97 with COVER extensions",
    "pure X3DV",
    "X3DV",
    "X3DOM html"
};

void PreviewSettingsDialog::LoadData()
{
    const char *command;
    int vrmlLevel;
    int useRemote;
    int xtypExecuteOrFork;
    const char *remoteCommand;
    const char *application;
    const char *topic;

    swBrowserGetSettings(TheApp->GetBrowser(), &command, 
                         &vrmlLevel, &useRemote, &xtypExecuteOrFork,
                         &remoteCommand, &application, &topic);
    swSetText(swGetDialogItem(m_dlg, IDC_COMMAND), command);

    SWND comboVrmlLevel = swGetDialogItem(m_dlg, IDC_BROWSER_VRML_LEVEL);
    swComboBoxDeleteAll(comboVrmlLevel);
    for (unsigned int i=0;i<(sizeof(vrmlLevelStrings)/sizeof(const char*));i++)
        swComboBoxAppendItem(comboVrmlLevel, vrmlLevelStrings[i]);

    int comboSelection = 0;
    switch (vrmlLevel) {
      default:
        comboSelection = 2;
        break;
      case PURE_VRML97:
        comboSelection = 1;
        break;
      case COVER:
        comboSelection = 3;
        break;
      case PURE_X3DV:
        comboSelection = 4;
        break;
      case X3DV:
        comboSelection = 5;
        break;
      case X3DOM:
        comboSelection = 6;
        break;
      case XITE:
        comboSelection = 0;
        break;
    }
    swComboBoxSetSelection(comboVrmlLevel, comboSelection);

    swSetCheck(swGetDialogItem(m_dlg, IDC_USE_REMOTE), useRemote);
#ifdef _WIN32
    swSetCheck(swGetDialogItem(m_dlg, IDC_XTYP_EXECUTE), xtypExecuteOrFork);
#else
    swSetCheck(swGetDialogItem(m_dlg, IDC_FORK_ON_PREVIEW), xtypExecuteOrFork);
    swSetCheck(swGetDialogItem(m_dlg, IDC_PREVIEW_CONSOLE), 
               swBrowserGetSettingsErrorToConsole(TheApp->GetBrowser()));
#endif
    swSetText(swGetDialogItem(m_dlg, IDC_REMOTE_COMMAND), remoteCommand);
    swSetText(swGetDialogItem(m_dlg, IDC_APPLICATION), application);
    swSetText(swGetDialogItem(m_dlg, IDC_TOPIC), topic);
}

void
PreviewSettingsDialog::SaveData()
{
    MyString command;
    int vrmlLevel = XITE;
    int useRemote;
    int xtypExecuteOrFork;
    MyString remoteCommand;
    MyString application;
    MyString topic;

    command = GetText(swGetDialogItem(m_dlg, IDC_COMMAND));
    useRemote = swGetCheck(swGetDialogItem(m_dlg, IDC_USE_REMOTE));
    OnUseRemote();
    int comboSelection = swComboBoxGetSelection(swGetDialogItem(m_dlg,
                                                IDC_BROWSER_VRML_LEVEL));
    switch (comboSelection) {
      case 1:
        vrmlLevel = PURE_VRML97;
        break;
      case 2:
        vrmlLevel = 0;
        break;    
      case 3:
        vrmlLevel = COVER;
        break;
      case 4:
        vrmlLevel = PURE_X3DV;
        break;
      case 5:
        vrmlLevel = X3DV;
        break;
      case 6:
        vrmlLevel = X3DOM;
        break;
      case 0:
        vrmlLevel = XITE;
        break;
    }
#ifdef _WIN32
    xtypExecuteOrFork = swGetCheck(swGetDialogItem(m_dlg, IDC_XTYP_EXECUTE));
#else
    xtypExecuteOrFork = swGetCheck(swGetDialogItem(m_dlg, IDC_FORK_ON_PREVIEW));
    if ((xtypExecuteOrFork != FALSE) && ((vrmlLevel == X3DOM) || 
                                        (vrmlLevel == XITE))) {
        xtypExecuteOrFork = FALSE;
        swSetCheck(swGetDialogItem(m_dlg, IDC_FORK_ON_PREVIEW), xtypExecuteOrFork);
    }
    int errorToConsole = swGetCheck(swGetDialogItem(m_dlg, IDC_PREVIEW_CONSOLE));
    swBrowserSetSettingsErrorToConsole(TheApp->GetBrowser(), errorToConsole);
#endif
    remoteCommand = GetText(swGetDialogItem(m_dlg, IDC_REMOTE_COMMAND));
    application = GetText(swGetDialogItem(m_dlg, IDC_APPLICATION));
    topic = GetText(swGetDialogItem(m_dlg, IDC_TOPIC));
    swBrowserSetSettings(TheApp->GetBrowser(), command, vrmlLevel, useRemote,
                         xtypExecuteOrFork, remoteCommand, application, topic);
}

void PreviewSettingsDialog::OnBrowse() 
{
    char buf[1024] = "";
    char buf2[1024] = "";

    if (TheApp->browseCommand(buf, 1024, IDS_PREVIEW_COMMAND_PROMPT)) {
        mysnprintf(buf2, 1024, "%s \"%%s\"", buf);
        swSetText(swGetDialogItem(m_dlg, IDC_COMMAND), buf2);
    }
}

void
PreviewSettingsDialog::OnUseRemote()
{
#ifdef _WIN32
    int flag = swGetCheck(swGetDialogItem(m_dlg, IDC_USE_REMOTE));

    swEnableWindow(swGetDialogItem(m_dlg, IDC_REMOTE_COMMAND), flag);
    swEnableWindow(swGetDialogItem(m_dlg, IDC_APPLICATION), flag);
    swEnableWindow(swGetDialogItem(m_dlg, IDC_TOPIC), flag);
#endif
}

void
PreviewSettingsDialog::OnSetDefault()
{
    swBrowserSetDefault(TheApp->GetBrowser());    
    LoadData();
}


/*
 * HelpSettingsDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White/2002 J. "MUFTI" Scheurich
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
#include "HelpSettingsDialog.h"
#include "DuneApp.h"
#include "resource.h"
#include <swt.h>

HelpSettingsDialog::HelpSettingsDialog(SWND parent)
  : Dialog(parent, IDD_HELP_SETTINGS)
{
    LoadData();
}

void
HelpSettingsDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    switch(id) {
      case IDC_BROWSE_HELP_COMMAND:
        OnBrowseHelpCommand();
        break;
      case IDC_BROWSE_DOC_DIRECTORY:
        OnBrowseDocDirectory();
        break;
      case IDC_BROWSE_VRML_HTML:
        OnBrowseVrmlDirectory();
        break;
      case IDC_BROWSE_X3D_HTML:
        OnBrowseX3dDirectory();
        break;
      case IDC_BUTTON_DEFAULT:
        OnSetDefaults();
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

void HelpSettingsDialog::LoadData()
{
    const char *helpCommand;
    const char *helpRemoteCommand;
    const char *helpUrl;
    const char *vrmlUrl;
    const char *x3dUrl;
    const char *application;
    const char *topic;

    swHelpBrowserGetSettings(TheApp->GetHelpBrowser(), 
                             &helpCommand, &helpRemoteCommand, 
                             &helpUrl, &vrmlUrl, &x3dUrl, &application, &topic);

    swSetText(swGetDialogItem(m_dlg, IDC_HELP_COMMAND), helpCommand);
    swSetText(swGetDialogItem(m_dlg, IDC_HELP_REMOTE_COMMAND), helpRemoteCommand);
    swSetText(swGetDialogItem(m_dlg, IDC_DOC_DIRECTORY), helpUrl);
    swSetText(swGetDialogItem(m_dlg, IDC_VRML_HTML), vrmlUrl);
    swSetText(swGetDialogItem(m_dlg, IDC_X3D_HTML), x3dUrl);
    swSetText(swGetDialogItem(m_dlg, IDC_APPLICATION), application);
    swSetText(swGetDialogItem(m_dlg, IDC_TOPIC), topic);
}

void
HelpSettingsDialog::SaveData()
{
    MyString helpCommand;
    MyString helpRemoteCommand;
    MyString helpUrl;
    MyString vrmlUrl;
    MyString x3dUrl;
    MyString application;
    MyString topic;

    helpCommand = GetText(swGetDialogItem(m_dlg, IDC_HELP_COMMAND));
    helpRemoteCommand = GetText(swGetDialogItem(m_dlg, IDC_HELP_REMOTE_COMMAND));
    helpUrl = GetText(swGetDialogItem(m_dlg, IDC_DOC_DIRECTORY));
    vrmlUrl = GetText(swGetDialogItem(m_dlg, IDC_VRML_HTML));
    x3dUrl = GetText(swGetDialogItem(m_dlg, IDC_X3D_HTML));
    application = GetText(swGetDialogItem(m_dlg, IDC_APPLICATION));
    topic = GetText(swGetDialogItem(m_dlg, IDC_TOPIC));

    swHelpBrowserSetSettings(TheApp->GetHelpBrowser(), 
                             helpCommand, helpRemoteCommand,
                             helpUrl, vrmlUrl, x3dUrl, application, topic);
}

void HelpSettingsDialog::OnBrowseHelpCommand() 
{
    char buf[1024] = "";
    char buf2[1024] = "";

    if (TheApp->browseCommand(buf, 1024, IDS_HELP_COMMAND_PROMPT)) {
        mysnprintf(buf2, 1024, "%s %%s &", buf);
        swSetText(swGetDialogItem(m_dlg, IDC_HELP_COMMAND), buf2);
    }
}

void HelpSettingsDialog::OnBrowseDocDirectory() 
{
    char buf[1024] = "";

    if (swOpenDirDialog(m_dlg, "Select", 
#ifdef _WIN32
                        "Directory or index.html(*)\0*\0\0",
# else
                        "*.html",
# endif
                        buf, 1024)) {
        swSetText(swGetDialogItem(m_dlg, IDC_DOC_DIRECTORY), buf);
    }
}

void HelpSettingsDialog::OnBrowseVrmlDirectory() 
{
    char buf[1024] = "";

    if (swOpenFileDialog(m_dlg, "Select",
#ifdef _WIN32
                         "nodesRef.html(*.html, *.htm)\0*.html;*.htm\0All Files (*)\0*\0\0",
#else
                         "*.html",
#endif
                         buf, 1024)) {
        swSetText(swGetDialogItem(m_dlg, IDC_VRML_HTML), buf);
    }
}

void HelpSettingsDialog::OnBrowseX3dDirectory() 
{
    char buf[1024] = "";

    if (swOpenDirDialog(m_dlg, "Select",
#ifdef _WIN32
                        "directory containing X3D standard(*)\0;*\0\0",
#else
                         "*",
#endif
                         buf, 1024)) {
        swSetText(swGetDialogItem(m_dlg, IDC_X3D_HTML), buf);
    }
}

void HelpSettingsDialog::OnSetDefaults()
{
    const char *helpCommand;
    const char *helpRemoteCommand;
    const char *helpUrl;
    const char *vrmlUrl;
    const char *x3dUrl;
    const char *application;
    const char *topic;

    swHelpBrowserSetDefaults(TheApp->GetHelpBrowser());

    swHelpBrowserGetSettings(TheApp->GetHelpBrowser(), 
                             &helpCommand, &helpRemoteCommand, 
                             &helpUrl, &vrmlUrl, &x3dUrl, &application, &topic);

    swSetText(swGetDialogItem(m_dlg, IDC_HELP_COMMAND), helpCommand);
    swSetText(swGetDialogItem(m_dlg, IDC_HELP_REMOTE_COMMAND), helpRemoteCommand);
    swSetText(swGetDialogItem(m_dlg, IDC_DOC_DIRECTORY), helpUrl);
    swSetText(swGetDialogItem(m_dlg, IDC_VRML_HTML), vrmlUrl);
    swSetText(swGetDialogItem(m_dlg, IDC_X3D_HTML), x3dUrl);
    swSetText(swGetDialogItem(m_dlg, IDC_APPLICATION), application);
    swSetText(swGetDialogItem(m_dlg, IDC_TOPIC), topic);
}


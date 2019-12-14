/*
 * TexteditSettingsDialog.cpp
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
#include "TexteditSettingsDialog.h"
#include "DuneApp.h"
#include "ExternTheApp.h"
#include "resource.h"
#include <swt.h>

TexteditSettingsDialog::TexteditSettingsDialog(SWND parent)
  : Dialog(parent, IDD_TEXT_OBJECT_EDIT_SETTINGS)
{
    LoadData();
}

void
TexteditSettingsDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    switch(id) {
      case IDC_BROWSE_TEXTEDIT:
        OnBrowseEditCommand(IDC_TEXTEDIT_COMMAND);
        break;
      case IDC_BROWSE_IMAGEEDIT:
        OnBrowseEditCommand(IDC_IMAGEEDIT_COMMAND);
        break;
      case IDC_BROWSE_IMAGEEDIT4KIDS:
        OnBrowseEditCommand(IDC_IMAGEEDIT4KIDS_COMMAND);
        break;
      case IDC_BROWSE_SOUNDEDIT:
        OnBrowseEditCommand(IDC_SOUNDEDIT_COMMAND);
        break;
      case IDC_BROWSE_MOVIEEDIT:
        OnBrowseEditCommand(IDC_MOVIEEDIT_COMMAND);
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

void TexteditSettingsDialog::LoadData()
{
    const char *textEditCommand;
    const char *textEditLinenumberOption;
    int textEditUseExtensionTxt;
    const char *imageEditCommand;
    const char *imageEdit4KidsCommand;
    const char *soundEditCommand;
    const char *movieEditCommand;

    swTextEditGetSettings(TheApp->GetTextedit(), &textEditCommand, 
                          &textEditLinenumberOption, &textEditUseExtensionTxt);
    imageEditCommand = swImageEditGetSettings(TheApp->GetTextedit());
    imageEdit4KidsCommand = swImageEdit4KidsGetSettings(TheApp->GetTextedit());
    soundEditCommand = swSoundEditGetSettings(TheApp->GetTextedit());
    movieEditCommand = swMovieEditGetSettings(TheApp->GetTextedit());

    swSetText(swGetDialogItem(m_dlg, IDC_TEXTEDIT_COMMAND), textEditCommand);
    swSetText(swGetDialogItem(m_dlg, IDC_TEXTEDIT_LINENUMBER_OPTION), 
              textEditLinenumberOption);
    swSetCheck(swGetDialogItem(m_dlg, IDC_TEXTEDIT_USE_EXTENSION_TXT), 
               textEditUseExtensionTxt);
    swSetText(swGetDialogItem(m_dlg, IDC_IMAGEEDIT_COMMAND), imageEditCommand);
    swSetText(swGetDialogItem(m_dlg, IDC_IMAGEEDIT4KIDS_COMMAND), 
                              imageEdit4KidsCommand);
    swSetText(swGetDialogItem(m_dlg, IDC_SOUNDEDIT_COMMAND), soundEditCommand);
    swSetText(swGetDialogItem(m_dlg, IDC_MOVIEEDIT_COMMAND), movieEditCommand);
}

void
TexteditSettingsDialog::SaveData()
{
    MyString textEditCommand;
    MyString textEditLinenumberOption;
    int textEditUseExtensionTxt;
    MyString imageEditCommand;
    MyString imageEdit4KidsCommand;
    MyString soundEditCommand;
    MyString movieEditCommand;

    textEditCommand = GetText(swGetDialogItem(m_dlg, IDC_TEXTEDIT_COMMAND));
    textEditLinenumberOption = GetText(swGetDialogItem(m_dlg, 
                                            IDC_TEXTEDIT_LINENUMBER_OPTION));
    textEditUseExtensionTxt = swGetCheck(swGetDialogItem(m_dlg, 
                                          IDC_TEXTEDIT_USE_EXTENSION_TXT));

    imageEditCommand = GetText(swGetDialogItem(m_dlg, IDC_IMAGEEDIT_COMMAND));
    imageEdit4KidsCommand = GetText(swGetDialogItem(m_dlg, 
                                    IDC_IMAGEEDIT4KIDS_COMMAND));
    soundEditCommand = GetText(swGetDialogItem(m_dlg, IDC_SOUNDEDIT_COMMAND));
    movieEditCommand = GetText(swGetDialogItem(m_dlg, IDC_MOVIEEDIT_COMMAND));
    swTextEditSetSettings(TheApp->GetTextedit(), textEditCommand, 
                          textEditLinenumberOption, textEditUseExtensionTxt);
    swImageEditSetSettings(TheApp->GetTextedit(), imageEditCommand);
    swImageEdit4KidsSetSettings(TheApp->GetTextedit(), imageEdit4KidsCommand);
    swSoundEditSetSettings(TheApp->GetTextedit(), soundEditCommand);
    swMovieEditSetSettings(TheApp->GetTextedit(), movieEditCommand);
}

void 
TexteditSettingsDialog::OnBrowseEditCommand(int idc_edit_command) 
{
    char buf[1026] = "";

    if (TheApp->browseCommand(buf + 1, 1024, IDS_TEXTEDIT_PROMPT))
#ifdef WIN32
        if (strchr(buf + 1, ' ') != NULL) {
            buf[0] = '"';
            int len = strlen(buf);
            buf[len + 1] = 0;
            buf[len] = '"';
            swSetText(swGetDialogItem(m_dlg, idc_edit_command), buf);
        } else
#endif
            swSetText(swGetDialogItem(m_dlg, idc_edit_command), buf + 1);
}

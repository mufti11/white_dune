/*
 * UploadSettingsDialog.cpp
 *
 * Copyright (C) 2003 J. "MUFTI" Scheurich, 1999 Stephen F. White
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
#include "UploadSettingsDialog.h"
#include "DuneApp.h"
#include "ExternTheApp.h"
#include "resource.h"
#include <swt.h>
#ifndef _WIN32
# include <sys/types.h>
# include <sys/mman.h>
#endif

UploadSettingsDialog::UploadSettingsDialog(SWND parent)
  : Dialog(parent, IDD_UPLOAD_SETTINGS)
{
    LoadData();
}

static MyString
GetText(SWND item)
{
    char buf[1024];
    swGetText(item, buf, 1024);
    return buf;
}

void UploadSettingsDialog::LoadData()
{
    const char *commandline;
    const char *htmlTag;
    const char *password;

    swUploadGetSettings(TheApp->GetUpload(), &commandline, &htmlTag, &password);

    swSetText(swGetDialogItem(m_dlg, IDC_UPLOAD_COMMANDLINE), commandline);
    swSetText(swGetDialogItem(m_dlg, IDC_UPLOAD_HTML_TAG), htmlTag);
    swSetText(swGetDialogItem(m_dlg, IDC_UPLOAD_PASSWORD), password);
}

void
UploadSettingsDialog::SaveData()
{
    MyString commandline;
    MyString htmlTag;
    char password[1024];

    commandline = GetText(swGetDialogItem(m_dlg, IDC_UPLOAD_COMMANDLINE));
    htmlTag = GetText(swGetDialogItem(m_dlg, IDC_UPLOAD_HTML_TAG));
#ifdef _WIN32
    VirtualLock(password, 1024);
#else
# ifndef __CYGWIN__
    mlock(password, 1024);
# endif
#endif
    swGetText(swGetDialogItem(m_dlg, IDC_UPLOAD_PASSWORD), password, 1023);


    swUploadSetSettings(TheApp->GetUpload(), commandline, htmlTag, password);
    for (int i = 0; i < 1024; i++)
        password[i] = (char) 0;
#ifdef _WIN32
    VirtualUnlock(password, 1024);
#else
# ifndef __CYGWIN__
    munlock(password, 1024);
# endif
#endif
    // zero copied password variable in memory of GUI element
    swSetTextCleanupPasswd(swGetDialogItem(m_dlg, IDC_UPLOAD_PASSWORD));
}

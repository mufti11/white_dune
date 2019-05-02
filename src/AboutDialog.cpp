/*
 * AboutDialog.cpp
 *
 * Copyright (C) 2000 Stephen F. White
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
#include "AboutDialog.h"
#include "resource.h"
#include "swt.h"
#include "MyString.h"

AboutDialog::AboutDialog(SWND parent)
  : Dialog(parent, IDD_ABOUTBOX)
{
    const char *vendor = (const char *) glGetString(GL_VENDOR);
    const char *renderer = (const char *) glGetString(GL_RENDERER);
    const char *version = (const char *) glGetString(GL_VERSION);
    const char *extensions = (const char *) glGetString(GL_EXTENSIONS);
    swLabelSetText(swGetDialogItem(m_dlg, IDC_GL_VENDOR), vendor);
    swLabelSetText(swGetDialogItem(m_dlg, IDC_GL_RENDERER), renderer);
    swLabelSetText(swGetDialogItem(m_dlg, IDC_GL_VERSION), version);
    MyString ext;
    for (const char *e = extensions; *e; e++) {
        if (*e == ' ')
            ext += swGetPathSeperator();
        else
            ext += *e;
    }
    swSetText(swGetDialogItem(m_dlg, IDC_GL_EXTENSIONS), ext);
}

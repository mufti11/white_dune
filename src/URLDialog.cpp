/*
 * URLDialog.cpp
 *
 * Copyright (C) 2003 J. "MUFTI" Scheurich
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
#include <ctype.h>
#include "resource.h"
#include "MyString.h"
#include "Path.h"
#include "Node.h"
#include "URLDialog.h"
#include "DuneApp.h"

URLDialog::URLDialog(SWND parent)
  : Dialog(parent, IDD_URL)
{
    strcpy(m_path,".");
    m_valid = false;
    LoadData();
}

URLDialog::~URLDialog()
{
}

bool
URLDialog::Validate()
{
    m_valid = true;
    return m_valid;
}

void
URLDialog::LoadData()
{
    swSetText(swGetDialogItem(m_dlg, IDC_URL_PATH), m_path);
}

void
URLDialog::SaveData()
{
    swGetText(swGetDialogItem(m_dlg, IDC_URL_PATH), m_path, 1024);
}


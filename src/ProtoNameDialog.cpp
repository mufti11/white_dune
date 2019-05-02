/*
 * ProtoNameDialog.cpp
 *
 * Copyright (C) 2013 J. "MUFTI" Scheurich
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

#include "stdafx.h"
#include "ProtoNameDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"
#include "DuneApp.h"
#include "NodeBox.h"

ProtoNameDialog::ProtoNameDialog(SWND parent, const char* name)
  : Dialog(parent, IDD_PROTO_NAME)
{
    m_name = "";
    m_name += name;
    LoadData();
}

ProtoNameDialog::~ProtoNameDialog()
{
}


void
ProtoNameDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
}

void
ProtoNameDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_PROTO_NAME), buf, 128);
    m_name = "";
    m_name += buf;
}

bool
ProtoNameDialog::Validate()
{
    if (m_name.length() < 1)
        return false;
    if (isalpha(m_name[0]) == 0)
        return false;
    return true;
}

void
ProtoNameDialog::LoadData()
{
    char buf[128];

    mystrncpy_secure(buf, m_name, 127);
    swSetText(swGetDialogItem(m_dlg, IDC_PROTO_NAME), buf);
}

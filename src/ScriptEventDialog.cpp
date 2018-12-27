/*
 * ScriptEventDialog.cpp
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
#include "ScriptEventDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"
#include "Proto.h"
#include "DuneApp.h"

ScriptEventDialog::ScriptEventDialog(SWND parent, int idd, MyString value,
                                     Node *node)
  : Dialog(parent, idd)
{
    m_value = "";
    m_value += value;
    m_node = node;
    LoadData();
}

ScriptEventDialog::~ScriptEventDialog()
{
}

void
ScriptEventDialog::SaveData()
{
    char buf[1024];
    swGetText(swGetDialogItem(m_dlg, IDC_VALUE), buf, 1023);
    m_value = "";
    m_value += buf;
}

void
ScriptEventDialog::LoadData()
{
    char buf[1024];

    mystrncpy_secure(buf, m_value, 1024);
    swSetText(swGetDialogItem(m_dlg, IDC_VALUE), buf);
}

bool
ScriptEventDialog::Validate(void)
{
    bool ret = true;
    if (m_node->getProto()->lookupEventIn(m_value, false) != -1)
        ret = false;
    if (m_node->getProto()->lookupEventOut(m_value, false) != -1)
        ret = false;
    if (m_node->getProto()->lookupField(m_value, false) != -1)
        ret = false;
    if (m_node->getProto()->lookupExposedField(m_value, false) != -1)
        ret = false;
    if (ret == false) {
        char msg[256];
        strcpy(msg, "event already exist");
        TheApp->MessageBox(msg);
    }        
    if ((m_value[0] >= '0') && (m_value[0] <= '9')) {
        ret = false;
        char msg[256];
        strcpy(msg, "first character may not be a number");
        TheApp->MessageBox(msg);
    }        
    return ret;
}


/*
 * FieldPipeDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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
#include "FieldPipeDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"

FieldPipeDialog::FieldPipeDialog(SWND parent, MyString nodeFilter, 
                                 MyString fieldFilter, MyString command)
  : Dialog(parent, IDD_FIELD_PIPE)
{
    m_nodeFilter = "";
    m_nodeFilter += nodeFilter;
    m_fieldFilter = "";
    m_fieldFilter += fieldFilter;
    m_command = "";
    m_command += command;
    LoadData();
}

FieldPipeDialog::~FieldPipeDialog()
{
}

void
FieldPipeDialog::SaveData()
{
    char buf[1024];
    swGetText(swGetDialogItem(m_dlg, IDC_PIPE_NODE_FILTER), buf, 1023);
    m_nodeFilter = "";
    m_nodeFilter += buf;

    swGetText(swGetDialogItem(m_dlg, IDC_PIPE_FIELD_FILTER), buf, 1023);
    m_fieldFilter = "";
    m_fieldFilter += buf;

    swGetText(swGetDialogItem(m_dlg, IDC_PIPE_COMMAND), buf, 1023);
    m_command = "";
    m_command += buf;
}

bool
FieldPipeDialog::Validate()
{
    if (strchr(m_nodeFilter, ' ') != NULL)
        return false;
    if (strchr(m_fieldFilter, ' ') != NULL)
        return false;
    return true;
}

void
FieldPipeDialog::LoadData()
{
    char buf[1024];

    mystrncpy_secure(buf, m_nodeFilter, 1024);
    swSetText(swGetDialogItem(m_dlg, IDC_PIPE_NODE_FILTER), buf);

    mystrncpy_secure(buf, m_fieldFilter, 1024);
    swSetText(swGetDialogItem(m_dlg, IDC_PIPE_FIELD_FILTER), buf);

    mystrncpy_secure(buf, m_command, 1024);
    swSetText(swGetDialogItem(m_dlg, IDC_PIPE_COMMAND), buf);
}


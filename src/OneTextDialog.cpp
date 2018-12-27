/*
 * OneTextDialog.cpp
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
#include "OneTextDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"

OneTextDialog::OneTextDialog(SWND parent, int idd, MyString value,
                             OneTextValidateCallback callback)
  : Dialog(parent, idd)
{
    m_value = "";
    m_value += value;
    m_callback = callback;
    LoadData();
}

OneTextDialog::~OneTextDialog()
{
}

void
OneTextDialog::SaveData()
{
    char buf[1024];
    swGetText(swGetDialogItem(m_dlg, IDC_VALUE), buf, 1023);
    m_value = "";
    m_value += buf;
}

bool
OneTextDialog::Validate()
{
    if (m_callback == NULL)
        return true;
    return m_callback(m_value);
}

void
OneTextDialog::LoadData()
{
    char buf[1024];

    mystrncpy_secure(buf, m_value, 1024);
    swSetText(swGetDialogItem(m_dlg, IDC_VALUE), buf);
}


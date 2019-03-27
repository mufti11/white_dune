/*
 * TimeShiftDialog.cpp
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
#include "TimeShiftDialog.h"
#include "DuneApp.h"

TimeShiftDialog::TimeShiftDialog(SWND parent)
  : Dialog(parent, IDD_TIME_SHIFT)
{
    m_fraction = 0;
    LoadData();
}

TimeShiftDialog::~TimeShiftDialog()
{
}

bool
TimeShiftDialog::Validate()
{
    if (m_fraction >= 1)
        return false;
    if (m_fraction <= -1)
        return false;
    return true;
}

void
TimeShiftDialog::LoadData()
{
    char buf[128];
    mysnprintf(buf, 128, "%f", m_fraction);
    swSetText(swGetDialogItem(m_dlg, IDC_TIME_SHIFT), buf);
    swSetCheck(swGetDialogItem(m_dlg, IDC_WRAP_AROUND), 1);
}

void
TimeShiftDialog::SaveData()
{
    char buf[128];
    swGetText(swGetDialogItem(m_dlg, IDC_TIME_SHIFT), buf, 128);
    m_fraction = atof(buf);
    if (swGetCheck(swGetDialogItem(m_dlg, IDC_WRAP_AROUND)))
        m_wrapAround = true;
    else
        m_wrapAround = false;

}


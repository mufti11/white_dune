/*
 * OneIntDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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
#include "OneIntDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"

OneIntDialog::OneIntDialog(SWND parent, int idd, int value,
                           int lowerlimit, int upperlimit)
  : Dialog(parent, idd)
{
    m_value = value;
    m_lowerlimit = lowerlimit;
    m_upperlimit = upperlimit;
    if (m_upperlimit < m_lowerlimit) {
        m_lowerlimit = INT_MIN;
        m_upperlimit = INT_MAX;
    }
    LoadData();
}

OneIntDialog::~OneIntDialog()
{
}

void
OneIntDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_VALUE), buf, 128);
    m_value = atoi(buf);
}

bool
OneIntDialog::Validate()
{
    return (m_value >= m_lowerlimit) && (m_value <= m_upperlimit);
}

void
OneIntDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%d", m_value);
    swSetText(swGetDialogItem(m_dlg, IDC_VALUE), buf);
}


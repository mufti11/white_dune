/*
 * OneFloatDialog.cpp
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
#include "OneFloatDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"

OneFloatDialog::OneFloatDialog(SWND parent, int idd, float value,
                               float lowerlimit, float upperlimit)
  : Dialog(parent, idd)
{
    m_value = value;
    m_lowerlimit = lowerlimit;
    m_upperlimit = upperlimit;
    if (m_upperlimit < m_lowerlimit) {
        m_lowerlimit = FLT_MIN;
        m_upperlimit = FLT_MAX;
    }
    LoadData();
}

OneFloatDialog::~OneFloatDialog()
{
}

void
OneFloatDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_VALUE), buf, 128);
    m_value = atof(buf);
}

bool
OneFloatDialog::Validate()
{
    return (m_value >= m_lowerlimit) && (m_value <= m_upperlimit);
}

void
OneFloatDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%f", m_value);
    swSetText(swGetDialogItem(m_dlg, IDC_VALUE), buf);
}


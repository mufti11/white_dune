/*
 * OneVec3fDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2020 J. "MUFTI" Scheurich  
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
#include "OneVec3fDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"

OneVec3fDialog::OneVec3fDialog(SWND parent, int idd, Vec3f value,
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

OneVec3fDialog::~OneVec3fDialog()
{
}

void
OneVec3fDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_VALUE_X), buf, 128);
    m_value.x = atof(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_VALUE_Y), buf, 128);
    m_value.y = atof(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_VALUE_Z), buf, 128);
    m_value.z = atof(buf);
}

bool
OneVec3fDialog::Validate()
{
    if ((m_value.x >= m_lowerlimit) && (m_value.x <= m_upperlimit))
        return true;
    if ((m_value.y >= m_lowerlimit) && (m_value.y <= m_upperlimit))
        return true;
    if ((m_value.z >= m_lowerlimit) && (m_value.z <= m_upperlimit))
        return true;
    return false;
}

void
OneVec3fDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%f", m_value.x);
    swSetText(swGetDialogItem(m_dlg, IDC_VALUE_X), buf);
    mysnprintf(buf, 128, "%f", m_value.y);
    swSetText(swGetDialogItem(m_dlg, IDC_VALUE_Y), buf);
    mysnprintf(buf, 128, "%f", m_value.z);
    swSetText(swGetDialogItem(m_dlg, IDC_VALUE_Z), buf);
}


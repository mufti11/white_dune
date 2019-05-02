/*
 * NurbsPlaneDialog.cpp
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
#include "NurbsPlaneDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"

NurbsPlaneDialog::NurbsPlaneDialog(SWND parent, int idd, int n1, int n2, 
                                   int uDegree, int vDegree)
  : Dialog(parent, idd)
{
    m_n1 = n1;
    m_n2 = n2;
    m_uDegree = uDegree;
    m_vDegree = vDegree;
    LoadData();
}

NurbsPlaneDialog::~NurbsPlaneDialog()
{
}

void
NurbsPlaneDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_N1), buf, 128);
    m_n1 = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_N2), buf, 128);
    m_n2 = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_U_DEGREE), buf, 128);
    m_uDegree = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_V_DEGREE), buf, 128);
    m_vDegree = atoi(buf);
}

bool
NurbsPlaneDialog::Validate()
{
    return m_uDegree > 0 && m_vDegree > 0 &&
           m_n1 > m_uDegree && m_n2 > m_vDegree;
}

void
NurbsPlaneDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%d", m_n1);
    swSetText(swGetDialogItem(m_dlg, IDC_N1), buf);
    mysnprintf(buf, 128, "%d", m_n2);
    swSetText(swGetDialogItem(m_dlg, IDC_N2), buf);
    mysnprintf(buf, 128, "%d", m_uDegree);
    swSetText(swGetDialogItem(m_dlg, IDC_U_DEGREE), buf);
    mysnprintf(buf, 128, "%d", m_vDegree);
    swSetText(swGetDialogItem(m_dlg, IDC_V_DEGREE), buf);
}

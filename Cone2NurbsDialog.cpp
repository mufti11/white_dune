/*
 * Cone2NurbsDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2003 Thomas Rothermel
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
#include "Cone2NurbsDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"
#include "DuneApp.h"
#include "NodeCone.h"

Cone2NurbsDialog::Cone2NurbsDialog(SWND parent, 
                                   Node* node,
                                   int narcs, int narea, int nshell,
                                   int uDegree, int vDegree)
  : Dialog(parent, IDD_CONE2NURBS)
{
    m_narcs = narcs;
    m_narea = narea;
    m_nshell = nshell;
    m_uDegree = uDegree;
    m_vDegree = vDegree;
    m_side = 0;
    if (((NodeCone *)node)->side()->getValue())
        m_side = 1;
    m_bottom = 0;
    if (((NodeCone *)node)->bottom()->getValue())
        m_bottom = 1;
    LoadData();
}

Cone2NurbsDialog::~Cone2NurbsDialog()
{
}

void
Cone2NurbsDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_CONE_NARCS), buf, 128);
    m_narcs = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_CONE_NAREA), buf, 128);
    m_narea = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_CONE_NSHELL), buf, 128);
    m_nshell = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_CONE_U_DEGREE), buf, 128);
    m_uDegree = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_CONE_V_DEGREE), buf, 128);
    m_vDegree = atoi(buf);
}

bool
Cone2NurbsDialog::Validate()
{
    bool valid = true;
    if (m_narcs < 3)
        valid = false;
    if (m_nshell < 2)
        valid = false;
    if (m_narea < 2)
        valid = false;
    if (m_vDegree > (m_bottom * m_narea + m_side * m_nshell - 1)) {
        valid = false;
        TheApp->MessageBoxId(IDS_VDEGREE_UNSUPPORTED_CONE);
    }
    if (valid)
        valid =  m_uDegree > 0 && m_vDegree > 0;
    return valid;
}

void
Cone2NurbsDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%d", m_narcs);
    swSetText(swGetDialogItem(m_dlg, IDC_CONE_NARCS), buf);
    mysnprintf(buf, 128, "%d", m_narea);
    swSetText(swGetDialogItem(m_dlg, IDC_CONE_NAREA), buf);
    mysnprintf(buf, 128, "%d", m_nshell);
    swSetText(swGetDialogItem(m_dlg, IDC_CONE_NSHELL), buf);
    mysnprintf(buf, 128, "%d", m_uDegree);
    swSetText(swGetDialogItem(m_dlg, IDC_CONE_U_DEGREE), buf);
    mysnprintf(buf, 128, "%d", m_vDegree);
    swSetText(swGetDialogItem(m_dlg, IDC_CONE_V_DEGREE), buf);
}

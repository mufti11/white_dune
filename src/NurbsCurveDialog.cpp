/*
 * NurbsCurveDialog.cpp
 *
 * Copyright (C) 2000 Stephen F. White, 2003 Th. Rothermel
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
#include "NurbsCurveDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"

NurbsCurveDialog::NurbsCurveDialog(SWND parent, int nPoints, int degree,
                                   int direction, bool keepDegree)
  : Dialog(parent, IDD_NEW_NURBS_CURVE)
{
    m_nPoints = nPoints;
    m_degree = degree;
    m_direction = direction;
    m_keepDegree = keepDegree;
    LoadData();
}

NurbsCurveDialog::~NurbsCurveDialog()
{
}

void
NurbsCurveDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_NPOINTS), buf, 128);
    m_nPoints = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_DEGREE), buf, 128);
    m_degree = atoi(buf);
    if (swGetCheck(swGetDialogItem(m_dlg, IDC_NURBS_CURVE_X)))
        m_direction = 0;
    else if (swGetCheck(swGetDialogItem(m_dlg, IDC_NURBS_CURVE_Z)))
        m_direction = 2;
    else 
        m_direction = 1;
}

bool
NurbsCurveDialog::Validate()
{
    bool valid = true;
    if (m_nPoints < 2) {
        valid = false;
    }
    if (m_degree > (m_nPoints-1)) {
        valid = false;
    }
    return valid;
}

void
NurbsCurveDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%d", m_nPoints);
    swSetText(swGetDialogItem(m_dlg, IDC_NPOINTS), buf);
    mysnprintf(buf, 128, "%d", m_degree);
    swSetText(swGetDialogItem(m_dlg, IDC_DEGREE), buf);
    swSetCheck(swGetDialogItem(m_dlg, IDC_NURBS_CURVE_X), m_direction==0 ? 1 : 0);
    swSetCheck(swGetDialogItem(m_dlg, IDC_NURBS_CURVE_Y), m_direction==1 ? 1 : 0);
    swSetCheck(swGetDialogItem(m_dlg, IDC_NURBS_CURVE_Z), m_direction==2 ? 1 : 0);
    if (m_keepDegree)
        swEnableWindow(swGetDialogItem(m_dlg, IDC_DEGREE), false);
}

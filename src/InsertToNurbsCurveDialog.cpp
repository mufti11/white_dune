/*
 * InsertToNurbsCurveDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2003 Thomas Rothermel
 *               2019 J. "MUFTI" Scheurich
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
#include "InsertToNurbsCurveDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"
#include "DuneApp.h"

InsertToNurbsCurveDialog::InsertToNurbsCurveDialog(SWND parent, 
                                       int from, int to) 
  : Dialog(parent, IDD_INSERT_TO_NURBS_CURVE)
{
    m_from = from;
    m_to = to;
    m_points = 2;
    LoadData();
}

InsertToNurbsCurveDialog::~InsertToNurbsCurveDialog()
{
}

void
InsertToNurbsCurveDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_NURBS_FROM), buf, 128);
    m_from = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_NURBS_TO), buf, 128);
    m_to = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_NURBS_POINTS), buf, 128);
    m_points = atoi(buf);
}

bool
InsertToNurbsCurveDialog::Validate()
{
    return true;
}

void
InsertToNurbsCurveDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%d", m_from);
    swSetText(swGetDialogItem(m_dlg, IDC_NURBS_FROM), buf);
    mysnprintf(buf, 128, "%d", m_to);
    swSetText(swGetDialogItem(m_dlg, IDC_NURBS_TO), buf);
    mysnprintf(buf, 128, "%d", m_points);
    swSetText(swGetDialogItem(m_dlg, IDC_NURBS_POINTS), buf);
}

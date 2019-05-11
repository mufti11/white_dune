/*
 * InsertToNurbsSurfaceDialog.cpp
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
#include "InsertToNurbsSurfaceDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"
#include "DuneApp.h"

InsertToNurbsSurfaceDialog::InsertToNurbsSurfaceDialog(SWND parent, 
                                                       int uFrom, int uTo, 
                                                       int vFrom, int vTo) 
  : Dialog(parent, IDD_INSERT_TO_NURBS_SURFACE)
{
    m_uFrom = uFrom;
    m_uTo = uTo;
    m_uPoints = 2;
    m_vFrom = vFrom;
    m_vTo = vTo;
    m_vPoints = 2;
    LoadData();
}

InsertToNurbsSurfaceDialog::~InsertToNurbsSurfaceDialog()
{
}

void
InsertToNurbsSurfaceDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_NURBS_FROM_U), buf, 128);
    m_uFrom = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_NURBS_TO_U), buf, 128);
    m_uTo = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_NURBS_POINTS_U), buf, 128);
    m_uPoints = atoi(buf);

    swGetText(swGetDialogItem(m_dlg, IDC_NURBS_FROM_V), buf, 128);
    m_vFrom = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_NURBS_TO_V), buf, 128);
    m_vTo = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_NURBS_POINTS_V), buf, 128);
    m_vPoints = atoi(buf);
}

bool
InsertToNurbsSurfaceDialog::Validate()
{
    return true;
}

void
InsertToNurbsSurfaceDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%d", m_uFrom);
    swSetText(swGetDialogItem(m_dlg, IDC_NURBS_FROM_U), buf);
    mysnprintf(buf, 128, "%d", m_uTo);
    swSetText(swGetDialogItem(m_dlg, IDC_NURBS_TO_U), buf);
    mysnprintf(buf, 128, "%d", m_uPoints);
    swSetText(swGetDialogItem(m_dlg, IDC_NURBS_POINTS_U), buf);

    mysnprintf(buf, 128, "%d", m_vFrom);
    swSetText(swGetDialogItem(m_dlg, IDC_NURBS_FROM_V), buf);
    mysnprintf(buf, 128, "%d", m_vTo);
    swSetText(swGetDialogItem(m_dlg, IDC_NURBS_TO_V), buf);
    mysnprintf(buf, 128, "%d", m_vPoints);
    swSetText(swGetDialogItem(m_dlg, IDC_NURBS_POINTS_V), buf);
}

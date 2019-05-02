/*
 * NurbsCurve2DDialog.cpp
 *
 * Copyright (C) 2000 Stephen F. White, 2018 J. "MUFTI" Scheurich
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
#include "NurbsCurve2DDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"
#include "Scene.h"
#include "NodeViewpoint.h"

NurbsCurve2DDialog::NurbsCurve2DDialog(SWND parent, int idd, Scene *scene,
                                       int points)
  : Dialog(parent, idd)
{
    m_size = 1;
    m_ccw = false;  
    m_points = points;
    LoadData();
}

NurbsCurve2DDialog::~NurbsCurve2DDialog()
{
}

void
NurbsCurve2DDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_NURBS_CURVE_2D_SIZE), buf, 128);
    m_size = atof(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_NURBS_CURVE_2D_POINTS), buf, 128);
    m_points = atoi(buf);
    m_ccw = swGetCheck(swGetDialogItem(m_dlg, IDC_NURBS_CURVE_2D_CCW));
}

bool
NurbsCurve2DDialog::Validate()
{
    if (m_size <= 0)
        return false;
    if (m_points <= 2)
        return false;
    return true;
}

void
NurbsCurve2DDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%f", m_size);
    swSetText(swGetDialogItem(m_dlg, IDC_NURBS_CURVE_2D_SIZE), buf);
    mysnprintf(buf, 128, "%d", m_points);
    swSetText(swGetDialogItem(m_dlg, IDC_NURBS_CURVE_2D_POINTS), buf);
    swSetCheck(swGetDialogItem(m_dlg, IDC_NURBS_CURVE_2D_CCW), 1);
}

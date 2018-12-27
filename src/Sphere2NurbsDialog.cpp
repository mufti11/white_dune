/*
 * Sphere2NurbsDialog.cpp
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
#include "Sphere2NurbsDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"
#include "DuneApp.h"

Sphere2NurbsDialog::Sphere2NurbsDialog(SWND parent, 
                                       int narcslong, int narcslat, 
                                       int uDegree, int vDegree,
                                       bool spherical)
  : Dialog(parent, IDD_SPHERE2NURBS)
{
    m_narcslong = narcslong;
    m_narcslat = narcslat;
    m_uDegree = uDegree;
    m_vDegree = vDegree;
    m_spherical = spherical;
    LoadData();
}

Sphere2NurbsDialog::~Sphere2NurbsDialog()
{
}

void
Sphere2NurbsDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_SPHERE_NARCSLONG), buf, 128);
    m_narcslong = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_SPHERE_NARCSLAT), buf, 128);
    m_narcslat = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_SPHERE_U_DEGREE), buf, 128);
    m_uDegree = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_SPHERE_V_DEGREE), buf, 128);
    m_vDegree = atoi(buf);
}

bool
Sphere2NurbsDialog::Validate()
{
    bool valid = true;
    if (m_spherical) {
        if (m_narcslong < 3)
            valid = false;
    } else {
        if (m_narcslong < 2)
            valid = false;
    }
    if (m_narcslat < 3)
        valid = false;
    if (valid)
        valid =  m_uDegree > 0 && m_vDegree > 0;
    return valid;

}

void
Sphere2NurbsDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%d", m_narcslong);
    swSetText(swGetDialogItem(m_dlg, IDC_SPHERE_NARCSLONG), buf);
    mysnprintf(buf, 128, "%d", m_narcslat);
    swSetText(swGetDialogItem(m_dlg, IDC_SPHERE_NARCSLAT), buf);
    mysnprintf(buf, 128, "%d", m_uDegree);
    swSetText(swGetDialogItem(m_dlg, IDC_SPHERE_U_DEGREE), buf);
    mysnprintf(buf, 128, "%d", m_vDegree);
    swSetText(swGetDialogItem(m_dlg, IDC_SPHERE_V_DEGREE), buf);
}

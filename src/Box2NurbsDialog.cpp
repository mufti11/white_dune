/*
 * Box2NurbsDialog.cpp
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
#include "Box2NurbsDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"
#include "DuneApp.h"
#include "NodeBox.h"

Box2NurbsDialog::Box2NurbsDialog(SWND parent, 
                                 int nuAreaPoints,
                                 int nvAreaPoints,
                                 int nzAreaPoints,
                                 int uDegree, int vDegree)
  : Dialog(parent, IDD_BOX2NURBS)
{
    m_nuAreaPoints = nuAreaPoints;
    m_nvAreaPoints = nvAreaPoints;
    m_nzAreaPoints = nzAreaPoints;
    m_uDegree = uDegree;
    m_vDegree = vDegree;
    m_have6Planes = false;
    LoadData();
}

Box2NurbsDialog::~Box2NurbsDialog()
{
}

void
Box2NurbsDialog::SaveData()
{
    char buf[128];

    if (swGetCheck(swGetDialogItem(m_dlg, IDC_6PLANES)))
        m_have6Planes = true;
    else
        m_have6Planes = false;
    swGetText(swGetDialogItem(m_dlg, IDC_BOX_NZAREAPOINTS), buf, 128);
    m_nzAreaPoints = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_BOX_NUAREAPOINTS), buf, 128);
    m_nvAreaPoints = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_BOX_NVAREAPOINTS), buf, 128);
    m_nvAreaPoints = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_BOX_U_DEGREE), buf, 128);
    m_uDegree = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_BOX_V_DEGREE), buf, 128);
    m_vDegree = atoi(buf);
}

bool
Box2NurbsDialog::Validate()
{

   bool valid = true;

   if (m_have6Planes) {
      if ((m_uDegree < 1) || (m_vDegree < 1))
          valid = false;
      if (m_nvAreaPoints < 2)
          valid = false;
      if (m_uDegree > (m_nvAreaPoints - 1))
          valid = false;
      if (m_nvAreaPoints < 2)
          valid = false;
      if (m_vDegree > (m_nvAreaPoints - 1))
          valid = false;
   } else {
       if ((m_uDegree < 1) || (m_vDegree < 1))
          valid = false;
       if (m_nvAreaPoints < 2)
          valid = false;
       if (m_uDegree > (m_nvAreaPoints - 1))
          valid = false;
       if (m_nvAreaPoints < 2)
          valid = false;
       if (m_vDegree > (m_nvAreaPoints - 1))
          valid = false;
       if (m_nzAreaPoints < 2)     
          valid = false;
    }
    if (valid)
        valid =  m_uDegree > 0 && m_vDegree > 0;
    return valid;
}

void
Box2NurbsDialog::LoadData()
{
    char buf[128];

    swSetCheck(swGetDialogItem(m_dlg, IDC_6PLANES), m_have6Planes ? 1 : 0);
    mysnprintf(buf, 128, "%d", m_nvAreaPoints);
    swSetText(swGetDialogItem(m_dlg, IDC_BOX_NUAREAPOINTS), buf);
    mysnprintf(buf, 128, "%d", m_nvAreaPoints);
    swSetText(swGetDialogItem(m_dlg, IDC_BOX_NVAREAPOINTS), buf);
    mysnprintf(buf, 128, "%d", m_nzAreaPoints);
    swSetText(swGetDialogItem(m_dlg, IDC_BOX_NZAREAPOINTS), buf);
    mysnprintf(buf, 128, "%d", m_uDegree);
    swSetText(swGetDialogItem(m_dlg, IDC_BOX_U_DEGREE), buf);
    mysnprintf(buf, 128, "%d", m_vDegree);
    swSetText(swGetDialogItem(m_dlg, IDC_BOX_V_DEGREE), buf);
}

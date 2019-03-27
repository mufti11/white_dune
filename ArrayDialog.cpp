/*
 * ArrayDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2004 J "MUFTI" Scheurich
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
#include "ArrayDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"
#include "DuneApp.h"
#include "NodeCone.h"

ArrayDialog::ArrayDialog(SWND parent)
  : Dialog(parent, IDD_ARRAY)
{
    m_numberCopies = 1;
    m_scale.setValue(0, 1);
    m_scale.setValue(1, 1);
    m_scale.setValue(2, 1);
    LoadData();
}

ArrayDialog::~ArrayDialog()
{
}

void
ArrayDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_ARRAY_COPYIES), buf, 128);
    m_numberCopies=atoi(buf);

    swGetText(swGetDialogItem(m_dlg, IDC_TRANS_X), buf, 128);
    m_translation.setValue(0, atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_TRANS_Y), buf, 128);
    m_translation.setValue(1, atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_TRANS_Z), buf, 128);
    m_translation.setValue(2, atof(buf));

    swGetText(swGetDialogItem(m_dlg, IDC_ROT_X), buf, 128);
    m_rotation.setValue(0, atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_ROT_Y), buf, 128);
    m_rotation.setValue(1, atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_ROT_Z), buf, 128);
    m_rotation.setValue(2, atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_ROT_ANGLE), buf, 128);
    m_rotation.setValue(3, atof(buf));
    m_rotation.normalize();

    swGetText(swGetDialogItem(m_dlg, IDC_CENTER_X), buf, 128);
    m_center.setValue(0, atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_CENTER_Y), buf, 128);
    m_center.setValue(1, atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_CENTER_Z), buf, 128);
    m_center.setValue(2, atof(buf));

    swGetText(swGetDialogItem(m_dlg, IDC_SCALE_X), buf, 128);
    m_scale.setValue(0, atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_SCALE_Y), buf, 128);
    m_scale.setValue(1, atof(buf));
    swGetText(swGetDialogItem(m_dlg, IDC_SCALE_Z), buf, 128);
    m_scale.setValue(2, atof(buf));
}

bool
ArrayDialog::Validate()
{
    bool valid = true;
    if (m_numberCopies < 1)
        valid = false;
    for (int i = 0 ; i < 3; i++)
        if (m_scale.getValue(i) <= 0) {
            TheApp->MessageBoxId(IDS_NEGATIVE_SCALE_UNSUPPORTED);
            return false;
        }
    bool changed = false;
    for (int i = 0 ; i < 3; i++)
        if (m_translation.getValue(i) !=  0)
            changed = true;
    if (m_rotation.getValue(3) != 0)
        changed = true;
    for (int i = 0 ; i < 3; i++)
        if (m_scale.getValue(i) != 1)
            changed = true;
    if (!changed)
        valid = false;
    return valid;
}

void
ArrayDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%d", m_numberCopies);
    swSetText(swGetDialogItem(m_dlg, IDC_ARRAY_COPYIES), buf);

    mysnprintf(buf, 128, "%g", m_translation.getValue(0));
    swSetText(swGetDialogItem(m_dlg, IDC_TRANS_X), buf);
    mysnprintf(buf, 128, "%g", m_translation.getValue(1));
    swSetText(swGetDialogItem(m_dlg, IDC_TRANS_Y), buf);
    mysnprintf(buf, 128, "%g", m_translation.getValue(2));
    swSetText(swGetDialogItem(m_dlg, IDC_TRANS_Z), buf);

    mysnprintf(buf, 128, "%g", m_rotation.getValue(0));
    swSetText(swGetDialogItem(m_dlg, IDC_ROT_X), buf);
    mysnprintf(buf, 128, "%g", m_rotation.getValue(1));
    swSetText(swGetDialogItem(m_dlg, IDC_ROT_Y), buf);
    mysnprintf(buf, 128, "%g", m_rotation.getValue(2));
    swSetText(swGetDialogItem(m_dlg, IDC_ROT_Z), buf);
    mysnprintf(buf, 128, "%g", m_rotation.getValue(3));
    swSetText(swGetDialogItem(m_dlg, IDC_ROT_ANGLE), buf);

    mysnprintf(buf, 128, "%g", m_center.getValue(0));
    swSetText(swGetDialogItem(m_dlg, IDC_CENTER_X), buf);
    mysnprintf(buf, 128, "%g", m_center.getValue(1));
    swSetText(swGetDialogItem(m_dlg, IDC_CENTER_Y), buf);
    mysnprintf(buf, 128, "%g", m_center.getValue(2));
    swSetText(swGetDialogItem(m_dlg, IDC_CENTER_Z), buf);

    mysnprintf(buf, 128, "%g", m_scale.getValue(0));
    swSetText(swGetDialogItem(m_dlg, IDC_SCALE_X), buf);
    mysnprintf(buf, 128, "%g", m_scale.getValue(1));
    swSetText(swGetDialogItem(m_dlg, IDC_SCALE_Y), buf);
    mysnprintf(buf, 128, "%g", m_scale.getValue(2));
    swSetText(swGetDialogItem(m_dlg, IDC_SCALE_Z), buf);
}

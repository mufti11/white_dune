/*
 * OneBoolDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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
#include "OneBoolDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"

OneBoolDialog::OneBoolDialog(SWND parent, int idd, bool value)
  : Dialog(parent, idd)
{
    m_value = value;
    LoadData();
}

OneBoolDialog::~OneBoolDialog()
{
}

void
OneBoolDialog::SaveData()
{
   if (swGetCheck(swGetDialogItem(m_dlg, IDC_VALUE)))
        m_value = true;
    else
        m_value = false;
}

void
OneBoolDialog::LoadData()
{
    swSetCheck(swGetDialogItem(m_dlg, IDC_VALUE), m_value);
}


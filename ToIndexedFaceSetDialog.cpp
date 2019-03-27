/*
 * ToIndexedFaceSetDialog.cpp
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
#include "ToIndexedFaceSetDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"

ToIndexedFaceSetDialog::ToIndexedFaceSetDialog(SWND parent)
  : Dialog(parent, IDD_TO_INDEXED_FACE_SET)
{
    m_triangulate = false;
    LoadData();
}

ToIndexedFaceSetDialog::~ToIndexedFaceSetDialog()
{
}

void
ToIndexedFaceSetDialog::SaveData()
{
    if (swGetCheck(swGetDialogItem(m_dlg, IDC_TRIANGULATE)))
        m_triangulate = true;
    else
        m_triangulate = false;
}

void
ToIndexedFaceSetDialog::LoadData()
{
    swSetCheck(swGetDialogItem(m_dlg, IDC_TRIANGULATE), 
                               false);
}

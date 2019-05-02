/*
 * Dialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White
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
#include "Dialog2.h"
#include "swt.h"
#include "resource.h"

static void
commandCB(void *data, int id)
{
    Dialog *dlg = (Dialog *) data;

    dlg->OnCommand(id);
}

Dialog::Dialog(SWND parent, int id)
{
    _parent = parent;
    _dlg = swLoadDialog(parent, id);
    swSetCommandCallback(_dlg, commandCB);
    swSetClientData(_dlg, this);
}

void
Dialog::OnCommand(int id)
{
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
///////////////////////////AUFGABE1//////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
}

void
Dialog::SaveData()
{
}

bool
Dialog::Validate()
{
    return true;
}

Dialog::~Dialog()
{
    swDestroyWindow(_dlg);
}

int
Dialog::DoModal()
{
    return swDialog(_dlg);
}

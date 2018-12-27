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
#include "Dialog.h"
#include "swt.h"
#include "DuneApp.h"
#include "resource.h"

static void
commandCB(void *data, void *id)
{
    Dialog *dlg = (Dialog *) data;

    dlg->OnCommand(id);
}

Dialog::Dialog(SWND parent, int id)
{
    m_parent = parent;
    m_dlg = swLoadDialog(parent, id + swGetLang());
    swSetCommandCallback(m_dlg, commandCB);
    swSetClientData(m_dlg, this);
}

void
Dialog::OnCommand(void *id)
{
    if (id == swToPtr(IDOK)) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == swToPtr(IDCANCEL)) {
        swEndDialog(IDCANCEL);
    }
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
    swDestroyWindow(m_dlg);
}

int
Dialog::DoModal()
{
    return swDialog(m_dlg);
}

// alternative Dialog with IDOK2, IDCANCEL2 for stacked dialogs
static void
commandCB2(void *data, void *id)
{
    Dialog *dlg = (Dialog *) data;

    dlg->OnCommand2(id);
}

void Dialog::setDialog2(void)
{
    swSetCommandCallback(m_dlg, commandCB2);
}

void
Dialog::OnCommand2(void *id)
{
    if (id == swToPtr(IDOK2)) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == swToPtr(IDCANCEL2)) {
        swEndDialog(IDCANCEL);
    }
}


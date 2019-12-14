/*
 * EcmaScriptSettingsDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2003 J. "MUFTI" Scheurich
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
#include "EcmaScriptSettingsDialog.h"
#include "DuneApp.h"
#include "ExternTheApp.h"
#include "resource.h"
#include "swt.h"

EcmaScriptSettingsDialog::EcmaScriptSettingsDialog(SWND parent)
  : Dialog(parent, IDD_ECMA_SCRIPT_SETTINGS)
{
    LoadData();
}

void EcmaScriptSettingsDialog::LoadData()
{
    swSetCheck(swGetDialogItem(m_dlg, IDC_ECMA_ADD_ALLOWED_VALUES),
               TheApp->GetEcmaScriptAddAllowedValues());

    swSetCheck(swGetDialogItem(m_dlg, IDC_ECMA_ADD_ALLOWED_COMPONENTS),
               TheApp->GetEcmaScriptAddAllowedComponents());

    swSetCheck(swGetDialogItem(m_dlg, IDC_ECMA_ADD_AVAILABLE_FUNCTIONS),
               TheApp->GetEcmaScriptAddAvailableFunctions());

    swSetCheck(swGetDialogItem(m_dlg, IDC_ECMA_ADD_MATH_OBJECT),
               TheApp->GetEcmaScriptAddMathObject());

    swSetCheck(swGetDialogItem(m_dlg, IDC_ECMA_ADD_BROWSER_OBJECT),
               TheApp->GetEcmaScriptAddBrowserObject());

    swSetCheck(swGetDialogItem(m_dlg, IDC_ECMA_ADD_EXAMPLE_USAGE),
               TheApp->GetEcmaScriptAddExampleUsage());
}

void
EcmaScriptSettingsDialog::SaveData() 
{
    SWND wnd;

    wnd = swGetDialogItem(m_dlg, IDC_ECMA_ADD_ALLOWED_VALUES);
    TheApp->SetEcmaScriptAddAllowedValues(swGetCheck(wnd) != FALSE);

    wnd = swGetDialogItem(m_dlg, IDC_ECMA_ADD_ALLOWED_COMPONENTS);
    TheApp->SetEcmaScriptAddAllowedComponents(swGetCheck(wnd) != FALSE);

    wnd = swGetDialogItem(m_dlg, IDC_ECMA_ADD_AVAILABLE_FUNCTIONS);
    TheApp->SetEcmaScriptAddAvailableFunctions(swGetCheck(wnd) != FALSE);

    wnd = swGetDialogItem(m_dlg, IDC_ECMA_ADD_MATH_OBJECT);
    TheApp->SetEcmaScriptAddMathObject(swGetCheck(wnd) != FALSE);

    wnd = swGetDialogItem(m_dlg, IDC_ECMA_ADD_BROWSER_OBJECT);
    TheApp->SetEcmaScriptAddBrowserObject(swGetCheck(wnd) != FALSE);

    wnd = swGetDialogItem(m_dlg, IDC_ECMA_ADD_EXAMPLE_USAGE);
    TheApp->SetEcmaScriptAddExampleUsage(swGetCheck(wnd) != FALSE);
}

/*
 * EcmaScriptApp.cpp
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

#include "swt.h"
#include "PreferencesApp.h"
#include "EcmaScriptApp.h"
#include "DuneApp.h"
#include "ExternTheApp.h"

EcmaScriptApp::EcmaScriptApp()
{
    m_ecmaScriptAddAvailableFunctions = false;
    m_ecmaScriptAddAllowedValues      = false;
    m_ecmaScriptAddAllowedComponents  = false;
    m_ecmaScriptAddExampleUsage       = false;
    m_ecmaScriptAddMathObject         = false;
    m_ecmaScriptAddBrowserObject      = false;
}

void EcmaScriptApp::EcmaScriptLoadPreferences()
{
    assert(TheApp != NULL);
    m_ecmaScriptAddAvailableFunctions = TheApp->GetBoolPreference(
                                        "EcmaScriptAddAvailableFunctions", false);
    m_ecmaScriptAddAllowedValues      = TheApp->GetBoolPreference(
                                        "EcmaScriptAddAllowedValues", false);
    m_ecmaScriptAddAllowedComponents  = TheApp->GetBoolPreference(
                                        "EcmaScriptAddAllowedComponents", false);
    m_ecmaScriptAddExampleUsage       = TheApp->GetBoolPreference(
                                        "EcmaScriptAddExampleUsage", false);
    m_ecmaScriptAddMathObject         = TheApp->GetBoolPreference(
                                        "EcmaScriptAddMathObject", false);
    m_ecmaScriptAddBrowserObject      = TheApp->GetBoolPreference(
                                        "EcmaScriptAddBrowserObject", false);
}

void EcmaScriptApp::EcmaScriptSavePreferences(void)
{
    TheApp->SetBoolPreference("EcmaScriptAddAvailableFunctions",
                              m_ecmaScriptAddAvailableFunctions);
    TheApp->SetBoolPreference("EcmaScriptAddAllowedValues",
                              m_ecmaScriptAddAllowedValues);
    TheApp->SetBoolPreference("EcmaScriptAddAllowedComponents",
                              m_ecmaScriptAddAllowedComponents);
    TheApp->SetBoolPreference("EcmaScriptAddExampleUsage",
                              m_ecmaScriptAddExampleUsage);
    TheApp->SetBoolPreference("EcmaScriptAddMathObject",
                              m_ecmaScriptAddMathObject);
    TheApp->SetBoolPreference("EcmaScriptAddBrowserObject", 
                              m_ecmaScriptAddBrowserObject);
}


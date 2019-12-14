/*
 * EcmaScriptApp.h
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

#pragma once

class EcmaScriptApp {
public:
                        EcmaScriptApp();
    void                EcmaScriptLoadPreferences();
    bool                GetEcmaScriptAddInitialise() const 
                              { return m_ecmaScriptAddInitialise; }
    bool                GetEcmaScriptAddShutdown() const 
                              { return m_ecmaScriptAddShutdown; }
    bool                GetEcmaScriptAddEventsProcessed() const 
                              { return m_ecmaScriptAddEventsProcessed; }
    bool                GetEcmaScriptAddAvailableFunctions() const 
                              { return m_ecmaScriptAddAvailableFunctions; }
    bool                GetEcmaScriptAddAllowedValues() const 
                              { return m_ecmaScriptAddAllowedValues; }
    bool                GetEcmaScriptAddAllowedComponents() const 
                              { return m_ecmaScriptAddAllowedComponents; }
    bool                GetEcmaScriptAddExampleUsage() const 
                              { return m_ecmaScriptAddExampleUsage; }

    bool                GetEcmaScriptAddMathObject() const
                              { return m_ecmaScriptAddMathObject; }
    bool                GetEcmaScriptAddBrowserObject() const
                              { return m_ecmaScriptAddBrowserObject; }

    void                SetEcmaScriptAddInitialise(bool s)
                              { m_ecmaScriptAddInitialise = s; }
    void                SetEcmaScriptAddShutdown(bool s)
                              { m_ecmaScriptAddShutdown = s; }
    void                SetEcmaScriptAddEventsProcessed(bool s)
                              { m_ecmaScriptAddEventsProcessed = s; }
    void                SetEcmaScriptAddAvailableFunctions(bool s)
                              { m_ecmaScriptAddAvailableFunctions =s; }
    void                SetEcmaScriptAddAllowedValues(bool s)
                              { m_ecmaScriptAddAllowedValues = s; }
    void                SetEcmaScriptAddAllowedComponents(bool s)
                              { m_ecmaScriptAddAllowedComponents = s; }
    void                SetEcmaScriptAddExampleUsage(bool s)
                              { m_ecmaScriptAddExampleUsage = s; }

    void                SetEcmaScriptAddMathObject(bool s)
                              { m_ecmaScriptAddMathObject = s; }
    void                SetEcmaScriptAddBrowserObject(bool s)
                              { m_ecmaScriptAddBrowserObject = s; }
    void                EcmaScriptSavePreferences(void);


private: 
    bool                m_ecmaScriptAddInitialise;
    bool                m_ecmaScriptAddShutdown;
    bool                m_ecmaScriptAddEventsProcessed;
    bool                m_ecmaScriptAddAvailableFunctions;
    bool                m_ecmaScriptAddAllowedValues;
    bool                m_ecmaScriptAddAllowedComponents;
    bool                m_ecmaScriptAddExampleUsage;
    bool                m_ecmaScriptAddMathObject;
    bool                m_ecmaScriptAddBrowserObject;
};

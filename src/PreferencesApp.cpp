/*
 * PreferencesApp.cpp
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

#include <sys/stat.h>
#include <fcntl.h>

#include "stdafx.h"
#include "swt.h"
#include "PreferencesApp.h"
#include "DuneApp.h"
#include "ExternTheApp.h"
#include "EulerAngles.h"

#define DEFAULT_NEAR_CLIPPING_PLANE_DIST 0.05f 
#define DEFAULT_FAR_CLIPPING_PLANE_DIST 7000.0f
#define DEFAULT_MAX_INLINES_TO_LOAD 1024
#define DEFAULT_MAX_KEYS_IN_CHANNELVIEW 64

#define COMPANY_NAME "WhiteDune"
#define APP_NAME "dune"

PreferencesApp::PreferencesApp()
{
    m_prefs = swLoadPreferences(COMPANY_NAME, APP_NAME);
    m_mouseMode = (MouseMode) GetIntPreference("MouseMode", MOUSE_EXAMINE);
    m_X11ErrorsLimit = GetIntPreference("X11ErrorsLimit", 10);    
    m_ttfFile = GetPreference("TtfFile", HAVE_DEFAULT_TTF_FILE);
    int f = open(m_ttfFile, O_RDONLY, 00666);
    if (f == -1)
        m_ttfFile = HAVE_DEFAULT_TTF_FILE;
    else
        close(f);
}

void
PreferencesApp::PreferencesDefaults()
{
    m_mouseMode = (MouseMode) MOUSE_EXAMINE;
    TheApp->ModellingDefaults();
    TheApp->RenderDefaults();
}


void PreferencesApp::SavePreferences()
{
    SetIntPreference("MouseMode", m_mouseMode);
    SetIntPreference("X11ErrorsLimit", m_X11ErrorsLimit);
    SetPreference("TtfFile", m_ttfFile);

    TheApp->EcmaScriptSavePreferences();
    TheApp->StereoViewSavePreferences();
    TheApp->OutputSavePreferences();
    TheApp->RouteViewSavePreferences();
    TheApp->StartWithSavePreferences();
    TheApp->InputDeviceSavePreferences();
    TheApp->ModellingSavePreferences();
    TheApp->RenderSavePreferences();

    swSavePreferences(m_prefs);
}

void PreferencesApp::SetMouseMode(MouseMode mouseMode)
{
    if (m_mouseMode != mouseMode)
        m_mouseMode = mouseMode;
}

bool
PreferencesApp::GetBoolPreference(const char *key, bool defaultValue)
{
    return swGetIntPreference(m_prefs, key, defaultValue ? 1 : 0) != 0;
}

int
PreferencesApp::GetIntPreference(const char *key, int defaultValue)
{
    return swGetIntPreference(m_prefs, key, defaultValue);
}

const char *
PreferencesApp::GetPreference(const char *key, const char *defaultValue)
{
    return swGetPreference(m_prefs, key, defaultValue);
}

const char *
PreferencesApp::GetArrayPreference(const char *key, int index)
{
    MyString keyWithIndex = "";
    keyWithIndex += key;
    keyWithIndex += "_";
    keyWithIndex.catInt(index);
    return swGetPreference(m_prefs, keyWithIndex, "");
}

void
PreferencesApp::SetBoolPreference(const char *key, bool value)
{
    swSetIntPreference(m_prefs, key, value ? 1 : 0);
}

void
PreferencesApp::SetIntPreference(const char *key, int value)
{
    swSetIntPreference(m_prefs, key, value);
}

void
PreferencesApp::SetPreference(const char *key, const char *value)
{
    swSetPreference(m_prefs, key, value);
}

void
PreferencesApp::SetArrayPreference(const char *key, int index, 
                                   const char *value)
{
    MyString keyWithIndex = "";
    keyWithIndex += key;
    keyWithIndex += "_";
    keyWithIndex.catInt(index);
    swSetPreference(m_prefs, keyWithIndex, value);
}

void PreferencesApp::unInstall(void)
{
    swUninstall(m_prefs, COMPANY_NAME, APP_NAME);
#ifdef HAVE_UNINSTALL_COMMENT
    printf("%s\n", HAVE_UNINSTALL_COMMENT);
#endif
}

void PreferencesApp::SetInputDeviceString(const char *string)
{
}

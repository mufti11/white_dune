/*
 * PreferencesApp.h
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

#ifndef _PREFERENCES_APP_H
#define _PREFERENCES_APP_H

#include "swttypedef.h"
#include "MyString.h"
#include "StringArray.h"

typedef enum {
    MOUSE_FLY,
    MOUSE_WALK,
    MOUSE_EXAMINE,
    MOUSE_ROLL,
    MOUSE_FOLLOW
} MouseMode;

class PreferencesApp {
public:

                        PreferencesApp();

    void                SavePreferences(void);

    STABLE              getPrefs() { return m_prefs; }

    // PreferencesApp data
    MouseMode           GetMouseMode() const { return m_mouseMode; }
    int                 GetX11ErrorsLimit(void)
                            { return m_X11ErrorsLimit; }

    void                SetMouseMode(MouseMode mouseMode);
    void                SetX11ErrorsLimit(int limit)
                            { m_X11ErrorsLimit = limit; }

    const char         *getTtfFile(void) { return m_ttfFile; }

    void                SetInputDeviceString(const char *string);
    int                 GetNumInputDeviceStrings(void)
                           { return m_inputDeviceString.size(); }
    const char         *GetInputDeviceString(int i)
                           { return m_inputDeviceString[i]; }

    void                PreferencesDefaults();

    bool                GetBoolPreference(const char *key, bool defaultValue);
    int                 GetIntPreference(const char *key, int defaultValue);
    const char         *GetPreference(const char *key,
                                      const char *defaultValue);
    const char         *GetArrayPreference(const char *key, int index);


    void                SetBoolPreference(const char *key, bool value);
    void                SetIntPreference(const char *key, int value);
    void                SetPreference(const char *key, const char *value);
    void                SetArrayPreference(const char *key, int index,
                                           const char *value);

    void                unInstall(void);

private: 

    STABLE              m_prefs;

    bool                m_showAllFields;
    int                 m_rotationOrder;
    const char         *m_rotationTitle;
    MouseMode           m_mouseMode;
    int                 m_X11ErrorsLimit;
    StringArray         m_inputDeviceString;
    MyString            m_ttfFile;
};

#endif

/*
 * RenderApp.h
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

#ifndef _RENDER_APP_H
#define _RENDER_APP_H

#include "swttypedef.h"
#include "MyString.h"
#include "StringArray.h"

class RenderApp {
public:
                        RenderApp();

    void                RenderLoadPreferences();
    void                RenderSavePreferences(void);

    // PreferencesApp data
    float               GetPointSetSize(void) { return m_pointSetSize; }
    bool                GetShowAllFields() const { return m_showAllFields; }
    float               GetNearClippingPlaneDist(void) 
                            { return m_nearClippingPlaneDist; }
    float               GetFarClippingPlaneDist(void) 
                            { return m_farClippingPlaneDist; }
    int                 GetMaxInlinesToLoad(void)
                            { return m_maxInlinesToLoad; }
    int                 GetMaxKeysInChannelView(void)
                            { return m_maxKeysInChannelView; }

    void                SetPointSetSize(float size);
    void                SetShowAllFields(bool s);
    void                SetNearClippingPlaneDist(float dist) 
                            { m_nearClippingPlaneDist = dist; }
    void                SetFarClippingPlaneDist(float dist) 
                            { m_farClippingPlaneDist = dist; }
    void                SetMaxInlinesToLoad(int max)
                            { m_maxInlinesToLoad = max; }
    void                SetMaxKeysInChannelView(int max)
                            { m_maxKeysInChannelView = max; }

    void                RenderDefaults();

private: 

    bool                m_showAllFields;
    float               m_pointSetSize;
    float               m_nearClippingPlaneDist;
    float               m_farClippingPlaneDist;
    int                 m_maxInlinesToLoad;
    int                 m_maxKeysInChannelView;
    bool                m_renderNicerSlower;
};

#endif

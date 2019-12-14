/*
 * ModellingApp.h
 *
 * Copyright (C) 1999 Stephen F. White, 2009 J. "MUFTI" Scheurich
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
x * Cambridge, MA 02139, USA.
 */

#pragma once

#include "swttypedef.h"
#include "MyString.h"
#include "StringArray.h"
#include "PreferencesApp.h"

typedef enum {
    HM_NONE,
    HM_SELECTED,
    HM_ALL,
    HM_TREE
} HandleMode;

class ModellingApp {
public:

                        ModellingApp();

    void                ModellingLoadPreferences();
    void                ModellingSavePreferences(void);

    // ModellingApp data
    HandleMode          GetHandleMode() const { return m_handleMode; }
    bool                GetHandleMeshAlways() { return  m_handleMeshAlways; }
    float               GetHandleSize();
    float               GetPreferenceHandleSize() { return m_handleSize; }
    float               GetHandleScale() { return m_handleScale; }
    int                 GetRotationOrder() const { return m_rotationOrder; }
    const char         *GetRotationTitle() const { return m_rotationTitle; }
    float               GetHandleEpsilon(void) const { return m_handleEpsilon; }
    bool                GetXSymetricMode() {return m_xSymetricMode;}

    void                SetHandleMode(HandleMode handleMode);
    void                SetHandleMeshAlways(bool b) {m_handleMeshAlways = b; }
    void                SetPreferenceHandleSize(float size);
    void                SetPreferenceHandleScale(float scale);
    void                SetRotationOrder(int rotationOrder);
    void                SetRotationTitle(const char * s) { m_rotationTitle = s; }
    void                SetHandleEpsilon(float f) { m_handleEpsilon = f; }
    void                SetXSymetricMode(bool flag) 
                            {m_xSymetricMode=flag;}
    float               getHumanoidScale(void) { return m_humanoidScale; }
    void                setHumanoidScale(float f) { m_humanoidScale = f; }
    void                setWeights1(bool flag) { m_weights1 = flag; }
    bool                getWeights1(void) { return m_weights1; }
    float               getExtrusionAmount(void)
                            { return m_extrusionAmount; }
    void                setExtrusionAmount(float f)
                            { m_extrusionAmount = f; }

    void                ModellingDefaults();

private: 

    int                 m_rotationOrder;
    const char         *m_rotationTitle;
    HandleMode          m_handleMode;
    bool                m_handleMeshAlways;
    float               m_handleSize;
    float               m_handleScale;
    float               m_handleEpsilon;
    float               m_humanoidScale;
    bool                m_xSymetricMode;
    int                 m_maxInlinesToLoad;
    int                 m_maxKeysInChannelView;
    bool                m_weights1;
    float               m_extrusionAmount;
};


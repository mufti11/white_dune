/*
 * ModellingApp.cpp
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
 * Cambridge, MA 02139, USA.
 */

#include "stdafx.h"
#include "swt.h"
#include "EulerAngles.h"
#include "ModellingApp.h"
#include "DuneApp.h"
#include "ExternTheApp.h"

ModellingApp::ModellingApp()
{
    ModellingDefaults();
}

void
ModellingApp::ModellingDefaults()
{
    m_rotationOrder = EulOrdXYZs;
    m_rotationTitle = "XYZs";
    m_handleMode = (HandleMode) HM_SELECTED;
    m_handleMeshAlways = false;
    m_handleSize = 3.0;
    m_handleScale = 1.0;
    m_handleEpsilon = 0.008;
    m_xSymetricMode = true;    
    m_humanoidScale = 0.05;
    m_weights1 = false;
    m_extrusionAmount = 0.2;
}

void ModellingApp::ModellingLoadPreferences()
{
    m_rotationOrder = TheApp->GetIntPreference("RotationOrder", EulOrdXYZs);
    m_rotationTitle = "XYZs";
    m_handleMode = (HandleMode)TheApp->GetIntPreference("HandleMode", 
                                                       HM_SELECTED);
    m_handleMeshAlways = TheApp->GetBoolPreference("HandleMeshAlways", false);
#ifdef HAVE_OLPC
    const char* handleSizeBuf = TheApp->GetPreference("HandleSize", "5.0");
#else
    const char* handleSizeBuf = TheApp->GetPreference("HandleSize", "3.0");
#endif
    m_handleSize = atof(handleSizeBuf);
    const char* handleScaleBuf = TheApp->GetPreference("HandleScale", "1.0");
    m_handleScale = atof(handleScaleBuf);
    const char* epsilonBuf = TheApp->GetPreference("HandleEpsilon", "0.008");
    m_handleEpsilon = atof(epsilonBuf);
    m_xSymetricMode = TheApp->GetBoolPreference("XSymetric", true);    
    const char* humaniodBuf = TheApp->GetPreference("HumanoidJointScale", 
                                                    "0.1");
    m_humanoidScale = atof(humaniodBuf);
    const char* extrusionBuf = TheApp->GetPreference("ExtrusionAmount", "0.2");
    m_extrusionAmount = atof(extrusionBuf);
    m_weights1 = TheApp->GetBoolPreference("Weights1", false);
}

void ModellingApp::ModellingSavePreferences()
{
    char buf[128];
    TheApp->SetIntPreference("RotationOrder", m_rotationOrder);
    TheApp->SetIntPreference("HandleMode", m_handleMode);
    TheApp->SetBoolPreference("HandleMeshAlways", m_handleMeshAlways);
    mysnprintf(buf, 127, "%f", m_handleSize);
    TheApp->SetPreference("HandleSize", buf);
    mysnprintf(buf, 127, "%f", m_handleScale);
    TheApp->SetPreference("HandleScale", buf);
    mysnprintf(buf, 127, "%f", m_handleEpsilon);
    TheApp->SetPreference("HandleEpsilon", buf);
    TheApp->SetBoolPreference("XSymetric", m_xSymetricMode);
    mysnprintf(buf, 127, "%f", m_humanoidScale);
    TheApp->SetPreference("HumanoidJointScale", buf);
    mysnprintf(buf, 127, "%f", m_extrusionAmount);
    TheApp->SetPreference("ExtrusionAmount", buf);
    TheApp->SetBoolPreference("Weights1", m_weights1);
}

void ModellingApp::SetRotationOrder(int rotationOrder)
{
    if (m_rotationOrder != rotationOrder) {
        m_rotationOrder = rotationOrder;
        TheApp->UpdateAllWindows();
    }
}

float ModellingApp::GetHandleSize(void)
{
    if (TheApp->useStereo())
        return TheApp->GetStereoHandleSizeMult() * m_handleSize;
    else
        return m_handleSize;         
}

void ModellingApp::SetPreferenceHandleSize(float size)
{
    m_handleSize = size;         
    TheApp->UpdateAllWindows();
}

void ModellingApp::SetPreferenceHandleScale(float scale)
{
    m_handleScale = scale;         
    TheApp->UpdateAllWindows();
}

void ModellingApp::SetHandleMode(HandleMode handleMode)
{
    if (m_handleMode != handleMode) {
        m_handleMode = handleMode;
        TheApp->UpdateAllWindows();
    }
}



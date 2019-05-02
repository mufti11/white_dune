/*
 * RenderApp.cpp
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
#include "swt.h"
#include "RenderApp.h"
#include "DuneApp.h"

#define DEFAULT_NEAR_CLIPPING_PLANE_DIST 0.05f 
#define DEFAULT_FAR_CLIPPING_PLANE_DIST 7000.0f
#define DEFAULT_MAX_INLINES_TO_LOAD 1024
#define DEFAULT_MAX_KEYS_IN_CHANNELVIEW 64

RenderApp::RenderApp()
{
    RenderDefaults();
}

void
RenderApp::RenderLoadPreferences()
{
    m_showAllFields = TheApp->GetBoolPreference("ShowAllFields", false);
    const char* pointSetSizeBuf = TheApp->GetPreference("PointSetSize", "0.0");
    m_pointSetSize = atof(pointSetSizeBuf);
    char buf[128];
    const char *buf2;
    mysnprintf(buf, 127, "%f", DEFAULT_NEAR_CLIPPING_PLANE_DIST);
    buf2 = TheApp->GetPreference("NearClippingPlaneDistance", buf); 
    if (atof(buf2) > 0)
        m_nearClippingPlaneDist = atof(buf2);
    else {
        m_nearClippingPlaneDist = DEFAULT_NEAR_CLIPPING_PLANE_DIST;
        swDebugf("NearClippingPlaneDistance preference");
        swDebugf(" must be greater zero\n");
        swDebugf("NearClippingPlaneDistance preference set to %f\n",
                 m_nearClippingPlaneDist);
    }
    mysnprintf(buf, 127, "%f", DEFAULT_FAR_CLIPPING_PLANE_DIST);
    buf2 = TheApp->GetPreference("FarClippingPlaneDistance", buf); 
    if (atof(buf2) > m_nearClippingPlaneDist)
        m_farClippingPlaneDist = atof(buf2);
    else {
        m_nearClippingPlaneDist = DEFAULT_NEAR_CLIPPING_PLANE_DIST;
        m_farClippingPlaneDist = DEFAULT_FAR_CLIPPING_PLANE_DIST;
        swDebugf("FarClippingPlaneDistance preference");
        swDebugf(" must be greater than NearClippingPlaneDistance preference\n");
        swDebugf("FarClippingPlaneDistance preference set to %f\n",
                 m_farClippingPlaneDist);
        swDebugf("NearClippingPlaneDistance preference set to %f\n",
                 m_nearClippingPlaneDist);
    }
    m_maxInlinesToLoad = TheApp->GetIntPreference("MaxInlinesToLoad", 
                                                 DEFAULT_MAX_INLINES_TO_LOAD);
    m_maxKeysInChannelView = TheApp->GetIntPreference("MaxKeysInChannelView", 
                                  DEFAULT_MAX_KEYS_IN_CHANNELVIEW);
}

void
RenderApp::RenderDefaults()
{
    m_showAllFields = false;
    m_pointSetSize = 0.0;
    m_nearClippingPlaneDist = DEFAULT_NEAR_CLIPPING_PLANE_DIST;
    m_farClippingPlaneDist = DEFAULT_FAR_CLIPPING_PLANE_DIST;
    m_maxInlinesToLoad = DEFAULT_MAX_INLINES_TO_LOAD;
    m_maxKeysInChannelView = DEFAULT_MAX_KEYS_IN_CHANNELVIEW;
}


void RenderApp::RenderSavePreferences()
{
    char buf[128];
    TheApp->SetBoolPreference("ShowAllFields", m_showAllFields);
    mysnprintf(buf, 127, "%f", m_nearClippingPlaneDist);
    TheApp->SetPreference("NearClippingPlaneDistance", buf);
    mysnprintf(buf, 127, "%f", m_farClippingPlaneDist);
    TheApp->SetPreference("FarClippingPlaneDistance", buf);
    TheApp->SetIntPreference("MaxInlinesToLoad", m_maxInlinesToLoad); 
    TheApp->SetIntPreference("MaxKeysInChannelView", m_maxKeysInChannelView);
}

void RenderApp::SetShowAllFields(bool showAllFields)
{
    if (m_showAllFields != showAllFields) {
        m_showAllFields = showAllFields;
        TheApp->UpdateAllWindows();
    }
}

void RenderApp::SetPointSetSize(float size)
{
    m_pointSetSize = size;         
    TheApp->UpdateAllWindows();
}


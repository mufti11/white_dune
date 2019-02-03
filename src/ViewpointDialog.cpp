/*
 * ViewpointDialog.cpp
 *
 * Copyright (C) 2000 Stephen F. White, 2018 J. "MUFTI" Scheurich
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
#include "ViewpointDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"
#include "Scene.h"
#include "NodeViewpoint.h"

ViewpointDialog::ViewpointDialog(SWND parent, int idd, Scene *scene)
  : Dialog(parent, idd)
{
    Node *viewpoint = scene->getCamera();
    m_dist = 0;
    Vec3f vec = viewpoint->getPosition();
    if (viewpoint)
        m_dist = sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
    m_viewpointData = NULL;
    LoadData();
}

ViewpointDialog::~ViewpointDialog()
{
}

void
ViewpointDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_DISTANCE), buf, 128);
    m_dist = atof(buf);
    if (swGetCheck(swGetDialogItem(m_dlg, IDC_VIEWPOINT_X_MINUS))) {
        m_data[0] = 10;
        m_data[1] = 0;
        m_data[2] = 0;
        m_data[3] = 0;
        m_data[4] = 1;
        m_data[5] = 0;
        m_data[6] = M_PI / 2;
        m_viewpointData = (float *)&m_data;
    } else if (swGetCheck(swGetDialogItem(m_dlg, IDC_VIEWPOINT_X_PLUS))) {
        m_data[0] = -10;
        m_data[1] = 0;
        m_data[2] = 0;
        m_data[3] = 0;
        m_data[4] = 1;
        m_data[5] = 0;
        m_data[6] = -M_PI / 2;
        m_viewpointData = (float *)&m_data;
    } else if (swGetCheck(swGetDialogItem(m_dlg, IDC_VIEWPOINT_Y_MINUS))) {
        m_data[0] = 0;
        m_data[1] = 10;
        m_data[2] = 0;
        m_data[3] = 1;
        m_data[4] = 0;
        m_data[5] = 0;
        m_data[6] = -M_PI / 2;
        m_viewpointData = (float *)&m_data;
    } else if (swGetCheck(swGetDialogItem(m_dlg, IDC_VIEWPOINT_Y_PLUS))) {
        m_data[0] = 0;
        m_data[1] = -10;
        m_data[2] = 0;
        m_data[3] = 1;
        m_data[4] = 0;
        m_data[5] = 0;
        m_data[6] = M_PI / 2;
        m_viewpointData = (float *)&m_data;
    } else if (swGetCheck(swGetDialogItem(m_dlg, IDC_VIEWPOINT_Z_MINUS))) {
        m_data[0] = 0;
        m_data[1] = 0;
        m_data[2] = 10;
        m_data[3] = 1;
        m_data[4] = 0;
        m_data[5] = 0;
        m_data[6] = 0;
        m_viewpointData = (float *)&m_data;
    } else if (swGetCheck(swGetDialogItem(m_dlg, IDC_VIEWPOINT_Z_PLUS))) {
        m_data[0] = 0;
        m_data[1] = 0;
        m_data[2] = -10;
        m_data[3] = 0;
        m_data[4] = 1;
        m_data[5] = 0;
        m_data[6] = M_PI;
        m_viewpointData = (float *)&m_data;
    }
    m_validDist = swGetCheck(swGetDialogItem(m_dlg, 
        IDC_SET_NAVIGATION_INFO_AVATARSIZE3));
}

bool
ViewpointDialog::Validate()
{
    bool radioButtons[6];
    
    radioButtons[0] = swGetCheck(swGetDialogItem(m_dlg, IDC_VIEWPOINT_X_PLUS));
    radioButtons[1] = swGetCheck(swGetDialogItem(m_dlg, IDC_VIEWPOINT_Y_PLUS));
    radioButtons[2] = swGetCheck(swGetDialogItem(m_dlg, IDC_VIEWPOINT_Z_PLUS));
    radioButtons[3] = swGetCheck(swGetDialogItem(m_dlg, IDC_VIEWPOINT_X_MINUS));
    radioButtons[4] = swGetCheck(swGetDialogItem(m_dlg, IDC_VIEWPOINT_Y_MINUS));
    radioButtons[5] = swGetCheck(swGetDialogItem(m_dlg, IDC_VIEWPOINT_Z_MINUS));

    bool first = radioButtons[0];
    for (int i = 1; i < 6; i++)   
        if (first && radioButtons[i])
            return false;
    return true;
}

void
ViewpointDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%f", m_dist);
    swSetText(swGetDialogItem(m_dlg, IDC_DISTANCE), buf);

    swSetCheck(swGetDialogItem(m_dlg, IDC_SET_NAVIGATION_INFO_AVATARSIZE3), 1);
}

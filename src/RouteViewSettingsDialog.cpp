/*
 * RouteViewSettingsDialog.cpp
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
#include "RouteViewSettingsDialog.h"
#include "DuneApp.h"
#include "resource.h"
#include "swt.h"

RouteViewSettingsDialog::RouteViewSettingsDialog(SWND parent)
  : Dialog(parent, IDD_ROUTE_VIEW)
{
    LoadData();
}

void
RouteViewSettingsDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == IDC_ROUTE_VIEW_DEFAULTS) {
        TheApp->RouteViewSetDefaults();
        LoadData();
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
}

void RouteViewSettingsDialog::LoadData()
{
    char buf[128];

    swSetCheck(swGetDialogItem(m_dlg, IDC_ROUTE_VIEW_SHOW_ALL), 
               TheApp->GetRouteViewShowAll());

    swSetCheck(swGetDialogItem(m_dlg, IDC_SHOW_ROUTES_AT_BEGIN), 
               TheApp->GetShowRoutesAtBegin());

    mysnprintf(buf, 128, "%d", TheApp->GetRouteViewQueueLength());
    swSetText(swGetDialogItem(m_dlg, IDC_ROUTE_VIEW_QUEUE_LENGTH), buf);

    mysnprintf(buf, 128, "%d", TheApp->GetRoutedNodeIndent());
    swSetText(swGetDialogItem(m_dlg, IDC_ROUTED_NODE_INDENT), buf);
}

bool 
RouteViewSettingsDialog::Validate()
{
    if (swGetCheck(swGetDialogItem(m_dlg, IDC_ROUTE_VIEW_SHOW_ALL))==0)
        if (swGetCheck(swGetDialogItem(m_dlg, IDC_SHOW_ROUTES_AT_BEGIN))==0) {
            TheApp->MessageBoxId(IDS_NEED_ROUTESBEGIN_AT_NOVIEWALL);
            return false;
        }
    return true;
}

void
RouteViewSettingsDialog::SaveData() 
{
    char buf[128];

    TheApp->SetRouteViewShowAll(swGetCheck(swGetDialogItem(m_dlg, 
            IDC_ROUTE_VIEW_SHOW_ALL))!=0);

    TheApp->SetShowRoutesAtBegin(swGetCheck(swGetDialogItem(m_dlg, 
            IDC_SHOW_ROUTES_AT_BEGIN))!=0);

    swGetText(swGetDialogItem(m_dlg, IDC_ROUTE_VIEW_QUEUE_LENGTH), buf, 128);
    TheApp->SetRouteViewQueueLength(atoi(buf));

    swGetText(swGetDialogItem(m_dlg, IDC_ROUTED_NODE_INDENT), buf, 128);
    TheApp->SetRoutedNodeIndent(atoi(buf));
}

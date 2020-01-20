/*
 * InteractionDialog.h
 *
 * Copyright (C) 2005 J. "MUFTI" Scheurich
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

#include "Scene.h"
#include "CheckBoxWindow.h"

class InteractionRouteData {
public:
    Node               *node;
    const char         *nodeProtoName;
    int                 eventInField;
    int                 eventOutField;
    int                 type;
    int                 level;
                        InteractionRouteData() {
                           node = NULL;
                           nodeProtoName = NULL;
                           eventInField = -1;
                           eventOutField = -1;
                           type = -1;
                           level = 0;
                        }
};

class InteractionDialog : public Dialog {
public:
                        InteractionDialog(SWND parent, Node* interactionNode,
                                          InteractionRouteData *routeData);

    void                buildInterfaceData(void);

protected:
    virtual SWND        dialog(void) { return m_dlg; }
    void                LoadData();
    void                SaveData();
    virtual bool        Validate();
    void                OnCommand(void *id);
    void                drawInterface(SDC dc);
    
protected:
    CheckBoxWindow        m_window;
    Node                 *m_interactionNode;
    InteractionRouteData *m_routeData;
    MyArray<int>          m_eventIns;        
};


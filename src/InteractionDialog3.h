/*
 * InteractionDialog3.h
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
#include "InteractionDialog.h"

class InteractionDialog3 : public Dialog {
public:
                        InteractionDialog3(SWND parent, Scene *scene,
                                           InteractionRouteData *routeData);

    bool                getEventInIsInteractive() 
                           { return m_eventInIsInteractive; }
    void                buildInterfaceData(void);

protected:
    virtual SWND        dialog(void) { return m_dlg; }
    void                LoadData();
    void                SaveData();
    virtual bool        Validate();
    void                OnCommand(int id);
    void                drawInterface(SDC dc);
    
protected:
    CheckBoxWindow        m_window;
    InteractionRouteData *m_routeData;
    Scene                *m_scene;
    bool                  m_okFlag;
    bool                  m_eventInIsInteractive;
    bool                  m_initIsInteractive;
    int                   m_level;
    MyArray<int>          m_eventOuts;        
};



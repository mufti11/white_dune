/*
 * AnimationDialog.h
 *
 * Copyright (C) 2003 J. "MUFTI" Scheurich
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

#ifndef _ANIMATION_DIALOG_H
#define _ANIMATION_DIALOG_H

#include "Scene.h"
#include "CheckBoxWindow.h"

class NodeTimeSensor;

class AnimationDialog : public Dialog {
public:
                        AnimationDialog(SWND parent, Node* oldNode);

    NodeTimeSensor     *getTimeSensor()        { return m_timeSensor; }
    float               getTimeSensorSeconds() { return m_newTimeSensorSeconds; }
    MyArray<int>       &getEventInFields()     { return m_eventInFields; }    
    MyArray<int>       &getEventInTypes()      { return m_eventInTypes; }    
    MyArray<bool>      &getEventInIsAnimated() { return m_eventInIsAnimated; }
    void                buildInterfaceData(void);

protected:
    virtual SWND        dialog(void) { return m_dlg; }
    void                LoadData();
    void                SaveData();
    virtual bool        Validate();
    int                 numEventInNames() { return m_eventInNames.size(); }
    
protected:
    CheckBoxWindow      m_window;
    Node               *m_animationNode;
    NodeTimeSensor     *m_timeSensor;
    float               m_newTimeSensorSeconds;
    bool                m_okFlag;
    MyArray<MyString>   m_eventInNames;
    MyArray<int>        m_eventInFields;
    MyArray<int>        m_eventInTypes;
    MyArray<bool>       m_eventInIsAnimated;
};

#endif


/*
 * CheckBoxWindow.h
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

#include "swt.h"
#include "swttypedef.h"
#include "MyString.h"
#include "Array.h"
#include "Dialog.h"

class CheckBoxWindow {
public:
                        CheckBoxWindow(void);
    virtual            ~CheckBoxWindow();
    void                initCheckBoxWindow(SWND parent, SWND dlg);
    void                accountYmax(void);
    void                initaliseCheckBoxWindow(void);
    virtual void        drawInterface(SDC dc);

    void                setString(int i, MyString string)
                           { m_strings[i] = string; }
    void                setButtonsPressed(int i, bool pressed)
                           { m_buttonsPressed[i] = pressed; }
    void                setInitButtonsPressed(int i, bool pressed)
                           { m_initButtonsPressed[i] = pressed; }
    bool                getChecked(long i) 
                           {
                           if (i >= m_buttons.size())
                               return false;
                           return swGetCheck(m_buttons[i]);
                           }
    bool                isValid(void);
    void                resize0(void); 
    void                invalidateWindow(void);

protected:
    MyArray<MyString>   m_strings;
    MyArray<SWND>       m_buttons;
    MyArray<bool>       m_buttonsPressed;
    MyArray<bool>       m_initButtonsPressed;
    int                 m_fontHeight;
    SWND                m_parent_window;
};


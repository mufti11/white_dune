/*
 * FieldPipeDialog.h
 *
 * Copyright (C) 1999 Stephen F. White, 2007 J. "MUFTI" Scheurich
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

#include "Dialog.h"
#include "swttypedef.h"
#include "MyString.h"

class FieldPipeDialog : public Dialog {
public:
                            FieldPipeDialog(SWND parent, MyString nodeFilter,
                                            MyString fieldFilter,
                                            MyString command);
    virtual                ~FieldPipeDialog();

    void                    LoadData();
    virtual void            SaveData();
    virtual bool            Validate();
    MyString                GetNodeFilter() const { return m_nodeFilter; }
    MyString                GetFieldFilter() const { return m_fieldFilter; }
    MyString                GetCommand() const { return m_command; }
private:
    MyString                m_nodeFilter;
    MyString                m_fieldFilter;
    MyString                m_command;
};


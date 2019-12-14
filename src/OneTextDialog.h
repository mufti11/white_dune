/*
 * OneTextDialog.h
 *
 * Copyright (C) 1999 Stephen F. White, 2004 J. "MUFTI" Scheurich
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

typedef bool (*OneTextValidateCallback)(MyString text);

class OneTextDialog : public Dialog {
public:
                            OneTextDialog(SWND parent, int idd, MyString value,
                                          OneTextValidateCallback callback);
    virtual                ~OneTextDialog();

    void                    LoadData();
    virtual void            SaveData();
    virtual bool            Validate();
    MyString                GetValue() const { return m_value; }
private:
    MyString                m_value;
    OneTextValidateCallback m_callback;
};


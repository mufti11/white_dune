/*
 * OneIntDialog.h
 *
 * Copyright (C) 1999 Stephen F. White, 2006 J. "MUFTI" Scheurich
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

#ifndef INT_MAX 
# include <limits.h>
#endif

class OneIntDialog : public Dialog {
public:
                        OneIntDialog(SWND parent, int idd, int value,
                                     int lowerlimit = INT_MAX, 
                                     int upperlimit = INT_MIN);
    virtual            ~OneIntDialog();

    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    int                 GetValue() const { return m_value; }
private:
    int         m_value;
    int         m_upperlimit;
    int         m_lowerlimit;
};


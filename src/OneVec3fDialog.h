/*
 * OneVec3fDialog.h
 *
 * Copyright (C) 1999 Stephen F. White, 2020 J. "MUFTI" Scheurich
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
#include "Vec3f.h"
#ifndef FLT_MAX 
# include <float.h>
#endif
#ifndef FLT_MIN
# include <float.h>
#endif

class OneVec3fDialog : public Dialog {
public:
                        OneVec3fDialog(SWND parent, int idd, Vec3f value,
                                       float lowerlimit = -FLT_MAX, 
                                       float upperlimit = FLT_MAX);
    virtual            ~OneVec3fDialog();

    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    Vec3f               GetValue() const { return m_value; }
private:
    Vec3f               m_value;
    float               m_upperlimit;
    float               m_lowerlimit;
};


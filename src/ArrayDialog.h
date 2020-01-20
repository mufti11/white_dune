/*
 * ArrayDialog.h
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

#include "SFVec3f.h"
#include "SFRotation.h"

class ArrayDialog : public Dialog 
{
public:
                        ArrayDialog(SWND parent);
    virtual            ~ArrayDialog();

    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    int                 getNumberCopies() {return m_numberCopies;}
    SFVec3f             getTranslation() {return m_translation;}
    SFRotation          getRotation() {return m_rotation;}
    SFVec3f             getCenter() {return m_center;}
    SFVec3f             getScale() {return m_scale;}
private:
    int                 m_numberCopies;
    SFVec3f             m_translation;
    SFRotation          m_rotation;
    SFVec3f             m_center;
    SFVec3f             m_scale;
};

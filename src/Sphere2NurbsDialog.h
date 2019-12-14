/*
 * Sphere2NurbsDialog.h
 *
 * Copyright (C) 1999 Stephen F. White, 2003 Thomas Rothermel
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

class Sphere2NurbsDialog : public Dialog 
{
public:
                        Sphere2NurbsDialog(SWND parent, 
                                           int narcslong, int narcslat,
                                           int uDegree, int vDegree,
                                           bool spherical = false);
    virtual            ~Sphere2NurbsDialog();

    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    int                 getNarcslong() {return m_narcslong;}
    int                 getNarcslat() {return m_narcslat;}
    int                 getuDegree() {return m_uDegree;}
    int                 getvDegree() {return m_vDegree;}
private:
    bool                m_spherical;
    int                 m_narcslong;
    int                 m_narcslat;
    int                 m_uDegree;
    int                 m_vDegree;
};


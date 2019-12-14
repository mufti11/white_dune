/*
 * Cone2NurbsDialog.h
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
#include "Node.h"

class Cone2NurbsDialog : public Dialog 
{
public:
                        Cone2NurbsDialog(SWND parent, 
                                         Node* node,
                                         int narcs, int narea, int nshell,
                                         int uDegree, int vDegree);
    virtual            ~Cone2NurbsDialog();

    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    int                 getNarcs() {return m_narcs;}
    int                 getNarea() {return m_narea;}
    int                 getNshell() {return m_nshell;}
    int                 getuDegree() {return m_uDegree;}
    int                 getvDegree() {return m_vDegree;}
private:
    int                 m_narcs;
    int                 m_narea;
    int                 m_nshell;
    int                 m_uDegree;
    int                 m_vDegree;
    int                 m_side;
    int                 m_bottom;
};


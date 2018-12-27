/*
 * NurbsPlaneDialog.h
 *
 * Copyright (C) 2000 Stephen F. White
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

#ifndef _NURBS_PLANE_DIALOG_H
#define _NURBS_PLANE_DIALOG_H

#ifndef _DIALOG_H
#include "Dialog.h"
#endif

#include "swttypedef.h"

class NurbsPlaneDialog : public Dialog {
public:
                        NurbsPlaneDialog(SWND parent, int idd, int n1, int n2,
                                         int uDegree, int vDegree);
    virtual            ~NurbsPlaneDialog();

    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    int                 GetN1() const { return m_n1; }
    int                 GetN2() const { return m_n2; }
    int                 GetUDegree() const { return m_uDegree; }
    int                 GetVDegree() const { return m_vDegree; }
private:
    int                 m_n1;
    int                 m_n2;
    int                 m_uDegree;
    int                 m_vDegree;
};

#endif

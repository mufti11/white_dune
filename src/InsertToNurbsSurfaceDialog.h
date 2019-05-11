/*
 * InsertToNurbsSurfaceDialog.h
 *
 * Copyright (C) 1999 Stephen F. White, 2003 Thomas Rothermel
 *               2019 J. "MUFTI" Scheurich
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

#ifndef INSERT_TO_NURBS_SURFACE_DIALOG_H
#define INSERT_TO_NURBS_SURFACE_DIALOG_H

#ifndef _DIALOG_H
#include "Dialog.h"
#endif

#include "swttypedef.h"

class InsertToNurbsSurfaceDialog : public Dialog 
{
public:
                        InsertToNurbsSurfaceDialog(SWND parent, 
                                                 int uFrom, int uTo,
                                                 int vFrom, int vTo);

    virtual            ~InsertToNurbsSurfaceDialog();

    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    int                 getFromU() {return m_uFrom;}
    int                 getToU() {return m_uTo;}
    int                 getPointsU() {return m_uPoints;}
    int                 getFromV() {return m_vFrom;}
    int                 getToV() {return m_vTo;}
    int                 getPointsV() {return m_vPoints;}
private:
    int                 m_uFrom;
    int                 m_uTo;
    int                 m_uPoints;
    int                 m_vFrom;
    int                 m_vTo;
    int                 m_vPoints;
};

#endif

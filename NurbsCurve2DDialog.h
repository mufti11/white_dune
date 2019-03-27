/*
 * NurbsCurve2DDialog.h
 *
 * Copyright (C) 2000 Stephen F. White, 2018 J. "MUFTI" Scheurich
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

#ifndef _NURBS_CURVE_2D_DIALOG_H
#define _NURBS_CURVE_2D_DIALOG_H

#ifndef _DIALOG_H
#include "Dialog.h"
#endif

class Scene;

#include "swttypedef.h"

class NurbsCurve2DDialog : public Dialog {
public:
                        NurbsCurve2DDialog(SWND parent, int idd, Scene *scene,
                                           int points);
    virtual            ~NurbsCurve2DDialog();

    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    int                 GetSize() const { return m_size; }
    int                 GetPoints() const { return m_points; }
    bool                GetCcw() const { return m_ccw; }
private:
    float               m_size;
    int                 m_points;
    bool                m_ccw;
};

#endif

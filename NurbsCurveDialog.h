/*
 * NurbsCurveDialog.h
 *
 * Copyright (C) 2000 Stephen F. White, 2003 Th. Rothermel
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

#ifndef _NURBS_CURVE_DIALOG_H
#define _NURBS_CURVE_DIALOG_H

#ifndef _DIALOG_H
#include "Dialog.h"
#endif

#include "swttypedef.h"

class NurbsCurveDialog : public Dialog {
public:
                        NurbsCurveDialog(SWND parent, int nPoints, int degree,
                                         int direction, 
                                         bool keepDegree = false);
    virtual            ~NurbsCurveDialog();

    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    int                 getnPoints() {return m_nPoints;}
    int                 getDegree() {return m_degree;}
    int                 getDirection() {return m_direction; }
private:
    int                 m_nPoints;
    int                 m_degree;
    int                 m_direction;
    bool                m_keepDegree;
};

#endif


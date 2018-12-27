/*
 * NurbsCurve2NurbsSurfDialog.h
 *
 * Copyright (C) 2003 Th. Rothermel
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

#include "Node.h"
#include "Vec3f.h"

#ifndef _DIALOG_H
#include "Dialog.h"
#endif

class NurbsCurve2NurbsSurfDialog : public Dialog {
public:
                 NurbsCurve2NurbsSurfDialog(SWND parent, Node *node,
                                            int narcs, int uDegree, 
                                            float rDegree, int method);
    int          getNarcs() {return m_narcs;}
    int          getuDegree() {return m_uDegree;}
    float        getrDegree() {return m_rDegree;}             
    Vec3f        getP1() {return m_P1;}
    Vec3f        getP2() {return m_P2;}
    int          getMethod() {return m_method; }
    bool         getFlatten() { return m_flatten; }

    virtual void SaveData();
    virtual bool Validate();
    virtual void LoadData();

protected:

    int          m_method;
    int          m_narcs;
    int          m_uDegree;
    float        m_rDegree;
    Vec3f        m_P1;
    Vec3f        m_P2;
    bool         m_flatten;
    Node        *m_node;
};

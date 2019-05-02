/*
 * NurbsCurve2NurbsSurfDialog.cpp
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
#include "SFBool.h"

#include "NurbsCurve2NurbsSurfDialog.h"
#include "Scene.h"
#include "Proto.h"
#include "resource.h"
#include "NodeNurbsCurve.h"
#include "DuneApp.h"
#include "swt.h"

NurbsCurve2NurbsSurfDialog::NurbsCurve2NurbsSurfDialog(SWND parent, Node *node,
                                                       int narcs, int uDegree,
                                                       float rDegree, int method)
  : Dialog(parent, IDD_NURBS_CURVE_ROTATE) 
{  
  m_P1.x = 0; m_P1.y = 0; m_P1.z = 0; 
  m_P2.x = 0; m_P2.y = 0; m_P2.z = 0; 
  m_node = node;
  m_narcs = narcs;
  m_uDegree = uDegree;
  m_rDegree = rDegree;
  m_method =  method; 
  LoadData();
}


void
NurbsCurve2NurbsSurfDialog::SaveData()
{
    char buf[128];

    if (swGetCheck(swGetDialogItem(m_dlg, IDC_POINTPOINT_ROTATE)))
        m_method = NURBS_ROT_POINT_TO_POINT;
    else if (swGetCheck(swGetDialogItem(m_dlg, IDC_X_ROTATE)))
        m_method = NURBS_ROT_X_AXIS;
    else if (swGetCheck(swGetDialogItem(m_dlg, IDC_Y_ROTATE)))
        m_method = NURBS_ROT_Y_AXIS;
    else if (swGetCheck(swGetDialogItem(m_dlg, IDC_Z_ROTATE)))
        m_method = NURBS_ROT_Z_AXIS;

    swGetText(swGetDialogItem(m_dlg, IDC_CURVE2SURF_NARCS), buf, 128);
    m_narcs = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_CURVE2SURF_U_DEGREE), buf, 128);
    m_uDegree = atoi(buf);
    swGetText(swGetDialogItem(m_dlg, IDC_CURVE2SURF_ANGLE), buf, 128);
    m_rDegree = atof(buf);
    m_flatten = swGetCheck(swGetDialogItem(m_dlg, IDC_CURVE2SURF_FLATTEN));
}

bool
NurbsCurve2NurbsSurfDialog::Validate()
{

    bool valid = true;

    if (m_rDegree >= 180 * m_narcs) 
        valid = false;
/*
    if (m_uDegree > 2) {
        valid = false;
        TheApp->MessageBox(IDS_UDEGREE_UNSUPPORTED);
    }
*/
    int length;
    NodeNurbsCurve *node = (NodeNurbsCurve *)m_node;
    switch (m_method) {
    case NURBS_ROT_X_AXIS:    
      m_P2.x = 1;
      break;
    case NURBS_ROT_Y_AXIS:    
      m_P2.y = 1;
      break;      
    case NURBS_ROT_Z_AXIS:    
      m_P2.z = 1;
      break;
    case NURBS_ROT_POINT_TO_POINT:      
      length = node->getControlPoints()->getSFSize();
      m_P1 = (Vec3f)node->getControlPoints()->getValue(0);
      m_P2 = (Vec3f)node->getControlPoints()->getValue(length-1);
      break;
    }    

    Vec3f vTest = m_P2 - m_P1;
    if ((vTest.x == 0) && (vTest.y == 0) && (vTest.z == 0))
        valid = false;

    if (valid)
        valid =  m_uDegree > 0;
    return valid;
}

void
NurbsCurve2NurbsSurfDialog::LoadData()
{
    char buf[128];

    if (m_method == NURBS_ROT_POINT_TO_POINT)
        swSetCheck(swGetDialogItem(m_dlg, IDC_POINTPOINT_ROTATE), 1);
    else if (m_method == NURBS_ROT_X_AXIS)
        swSetCheck(swGetDialogItem(m_dlg, IDC_X_ROTATE), 1);
    else if (m_method == NURBS_ROT_Y_AXIS)
        swSetCheck(swGetDialogItem(m_dlg, IDC_Y_ROTATE), 1);
    else if (m_method == NURBS_ROT_Z_AXIS)
        swSetCheck(swGetDialogItem(m_dlg, IDC_Z_ROTATE), 1);
    mysnprintf(buf, 128, "%d", m_narcs);
    swSetText(swGetDialogItem(m_dlg, IDC_CURVE2SURF_NARCS), buf);
    mysnprintf(buf, 128, "%d", m_uDegree);
    swSetText(swGetDialogItem(m_dlg, IDC_CURVE2SURF_U_DEGREE), buf);
    mysnprintf(buf, 128, "%g", m_rDegree);
    swSetText(swGetDialogItem(m_dlg, IDC_CURVE2SURF_ANGLE), buf);
    swSetCheck(swGetDialogItem(m_dlg, IDC_CURVE2SURF_FLATTEN), 1);
}


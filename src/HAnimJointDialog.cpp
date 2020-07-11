/* 
 * HAnimJointDialog.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2018 J. "MUFTI" Scheurich
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

#include "stdafx.h"
#include "HAnimJointDialog.h"
#include <stdio.h>
#include <stdlib.h>
#include "swt.h"
#include "resource.h"
#include "DuneApp.h"
#include "NodeList.h"
#include "Scene.h"
#include "Path.h"
#include "NodeHAnimHumanoid.h"

HAnimJointDialog::HAnimJointDialog(SWND parent, int idd, Scene *scene,
                                   float value, bool nodesFromScene,
                                   bool newJoint)
  : Dialog(parent, idd)
{
    m_weight = value;
    m_lowerlimit = 0.0f;
    m_upperlimit = 1.0f;
    m_scene = scene;
    if (nodesFromScene) {
        Node *current = m_scene->getSelection()->getNode();
        NodeHAnimHumanoid *human = current->getHumanoid();
        if (human) {
            m_node = m_scene->getLastSelectedHAnimJoint();
        } else
            m_node = NULL;
    } else {
        m_node = NULL;
    }
    m_newJoint = newJoint;
    LoadData();
}

HAnimJointDialog::~HAnimJointDialog()
{
}

void
HAnimJointDialog::SaveData()
{
    char buf[128];

    swGetText(swGetDialogItem(m_dlg, IDC_VALUE), buf, 128);
    m_weight = atof(buf);

    SWND comboNode = swGetDialogItem(m_dlg, IDC_JOINT_NODE);
    int nodeIndex = swComboBoxGetSelection(comboNode);
    if ((nodeIndex == 0) && (m_node == NULL))
        m_node = NULL;
    else if (nodeIndex == 1)
        m_node = NULL;
    else if (nodeIndex > 1)
        m_node = m_scene->use(m_joints[nodeIndex]);

    m_parent = NULL;
}

bool
HAnimJointDialog::Validate()
{
    SWND comboNode = swGetDialogItem(m_dlg, IDC_JOINT_NODE);
    int nodeIndex = swComboBoxGetSelection(comboNode);
    if (nodeIndex < 0)
        return false;
    if ((m_weight <= m_lowerlimit) && (m_weight > m_upperlimit)) {
        char str[256], title[256];
        swLoadString(IDS_DUNE, title, 256);
        swLoadString(IDS_WEIGHT_0_1, str, 256);
        swMessageBox(TheApp->mainWnd(), str, title, SW_MB_OK, SW_MB_ERROR);
        return false;
    }
    return true;
}

void
HAnimJointDialog::LoadData()
{
    char buf[128];

    mysnprintf(buf, 128, "%f", m_weight);
    swSetText(swGetDialogItem(m_dlg, IDC_VALUE), buf);

    SWND comboNode = swGetDialogItem(m_dlg, IDC_JOINT_NODE);
    swComboBoxDeleteAll(comboNode);

    SWND comboParent = swGetDialogItem(m_dlg, IDC_JOINT_PARENT);
    swComboBoxDeleteAll(comboParent);  

    if (m_node != NULL)
        m_joints.append(m_node->getName());
    else
        m_joints.append("NULL");
    if (m_newJoint) 
        m_joints.append("new HAimJoint"); 
    else
        m_joints.append("split HAimJoint into 2 joints"); 

    const NodeList *nodes = m_scene->getNodes();
    for (long i = 0; i < nodes->size(); i++) {
        Node *node = nodes->get(i);
        if (node->isInScene(m_scene))
            if (node->getType() == X3D_HANIM_JOINT)
                if (node->hasName()) {
                    MyString text = "";
                    text += node->getName();
                    m_joints.append(text);
                }
    }
    
    for (long i = 0;i < m_joints.size(); i++)
        swComboBoxAppendItem(comboNode, m_joints[i]);

    m_joints[0] = "NULL";
     
    for (long i = 1; i < m_joints.size(); i++)
        swComboBoxAppendItem(comboParent, m_joints[i]);
}


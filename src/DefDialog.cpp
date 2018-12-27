/*
 * DefDialog.cpp
 *
 * Copyright (C) 2003 J. "MUFTI" Scheurich
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

#include "resource.h"
#include "MyString.h"
#include "Path.h"
#include "Node.h"
#include "DefDialog.h"
#include "DuneApp.h"

DefDialog::DefDialog(SWND parent, Scene* scene)
  : Dialog(parent, IDD_DEF)
{
    m_scene = scene;
    m_node = m_scene->getSelection()->getNode();
    m_valid = false;
    LoadData();
}

DefDialog::~DefDialog()
{
}

bool
DefDialog::Validate()
{
    return m_valid;
}

void
DefDialog::LoadData()
{
    if (m_node->hasName())
        swSetText(swGetDialogItem(m_dlg, IDC_DEF_NAME), m_node->getName());
    else
        swSetText(swGetDialogItem(m_dlg, IDC_DEF_NAME), 
                  m_scene->getUniqueNodeName(m_node));
}

void
DefDialog::SaveData()
{
    char name[1024] = "";
    swGetText(swGetDialogItem(m_dlg, IDC_DEF_NAME), name, 1024);
    int positionInvalid = TheApp->searchIllegalChar(name);
    if (positionInvalid != -1) {
        m_valid = false;
        char wrongChar[2];
        wrongChar[0] = name[positionInvalid];
        wrongChar[1] = 0;
        if (wrongChar[0] == ' ')
            TheApp->MessageBox(IDS_WRONG_SPACE_IN_DEF_ERROR, wrongChar);
        else
            TheApp->MessageBox(IDS_WRONG_DEF_ERROR, wrongChar);
    } else if ((m_node->hasName()) && (strcmp(name, m_node->getName()) == 0)) {
        m_valid = true;
        m_scene->def(m_node);
    } else if (m_scene->hasAlreadyName(name)) {
        m_valid = false;
        TheApp->MessageBoxId(IDS_DEF_ALREADY_ERROR);
    } else {
        m_valid = true;
        m_scene->def(m_node);
        m_scene->def(name, m_node);
    }
}


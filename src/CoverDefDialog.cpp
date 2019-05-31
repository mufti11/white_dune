/*
 * CoverDefDialog.cpp
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
#include "CoverDefDialog.h"
#include "DuneApp.h"

static const char *coverNames[] = { 
    "",
    "StaticCave",   // for Transform 
    "Animated",     // for animated mesh objects
    "combineTextures",
    "combineEnvTextures",
    "coShader",     // (coShaderSHADERNAME_something see covise/materials/*.xml)
    "coBump",       // apply builtin normal map shader to object
    "coBumpCube",   // apply builtin normal map shader with a cube map to object
    "coBumpEnv",    // apply builtin normal map shader with a spherical 
                    //                          environment map to object
    "coCgShader",
    "coDepthOnly",  // only render the object into the Z-buffer
    "coDontMirror",
    "coGlass",      // apply test shader, does not work
    "coMirror",     // reflection for plain objects
    "coNoColli",    // objects below this node are not included in collision 
                    //                                 detection (walk mode)
    "coNoDepth",
    "coNoIntersection",
    "coNoIsect",    // objects below this node are not included in 
                    //                             intersection testing (wand)
    "coNoWalk",
    "coOccluder",   
    "Cached"        // only for Inline

// deprecated DEF names:
// combineTextures     applies a builtin texture combiner (deprecated, use shaders instead)
// combineEnvTextures  applies a builtin texture combiner (deprecated, use shaders instead)
// coCgShader          not working anymore as far as Uwe Woessner remember
// "coTest"            apply test shader, does not work
};

CoverDefDialog::CoverDefDialog(SWND parent, Scene* scene)
  : Dialog(parent, IDD_COVER_DEF)
{
    m_scene = scene;
    m_node = m_scene->getSelection()->getNode();
    m_valid = false;
    LoadData();
}

CoverDefDialog::~CoverDefDialog()
{
}

void
CoverDefDialog::OnCommand(void *vid)
{
    int id = swFromPtr(vid);
    if (id == IDOK) {
        SaveData();
        if (Validate()) {
            swEndDialog(IDOK);
        }
    } else if (id == IDC_COVER_NAMES) {
        int sel = swComboBoxGetSelection(swGetDialogItem(m_dlg, 
                                                         IDC_COVER_NAMES));
        char buffer[1024] = "";
        swGetText(swGetDialogItem(m_dlg, IDC_DEF_NAME), buffer, 1024);
        char name[1024] = "";
        mysnprintf(name, 1024, "%s%s", coverNames[sel], buffer);
        swSetText(swGetDialogItem(m_dlg, IDC_DEF_NAME), name);
    } else if (id == IDCANCEL) {
        swEndDialog(IDCANCEL);
    }
}

bool
CoverDefDialog::Validate()
{
    return m_valid;
}

void
CoverDefDialog::LoadData()
{
    SWND comboCoverNames = swGetDialogItem(m_dlg, IDC_COVER_NAMES);
    swComboBoxDeleteAll(comboCoverNames);
    for (unsigned int i=0; i<(sizeof(coverNames)/sizeof(const char*)); i++)
        swComboBoxAppendItem(comboCoverNames, coverNames[i]);

    if (m_node->hasName())
        swSetText(swGetDialogItem(m_dlg, IDC_DEF_NAME), m_node->getName());
    else
        swSetText(swGetDialogItem(m_dlg, IDC_DEF_NAME), 
                  m_scene->getUniqueNodeName(m_node));
}

void
CoverDefDialog::SaveData()
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
    } else if ((m_node->hasName()) && 
               (strcmp(name, m_node->getName()) == 0)) {
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


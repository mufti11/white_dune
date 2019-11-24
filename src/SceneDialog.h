/*
 * SceneDialog.h
 *
 * Copyright (C) 2005 J. "MUFTI" Scheurich
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

#ifndef _SCENE_DIALOG_H
#define _SCENE_DIALOG_H

#include "Dialog.h"
#include "Scene.h"
#include "NodeVrmlCut.h"

class SceneDialog : public Dialog {
public:
                        SceneDialog(SWND parent, Node* node, bool add,
                                    int sceneNumber, NodeVrmlCut *vrmlCut);

    int                 getSceneNumber() { return m_sceneNumber; }

protected:
    virtual SWND        dialog(void) { return m_dlg; }
    void                LoadData();
    void                SaveData();
    virtual bool        Validate();
    void                OnCommand(void *id);

    double              getSceneLength(void);
    void                setSceneLength(double length);
    void                loadComboxData(void);
    
protected:
    int                 m_sceneNumber;
    Node               *m_node;
    NodeVrmlCut        *m_vrmlCut;
    bool                m_add;
    int                 m_oldSceneNumber;
};

#endif

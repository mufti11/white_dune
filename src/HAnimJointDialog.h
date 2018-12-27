/*
 * HAnimJointDialog.h
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

#ifndef _HANIM_JOINT_DIALOG_H
#define _HANIM_JOINT_DIALOG_H

#ifndef _DIALOG_H
#include "Dialog.h"
#endif

#include "swttypedef.h"
#ifndef FLT_MAX 
# include <float.h>
#endif
#ifndef FLT_MIN
# include <float.h>
#endif

#include "Array.h"

class Scene;
class Node;
class MyString;

class HAnimJointDialog : public Dialog {
public:
                        HAnimJointDialog(SWND parent, int idd, 
                                         Scene *scene, float value,
                                         bool nodesFromScene = false);
    virtual            ~HAnimJointDialog();

    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    float               GetWeight() const { return m_weight; }
    Node               *GetNode() const { return m_node; }
    Node               *GetParent() const { return m_parent; }
private:
    Scene              *m_scene;
    float               m_weight;
    Node               *m_node;
    Node               *m_parent;
    float               m_upperlimit;
    float               m_lowerlimit;
    Array<MyString>     m_joints;
};

#endif

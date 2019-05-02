/*
 * ViewpointDialog.h
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

#ifndef _VIEWPOINT_DIALOG_H
#define _VIEWPOINT_DIALOG_H

#ifndef _DIALOG_H
#include "Dialog.h"
#endif

class Scene;

#include "swttypedef.h"

class ViewpointDialog : public Dialog {
public:
                        ViewpointDialog(SWND parent, int idd, Scene *scene);
    virtual            ~ViewpointDialog();

    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    int                 GetDist() const { return m_dist; }
    float              *GetViewpointData() const { return m_viewpointData; }
    bool                GetValidDist() const { return m_validDist; }
private:
    float               m_dist;
    float              *m_viewpointData;
    float               m_data[7];
    bool                m_validDist;
};

#endif

/*
 * ElevationGridDialog.h
 *
 * Copyright (C) 1999 Stephen F. White
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

#ifndef _ELEVATION_GRID_DIALOG_H
#define _ELEVATION_GRID_DIALOG_H

#ifndef _DIALOG_H
#include "Dialog2.h"
#endif

#include "swttypedef.h"

class ElevationGridDialog : public Dialog {
public:
                        ElevationGridDialog(SWND parent, int width, int depth);
    virtual            ~ElevationGridDialog();
    void                OnCommand(int id);
    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    int                 GetWidth() const { return _width; }
    int                 GetDepth() const { return _depth; }
    int                 GetCharacterPosition(SWND swnd, char* number);
private:
    int                 _width;
    SWND                _widthSwnd;
    int                 _depth;
    SWND                _depthSwnd;
};

#endif

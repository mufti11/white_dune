/*
 * TransformMode.h
 *
 * Copyright (C) 1999 Stephen F. White, 2002 J. "MUFTI" Scheurich
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

#pragma once

typedef enum {
    TM_TRANSLATE,
    TM_ROTATE,
    TM_SCALE,
    TM_UNIFORM_SCALE,
    TM_CENTER,
    TM_6D,
    TM_6DLOCAL,
    TM_ROCKET,
    TM_HOVER
} TMode;

typedef enum {
    TM_1D,
    TM_2D,
    TM_3D
} TDimension;

typedef enum {
    TM_UP_DOWN,
    TM_NEAR_FAR
} T2axes;

class TransformMode {
public:
    TMode tmode;
    TDimension tdimension;
    T2axes t2axes;  
    TransformMode(TMode tm,TDimension td, T2axes t2) {
       tmode=tm;
       tdimension=td;
       t2axes=t2;
       } 
    bool hasRotation(void);
    bool hasTranslation(void);
    bool isLocal(void);
};


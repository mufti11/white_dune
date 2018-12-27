/*
 * InterfaceArray.h
 *
 * Copyright (C) 1999 Stephen F. White, 2005 J. "MUFTI" Scheurich
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

#ifndef _INTERFACE_ARRAY_H
#define _INTERFACE_ARRAY_H

#ifndef _ARRAY_H
#include "Array.h"
#endif

class InterfaceData {
public:
    InterfaceData(int elementEnum, int elementIndex) 
       { m_elementEnum=elementEnum; m_elementIndex=elementIndex; }
    
    int m_elementEnum;
    int m_elementIndex;
};

typedef Array<InterfaceData *> InterfaceArray;


#endif


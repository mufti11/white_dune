/*
 * FieldIndex.h
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

#pragma once

class FieldIndex {
// in principle, this is a readonly integer
// it is used to contain the index of field in a VRML node
// e.g. field "center" of the VRML Transform node is 0, field children is 1 etc.
public:
    FieldIndex()         { m_written = 0; }
    FieldIndex(int i)    
                         { 
                         m_written = 0;
                         set(i); 
                         }
    void set(int i)      { 
                         if (m_written == 0) {
                             m_written = 1;
                             if (i > -1)
                                 m_fieldIndex = i;
                         } else 
                             assert(0);
                         }
    operator int() const { 
                         if (m_written != 1) assert(0); 
                             return m_fieldIndex; 
                         }
protected:
    int m_written;
    int m_fieldIndex;
};

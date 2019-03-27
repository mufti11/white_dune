/*
 * Array2D.h
 *
 * Copyright (C) 2003 Th. Rothermel
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
#ifndef _ARRAY_2D_H
#define _ARRAY_2D_H

#include <stdlib.h>

#ifndef _WIN32
# include "stdlib.h"
#endif

#ifndef _ARRAY_H
#include "Array.h"
#endif
#ifndef _STDAFX_H
#include "stdafx.h"
#endif

template<class T> class array2D 
{
public: 
                    array2D(int size1, int size2)
                       {
                       m_size_i = size1;
                       m_size_j = size2;
                       m_data = new T[(m_size_i*m_size_j)];
                    }
                    ~array2D(){  
                      delete[] m_data;
                    }
               void set(int i, int j, T t){
                      assert(((i>=0)&&(i<(m_size_i))) != 0);
                      assert(((j>=0)&&(j<(m_size_j))) != 0);
                      m_data[(j*m_size_i)+i] = t;
                    }
               T    get(int i, int j){
                      assert(((i>=0)&&(i<(m_size_i))) != 0);
                      assert(((j>=0)&&(j<(m_size_j))) != 0);
                      return m_data[(j*m_size_i)+i];
                    }

protected:
               int      m_size_i;
               int      m_size_j;
               T        *m_data;
};
  
#endif // _ARRAY_2D_H   

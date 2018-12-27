/*
 * Array.h
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

#ifndef _ARRAY_H
#define _ARRAY_H

#define DEFAULT_CAPACITY 8

#ifdef IRIX
#pragma set woff 3303
#endif

#include "stdafx.h"
#include <stdio.h>

// Template for a C type array. Index: 0 ... size()-1 

template<class T> class Array {
public:
                Array(int capacity = DEFAULT_CAPACITY )
                { 
                   if (capacity == 0)
                       capacity = DEFAULT_CAPACITY;
                   m_capacity = capacity; 
                   m_data = new T[capacity]; 
                   m_size = 0;
                }
                Array(const Array<T> &a)
                {
                  m_capacity = m_size = a.m_size;
                  if (m_size == 0) 
                      m_capacity = DEFAULT_CAPACITY;
                  m_data = new T[m_capacity];
                  for (int i = 0; i < MIN(m_capacity, m_size); i++) 
                      m_data[i] = a.m_data[i];
                }
                Array(const T *a, int len)
                {
                  setData(a, len);
                }
               ~Array()
                {
                  if (m_data)
                      delete[] m_data; 
                  m_data = (T *)NULL;
                  m_size = 0;
                }

//    Array<T>   *copy() { return new Array<T>(*this); }
    const T    &get(int index) const
                { return m_data[index]; }
    void        set(int index, T t)
                { if (index >= m_size) resize(index+1); m_data[index] = t; }
    T          &operator[](int index)
                { if (index >= m_size) resize(index+1); return m_data[index]; }
    const T    &operator[](int index) const
                { return m_data[index]; }
    const T    *getData() const { return m_data; }
    void        setData(const T *a, int len)
                {
                  m_capacity = m_size = len; 
                  if (m_size == 0) {
                      m_capacity = DEFAULT_CAPACITY;
                      m_data = new T[m_capacity];
                  } else
                      m_data = (T *)a;
                }
    T          *extractData() { T *data = m_data; m_data = 0; return data; }
    int         size() const
                { return m_size; }
    void        append(T t)
                { (*this)[m_size] = t; }
    void        insert(T t, int index) {                   
                   if (index < m_size) {
                       resize(m_size + 1);
                       for (int i = m_size - 1; i > index; i--)
                           (*this)[i] = (*this)[i-1];
                   }
                   (*this)[index] = t; 
                }
    void        remove(int pos) {
                    if (pos < 0)
                        return;
                    for (int i = pos; i < m_size - 1; i++)
                        m_data[i] = m_data[i + 1];
                    m_size--;
                    if (m_size <= 0) {
                        resize(0);
                        m_size = 0;
                    }
                }
    void        remove(int start, int end) {
                assert(start >= 0 && start < m_size);
                assert(end >= 0 && end < m_size);
                int len = end - start + 1;
                int deleted = 0;
                for (int i = start; i <= end; i++)
                    if ((i + len) < m_size) {
                         deleted++;
                         m_data[i] = m_data[i + len];
                    }
                m_size -= deleted;
                if (m_size <= 0)
                    resize(0);
                }
    void        resize(int size) {
                    if (size == 0) {
                        if (m_size > 0) {
                            delete[] m_data;
                            m_capacity = DEFAULT_CAPACITY; 
                            m_data = new T[m_capacity]; 
                        }
                        m_size = 0;
                    } else if (m_capacity < size) {
                        if (m_capacity < DEFAULT_CAPACITY)
                            m_capacity = DEFAULT_CAPACITY;
                        while (m_capacity < size)
                            m_capacity <<= 1;
                        T *newData = new T[m_capacity];
                        for (int i = 0; i < m_size; i++)
                            newData[i] = m_data[i];
                        delete[] m_data;
                        m_data = newData;
                    }
                    m_size = size;
                }
    int         find(T t) const {
                    for (int i = 0; i < m_size; i++)
                        if (m_data[i]==t) return i;
                    return -1;
                }
    int         findBackward(T t) const {
                    for (int i = m_size - 1; i >= 0; i--)
                        if (m_data[i]==t) return i;
                    return -1;
                }
    bool        contains(T t) const {
                    if (find(t) == -1)
                        return false;
                    else
                        return true;
                }
    void        eliminate(T t) { remove(find(t)); }
                    
        
protected:
    int         m_size;
    int         m_capacity;
    T          *m_data;
};

#endif // ARRAY_H

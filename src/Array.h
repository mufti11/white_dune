/*
 * Array.h
 *
 * Copyright (C) 1999 Stephen F. White, 2019 J. "MUFTI" Scheurich
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

#define DEFAULT_CAPACITY 8

#define NO_RESIZE 1

#ifdef IRIX
#pragma set woff 3303
#endif

#include "stdafx.h"
#include <stdio.h>
#ifdef HAVE_OMP
# include <omp.h>
#endif

// Template for a C type array. Index: 0 ... size()-1 

template<class T> class MyArray {
public:
                MyArray(long capacity = DEFAULT_CAPACITY ) { 
                    if (capacity == 0)
                        capacity = DEFAULT_CAPACITY;
                    m_capacity = capacity; 
                    m_data = new T[capacity]; 
                    m_size = 0;
                    m_flags = 0;
                }
                MyArray(const MyArray<T> &a) {
                    m_capacity = m_size = a.m_size;
                    if (m_size == 0) 
                        m_capacity = DEFAULT_CAPACITY;
                    m_data = new T[m_capacity];
                    for (long i = 0; i < MIN(m_capacity, m_size); i++) 
                        m_data[i] = a.m_data[i];
                    m_flags = 0;
                }
                MyArray(const T *a, long len) {
                    m_size = 0;
                    setData(a, len);
                    m_flags = 0;
                }
                ~MyArray() {
                    if (m_data)
                        delete[] m_data; 
                    m_data = (T *)NULL;
                    m_size = 0;
                }

    const T    &get(long index) const
                { return m_data[index]; }
    void        set(long index, T t)
                { if (index >= m_size) resize(index+1); m_data[index] = t; }
    T          &operator[](long index)
                { if (index >= m_size) resize(index+1); return m_data[index]; }
    const T    &operator[](long index) const 
                { 
                    if (index >= m_size) {
                        ((MyArray *)this)->resize(index + 1);
                        static T t;
                        return t;
                    }
                    return m_data[index]; 
                }
    const T    *getData() const { return m_data; }
    void        setData(const T *a, long len) {
                    m_capacity = m_size = len; 
                    m_data = (T*)a;
                }
    T          *extractData() 
                {
                    T *data = m_data;
                    m_data = 0;
                    return data; 
                }
    long        size() const
                { return m_size; }
    void        append(T t)
                { 
                    // allow resize for append()
                    int flags = m_flags;
                    m_flags = 0;
                    (*this)[m_size] = t; 
                    m_flags = flags;
                }
    void        insert(T t, long index) {                   
                    resize(m_size + 1);
                    for (long i = (long)m_size - 1; i > (long)index; i--)
                         (*this)[i] = (*this)[i-1];
                    (*this)[index] = t; 
                }
    void        remove(long pos) {
                    if (pos + 1 != m_size)
                        for (long i = pos; i < (long)m_size - 1; i++)
                            m_data[i] = m_data[i + 1];
                    if (m_size > 0)
                        m_size--;
                    if (m_size <= 0) {
                        resize(0);
                        m_size = 0;
                    }
                }
    void        remove(long start, long end) {
                    assert(start >= 0 && start < m_size);
                    assert(end >= 0 && end < m_size);
                    long len = end - start + 1;
                    long deleted = 0;
                    for (long i = start; i <= end; i++)
                         if ((i + len) < m_size) {
                             deleted++;
                             m_data[i] = m_data[i + len];
                         }
                    m_size -= deleted;
                    if (m_size <= 0)
                        resize(0);
                }
    void        setNoResize(void) { m_flags = NO_RESIZE; }
    void        resize(long size) {
                    if (m_flags == NO_RESIZE)
                        return;
                    else if ((size >= 0) && (m_capacity < size)) {
                        if (m_capacity < DEFAULT_CAPACITY)
                            m_capacity = DEFAULT_CAPACITY;
                        while (m_capacity < size)
                            m_capacity <<= 1;
                        T *newData = new T[m_capacity];
                        for (long i = 0; i < m_size; i++)
                            newData[i] = m_data[i];
                        delete[] m_data;
                        m_data = newData;
                    }
                    m_size = size;
                }
    long        find(T t) const {
                    long ret = -1;
                    if (m_capacity < m_size)
                        return -1;
                    bool stopped = false;
                    #pragma omp parallel
                    {
                        #pragma omp for
                        for (long i = 0; i < m_size; i++)
                            if (m_data[i] == t) {
                               ret = i;
                               #pragma omp cancel for
                               i = m_size; // break
                               stopped = true;
                           }
                        #pragma omp barrier
                    }
                    if (!stopped)
                        return -1; // not found
                    return ret;
                }
    long        findBackward(T t) const {
                    if (m_size == 0)
                        return -1;
                    for (long i = (long)m_size - 1; i >= 0; i--)
                        if (m_data[i] == t) return i;
                    return -1;
                }
    long        findNumItems(T t) const {
                    long items = 0;
                    for (long i = (long)m_size - 1; i >= 0; i--)
                        if (m_data[i] == t) items++;
                    return items;
                }
    bool        contains(T t) const {
                    if (find(t) == -1)
                        return false;
                    else
                        return true;
                }
    void        eliminate(T t) { remove(find(t)); }
                    
        
protected:
    long        m_size;
    long        m_capacity;
    T          *m_data;
    int         m_flags;
};

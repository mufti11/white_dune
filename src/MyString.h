/*
 * MyString.h
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

#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mysnprintf.h"
#include "StringArray.h"

class MyString {

    class StringBuf {
    public:
        friend class MyString;
                StringBuf(const char *d)
                { 
                   if ((d != NULL) && strlen(d)) {
                       m_data = mystrdup(d); 
                   } else {
                       m_data = (char*) malloc(1);
                       m_data[0] = 0;
                   }
                   m_refs = 1; 
                   m_len = strlen(m_data); 
                   m_capacity = m_len + 1; 
                }
               ~StringBuf()
                {
                    if (m_refs == 0) {
                        free(m_data);
                        m_refs = -1; 
                    }
                }
    protected:
        char   *m_data;
        int     m_refs;
        int     m_len;
        int     m_capacity;
    };

    public:
                MyString();
                MyString(const char *str);
                MyString(char c);
                MyString(const MyString &s);
                MyString(int i);
               ~MyString();

    MyString   &operator =(const MyString &s);
    MyString   &operator +=(char c);
    MyString   &operator +=(const char *s);
                operator const char *() const
                   { 
                   if (m_stringBuf)
                       return m_stringBuf->m_data; 
                   return NULL;
                   }
    int         operator ==(const MyString &str) const;
    MyString   &copy(void);
    int         length() const { return m_stringBuf->m_len; }
    int         write(int filedes);
    const char *getData(void) const { return m_stringBuf->m_data; }

    bool        sub(MyString what, MyString with);
    bool        gsub(MyString what, MyString with);
    // replace string "what" with string "with"
    // but do not replace if "with" is found
    bool        gsubOnce(MyString what, MyString with);
    int         split(StringArray *array, MyString seperator);

    // number concatinators
    void        catInt(int number);
    void        catFloat(float number);
    void        catDouble(double number);
protected:
    StringBuf  *m_stringBuf;
};

extern int myhash(MyString key);


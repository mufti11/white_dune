/*
 * MyString.cpp
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

#include "stdafx.h"

#include "MyString.h"
#include "StringArray.h"

MyString &
MyString::operator=(const MyString &s)
{
    if ((!--m_stringBuf->m_refs) && (m_stringBuf->m_len != 0))
        delete m_stringBuf;
    m_stringBuf = s.m_stringBuf; 
    if (m_stringBuf->m_refs < 0)
        assert(0); 
    m_stringBuf->m_refs++;
    return *this;
}

MyString::MyString()
{
    m_stringBuf = new StringBuf("");
}

MyString::MyString(const char *str)
{
    m_stringBuf = new StringBuf(str); 
}

MyString::MyString(char c)
{
    char str[2];

    str[0] = c;  
    str[1] = '\0';
    m_stringBuf = new StringBuf(str);
}

MyString::MyString(const MyString &s)
{
    m_stringBuf = s.m_stringBuf; 
    m_stringBuf->m_refs++; 
}

MyString::MyString(int i)
{
    m_stringBuf = new StringBuf(""); 
}


MyString::~MyString()
{
    if (!--m_stringBuf->m_refs) 
        delete m_stringBuf;
}

int
MyString::operator ==(const MyString &str) const
{
    return m_stringBuf == str.m_stringBuf
                         || !strcmp(m_stringBuf->m_data, 
                             str.m_stringBuf->m_data);
}

MyString &
MyString::operator +=(const char *s)
{
    if (s == NULL)
        return *this;
    int len = strlen(s);
    int newLen = m_stringBuf->m_len + len;

    if (m_stringBuf->m_capacity == 0)
        m_stringBuf->m_capacity = 1;
    if (newLen >= m_stringBuf->m_capacity) {
        while (newLen >= m_stringBuf->m_capacity) {
            m_stringBuf->m_capacity <<= 1;
        }
        m_stringBuf->m_data = (char *) realloc(m_stringBuf->m_data, 
                                               m_stringBuf->m_capacity);
    }
    strcpy(m_stringBuf->m_data + m_stringBuf->m_len, s);
    m_stringBuf->m_len = newLen;
    return *this;
}

MyString &
MyString::operator +=(char c)
{
    char str[2];

    str[0] = c;  str[1] = '\0';
    return operator +=(str);
}

MyString &
MyString::copy(void)
{
    return *(new MyString(mystrdup(getData())));
}
 
int MyString::write(int f)
{
   return mywritestr(f, m_stringBuf->m_data);
}

int myhash(MyString key) 
{
    const char *p;
    unsigned h = 0, g;
    for (p = (const char *) key; *p; p++) {
        h = (h << 4) + (*p);
        g = h & 0xf0000000;
        if (g) {
            h = h ^ (g >> 24);
            h = h ^ g;
        }
    }
    return h;
}

// replace string "what" with string "with"
bool
MyString::gsub(MyString what, MyString with)
{
    MyString temp = *this;
    MyString newString = "";
    for (int i = 0; i < length(); i++)
        if (strncmp(&temp[i], what, what.length()) == 0) {
            newString += with;
            i += what.length() - 1;
        } else
            newString += temp[i];
    m_stringBuf = new StringBuf((const char *)newString);
    return true;
}

// replace string "what" with string "with"
// but do not replace if "with" is found

bool
MyString::gsubOnce(MyString what, MyString with)
{
    MyString temp = *this;
    MyString newString = "";
    for (int i = 0; i < length(); i++)
        if (strncmp(&temp[i], with, with.length()) == 0) {
            newString += with;
            i += with.length() - 1;
        } else if (strncmp(&temp[i], what, what.length()) == 0) {
            newString += with;
            i += what.length() - 1;
        } else
            newString += temp[i];
    m_stringBuf = new StringBuf((const char *)newString);
    return true;
}

int
MyString::split(StringArray *array, MyString seperator)
{
    if (seperator.length() == 0)
        return 0;
    int parts = 0;
    char firstChar = seperator[0];
    const char *searchPointer = *this;
    int seperatorSize = strlen(seperator);
    const char *foundPointer = NULL;
    while ((foundPointer = strchr(searchPointer, firstChar)) != NULL) {
       if (strncmp(foundPointer, seperator, seperatorSize) == 0) {
           // found
           (*array)[parts] = "";
           for (const char *ptr = searchPointer; ptr < foundPointer; ptr++)
               (*array)[parts] += *ptr;
           parts++;
       }
       searchPointer = foundPointer + 1;
    }
    return parts; 
}

void        
MyString::catInt(int number)
{
    char buf[128];
    mysnprintf(buf, 127, "%d", number);
    *this += buf;
}

void        
MyString::catFloat(float number)
{
    char buf[128];
    mysnprintf(buf, 127, "%f", number);
    *this += buf;
}

void        
MyString::catDouble(double number)
{
    char buf[128];
    mysnprintf(buf, 127, "%lf", number);
    *this += buf;
}


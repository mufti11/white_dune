/*
 * URL.h
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

#include "MyString.h"

class URL
{
public:
// constructors
                        URL(const char *u);
                        URL(const char *base, const char *url);
                        URL(void);

// parsing
    bool                Parse(const char *u = NULL);
    void                Rebuild(bool withTopic = true);
    void                TrimTopic(void) { Rebuild(false); }
#ifdef _WIN32
    void                TruncateToFit(HDC hDC, int pixels, MyString *out);
#endif
    operator const char *(void) const { return (const char *) m_url; }
    MyString            GetFileName();
    char               *GetFileNameWithoutExtension();
    const MyString     &GetPath() const { return m_path; }
    const MyString     &GetDir();
    MyString            ToPath() const;
    void                FromPath(const char *path);
    MyString            RelativeTo(const char *parentURL);
    static bool         isCurrentDirectory(const char *path);
    const char *        getScheme(void) const { return m_scheme; }
    const char *        getHostname(void) const { return m_hostname; }

private:
    void                SimplifyPath();
    bool                isJavascript(void);
    bool                isEcmascript(void);
    bool                isSortOfEcmascript(void);
    bool                notURN(void);
    bool                sameDrive(const char* url1, const char* url2);

private:
    MyString            m_url;
    MyString            m_scheme;
    MyString            m_username;
    MyString            m_password;
    MyString            m_hostname;
    unsigned short      m_port;
    MyString            m_path;
    MyString            m_topic;
};

MyString rewriteURL(const char *url, const char *oldBase, const char *newBase);
MyString fixCurrentPath(const char* url);
char getDriveLetter(const char* url);
bool isJavascript(const char* url);
bool isEcmascript(const char* url);
bool isX3domscript(const char* url);
bool isSortOfEcmascript(const char* url);
bool notURN(const char* url);
char *replaceHome(const char *path);
bool mkdir_parents4file(const char *path);
void myperror(const char *string);
void toPosixPath(char *target, char *source, unsigned int len);


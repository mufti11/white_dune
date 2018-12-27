/*
 * URL.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#ifdef _WIN32
# include <direct.h>
#endif
#include "stdafx.h"
#ifdef AIX
# include "strings.h"
#endif
#include "URL.h"
#include "maxpath.h"
#include "DuneApp.h"
#include "swt.h"

URL::URL(const char *u)
{
    Parse(u);
}

URL::URL(void)
{
    m_url = "";
    m_scheme = "";
    m_hostname = "";
    m_port = 80;
    m_path = "";
}

URL::URL(const char *base, const char *url)
{
    if (strstr(url, "://")) {   /* URL is already fully qualified */
        Parse(url);
    } else if (url[0] == '/') { /* URL is absolute */
        Parse(base);
        m_path = url;
        SimplifyPath();
    } else if (strstr(url, ":/")) {  /* URL is MS-DOS style */
        Parse(base);
#ifdef _WIN32
        m_path="";
#else 
        m_path="/";
#endif
        m_path+=url;
        SimplifyPath();
    } /* else if (strstr(url, "../")) { URL is relative 
        Parse(base);
        m_path=url;
    } */
       else {
        int len = strlen(base);
        if((len != 0) && (base[len-1] != '/')) {
            char *newBase = new char[len+1];
            strcpy(newBase, base);
            char *b = strrchr(newBase, '/');
#ifdef _WIN32
            char *w = strrchr(newBase, '\\');
#endif
            if (b) {
                *(b+1) = '\0';
                Parse(newBase);
                delete [] newBase;
#ifdef _WIN32
            } else if (w) {
                *(w+1) = '\0';
                Parse(newBase);
                delete [] newBase;
#endif
            } else
                Parse("./");
        } else {
            Parse(base);
        }
        m_path += url;
        SimplifyPath();
    }
    Rebuild();
}

bool
URL::Parse(const char *u)
{
    enum {
        START, SCHEME, SLASH1, SLASH2, HOST, PORT1, PORT, PATH, TOPIC, ERR
    } state = START;
    
    if (u == NULL) {
        u = m_url;
    } else {
        m_url = "";
        m_url += u;
    }
    
    m_scheme = "";
    m_hostname = "";
    m_port = 80;
    m_path = "";

    if (!u[0]) {
        return false;
    }

    bool dospath=false;
    char driveLetter=(char) 0;
#ifdef _WIN32
    if ((strlen(u) >= 2) && (u[1] == ':')) {
        dospath=true;
        driveLetter=u[0];
        m_path+=u[0];
        m_path+=u[1];
        m_path+="\\";
    }
#endif
    while(*u && state != ERR) {
        if ((*u == '|') && (u != m_url)) {
            dospath=true;
            driveLetter=*(u-1);
        }
        switch(state) {
          case START:
            if (*u != ' ') {
                m_scheme += *u;
                state = SCHEME;
            }
            break;
          case SCHEME:  // getting scheme
            if (isalnum(*u) || *u == '+' || *u == '-' || *u == '.') {
                m_scheme += *u;
            } else if (*u == ':') {
                state = SLASH1;
            } else if (*u == '/') {
                m_hostname = "";
                m_path = m_scheme;
                m_path += "/";
                m_scheme = "";
                state = PATH;
            } else {
                m_scheme += *u;
                m_hostname = "";
                m_path = m_scheme;
                state = PATH;
            }
            break;
          case SLASH1:  // getting first slash
            if (*u == '/') {
                state = SLASH2;
            } else if (isdigit(*u)) {  // another hack
                m_hostname = m_scheme;
                m_scheme = "";
                m_port = (short) (*u - '0');
                state = PORT;
            } else if (isalpha(*u)) {  // yet another hack, for IExplorer
                m_path += *u;
                state = PATH;
            }
            break;
          case SLASH2:  // getting second slash
            if (*u == '/') state = HOST; else state = ERR;
            break;
          case HOST:  // getting hostname
            if (isalnum(*u) || *u == '.' || *u == '-') {
                m_hostname += *u;
            } else if (*u == ':') {
                state = PORT1;
            } else if (*u == '/') {
                if (dospath) {
                     dospath=false;
                     m_path=driveLetter;
                     m_path+=':';
                }
                m_path += *u;
                state = PATH;
            } else {
                state = ERR;
            }
            break;
          case PORT1:
            if (isdigit(*u)) {
                m_port = (short) (*u - '0');
                state = PORT;
            } else {
                state = ERR;
            }
            break;
          case PORT: // waiting for port
            if (isdigit(*u)) {
                m_port = m_port * 10 + (*u - '0');
            } else if (*u == '/') {
                m_path += *u;
                state = PATH;
            } else {
                state = ERR;
            }
            break;
          case PATH:  // getting path
            if (*u == '#') {
                state = TOPIC;
            } else {
                m_path += *u;
            }
            break;
          case TOPIC:
            m_topic += *u;
            break;
          default:
            break;
        }
        u++;
    }

    if (state == SCHEME) {
        m_hostname = m_scheme;
        m_scheme = "";
        if (m_path.length() == 0) {
            m_path = "./";
            m_path += m_hostname;
            m_hostname = ""; 
        }
    }
    if (m_scheme.length() == 0) {
        if (!stringncmp(m_hostname, "ftp")) {
            m_scheme = "ftp";
        } else if (!stringncmp(m_hostname, "http")) {
            m_scheme = "http";
        } else {
            m_scheme = "file";
        }
    }
    if (m_path.length() == 0) {
        m_path = "/";
    }

    Rebuild();
    return (state != ERR);
}

static char *getpwd(void)
{
    char *pwd;
    if ((pwd = getenv("PWD")) != NULL) 
        pwd = mystrdup(pwd);
    else {
        char buf[MY_MAX_PATH];
        if (getcwd(buf, MY_MAX_PATH) == NULL)
            while (write(2, "getcwd failed\n", 14) == -1);
        pwd = mystrdup(buf);
    }

    // handle a path of form '"D:something"'
    if (pwd[0] == '"') {
        char *newPwd = mystrdup(pwd + 1);
        free(pwd);
        pwd = newPwd;
    }
    if (pwd[strlen(pwd)-1] == '"')
        pwd[strlen(pwd)-1] = (char) 0;

    return pwd;
}
    
void  URL::Rebuild(bool withTopic /* = true*/)
{
    if (!strcasecmp(m_scheme, "news") || 
        !strcasecmp(m_scheme, "mailto") ||
        !strcasecmp(m_scheme, "nntp")) {
        m_url = m_scheme;
        m_url += ":";
    } else {
        m_url = (const char *) m_scheme;
        if (m_scheme[0])
           m_url += "://";
        m_url += m_hostname;
        if (!strcasecmp(m_scheme, "http") && m_port != 80) {
            char buf[128];
            mysnprintf(buf, 128, ":%d", m_port);
            m_url += buf;
        }
    }
    m_url += m_path;
    if (withTopic && m_topic.length() > 0) {
        m_url += '#';
        m_url += m_topic;
    }
}

#ifdef _WIN32
void  URL::TruncateToFit(HDC hDC, int pixels, MyString *out)
{
    MyString prefix;
    const char *newpath = m_path;
    MyString filename;
    SIZE size;
    
    *out = m_scheme;
    *out += "://";
    *out += m_hostname;
    if (m_port != 80 || m_scheme != "http") {
        char buf[128];
        mysnprintf(buf, 128, ":%d", m_port);
        *out += buf;
    }
    *out += newpath;
    GetTextExtentPoint(hDC, *out, out->length(), &size);
    if (size.cx > pixels) {
        *out = m_scheme;
        *out += "://...";
        *out += newpath;
    }
    while (GetTextExtentPoint(hDC, *out, out->length(), &size) && 
           (size.cx > pixels)) {
        const char *slash = strchr(newpath + 1, '/');
        if (slash) {
            newpath = slash;
            *out = m_scheme;
            *out += "://...";
            *out += newpath;
        } else {
            break;
        }
    }
    if (GetTextExtentPoint(hDC, *out, out->length(), &size) && 
        (size.cx > pixels)) {
        // last ditch attempt to squeeze it smaller
        *out = ".../";
        *out += GetFileName();
    }
}
#endif

MyString URL::GetFileName(void)
{
    MyString filename = "";
    const char *slash = strrchr(m_path, '/');

    if (!slash) {
        slash = strrchr(m_path, '\\');
    }
    if (slash) {
        filename += slash + 1;
    } else {
        filename += m_path;
    }
    return filename;
}

char *
URL::GetFileNameWithoutExtension(void)
{
    char *filename = NULL;
    if (strlen(GetFileName()) == 0)
        filename = strdup("Untitled");
    else
        filename = strdup(GetFileName());
    char *dot = strchr(filename, '.');
    if (dot != NULL)
        dot[0] = 0;
    return filename;
}

MyString URL::RelativeTo(const char *parentURL)
{
#ifdef m_WIN32
    if (!sameDrive(m_url, parentURL))
        return m_url;
#endif

    URL parent(parentURL);
    MyString ret = "";
    const char *c, *p;

    if (m_scheme == parent.m_scheme && !strcasecmp(m_hostname, parent.m_hostname)
        && m_port == parent.m_port) {
        for (p = parent.m_path, c = m_path; *p && *p == *c; p++, c++) {}
        if (p == parent.m_path + 1) return m_path;
        if (*p && *c == '/') c--;
#ifdef _WIN32
        // test for paths starting with "Driveletter:" 
        if ((c == m_path) && (c[0]!=0) && (c[1]==':'))
            return(m_url);
#endif
        while ((p = strchr(p, '/')) != NULL) {
            ret += "../";
            p++;
        }
        while (c > (const char *) m_path && *c != '/') {
            c--;
        }
        ret += c + 1;
        return ret;
    } else {
        return m_url;
    }
}

MyString URL::ToPath() const
{
    MyString path;

#ifdef _WIN32
    if (m_hostname.length() != 0) {
        path = "\\\\";
        path += m_hostname;
        path += "\\";
    } else {
        path = "";
    }
    int i = 0;
    for (const char *c = m_path; *c; c++) {
        switch (*c) {
          case '/':
            if (i != 0)
                path += '\\';
            break;
          case '|':
            path += ':';
            break;
          default:
            path += *c;
            break;
        }
        i++;
    }
    return path;
#else
    return m_path;
#endif
}

void URL::SimplifyPath()
{
    const char *s1 = m_path;
    char *buf = new char[strlen(s1) + 1];
    char *s2 = buf;
    char *topic;

    *s2 = '\0';
    while (*s1) {
#ifdef _WIN32
        // finding /D: means D:windowspath/something
        if (s1[2] == ':') {
            m_path = mystrdup(s1 + 1);
            return;
        }
#endif
        if (!strncmp(s1, "/./", 3)) {
            s1 += 2;
        } else  {
            do {
                *s2++ = *s1++;
            } while (*s1 && *s1 != '/');
        }
        *s2 = '\0';
    }
#ifdef _WIN32
    // finding /D: means D:windowspath/something
    bool hasDriveColon = false;
    char *driveColon = strchr(buf, ':');
    if (driveColon != NULL) 
        if (strlen(buf) > (strlen(driveColon) + 1))
            if (*(driveColon - 2) == '/') {
                hasDriveColon = true;
                m_path = mystrdup(driveColon - 1);
            }        
    if (!hasDriveColon)
#endif
    topic = strstr(buf, "#");
    if (topic == NULL)
        m_path = mystrdup(buf);
    else {
        *topic = '\0';
        m_path = mystrdup(buf);
        m_topic = mystrdup(topic + 1);
    }
    delete [] buf;
}

void URL::FromPath(const char *path)
{
    MyString ret = "";
    URL myurl(path);
    if ((strcasecmp(myurl.getScheme(), "https") == 0) ||
        (strcasecmp(myurl.getScheme(), "http") == 0) ||
        (strcasecmp(myurl.getScheme(), "ftp") == 0)) {
        const char *myPath = TheApp->getDownloadDirectory();
        myPath = replaceHome(myPath);
        ret += myPath;
        free((void *)myPath);
        ret += swGetPathSeperator();
        ret += myurl.getHostname();
        ret += swGetPathSeperator();
        ret += '/';
        ret += myurl.ToPath();
        Parse(ret);
        path = ret;
     }
#ifdef _WIN32
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(path, drive, dir, fname, ext);
    m_url = "";
    m_scheme = "file";
    m_hostname = "";
    m_port = 80;
    m_path = '/';
    m_path += drive[0];
    m_path += '|';
    for (const char *p = dir; *p; p++) {
        if (*p == '\\') {
            m_path += '/';
        } else {
            m_path += *p;
        }
    }
    m_path += fname;
    m_path += ext;
    Rebuild();
#else
    m_scheme = "file";
    m_hostname = "";
    m_port = 80;
    if (path[0] == '/') {
        m_path = path;
    } else {        
        m_path = "";
        char *pwd = getpwd();
        m_path += pwd;
        free(pwd);
        m_path += '/';
        m_path += path;
        SimplifyPath();
    }
    Rebuild();
#endif
}

bool URL::isJavascript(void)
{
   return ::isJavascript(m_url);
}

bool URL::isEcmascript(void)
{
   return ::isEcmascript(m_url);
}

bool URL::isSortOfEcmascript(void)
{
   return ::isSortOfEcmascript(m_url);
}

bool URL::notURN(void)
{
   return ::notURN(m_url);
}

const MyString     &
URL::GetDir() 
{
    static MyString ret;
    int len;
    for (len=m_path.length()-1;len>=0;len--)
        if ((m_path[len]=='/') || (m_path[len]=='\\'))
           break;
    if (len<0)
        ret=m_path;
    else {
        ret="";
        for (int i=0;i<len;i++)
           ret+=m_path[i];  
    }

    return ret;
}

MyString 
fixCurrentPath(const char* url)
{
    // skip already existing "file:"
    if (stringncmp(url,"file:")==0) {
        // take care about file://Driverletter| constructs
        if (!getDriveLetter(url)) {
            if (stringncmp(url,"file://")==0)
                url += 7;
            else
                url += 5;
        }
    }

    if (url[0] == '"')
        url++;

    MyString* ret = new MyString(url);
    if ((strcmp(url,".") == 0) || 
        (strcmp(url,"./") == 0)) {
#ifdef _WIN32
        *ret = "file://";        
#endif
        char *pwd = getpwd();
        *ret += pwd;
        free(pwd);
        *ret += "/";
    }
    return *ret;
}

MyString
rewriteURL(const char *url, const char *oldBase, const char *newBase)
{
    if ((!isSortOfEcmascript(url)) && notURN(url)) {
        URL oldURL(fixCurrentPath(oldBase), fixCurrentPath(url));
        MyString newDir = "";
        if (URL::isCurrentDirectory(newBase)) {
            char *pwd = getpwd();
            newDir += pwd;
            free(pwd);
            newDir += "/";
        } else
            newDir += newBase;
        MyString newURL(oldURL.RelativeTo(fixCurrentPath(newDir)));
        return newURL;
    } else {
        MyString newURL(url);
#ifndef HAVE_DONT_REPLACE_VRMLSCRIPT
        if (strncasecmp(url,"vrmlscript:",strlen("vrmlscript:"))==0){
            newURL="java";
            newURL+=(const char*)(url+4);        
        }
#endif
        return newURL;
    }
}

bool                    
isJavascript(const char* url)
{
   char *string = (char *)url;
   while (isspace(string[0]))
       string++;
   if ((strncasecmp(string,"javascript:",strlen("javascript:"))==0) || 
       (strncasecmp(string,"vrmlscript:",strlen("vrmlscript:"))==0))
       return true;
   return false;
}

bool 
isEcmascript(const char* url)
{
   char *string = (char *)url;
   while (isspace(string[0]))
       string++;
   if (strncasecmp(string,"ecmascript:",strlen("ecmascript:"))==0)
       return true;
   return false;
}

bool 
isX3domscript(const char* url)
{
   char *string = (char *)url;
   while (isspace(string[0]))
       string++;
   if (strncasecmp(string,"x3domscript:",strlen("x3domscript:"))==0)
       return true;
   return false;
}

bool isSortOfEcmascript(const char* url)
{
   return isEcmascript(url) || isJavascript(url);
}


bool                    
notURN(const char* url)
{
   if (strncasecmp(url,"urn:",strlen("urn:"))==0) 
       return false;
   return true;
}

// check for file:///D| or file://D: (D is driverletter)
char getDriveLetter(const char* url)
{
    bool isDrive = true;
    if (stringncmp(url, "file:///") != 0) 
        isDrive = false;
    if (isDrive) {
        if (strlen(url) < strlen("file:///D|"))
            isDrive = false;
        if (isDrive) {
            if ((url[strlen("file:///D|")-1] != '|') &&
                (url[strlen("file:///D:")-1] != ':'))
                    isDrive = false;
                 else
                    return url[strlen("file:///D:")-2];
        }
    }
    if (!isDrive) {
        isDrive = true;
        if (stringncmp(url, "file://") != 0) 
            isDrive = false;
        if (isDrive) {
            if (strlen(url) < strlen("file://D:"))
                isDrive = false;
            if (isDrive) {
                if ((url[strlen("file://D|")-1] != '|') &&
                    (url[strlen("file://D:")-1] != ':'))
                    isDrive = false;
                else
                    return url[strlen("file://D:")-2];
            }
        }
    }

    return 0;
}

bool URL::sameDrive(const char* url1, const char* url2)
{
    char letter1 = getDriveLetter(url1);
    char letter2 = getDriveLetter(url2);
    if ((!letter1) || (!letter2))
        return false;
    // are the driveletters identical ?
    if (letter1 == letter2)
        return true;
    return false;
}

bool                
URL::isCurrentDirectory(const char *path)
{
    int numSlash = 0;
    for (int i = 0; i < strlen(path); i++)
        if (path[i] == '/')
            numSlash++;
    if (numSlash == 0)
        return true;
    else if (numSlash == 1)
        if (stringncmp(path, "./") == 0)
            return true;
    return false;
}

char *replaceHome(const char *path)
{
     MyString ret = "";
     int start = 0;
     if (path[0] == '~') {
         start = 1;
#ifdef _WIN32
         ret += getenv("HOMEDRIVE");
         ret += getenv("HOMEPATH");
#else
         ret += getenv("HOME");
#endif
         ret += swGetPathSeperator();     
     }
     ret += path + start;
     return strdup(ret);
}

bool mkdir_parents4file(const char *mypath)
{
     MyString ret = "";
     const char *path = replaceHome(mypath);
     for (int i = 0; i < strlen(path); i++)
         if ((i != 0) && ((path[i] == '/') || (path[i] == '\\'))) {
#ifdef _WIN32
             if (_mkdir(ret) == -1)
#else
             if (mkdir(ret, 0755) == -1)
#endif
                 if (errno != EEXIST) {
                     TheApp->MessageBoxPerror(ret);
                     return false;
                 }
             ret += swGetPathSeperator();
         } else
             ret += path[i];
     free((void *)path);
     return true;
}

void myperror(const char *string)
{
    TheApp->MessageBoxPerror(string);
}

void toPosixPath(char *target, char *source, int len)
{
    if (len > 0) {
        target[0] = '/';
        int i;
        for (i = 0; (i < strlen(source)) && (i < len - 1); i++)
            if (source[i] == '\\')
                target[i + 1] = '/';
            else
                target[i + 1] = source[i];
        target[i + 1] = 0;
    }
}

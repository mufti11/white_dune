/*
 * illegal2vrml.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2003 J. "MUFTI" Scheurich
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
#include "stdafx.h"

#include "DuneApp.h"
#include "Scene.h"
#include "xerrorhandler.h"

#ifdef _WIN32
#  define errorprintf msgboxprintf
#else
#  define errorprintf fprintf   
#endif

static char* prefix=NULL;

static
bool illegal2vrmlParseCommandlineArgument(int & i,int argc, char** argv)
{
    bool found=false;
    if (strcmp(argv[i],"-prefix")==0) {
       if (i++>=argc) return found;
       found=true;
       prefix=argv[i];
#ifdef MACOSX
    } else if (stringncmp(argv[i],"-psn_")==0) {
       // parse special MacOSX commandline parameters
       found=true;
#endif
    } else
       found=false;
    return found;    
}

#define errormsg \
   "usage:  illegal2vrml [-prefix prefix] file_with_protos.wrl file.wrl\n"

int illegal2vrml(int argc, char *argv[])
{
    TheApp = new DuneApp();
    bool *files=new bool[argc+1];
    for (int i=2;i<argc;i++)
       files[i]=false;
    int firstFileIndex=0;
    int secondFileIndex=0;
    int i=2;
    while (i<argc) {
       files[i]=!illegal2vrmlParseCommandlineArgument(i,argc,argv);
       if (files[i]) {
          if (firstFileIndex==0)
             firstFileIndex=i;
          else if (secondFileIndex==0)
             secondFileIndex=i;
          else {
             errorprintf(stderr, errormsg);
             return 1;
          }
       }
       i++;
    }
    if ((i>argc) || (firstFileIndex==0)) {
       errorprintf(stderr, errormsg);
       return 1;
       }
    Scene *scene = new Scene();
    scene->setExternProtoWarning(false);
    if (!TheApp->AddFile(argv[firstFileIndex], scene)) {
       myperror(argv[firstFileIndex]);
       return(1);
    }
    if (!scene->setPrefix(prefix)) {
       errorprintf(stderr, "prefix can not be generated, use \"-prefix\"\n");
       errorprintf(stderr, errormsg);
       return(1);
    }
    if ((i>argc) || (secondFileIndex==0)) {
       errorprintf(stderr, errormsg);
       return 1;
       }
    scene->setExternProtoWarning(true);
    if (!TheApp->AddFile(argv[secondFileIndex], scene)) {
       myperror(argv[secondFileIndex]);
       return(1);
    }
    scene->write(1, "./export.wrl", TEMP_SAVE);
    delete TheApp;
    return 0;
}


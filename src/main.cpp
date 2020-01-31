/*
 * main.cpp
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
#include "stdafx.h"

#include "swt.h"

#include "DuneApp.h"
#include "MainWindow.h"
#include "xerrorhandler.h"

#ifdef _WIN32
#  define errorprintf msgboxprintf
#else
#  define errorprintf fprintf   
#endif

extern bool parseCommandlineArgument(int &i,int argc, char** argv);
extern void parseCommandlineUsage(
#ifdef _WIN32
                                  HINSTANCE hInstance,
#endif
                                  int argc, char** argv);

#ifdef _WIN32
static int testCommandline8Bit(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
        if (strcmp(argv[i], "-8bit") == 0)
            return TRUE;
    return FALSE;
}
#endif

#ifndef _WIN32
static void testCommandlineFont(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
        if (stringncmp(argv[i],"-fn")==0) {
            if (i++>=argc) return;
            swSetDefaultFontSpec(argv[i]);
        } else if (stringncmp(argv[i],"-fp")==0) {
            if (i++>=argc) return;
            swSetFontPath(argv[i]);
        }
}
#endif

static int testCommandlineStereo(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
        if (strcmp(argv[i], "-stereo") == 0)
            return FORCE_QUAD_BUFFER_STEREO;
        else if (strcmp(argv[i], "-anaglyph") == 0) {
            if (i++>=argc) {
                errorprintf(stderr, 
                            "glasses type missing after -anaglyph option");
                return NO_STEREO;
            }
            if (strcmp(argv[i],"red_green") == 0)
                return RED_GREEN_ANAGLYPH_STEREO;
            else if (strcmp(argv[i],"green_red") == 0)
                return GREEN_RED_ANAGLYPH_STEREO;
            else if (strcmp(argv[i],"red_blue") == 0)
                return RED_BLUE_ANAGLYPH_STEREO;
            else if (strcmp(argv[i],"blue_red") == 0)
                return BLUE_RED_ANAGLYPH_STEREO;
            else if (strcmp(argv[i],"red_cyan") == 0)
                return RED_CYAN_ANAGLYPH_STEREO;
            else if (strcmp(argv[i],"cyan_red") == 0)
                return CYAN_RED_ANAGLYPH_STEREO;
            else {
                errorprintf(stderr,
                      "wrong glasses type. Currently supported: red_green green_red red_blue blue_red red_cyan cyan_red");
                return NO_STEREO;
            }
                
        }
#ifndef _WIN32
    return QUAD_BUFFER_STEREO;
#else
    return NO_STEREO;
#endif
}

#ifdef _WIN32
void emergency_rescue(int i)
{
    TheApp->emergency_rescue();
}

void startmainloop(void)
{
#ifndef HAVE_WANT_CORE
    __try {
#endif
       TheApp->initCallback();
       swMainLoop();
#ifndef HAVE_WANT_CORE
    } __except(EXCEPTION_EXECUTE_HANDLER) {
       emergency_rescue();
    }
#endif
}

// count words in a string with spaces

int get_argc_lpCmdLine(LPSTR lpCmdLine)
{
    int number_arguments=1;
    if (lpCmdLine[0]==0)
       return(number_arguments);
    for (int i=1;i<strlen(lpCmdLine);i++)
       if ((lpCmdLine[i]==' ') && (lpCmdLine[i-1]!=' '))
          number_arguments++;
    if (lpCmdLine[strlen(lpCmdLine)-1]!=' ')
       number_arguments++;
    return number_arguments;
}

// split string with spaces into unixlike argv structure

void get_argv_lpCmdLine(char** argv,int argc,LPSTR lpCmdLine)
{
    char* startptr=lpCmdLine;
    static char* dunename="dune";
    argv[0]=dunename;
    int number_argument=1;
    int i;
    for (i=1;i<strlen(lpCmdLine);i++) {
       if ((lpCmdLine[i]==' ') && (lpCmdLine[i-1]!=' ')) {
          argv[number_argument]=(char*) malloc(lpCmdLine+i-startptr+1);
          mystrncpy_danger(argv[number_argument],startptr,lpCmdLine+i-startptr);
          number_argument++;
       }
       if ((lpCmdLine[i]!=' ') && (lpCmdLine[i-1]==' '))
         startptr=lpCmdLine+i;
    }
    if (lpCmdLine[strlen(lpCmdLine)-1]!=' ') {
       argv[number_argument]=(char*) malloc(lpCmdLine+i-startptr+1);
       mystrncpy_danger(argv[number_argument],startptr,lpCmdLine+i-startptr);
    }
}

// skip arguments with a '-' sign in front and return rest

char* skiparguments(LPSTR lpCmdLine)
{
    if (lpCmdLine[0]==0)
       return lpCmdLine;

    if (lpCmdLine[0]!='-')
       if (lpCmdLine[0]!=' ')
          return lpCmdLine;

    for (int i=1;i<strlen(lpCmdLine);i++) {
       if (lpCmdLine[i]!='-')
          if ((lpCmdLine[i]!=' ') && (lpCmdLine[i-1]==' '))
             return lpCmdLine+i;
    }
    // all are arguments 
    return NULL;
}

// define if you have the CommandLineToArgvW(function) (WindowsNT 3.5 and later)
// #define HAVE_COMMANDLINE_TO_ARGV_W 1

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
#ifdef HAVE_WINDOWS_NX
# ifndef PROCESS_DEP_ENABLE
#  define PROCESS_DEP_ENABLE 1
# endif
    HMODULE hK = GetModuleHandleW(L"KERNEL32.DLL");
    BOOL (WINAPI *pfnSetDEP)(DWORD);
    *(FARPROC *) &pfnSetDEP = GetProcAddress(hK, "SetProcessDEPPolicy");
    (*pfnSetDEP)(PROCESS_DEP_ENABLE);
#endif
    TheApp = new DuneApp();
    int argc;
    char** argv;
    argc=get_argc_lpCmdLine(lpCmdLine);
#ifdef HAVE_COMMANDLINE_TO_ARGV_W
    argv=CommandLineToArgvW(lpCmdLine,&argc);
#else
    argv=(char**) malloc((argc+1)*sizeof(char**));
    get_argv_lpCmdLine(argv,argc,lpCmdLine);
#endif
    parseCommandlineUsage(hInstance, argc, argv);
    int stereoType = testCommandlineStereo(argc, argv);
    bool wantStereo = TheApp->GetBoolPreference("WantStereo", true);
    switch (stereoType) {
      case RED_GREEN_ANAGLYPH_STEREO:
      case GREEN_RED_ANAGLYPH_STEREO:
      case RED_BLUE_ANAGLYPH_STEREO:
      case BLUE_RED_ANAGLYPH_STEREO:
      case RED_CYAN_ANAGLYPH_STEREO:
      case CYAN_RED_ANAGLYPH_STEREO:
      case QUAD_BUFFER_STEREO:
      case FORCE_QUAD_BUFFER_STEREO:
        wantStereo = true; 
        break;
      default:
        if (wantStereo)
            stereoType = QUAD_BUFFER_STEREO;
    }
    if (stereoType == FORCE_QUAD_BUFFER_STEREO)
        stereoType = QUAD_BUFFER_STEREO;
    if (!wantStereo)
        stereoType = NO_STEREO;
    swInitialize(hInstance, stereoType, testCommandline8Bit(argc, argv));
    TheApp->setStereoType(stereoType);
    if ((stereoType != NO_STEREO) && (wantStereo)) { 
       TheApp->setUseStereo(true);
    } else {
       TheApp->setUseStereo(false);
    }
    int i=1;
    bool fileflag=false;
    bool fileerrorflag=false;
    while (i<argc) {
       if (!parseCommandlineArgument(i,argc,argv)) {
          // skip quotes
          if (argv[i][0] == '"')
              argv[i]++;
          if (argv[i][strlen(argv[i]) - 1] == '"')
              argv[i][strlen(argv[i]) - 1] = (char) 0;
          if (!TheApp->OpenFile(argv[i])) {
             // some of this buggy M$Windows filenames with blanks ?
             fileerrorflag=true; 
          } else {
             fileflag=true;
          }
       }
       i++;
    }
    if ((fileerrorflag==false) && (i>argc)) {
       errorprintf(stderr,"usage:  %s [options] [file.wrl]\n",argv[0]);
       normalExit(1);
       return 1;
    }
    if (fileerrorflag==true) { 
       if (skiparguments(lpCmdLine)==NULL) {
          // all arguments, but one must be wrong (cause of fileerrorflag)
          errorprintf(stderr,"some of the options is invalid\n");
          normalExit(1);
          return 1;
       }
       // skip quotes
       char *arguments = mystrdup(skiparguments(lpCmdLine));
       if (arguments[0] == '"')
           arguments++;
       if (arguments[strlen(arguments) - 1] == '"')
           arguments[strlen(arguments) - 1] = (char) 0;
       if (TheApp->OpenFile(arguments))
          fileflag=true; 
       else {
          errorprintf(stderr,"can not open %s\n",skiparguments(lpCmdLine));
          if (TheApp->getVrml1Error())
              normalExit(97);
          else
              normalExit(1);
          return 1;
       }
    }
    if (!fileflag)
       TheApp->OnFileNewWindow();
    startmainloop();
//    delete TheApp;
    normalExit(0);
    return 0;
}

#else
int main(int argc, char *argv[])
{
    if (argc < 0)
        argc = 0;
    TheApp = new DuneApp();
    parseCommandlineUsage(argc, argv);
    bool *files=new bool[argc+1];
    for (int i=1;i<argc;i++)
       files[i]=false;
    testCommandlineFont(argc, argv);
    // scan for parameters needed for X11/OpenGL start
    int stereoType = testCommandlineStereo(argc, argv);
    for (int i=1;i<argc;i++) {
       if (strcmp(argv[i], "-nostereo") == 0)
          stereoType = NO_STEREO;
#ifdef MACOSX
       if (strcmp(argv[i], "-startX11aqua") == 0)
           set_start_aqua_X11();       
#endif
    }
    bool wantStereo = TheApp->GetBoolPreference("WantStereo", true);
    switch (stereoType) {
      case RED_GREEN_ANAGLYPH_STEREO:
      case GREEN_RED_ANAGLYPH_STEREO:
      case RED_BLUE_ANAGLYPH_STEREO:
      case BLUE_RED_ANAGLYPH_STEREO:
      case RED_CYAN_ANAGLYPH_STEREO:
      case CYAN_RED_ANAGLYPH_STEREO:
      case QUAD_BUFFER_STEREO:
      case FORCE_QUAD_BUFFER_STEREO:
        wantStereo = true; 
    }
    if (stereoType == FORCE_QUAD_BUFFER_STEREO)
        stereoType = QUAD_BUFFER_STEREO;
    if (!wantStereo)
        stereoType = NO_STEREO;
    stereoType = swInitialize(&argc, argv, stereoType);
    TheApp->setStereoType(stereoType);
    if ((stereoType != NO_STEREO) && (wantStereo)) { 
       TheApp->setUseStereo(true);
    } else {
       TheApp->setUseStereo(false);
    }
    int i=1;
    bool fileflag=false;
    while (i<argc) {
       files[i]=false;
       if (!parseCommandlineArgument(i,argc,argv)) {
          files[i]=true;
          fileflag=true;
       }
       i++;
    }
    TheApp->accoutMaxNumberAxesInputDevices();
    if (i>argc) {
       errorprintf(stderr, 
            "usage:  %s [options] [file.wrl]\n",
             argv[0]);
       normalExit(1);
       return 1;
       }
    for (i=1;i<argc;i++)
       if (files[i]==true)
          if (!TheApp->OpenFile(argv[i])) {
             if (errno != 0)
                errorprintf(stderr,"%s: %s\n",strerror(errno),argv[i]); 
             else
                errorprintf(stderr,"failed: %s\n",argv[i]); 
             if (TheApp->getVrml1Error())
                 normalExit(97);
             else
                 normalExit(1);
             return 1;
          }
    if (!fileflag)
       TheApp->OnFileNewWindow();
#ifdef HAVE_FPU_ENABLE_INTERRUPTS
    fpu_enable_interrupts();
#endif
    TheApp->initCallback();
    swMainLoop();
//    delete TheApp;
    normalExit(0);
}
#endif

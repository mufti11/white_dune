/*
 * xerrorhandler.cpp
 *
 * Copyright (C) 2001 J. "MUFTI" Scheurich
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License
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
 *
 * Additional License - if you need another License 8-) 
  
     Permission is hereby granted, free of charge, to any person obtaining a
     copy of this file, to deal in this file without restriction, including
     without limitation the rights to use, copy, modify, merge, publish,
     distribute, and/or sell copies of this file, and to permit persons
     to whom this file is furnished to do so, provided that the above
     copyright notice(s) and this permission notice appear in all copies of
     this file and that both the above copyright notice(s) and this
     permission notice appear in supporting documentation.

     THIS FILE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
     OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
     OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
     HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
     INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
     FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
     NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
     WITH THE USE OR PERFORMANCE OF THIS FILE.

     Except as contained in this notice, the name of a copyright holder
     shall not be used in advertising or otherwise to promote the sale, use
     or other dealings in this Software without prior written authorization
     of the copyright holder.
 */

/* 
 * tries to catch signals and X11 errors and 
 * try to save the content in case of crashes
 */

#include "xerrorhandler.h"
#include "stdafx.h"
#include "MainWindow.h"
#include "DuneApp.h"
#include "swt.h"
#include "maxpath.h"

void coredump(void)
   {
   int *nix=NULL;
   nix[0]=1;
   }

static int normal_exit=0;
#ifdef _WIN32
extern "C" {
    int popupConsole();
}
#endif    

void setNormalExit(void)
{
    normal_exit=1;
}

void normalExit(int status)
{
    myflushall();
#ifdef _WIN32
    if (popupConsole()) {
        printf("Press Enter to Exit:");
        getchar();
    }
#endif    
    setNormalExit();
    exit(status);
}

#ifdef EXIT_HANDLER
void exit_handler(void)
{
//    if (normal_exit==0)
//        TheApp->emergency_rescue();        
}
#endif

#ifndef HAVE_WANT_CORE
# ifdef HAVE_NEW_NEW_HANDLER
#  ifndef HAVE_NEW_HANDLER
#   define HAVE_NEW_HANDLER 1
#  endif
# endif

# ifdef HAVE_NEW_HANDLER
#  ifdef HAVE_NEW_NEW_HANDLER
#   include <new>
#  else
#   include <new.h>
#  endif

static void dune_new_handler(void)
{
    TheApp->emergency_rescue();
//    normalExit(1);
}
# endif
#endif

/*
 *  Try to save scenegraphs into files
 */

#ifdef _WIN32
#include <process.h>
#include "stdafx.h"
#include "Windows.h"
#   define sleep(x)
#   define myMessageBox(x) MessageBox(NULL,x,"White_dune",MB_ICONINFORMATION);
#else
#   define mywriteerr(x) while(write(2,x,mystrlen(x)) == -1);
#endif


char* home;

int DuneApp::emergency_rescue(int sig)
#ifdef  __clang__
# if __MAGEIA__
 __attribute__((noreturn))
# endif
# ifdef __FreeBSD__
 __attribute__((noreturn))
# endif
# ifdef MACOSX
 __attribute__((noreturn))
# endif
#endif
   {
   int filenum=0;

   int console;
   
   myflushall();
   swUploadCleanupPasswd(m_upload);
   
#ifndef _WIN32
   console=open("/dev/console",O_WRONLY | O_NONBLOCK);
   if (console==-1)
      console=2;

   if ((sig != SIGPIPE) && !isatty(0)) {
       mywritestr(console,"white_dune: Internal Crash !\nwhite_dune: Try to save Files\n");
       mywriteerr("Internal Crash !\a\n Try to save Files\n");
   }
#endif
   for (List<MainWindow *>::Iterator *i=m_windows.first(); i; i=i->next())
      {
      char mypath[MY_MAX_PATH+1];
      char filename[MY_MAX_PATH+1];
      struct stat fileStat;
      bool x3d = i->item()->GetScene()->isX3d();
      const char *ext = "wrl";
      if (x3d)
          ext = "x3dv";
      int flags = TEMP_SAVE | (x3d ? X3DV : 0);
      int f = -1;
      do {
          filenum++;
          mysnprintf(filename,MY_MAX_PATH,".dune_crash_%d_%d.%s",getpid(),
                     filenum, ext);
#ifdef _WIN32
          mysnprintf(mypath,MY_MAX_PATH,"%s%s\\%s",getenv("HOMEDRIVE"),
                                                   getenv("HOMEPATH"),filename);
#else
          mysnprintf(mypath,MY_MAX_PATH,"%s/%s",home,filename);
#endif
          f = open(mypath, O_WRONLY | O_CREAT 
/* writing with O_SYNC is too slow on most systems... */
#     ifdef PARANOIA
#       ifdef O_SYNC
                                              | O_SYNC
#       endif 
#     endif 
#ifdef _WIN32
                                              ,_S_IREAD | _S_IWRITE);
#else
                                              ,S_IRUSR | S_IWUSR);
#endif
      } while (f == -1); 
#ifndef _WIN32
      mywritef(console, "%s:  %s\n", mypath, strerror(errno));
      mywritef(console,"white_dune: %s:  %s\n", mypath, strerror(errno));
#endif
      int writeError = false;
      if (i->item()->GetScene()->write(f, mypath, flags) < 0)
         writeError = true;
      if (!writeError && swTruncateClose(f))
         writeError = true;
      if (writeError)
         {
         mywritef(console, "write %s:  %s\n", mypath, strerror(errno));
         mywritef(console," error write white_dune: %s:  %s\n", mypath, 
                  strerror(errno));
         }
      else 
         {
         TheApp->setCrashFile(i->item()->GetScene()->getPath());
         TheApp->AddToRecentFiles(mypath);
         TheApp->SaveRecentFileList();
         TheApp->SavePreferences();
#ifdef _WIN32
         char message[1024];
         mysnprintf(message,1023,"file successfully written to %s",mypath);
         myMessageBox(message);
#else
         mywritestr(console,"white_dune: write successful to ");
         mywritestr(console,mypath);
         mywritestr(console,"\n");
#endif
      }
      if (TheApp->returnTracker())
         {
#ifndef _WIN32
         mywritestr(console,"white_dune: tracker stopped successfully\n");
         mywritestr(2,"tracker stopped successfully\n");
#endif
         }
      } 
#ifndef _WIN32
   if ((filenum==0) && ((sig != SIGPIPE) && !isatty(0)))
      {
      mywritestr(console, "white_dune: Nothing to save anymore\n");
      mywriteerr("Nothing to save anymore\n");
      }
#endif
   return filenum;
   }   

#ifdef _WIN32

    void init_handlers(void)
       {
# ifndef HAVE_WANT_CORE
#  ifdef HAVE_NEW_HANDLER
#   ifdef HAVE_NEW_NEW_HANDLER
       std::set_new_handler(dune_new_handler);
#   else
       set_new_handler(dune_new_handler);
#   endif
#  endif
# endif
       return;
       }

#else

/* 
 *  Handler for nofatal X11 Errors: you ignore them
 */ 

   int nofatal_handler(Display *display, XErrorEvent *xerrorevent)
      {
      static int numberX11Errors = 0;
      int limit = TheApp->GetX11ErrorsLimit();
      numberX11Errors++;
      if ((limit == -1) || (numberX11Errors <= limit))
         {
         /* so what ?*/
         char errortext[1025];   
   
         mywriteerr("nofatal X11 Error ignored\n");
         XGetErrorText(display,xerrorevent->error_code,errortext,1024);
         mywriteerr(errortext);
         mywriteerr("\n");
         } 
      else if (numberX11Errors == (limit + 1) && limit != 0)
         mywriteerr("X11ErrorsLimit (see $HOME/.dunerc) exceeded\n");
      return(1);
      }

/* 
 *  Handler for fatal X11 Errors: try to save files
 */ 

   int fatal_handler(Display *display)
      {
      TheApp->emergency_rescue();
      normalExit(2);
      return(0);
      }

/* 
 *  Handler for fatal Xt Errors: try to save files
 */ 

   void fatal_xthandler(char* message)
      {
      bool exitFlag = true;
      // the following error is (hopefully) not really fatal for white_dune
      if (strcmp(message,
                 "XtPopdown requires a subclass of shellWidgetClass") == 0) 
         exitFlag = false;
      // no file to save, not really a fatal problem ?
      if (exitFlag)
         if (TheApp->emergency_rescue() == 0)
            exitFlag = false;
      if (exitFlag) 
         {
         mywriteerr(message);
         mywriteerr("\n");
         normalExit(2);
         }
      }


#ifndef HAVE_WANT_CORE
/* 
 *  Handler for dangerous Unix Signals: try to save files
 */ 

volatile int fatal_error_in_progress = 0;   

# ifdef  HAVE_SIGNAL_INT
   void signal_fatal_handler(int sig)
      {
      signal (sig, SIG_IGN);  
      if (!fatal_error_in_progress)
         {
         fatal_error_in_progress = 1;
         TheApp->emergency_rescue(sig);
         signal (sig, SIG_DFL);
         mywritef(2,"killed by signal %d\n",sig);
         raise (sig);
         }
      }
# else
   void signal_fatal_handler()
      {
      for (int i=1;i<=64;i++)
         signal (i, SIG_IGN);  
      if (!fatal_error_in_progress) 
         {
         fatal_error_in_progress = 1; 
         TheApp->emergency_rescue();
         signal (SIGTERM, SIG_DFL);
         raise (SIGTERM);
         }
      }
# endif
#endif

/*
 *  Initalize X11 and Unix Signal Handlers
 */
   
#   ifndef TRUE
#      define TRUE 1
#      define FALSE 0
#   endif
   
    void init_handlers(void)
      {
      int dangerous_signals[65];
   
      /* initialise path to $HOME for usage in handler */
      home=getenv("HOME");

#ifdef EXIT_HANDLER
      atexit(&exit_handler);
#endif
# ifdef HAVE_NEW_HANDLER
  # ifdef HAVE_NEW_NEW_HANDLER
      std::set_new_handler(dune_new_handler);
  #else
      set_new_handler(dune_new_handler);
  # endif
# endif
      XSetErrorHandler(&nofatal_handler);
      XSetIOErrorHandler(&fatal_handler);
      XtSetErrorHandler(&fatal_xthandler);   
      for (int i=1;i<=32;i++)
         dangerous_signals[i]=TRUE;
   
      /* list of not dangerous signals */
      dangerous_signals[SIGCHLD]=FALSE;
      dangerous_signals[SIGWINCH]=FALSE;
      dangerous_signals[SIGURG]=FALSE;
      dangerous_signals[SIGSTOP]=FALSE;
      dangerous_signals[SIGTSTP]=FALSE;
      dangerous_signals[SIGCONT]=FALSE;
      dangerous_signals[SIGTTIN]=FALSE;
      dangerous_signals[SIGTTOU]=FALSE;
# ifdef SIGCKPT
      dangerous_signals[SIGCKPT]=FALSE;
# endif
# ifdef SIGRESTART
      dangerous_signals[SIGRESTART]=FALSE;
# endif
      /*
      dangerous_signals[SIGPWR]=FALSE;   
      */

      /* handle "dangerous Signals */
      for (int i=1;i<=32;i++)
         if (dangerous_signals[i]==TRUE)
            signal(i,&signal_fatal_handler);

      /* ignore CTRL-C interrupts */
      signal(SIGINT,SIG_IGN);

      /* handle known interrupts > 32 */
# ifdef SIGRTMIN
      signal(SIGRTMIN,&signal_fatal_handler);
# endif
# ifdef SIGRTMAX
#  ifdef __linux__ 
#   ifdef HAVE_GNUC_ECGS_2_8
     signal(SIGRTMAX,&signal_fatal_handler);
#   endif
#  else
    signal(SIGRTMAX,&signal_fatal_handler);
#  endif
# endif
#endif
   }

#ifdef HAVE_FPU_ENABLE_INTERRUPTS

# ifdef HAVE_FPU_SETCW
#  include <fpu_control.h>
# endif

# ifdef HAVE_HANDLE_SIGFPES
#  include <sigfpe.h>
# endif

void fpu_enable_interrupts(void)
   {

#  ifdef HAVE_FPU_SETCW
   unsigned int mask = _FPU_MASK_IM | 
                       _FPU_MASK_DM | 
                       _FPU_MASK_ZM | 
_FPU_MASK_PM | 
_FPU_MASK_UM | 
                       _FPU_MASK_OM ;
   mask=~mask & _FPU_DEFAULT;
   _FPU_SETCW(mask);  
#  endif

# ifdef HAVE_HANDLE_SIGFPES
   sigfpe_[_UNDERFL].abort   =1;
   sigfpe_[_OVERFL].abort    =1;
   sigfpe_[_INVALID].abort   =1;
   sigfpe_[_INT_OVERFL].abort=1;
   sigfpe_[_DIVZERO].abort   =1;

   unsigned int mask = /* _EN_UNDERFL | */
                       _EN_OVERFL  | 
                       _EN_DIVZERO | 
                       _EN_INVALID | 
                       _EN_INT_OVERFL;
   handle_sigfpes(_ON,mask,0,0,0);


# endif

   }

#endif



/*
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

#pragma once

void init_handlers(void);

extern "C" 
   {
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <errno.h>
#  include <stdlib.h>
#  include <stdio.h>
   }

void emergency_rescue(int sig = -1);

void setNormalExit(void);
void normalExit(int status);

#ifndef _WIN32

extern "C" 
   {
#  include <syslog.h>
#  include <unistd.h>
#  include <signal.h>
#  include <X11/Xlib.h>
#  include <X11/Intrinsic.h>
   }

   int nofatal_handler(Display *display, XErrorEvent *xerrorevent);
   int fatal_handler(Display *display);

#  ifdef HAVE_SIGNAL_INT
      // e.g. SGI IRIX 6.5 MIPSpro 7.3 or Linux
      void signal_fatal_handler(int sign);
#  else
      // e.g. SGI IRIX 6.5 gcc 2.95.2 
      void signal_fatal_handler();
#  endif

#  ifdef HAVE_FPU_ENABLE_INTERRUPTS
   void fpu_enable_interrupts(void);
#  endif 
#endif



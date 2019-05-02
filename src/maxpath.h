/*
 * maxpath.h
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

#ifndef _MAXPATH_H
#define _MAXPATH_H

#include <limits.h>  /* For the definition of PATH_MAX */

#ifdef PATH_MAX  
#   define MY_MAX_PATH PATH_MAX
#else
#   ifdef _POSIX_PATH_MAX
#      define MY_MAX_PATH _POSIX_PATH_MAX
#   else
#      define MY_MAX_PATH 255
#   endif
#endif

#endif

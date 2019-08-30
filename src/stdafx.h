/*
 * stdafx.h
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

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef _STDAFX_H
#define _STDAFX_H

// yet another stringcompare...
#define stringncmp(x,y) strncmp(x,y,strlen(y))

#ifdef _WIN32

#if _MSC_VER < 1800
#pragma string_gs_check(on)
#endif
#ifndef M_PI
# define M_PI              3.14159265358979323846
#endif
#define VC_EXTRALEAN      // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <sys/timeb.h>
#include <io.h>
#include <fcntl.h>
#include <direct.h>

#define strcasecmp _stricmp
#define strncasecmp _strnicmp

#include <stdio.h>

#define fileno _fileno

#include <float.h>
#include <limits.h>

#include <assert.h>

// redefine assert for a save before crash

#ifndef HAVE_WANT_CORE
#include "xerrorhandler.h"
#  undef _ASSERT_H
#  define _ASSERT_H
#  undef assert
#  define assert(x) { if (!(x)) emergency_rescue(); }
#endif

# define S_ISREG(x) (x & _S_IFREG)

// "__cdecl" is a special M$Windows visual studio compiler keyword
// rather useless but needed here, otherwise there are problems with the linker
#define compareFaceFunction __cdecl compareFace

#else // not WIN32

#include "config.h"

#include <unistd.h>

#include <limits.h>
/*
#include <values.h>
*/

#ifndef HAVE_GLDELETETEXTURES
#define glDeleteTextures glDeleteTexturesEXT
#endif

/* Define if you have glGenTextures.  */
#ifndef HAVE_GLGENTEXTURES
#define glGenTextures glGenTexturesEXT
#endif

#ifndef HAVE_GLBINDTEXTURE
#define glBindTexture glBindTextureEXT
#endif

#define compareFaceFunction compareFace

#endif // WIN32

#include <GL/gl.h>
#include "GL/glu.h"

// the include of gl.h should do this
#ifdef HAVE_GLFOGCOORDF
 extern "C" void glFogCoordf(GLfloat coord);
#endif

#define MY_CPLUSPLUS 1
#include "mysnprintf.h"

#ifndef MAX
#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b)   ((a) <= (b) ? (a) : (b))
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef CLAMP
#define CLAMP(v, min, max)  MIN(MAX(v, min), max)
#endif

//#ifndef PI
//#define PI 3.141592653589f
//#endif

#define FEPSILON 1E-6
#define EQUALF(a, b) (fabs((a) - (b)) < FEPSILON)

#ifndef RAD2DEG
#define RAD2DEG(a) ((a) * (180.0f / M_PI))
#define DEG2RAD(a) ((a) * (M_PI / 180.0f))
#endif

/* strlen("+2147483647") */
#define LEN_DEZIMAL_INT_MAX 11 

#include <assert.h>

#ifdef HAVE_WANT_CORE
    extern void coredump(void);
#   ifndef NDEBUG
#      undef _ASSERT_H
#      define _ASSERT_H
#      undef assert
#      define assert(x) { if (!(x)) coredump(); }
#   endif
#endif

#endif // _STDAFX_H


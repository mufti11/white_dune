/* 
 * openglutfont.h 
 * 
 * headerfile of implementation of 
 *
 * glutStrokeCharacter(GLUT_STROKE_ROMAN, 
 *
 * build from the sources of the OpenGLUT library 
 */

/*
 * parts of openglut_std.h
 *
 * The GLUT-compatible part of the OpenGLUT library include file
 *
 * Portions copyright (c) 2004, the OpenGLUT project contributors.
 * OpenGLUT branched from freeglut in Feburary, 2004.
 *
 * Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.
 * Written by Pawel W. Olszta, <olszta@sourceforge.net>
 * Creation date: Thu Dec 2 1999
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * GLUT API macro definitions -- fonts definitions
 */
#define  GLUT_STROKE_ROMAN               ((void *)0x0000)

/*
 * Font stuff, see og_font.c
 */
void glutStrokeCharacter( void* font, int character );


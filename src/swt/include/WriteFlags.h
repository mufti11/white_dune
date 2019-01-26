/*
 * WriteFlags.h
 *
 * Copyright (C) 2005 J. "MUFTI" Scheurich
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

#ifndef _WRITE_FLAGS_H
#define _WRITE_FLAGS_H

enum {
    WITHOUT_VALUE      = 1<<0,
    NULL_VALUE         = 1<<1,
    TEMP_SAVE          = 1<<2,
    TEMP_EXPORT        = 1<<3,
    PURE_VRML97        = 1<<4,
    PURE_X3DV          = 1<<5,
    X3DV               = 1<<6,
    X3D_XML            = 1<<7,
    PURE_X3D           = 1<<8,
    X3D_4_WONDERLAND   = 1<<9,
    TRIANGULATE        = 1<<10,
    QUAD_TRIANGULATE   = 1<<11,
    COVER              = 1<<12,
    KANIM              = 1<<13,
    X3DOM              = 1<<14,
    XITE               = 1<<15,
    CONVERT2X3D        = 1<<16,
    CONVERT2VRML       = 1<<17,
    CONVERT_TO_XML     = 1<<18,
    CONVERT_FROM_XML   = 1<<19,
    AC3D               = 1<<20,
    AC3D_4_RAVEN       = 1<<21,
    LDRAW_DAT          = 1<<22,
    C_SOURCE           = 1<<23,
    CC_SOURCE          = 1<<24,
    JAVA_SOURCE        = 1<<25,
    WONDERLAND         = 1<<26,
    MANY_JAVA_CLASSES  = 1<<27,
    OUTSIDE_JAVA_CLASS = 1<<28,
    RIB                = 1<<29,
    SKIP_SAVED_TEST    = 1<<30,
};

#define WRITE_JAVA_MEMBER_VARIABLES WRITE_JAVA_VARS

#endif

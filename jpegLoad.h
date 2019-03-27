/*
 * jpegLoad.h - a simplified interface to the jpeg library.
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

#ifndef _JPEG_LOAD_H
#define _JPEG_LOAD_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define EXPORT _declspec( dllexport )
#else
#define EXPORT
#endif

extern EXPORT int jpegOpen(const char *filename, int *width, int *height, int *components);
extern EXPORT void jpegRead(unsigned char *data);

#ifdef __cplusplus
}
#endif

#endif /* _JPEG_LOAD_H */

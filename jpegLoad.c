/*
 * jpegLoad.c
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
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "jpegLoad.h"

#ifdef HAVE_LIBJPEG
# include "jpeglib.h"
static FILE *infile = NULL;
static struct jpeg_decompress_struct cinfo;
static struct jpeg_error_mgr jerr;

#include <setjmp.h>
jmp_buf jpegenv;

int errorflag;

EXPORT void dummy_error_exit(j_common_ptr cinfo)
   {
   errorflag=1;
   longjmp(jpegenv,1);
   }

EXPORT int
jpegOpen(const char *filename, int *width, int *height, int *components)
{
    errorflag=0;
    if (!filename || !filename[0]) return FALSE;
    if (!(infile = fopen(filename, "rb"))) return FALSE;

    cinfo.err = jpeg_std_error(&jerr);
    cinfo.err->error_exit=dummy_error_exit;
    if (setjmp(jpegenv)!=0)
       return(FALSE);
    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    /* look at header parameters here? */
    jpeg_start_decompress(&cinfo);

    *width = cinfo.image_width;
    *height = cinfo.image_height;
    *components = cinfo.output_components;
    
    if (errorflag)
       return FALSE;
    else
       return TRUE;
}

EXPORT void
jpegRead(unsigned char *data)
{
    JSAMPARRAY buffer;
    int row_stride;
    unsigned char *d;

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)
             ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    d = data + (cinfo.image_height-1) * row_stride;

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(d, buffer[0], row_stride);
        d -= row_stride;
    }

    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
}
#else
EXPORT int
jpegOpen(const char *filename, int *width, int *height, int *components)
{
   return 0;
}

EXPORT void
jpegRead(unsigned char *data)
{
}
#endif

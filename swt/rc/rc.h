/*
 * rc.h
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

#ifndef _RC_H
#define _RC_H

#define ESCAPE_A (007)
#define ESCAPE_B (010)
#define ESCAPE_F (014)
#define ESCAPE_N (012)
#define ESCAPE_R (015)
#define ESCAPE_T (011)
#define ESCAPE_V (013)

extern char *rc_filename;
extern int rc_lineno;
extern char *yytext;

extern int yyparse(void);
extern int yylex(void);

extern RCBitmap **bitmaps;
extern RCNode **nodes;
extern RCString *rcStrings;

extern int numBitmaps;
extern int numDialogs;
extern int numMenus;
extern int numNodes;
extern int numStrings;

typedef struct BMPHeader {
    unsigned short type;  /* "BM" */
    unsigned long  size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned long  offset_to_data;
    unsigned long  header_size;
    long           width;
    long           height;
    unsigned short planes;  /* always 1 */
    unsigned short bits_per_pixel;
    unsigned long  compression_type;
    unsigned long  size_in_bytes;
    long           x_pixels_per_meter;
    long           y_pixels_per_meter;
    unsigned long  colours;
    unsigned long  important_colours;
} BMPHeader;

extern void rcparse_warning(const char *msg);

#endif

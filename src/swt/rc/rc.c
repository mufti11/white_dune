/*
 * rc.c
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

#include "res.h"
#include "rc.h"

#include <stdio.h>
#include <stdlib.h>

/* parse microsoft resource files, and output static data for swt resources */

static void rcdump(FILE *f);
static void dumpBitmap(FILE *f, RCBitmap *bitmap, int index);
static void dumpBitmapData(FILE *f, RCBitmap *bitmap, int index);
static const char *typeName(int type);
static void dumpQuotedString(FILE *f, const char *str);

int
main(int argc, char **argv)
{
    yyparse();
    rcdump(stdout);
    return 0;
}

void
rcparse_warning (const char *msg)
{
    fprintf (stderr, "%s:%d: %s\n", rc_filename, rc_lineno, msg);
}

static void rcdump(FILE *f)
{
    int i;

    fprintf(f, "#include <res.h>\n");
    fprintf(f, "\n");

    fprintf(f, "int swNumStrings = %d;\n\n", numStrings);

    if (numStrings == 0) {
        fprintf(f, "RCString swStrings[1] = { 0 };\n\n");
    } else {
        fprintf(f, "RCString swStrings[%d] = {\n", numStrings);

        for (i = 0; i < numStrings; i++) {
            fprintf(f, "    { %d, ", rcStrings[i].id);
            dumpQuotedString(f, rcStrings[i].str);
            fprintf(f, " },\n");
        }
        fprintf(f, "};\n\n");
    }

    fprintf(f,"/* only swString is needed for M$Windows */\n\n");
    fprintf(f, "#ifndef _WIN32\n\n");

    fprintf(f, "int swNumResources = %d;\n\n", numNodes);

    fprintf(f, "RCNode swResources[%d] = {\n", numNodes);

    for (i = 0; i < numNodes; i++) {
        RCNode *node = nodes[i];
        fprintf(f, "    { %s, %d, \"%s\", %d, %d, %d, %d, %d, %d, %d, %d },\n",
                typeName(node->type), node->id, node->name, node->x, node->y,
                node->width, node->height, node->flags, node->index, node->next,
                node->children);
    }
    fprintf(f, "};\n\n");

    for (i = 0; i < numBitmaps; i++) {
        dumpBitmapData(f, bitmaps[i], i);
    }

    fprintf(f, "int swNumBitmaps = %d;\n\n", numBitmaps);

    fprintf(f, "RCBitmap swBitmaps[%d] = {\n", numBitmaps);
    for (i = 0; i < numBitmaps; i++) {
        dumpBitmap(f, bitmaps[i], i);
    }
    fprintf(f, "};\n");

    fprintf(f, "\n\n#endif\n\n");
}

static const char *
typeName(int type)
{
    switch(type) {
      case RCT_DIALOG:
        return "RCT_DIALOG";
      case RCT_BUTTON:
        return "RCT_BUTTON";
      case RCT_SCROLLBAR:
        return "RCT_SCROLLBAR";
      case RCT_RADIOBUTTON:
        return "RCT_RADIOBUTTON";
      case RCT_CHECKBOX:
        return "RCT_CHECKBOX";
      case RCT_COMBOBOX:
        return "RCT_COMBOBOX";
      case RCT_GROUPBOX:
        return "RCT_GROUPBOX";
      case RCT_MENU:
        return "RCT_MENU";
      case RCT_MENUBAR:
        return "RCT_MENUBAR";
      case RCT_MENUITEM:
        return "RCT_MENUITEM";
      case RCT_LABEL:
        return "RCT_LABEL";
      case RCT_SEPARATOR:
        return "RCT_SEPARATOR";
      case RCT_TEXTEDIT:
        return "RCT_TEXTEDIT";
      case RCT_TOOLBAR:
        return "RCT_TOOLBAR";
      default:
        return "-1";
    }
}

static void
dumpBitmapData(FILE *f, RCBitmap *bitmap, int index)
{
    int i;
    int nc = (1 << bitmap->bits_per_pixel);
    
    fprintf(f, "static int palette%d[%d] = {\n    ", index, nc);
    for (i = 0; i < nc; i++) {
        fprintf(f, "0x%08x, ", bitmap->palette[i]);
        if (i % 4 == 3) fprintf(f, "\n    ");
    }
    fprintf(f, "};\n\n");
    
    fprintf(f, "static unsigned char bitmap%d[%d] = {\n    ", index, bitmap->size);
    for (i = 0; i < bitmap->size; i++) {
        fprintf(f, "0x%02x, ", bitmap->data[i]);
        if (i % 10 == 9) fprintf(f, "\n    ");
    }
    fprintf(f, "};\n\n");
}

static void
dumpBitmap(FILE *f, RCBitmap *bitmap, int index)
{
    fprintf(f, "    { ");
    fprintf(f, "%d, ", bitmap->id);
    fprintf(f, "%d, ", bitmap->size);
    fprintf(f, "%d, ", bitmap->width);
    fprintf(f, "%d, ", bitmap->height);
    fprintf(f, "%d, ", bitmap->bits_per_pixel);
    fprintf(f, "palette%d, ", index);
    fprintf(f, "bitmap%d ", index);

    fprintf(f, "},\n");
}

static void
dumpQuotedString(FILE *f, const char *str)
{
    const char *p;

    fputc('"', f);
    for (p = str; *p; p++) {
        if (*p == '\r') {
            /* do nothing */
        } else if (*p == '\n') {
            fputs("\\n", f);
        } else if (*p == '\t') {
            fputs("\\t", f);
        } else {
            fputc(*p, f);
        }
    }
    fputc('"', f);
}

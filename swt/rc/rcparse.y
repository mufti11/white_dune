/* rcparse.y - a yacc grammar for windows resource files */
/* heavily hacked up from the binutils parser (don't blame them)  */

/* Copyright 1997, 1998 Free Software Foundation, Inc.
   Written by Ian Lance Taylor, Cygnus Support.

   This file is part of GNU Binutils.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <res.h>
#include "rc.h"

static RCNode *newNode(int type, const char *name, int id, int x, int y,
                       int width, int height, int children);
static RCString *newString(int id, const char *str);
static RCNode *newControl(const char *name, int id, const char *type,
                          int flags, int x, int y, int width, int height);
static RCNode *append(RCNode *node1, RCNode *node2);
static RCBitmap *newBitmap(int id, const char *filename);
static void addAccelerator(int key, int modifiers, int command);
static int getVirtkey(const char *str);

void yyerror (char const *s);

RCBitmap **bitmaps = NULL;
RCNode **nodes = NULL;
RCString *rcStrings = NULL;

int numBitmaps = 0;
int numNodes = 0;
int numStrings = 0;

static int bitmapsAllocated = 0;
static int nodesAllocated = 0;
static int stringsAllocated = 0;
static RCNode *curDlg = NULL;
%}

%union
{
    RCNode *node;
    int i;
    const char *s;
};

%token BEG END
%token ACCELERATORS VIRTKEY ASCII NOINVERT SHIFT CONTROL ALT
%token BITMAP
%token CURSOR
%token DIALOG DIALOGEX EXSTYLE CAPTION CLASS STYLE
%token AUTO3STATE AUTOCHECKBOX AUTORADIOBUTTON CHECKBOX COMBOBOX CTEXT
%token DEFPUSHBUTTON EDITTEXT GROUPBOX LISTBOX LTEXT PUSHBOX PUSHBUTTON
%token RADIOBUTTON RTEXT SCROLLBAR STATE3 USERBUTTON
%token BEDIT HEDIT IEDIT
%token FONT
%token ICON
%token LANGUAGE CHARACTERISTICS VERSIONK
%token MENU MENUEX MENUITEM SEPARATOR POPUP CHECKED GRAYED HELP INACTIVE
%token MENUBARBREAK MENUBREAK
%token ES_READONLY ES_MULTILINE ES_PASSWORD
%token BS_AUTOCHECKBOX BS_AUTORADIOBUTTON
%token TOOLBAR BUTTON
%token MESSAGETABLE
%token RCDATA
%token STRINGTABLE
%token VERSIONINFO FILEVERSION PRODUCTVERSION FILEFLAGSMASK FILEFLAGS
%token FILEOS FILETYPE FILESUBTYPE BLOCKSTRINGFILEINFO BLOCKVARFILEINFO
%token VALUE
%token <s> BLOCK
%token MOVEABLE FIXED PURE IMPURE PRELOAD LOADONCALL DISCARDABLE
%token NOT
%token <s> QUOTEDSTRING STRING
%token <i> NUMBER
%token <ss> SIZEDSTRING
%token DLGINIT

%type <node>    params control controls menuitems menuitem
%type <node>    menuexitems menuexitem buttons button
%type <s>       optstringc file_name
%type <i>       id acc_event acc_option acc_options menuitem_flag menuitem_flags
%type <i>       style styleexpr optstyleexpr parenstyle

%left '|'
%left '^'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%right '~'
%left  NEGATE

%type <i>       expr

%%

stmts:
          /* empty */
        | stmts stmt
        ;

stmt:
          accelerator
        | bitmap
        | cursor
        | dialog
        | dlginit
        | font
        | icon
        | language
        | menu
        | menuex
        | toolbar
        | messagetable
        | rcdata
        | stringtable
        | user
        | versioninfo
        ;

accelerator:
          id ACCELERATORS suboptions BEG acc_entries END {}
        ;

acc_entries:
          /* empty */
        | acc_entries acc_entry
        ;

acc_entry:
          acc_event ',' id
          { addAccelerator($1, 0, $3); }
        | acc_event ',' id ',' acc_options
          { addAccelerator($1, $5, $3); }
        ;

acc_event:
          QUOTEDSTRING
          { $$ = $1[0]; }
        | id
          { $$ = getVirtkey(yytext); }
        ;

acc_options:
          acc_option
        | acc_options ',' acc_option
          { $$ = $1 | $3; }
        /* I've had one report that the comma is optional.  */
        | acc_options acc_option
          { $$ = $1 | $2; }
        ;

acc_option:
          VIRTKEY       { $$ = 0; }
        | ASCII         { $$ = 0; }
        | NOINVERT      { $$ = 0; }
        | SHIFT         { $$ = SW_SHIFT; }
        | CONTROL       { $$ = SW_CONTROL; }
        | ALT           { $$ = SW_ALT; }
        ;

/* Bitmap resources.  */

bitmap:
          id BITMAP memflags file_name
          { newBitmap($1, $4); }
        ;

/* Cursor resources.  */

cursor:
          id CURSOR memflags file_name {}
        ;

/* Dialog resources.  */

dialog:
          id dlg_token memflags exstyle expr ',' expr ',' expr ',' expr
          { curDlg = newNode(RCT_DIALOG, "", $1, $5, $7, $9, $11, -1); }
          styles BEG controls END
          { curDlg->children = $15->index; curDlg = NULL; }
        ;

dlg_token:
          DIALOG
        | DIALOGEX
        ;

exstyle:
          /* empty */
        | EXSTYLE '=' NUMBER
        ;

styles:
          /* empty */
        | styles CAPTION QUOTEDSTRING
          { if (curDlg) curDlg->name = strdup($3); }
        | styles CLASS id
        | styles STYLE styleexpr
        | styles EXSTYLE expr
        | styles FONT expr ',' QUOTEDSTRING
        | styles FONT expr ',' QUOTEDSTRING ',' expr ',' expr
        | styles FONT expr ',' QUOTEDSTRING ',' expr ',' expr ',' expr 
        | styles MENU id
        | styles CHARACTERISTICS expr
        | styles LANGUAGE id ',' id
        | styles VERSIONK expr
        ;

controls:
          /* empty */                   { $$ = NULL; }
        | controls control              { $$ = append($1, $2); }
        ;

control:
          AUTO3STATE params             { $$ = $2; $$->type = RCT_BUTTON; }
        | AUTOCHECKBOX params           { $$ = $2; $$->type = RCT_CHECKBOX; }
        | AUTORADIOBUTTON params        { $$ = $2; $$->type = RCT_RADIOBUTTON; }
        | BEDIT params                  { $$ = $2; }
        | CHECKBOX params               { $$ = $2; $$->type = RCT_CHECKBOX; }
        | COMBOBOX params               { $$ = $2; $$->type = RCT_COMBOBOX; }
        | CONTROL optstringc id ',' QUOTEDSTRING ',' styleexpr ',' expr
            ',' expr ',' expr ',' expr optexpr opt_control_data
                                        { $$ = newControl($2, $3, $5, $7, $9, 
                                                          $11, $13, $15); }
        | CTEXT params                  { $$ = $2; }
        | LTEXT params                  { $$ = $2; $$->type = RCT_LABEL; }
        | DEFPUSHBUTTON params          { $$ = $2; $$->type = RCT_BUTTON; }
        | EDITTEXT params               { $$ = $2; $$->type = RCT_TEXTEDIT;
                                          if (!($$->flags & SW_MULTI_LINE))
                                               $$->flags |= SW_SINGLE_LINE;
                                          if (!($$->flags & SW_NON_EDITABLE))
                                               $$->flags |= SW_EDITABLE; }
        | GROUPBOX params               { $$ = $2; $$->type = RCT_GROUPBOX; }
        | HEDIT params                  { $$ = $2; }
        | ICON optstringc id ',' id ',' expr ',' expr ',' expr ',' expr 
                                        { $$ = newNode(-1, $2, $3, $7, $9, $11,
                                                       $13, -1); }
        | IEDIT params                  { $$ = $2; }
        | LISTBOX params                { $$ = $2; }
        | PUSHBOX params                { $$ = $2; }
        | PUSHBUTTON params             { $$ = $2; $$->type = RCT_BUTTON; }
        | RADIOBUTTON params            { $$ = $2; $$->type = RCT_RADIOBUTTON; }
        | RTEXT params                  { $$ = $2; }
        | SCROLLBAR params              { $$ = $2; $$->type = RCT_SCROLLBAR; }
        | STATE3 params                 { $$ = $2; }
        | USERBUTTON params             { $$ = $2; }
        ;

params:
        optstringc id ',' expr ',' expr ',' expr ',' expr
          optstyleexpr opt_control_data
        { $$ = newNode(-1, $1, $2, $4, $6, $8, $10, -1);
          $$->flags = $11; }
        ;

opt_control_data:
          /* empty */
        | BEG optrcdata_data END
        ;

/* Font resources.  */

font:
          id FONT memflags file_name
          {}
        ;

icon:
          id ICON memflags file_name
          {}
        ;

language:
          LANGUAGE id ',' id
        ;

dlginit:
          id DLGINIT BEG dlgdataitems END
          {}
        ;

dlgdataitems:
          /* nothing */
        | dlgdataitems dlgdataitem
        ;

dlgdataitem:
          id ',' expr ',' expr ',' expr hexdata
          {}
        ;

hexdata:
          /* empty */
        | hexdata hexdatum
        ;

hexdatum:
          NUMBER                {}
        | QUOTEDSTRING          {}
        | NUMBER ','            {}
        | QUOTEDSTRING ','      {}
        ;

/* Menu resources.  */

menu:
          id MENU suboptions BEG menuitems END
          { newNode(RCT_MENUBAR, "", $1, 0, 0, 0, 0, $5->index); }
        ;

menuitems:
          /* empty */           { $$ = NULL; }
        | menuitems menuitem    { $$ = append($1, $2); }
        ;

menuitem:
          MENUITEM QUOTEDSTRING ',' id menuitem_flags
          { $$ = newNode(RCT_MENUITEM, $2, $4, 0, 0, $5, 0, -1); }
        | MENUITEM SEPARATOR
          { $$ = newNode(RCT_SEPARATOR, "---", -1, 0, 0, 0, 0, -1); }
        | POPUP QUOTEDSTRING menuitem_flags BEG menuitems END
          { $$ = newNode(RCT_MENU, $2, -1, 0, $3, 0, 0, $5->index); }
        ;

menuitem_flags:
          /* empty */                           { $$ = 0; }
        | menuitem_flags ',' menuitem_flag      { $$ = $1 | $3; }
        | menuitem_flags menuitem_flag          { $$ = $1 | $2; }
        ;

menuitem_flag:
          CHECKED                       { $$ = SW_MENU_CHECKED; }
        | GRAYED                        { $$ = SW_MENU_DISABLED; }
        | HELP                          { $$ = 0; }
        | INACTIVE                      { $$ = 0; }
        | MENUBARBREAK                  { $$ = 0; }
        | MENUBREAK                     { $$ = 0; }
        ;

/* Menuex resources.  */

menuex:
          id MENUEX suboptions BEG menuexitems END
          { newNode(RCT_MENU, "", $1, 0, 0, 0, 0, $5->index); }
        ;

menuexitems:
          /* empty */                   { $$ = NULL; }
        | menuexitems menuexitem        { $$ = append($1, $2); }
        ;

menuexitem:
          MENUITEM QUOTEDSTRING
          { $$ = newNode(RCT_MENUITEM, $2, -1, 0, 0, 0, 0, -1); }
        | MENUITEM QUOTEDSTRING ',' expr
          { $$ = newNode(RCT_MENUITEM, $2, $4, 0, 0, 0, 0, -1); }
        | MENUITEM QUOTEDSTRING ',' expr ',' expr optexpr
          { $$ = newNode(RCT_MENUITEM, $2, $4, 0, 0, 0, 0, -1); }
        | POPUP QUOTEDSTRING BEG menuexitems END
          { $$ = newNode(RCT_MENU, $2, -1, 0, 0, 0, 0, $4->index); }
        | POPUP QUOTEDSTRING ',' expr BEG menuexitems END
          { $$ = newNode(RCT_MENU, $2, $4, 0, 0, 0, 0, $6->index); }
        | POPUP QUOTEDSTRING ',' expr ',' expr BEG menuexitems END
          { $$ = newNode(RCT_MENU, $2, $4, 0, 0, 0, 0, $8->index); }
        | POPUP QUOTEDSTRING ',' expr ',' expr ',' expr optexpr
            BEG menuexitems END
          { $$ = newNode(RCT_MENU, $2, $4, 0, 0, 0, 0, $11->index); }
        ;

/* Toolbar resources */

toolbar:
          id TOOLBAR memflags expr ',' expr BEG buttons END
          { newNode(RCT_TOOLBAR, "", $1, 0, 0, 0, 0, $8->index); }
        ;

buttons:
          /* empty */                   { $$ = NULL; }
        | buttons button                { $$ = append($1, $2); }
        ;

button:
          BUTTON id
          { $$ = newNode(RCT_BUTTON, "", $2, 0, 0, 0, 0, -1); }
        | SEPARATOR
          { $$ = newNode(RCT_SEPARATOR, "", -1, 0, 0, 0, 0, -1); }
        ;

/* Messagetable resources.  */

messagetable:
          id MESSAGETABLE memflags file_name     {}
        ;

/* Rcdata resources.  */

rcdata:
          id RCDATA suboptions BEG optrcdata_data END    {}
        ;

optrcdata_data:
          /* empty */
        | rcdata_data
        ;

rcdata_data:
          SIZEDSTRING                   {}
        | expr                          {}
        | rcdata_data ',' SIZEDSTRING
        | rcdata_data ',' expr
        ;

/* Stringtable resources.  */

stringtable:
          STRINGTABLE suboptions BEG string_data END
        ;

string_data:
          /* empty */
        | string_data id QUOTEDSTRING                   { newString($2, $3); }
        | string_data id ',' QUOTEDSTRING               { newString($2, $4); }
        ;

user:
          id id suboptions BEG optrcdata_data END       {}
        | id id suboptions file_name                    {}
        ;

versioninfo:
          id VERSIONINFO fixedverinfo BEG verblocks END {}
        ;

fixedverinfo:
          /* empty */
        | fixedverinfo FILEVERSION expr ',' expr ',' expr ',' expr
        | fixedverinfo PRODUCTVERSION expr ',' expr ',' expr ',' expr
        | fixedverinfo FILEFLAGSMASK expr
        | fixedverinfo FILEFLAGS expr
        | fixedverinfo FILEOS expr
        | fixedverinfo FILETYPE expr
        | fixedverinfo FILESUBTYPE expr
        ;

/* To handle verblocks successfully, the lexer handles BLOCK
   specially.  A BLOCK "StringFileInfo" is returned as
   BLOCKSTRINGFILEINFO.  A BLOCK "VarFileInfo" is returned as
   BLOCKVARFILEINFO.  A BLOCK with some other string returns BLOCK
   with the string as the value.  */

verblocks:
          /* empty */
        | verblocks BLOCKSTRINGFILEINFO BEG BLOCK BEG vervals END END
        | verblocks BLOCKVARFILEINFO BEG VALUE QUOTEDSTRING vertrans END
        ;

vervals:
          /* empty */
        | vervals VALUE QUOTEDSTRING ',' QUOTEDSTRING
        ;

vertrans:
          /* empty */
        | vertrans ',' expr ',' expr
        ;

/* A resource ID.  */

id:
          NUMBER                { $$ = $1; }
        | '-' NUMBER            { $$ = -$2; }
        | STRING                { $$ = -1; }
        ;

/* Generic suboptions.  These may appear before the BEGIN in any
   multiline statement.  */

suboptions:
          /* empty */
        | suboptions suboption
        ;

suboption:
          memflag
        | CHARACTERISTICS expr
        | LANGUAGE id ',' id
        | VERSIONK expr
        ;

memflags:
          /* empty */
        | memflags memflag
        ;

memflag:
          MOVEABLE
        | FIXED
        | PURE
        | IMPURE
        | PRELOAD
        | LOADONCALL
        | DISCARDABLE
        ;

/* A file name.  */

file_name:
          QUOTEDSTRING
        | STRING
        ;

optstyleexpr:
          /* empty */                   { $$ = 0; }
        | ',' styleexpr                 { $$ = $2; }
        ;

styleexpr:
          parenstyle                    { $$ = $1; }
        | NOT parenstyle                { $$ = 0; }
        | styleexpr '|' parenstyle      { $$ = $1 | $3; }
        | styleexpr '|' NOT parenstyle  { $$ = $1; }
        ;

parenstyle:
          style                         { $$ = $1; }
        | '(' style ')'                 { $$ = $2; }
        ;

style:
          ES_MULTILINE                  { $$ = SW_MULTI_LINE; }
        | ES_READONLY                   { $$ = SW_NON_EDITABLE; }
        | ES_PASSWORD                   { $$ = SW_PASSWORD; }
        | BS_AUTOCHECKBOX               { $$ = SW_AUTOCHECKBOX; }
        | BS_AUTORADIOBUTTON            { $$ = SW_AUTORADIOBUTTON; }
        | id                            { $$ = 0; }
        ;

optexpr:
          /* empty */
        | ',' expr
        ;

optstringc:
          /* empty */
        { $$ = ""; }
        | QUOTEDSTRING ','
        { $$ = $1; }

        ;

expr:
          NUMBER                { $$ = $1; }
        | '(' expr ')'          { $$ = $2; }
        | '~' expr              { $$ = ~ $2; }
        | '-' expr %prec NEGATE { $$ = - $2; }
        | expr '*' expr         { $$ = $1 * $3; }
        | expr '/' expr         { $$ = $1 / $3; }
        | expr '%' expr         { $$ = $1 % $3; }
        | expr '+' expr         { $$ = $1 + $3; }
        | expr '-' expr         { $$ = $1 - $3; }
        | expr '&' expr         { $$ = $1 & $3; }
        | expr '^' expr         { $$ = $1 ^ $3; }
        | expr '|' expr         { $$ = $1 | $3; }
        

%%

void
yyerror(const char *s)
{
    fprintf(stderr, "%s:%d: %s\n", rc_filename, rc_lineno, s);
    exit(1);
}

static RCNode *
newNode(int type, const char *name, int id, int x, int y,
        int width, int height, int children)
{
    RCNode *node = (RCNode *) malloc(sizeof(RCNode));
    if (nodesAllocated == 0) {
        nodesAllocated = 16;
        nodes = (RCNode **) malloc(nodesAllocated * sizeof(RCNode *));
    } else if (numNodes >= nodesAllocated) {
        nodesAllocated *= 2;
        nodes = (RCNode **) realloc(nodes, nodesAllocated * sizeof(RCNode *));
    }
    node->type = type;
    node->name = strdup(name);
    node->id = id;
    node->x = x;
    node->y = y;
    node->width = width;
    node->height = height;
    node->index = numNodes;
    node->flags = 0;
    node->children = children;
    node->next = -1;

    nodes[numNodes++] = node;
    return node;
}

static RCNode *
newControl(const char *name, int id, const char *type, int flags, int x, int y,
           int width, int height)
{
    if (strcmp(type, "Button") == 0) {
        if (flags & SW_AUTOCHECKBOX) {
            return newNode(RCT_CHECKBOX, name, id, x, y, width, height, -1);
        } else if (flags & SW_AUTORADIOBUTTON) {
            return newNode(RCT_RADIOBUTTON, name, id, x, y, width, height, -1);
        }
    }
    fprintf(stderr, "warning: unhandled type(\"%s\")/style in control\n", type);
    return newNode(-1, name, id, x, y, width, height, -1);
}

static RCString *
newString(int id, const char *str)
{
    RCString *s;
    if (stringsAllocated == 0) {
        stringsAllocated = 16;
        rcStrings = (RCString *) malloc(stringsAllocated * sizeof(RCString));
    } else if (numStrings >= stringsAllocated) {
        stringsAllocated *= 2;
        rcStrings = (RCString *) realloc(rcStrings,
                                         stringsAllocated * sizeof(RCString));
    }
    s = rcStrings + numStrings++;
    s->id = id;
    s->str = strdup(str);
    return s;
}

static short
readShort(FILE *f)
{
    unsigned char c1, c2;

    c1 = getc(f);
    c2 = getc(f);

    return c1 | c2 << 8;
}

static long
readLong(FILE *f)
{
    unsigned char c1, c2, c3, c4;

    c1 = getc(f);
    c2 = getc(f);
    c3 = getc(f);
    c4 = getc(f);

    return c1 | c2 << 8 | c3 << 16 | c4 << 24;
}

static RCBitmap *
newBitmap(int id, const char *filename)
{
    BMPHeader header;
    RCBitmap *bitmap;
    FILE *f;
    int ncolours;
    int y, width_in_bytes, i;
    int pad;
    char padbuf[4];

    f = fopen(filename, "rb");

    if (!f)
    {
        perror(filename);
        return NULL;
    }

    header.type = readShort(f);
    header.size = readLong(f);
    header.reserved1 = readShort(f);  /* reserved */
    header.reserved2 = readShort(f);
    header.offset_to_data = readLong(f);
    header.header_size = readLong(f);
    header.width = readLong(f);
    header.height = readLong(f);
    header.planes = readShort(f);
    header.bits_per_pixel = readShort(f);
    header.compression_type = readLong(f);
    header.size_in_bytes = readLong(f);
    header.x_pixels_per_meter = readLong(f);
    header.y_pixels_per_meter = readLong(f);
    header.colours = readLong(f);
    header.important_colours = readLong(f);

    bitmap = (RCBitmap *) malloc(sizeof(RCBitmap));

    bitmap->id = id;
    bitmap->width = header.width;
    bitmap->height = header.height;
    bitmap->bits_per_pixel = header.bits_per_pixel;

    if (bitmapsAllocated == 0) {
        bitmapsAllocated = 16;
        bitmaps = (RCBitmap **) malloc(bitmapsAllocated * sizeof(RCNode *));
    } else if (numBitmaps >= bitmapsAllocated) {
        bitmapsAllocated *= 2;
        bitmaps = (RCBitmap **) realloc(bitmaps, bitmapsAllocated * sizeof(RCNode *));
    }

    bitmap->size = header.size_in_bytes;
    bitmap->data = (unsigned char *) malloc(header.size);
    ncolours = 1 << bitmap->bits_per_pixel;
    bitmap->palette = (int *) malloc (ncolours * sizeof(int));
    for (i = 0; i < ncolours; i++) {
        unsigned char b = getc(f);
        unsigned char g = getc(f);
        unsigned char r = getc(f);
        (void) getc(f);
        bitmap->palette[i] = (b << 16) | (g << 8) | r;
    }
    width_in_bytes = bitmap->width * bitmap->bits_per_pixel / 8;
    pad = (4 - width_in_bytes) % 4;
    for (y = 0; y < bitmap->height; y++) {
        while (fread(bitmap->data + (bitmap->height - 1 - y) * width_in_bytes,
                     1, width_in_bytes, f) == -1);
        if (pad > 0) while (fread(padbuf, 1, pad, f) == -1);
    }
    fclose (f);
    bitmaps[numBitmaps++] = bitmap;
    return bitmap;
}

static void
addAccelerator(int key, int modifiers, int command)
{
    int i;
    
    /* search for menu corresponding to command */
    for (i = 0; i < numNodes; i++) {
        if (nodes[i]->type == RCT_MENUITEM && nodes[i]->id == command) {
            nodes[i]->x = key;
            nodes[i]->y = modifiers;
        }
    }
}

static RCNode *
append(RCNode *node1, RCNode *node2)
{
    RCNode *s;

    if (node1) {
        for (s = node1; s->next != -1; s = nodes[s->next])
            ;
        s->next = node2->index;
        return node1;
    } else {
        return node2;
    }
}

static int
getVirtkey(const char *str)
{
    if (!strcmp(str, "VK_INSERT"))
        return SW_INSERT;
    if (!strcmp(str, "VK_DELETE"))
        return SW_DELETE;
    return 0;
}

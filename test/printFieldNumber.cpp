/*
 * printNodeType.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2009 J. "MUFTI" Scheurich
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
#include "stdafx.h"

#include "swt.h"

#include "DuneApp.h"
#include "MainWindow.h"
#include "MoveCommand.h"
#include "xerrorhandler.h"
#include "Field.h"
#include "Types.h"

#define errorprintf fprintf   

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s fieldname\n", argv[0]);
        exit(1);
    }
    
    TheApp = new DuneApp();
    Scene* scene = new Scene();
    bool x3d = true;
    StringArray *names = scene->getAllNodeNames();
    for (int i=0; i < names->size(); i++) {
        Node* node = scene->createNode(names->get(i)); 
        if (node->getProto()->lookupField(argv[1]) > 0)
            printf("%s %d\n", (const char *)node->getProto()->getName(x3d),
                                  node->getProto()->lookupField(argv[1]));
    }
    delete TheApp;
    normalExit(0);
    return 0;
}


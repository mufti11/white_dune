/*
 * selftest.cpp
 *
 * Copyright (C) 1999 Stephen F. White, 2003 J. "MUFTI" Scheurich
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

#include <time.h>

#include "swt.h"

#include "DuneApp.h"
#include "MainWindow.h"
#include "MoveCommand.h"
#include "xerrorhandler.h"
#include "Field.h"
#include "Types.h"

bool cover = false;
bool kambi = false;
bool x3dom = false;

#define errorprintf fprintf   

long seed;

void printFirstPart()
{
    printf("#include <stdio.h>\n");

    printf("#include \"stdafx.h\"\n");
    printf("\n");
    printf("#include \"swt.h\"\n");
    printf("\n");
    printf("#include \"DuneApp.h\"\n");
    printf("#include \"MainWindow.h\"\n");
    printf("#include \"MoveCommand.h\"\n");
    printf("#include \"xerrorhandler.h\"\n");
    printf("#include \"Field.h\"\n");
    printf("#include \"AllNodes.h\"\n");
    printf("\n");
    printf("#define errorprintf fprintf   \n");
    printf("\n");
    printf("int main(int argc, char *argv[])\n");
    printf("{\n");
    printf("    TheApp = new DuneApp();\n");
    if (cover)
        printf("    TheApp->setCoverMode();;\n");   
    if (kambi)
        printf("    TheApp->setKambiMode();;\n");   
    if (x3dom)
        printf("    TheApp->setX3domMode();;\n");   
    printf("    Scene* scene = new Scene();\n");
    printf("    scene->setX3d();\n");
}

void printLastPart()
{
    printf("    delete TheApp;\n");
    printf("    normalExit(0);\n");
    printf("    return 0;\n");
    printf("}\n");
}


int main(int argc, char *argv[])
{
    bool x3d = false;
    TheApp = new DuneApp();
    int i;
    bool fileflag = false;
    Scene* scene = new Scene();
    for (i=1;i<argc;i++) {
        if (strcmp(argv[i], "-x3d") == 0)
            x3d = true;
        else if (strcmp(argv[i], "-cover") == 0)
            cover = true;
        else if (strcmp(argv[i], "-kambi") == 0)
            kambi = true;
        else if (strcmp(argv[i], "-x3dom") == 0)
            x3dom = true;
        else if (!TheApp->ImportFile(argv[i], scene)) {
            fileflag = true;
            delete scene;
            perror(argv[i]);
            return(1);
        }
    }
    if (fileflag == false) {
       StringArray *names = scene->getAllNodeNames();
       printFirstPart();
       if (x3d)
           scene->setX3dv();
       if (cover)
           TheApp->setCoverMode();
       if (kambi)
           TheApp->setKambiMode();
       if (x3dom)
           TheApp->setX3domMode();
       for (i=0; i < names->size(); i++) {
           Node* node = scene->createNode(names->get(i)); 
           const char *str = names->get(i);
           // skip fake nodes
           if (strcmp(str,"#") == 0)
               continue;
           if (strcmp(str,"IMPORT") == 0)
               continue;
           if (strcmp(str,"EXPORT") == 0)
               continue;
           if (!cover)
               if (node->getProto()->isCoverProto() && !TheApp->getCoverMode())
                   continue;
           if (stringncmp(str,"Text3D") == 0)
               continue;
           if (strcmp(str, "ContourPolyline2D") == 0)
               continue;
           if (strcmp(str, "RenderedTexture") == 0)
               continue;
           if (strcmp(str, "MultiPart") == 0)
               continue;
           if (strcmp(str, "CollidableShape") == 0)
               continue;
           if (strcmp(str, "BooleanSequencer") == 0)
               continue;
           if (strcmp(str, "IntegerSequencer") == 0)
               continue;
           if (strcmp(str, "NurbsCurve") == 0)
               continue;
           if (strcmp(str, "NurbsSweptSurface") == 0)
               continue;
           if (strcmp(str, "NurbsSwungSurface") == 0)
               continue;

           seed = time(0);
           printf("    srand48(%ld%s);\n", seed, "L");

           printf("    Node%s* node%s = (Node%s*)scene->createNode(\"%s\"); \n",
                  str,str,str,str);
           for (int j = 0; j < node->getProto()->getNumFields(); j++) {
               Field *field = node->getProto()->getField(j);
               if (scene->isInvalidElement(field))
                   continue;
               if (x3d && (field->getFlags() & FF_VRML_ONLY))
                   continue;
               if (field->getFlags() & FF_X3D_ONLY)
                   continue;
               if (!cover && (field->getFlags() & FF_COVER_ONLY))
                   continue;
               if (!kambi && (field->getFlags() & FF_KAMBI_ONLY))
                   continue;
               if (!x3dom && (field->getFlags() & FF_X3DOM_ONLY))
                   continue;
               const char *fieldName = field->getName(false);
               if (strcmp(fieldName, "metadata") != 0)
                   if (((strcmp(str, "NurbsCurve") == 0) ||
                        (strcmp(str, "NurbsSurface") == 0) ||
                        (strcmp(str, "NurbsTrimmedSurface") == 0) ||
                        (strcmp(str, "CurveAnimation") == 0)) &&
                       ((strcmp(fieldName, "controlPoint") == 0) ||
                        (strcmp(fieldName, "controlPointX3D") == 0)))
                       printf("    node%s->createControlPoints(new MFVec3f());\n",
                              str);
                   else {
                       printf("    node%s->%s((%s *)node%s->%s()%s%d%s",
                              str, fieldName, 
                              typeEnumToString(field->getType()),
                              str, fieldName, "->getRandom(scene, ", 
                              field->getNodeType(), "));");
                       printf("\n");
                   }
           }
           printf("    scene->execute(new MoveCommand(node%s, NULL, -1, scene->getRoot(), \n",
                  str);
           printf("                                   scene->getRootField()));\n");
       }
       if (x3d)
            printf("    scene->write(STDOUT_FILENO,\"\",X3DV);\n");
       else
            printf("    scene->write(STDOUT_FILENO,\"\",true);\n");
       printLastPart();
    }
    delete TheApp;
    normalExit(0);
    return 0;
}


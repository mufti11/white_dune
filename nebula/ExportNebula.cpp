/*
 * ExportNebula.cpp
 *
 * This file adds the SAND to Dune:  Save As Nebula Device
 *
 * This File: Copyright (C) 2001 Aaron Cram
 * 
 * Dune: Copyright (C) 2000 Stephen F. White
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


#include "../stdafx.h"
#include "../mysnprintf.h"


#ifdef HAVE_SAND     /////  skip this file if feature not enabled

#include "ExportNebula.h"
#include "../resource.h"

#include "tools/wftoolbox.h"
#include "tools/wftools.h"

#ifdef WIN32
# include <direct.h>  // for mkdir()
#else
# include <sys/stat.h>
# define _stricmp strcasecmp 
#endif

#include "../StatusBar.h"
#include "../Scene.h"
#include "../Node.h"
#include "../Proto.h"
#include "../ExposedField.h"
#include "../Field.h"
#include "../MFFloat.h"
#include "../MFNode.h"
#include "../MFVec3f.h"
#include "../MFVec2f.h"
#include "../MFInt32.h"
#include "../MFString.h"
#include "../SFInt32.h"
#include "../SFBool.h"
#include "../SFNode.h"
#include "../SFVec3f.h"
#include "../SFColor.h"
#include "../SFRotation.h"
#include "../Vec3f.h"
#include "../NodeImageTexture.h"




ExportNebula::ExportNebula(void)
{
    _scene = NULL;
    _currentDepth = _startDepth = 1;

    SetIndent(_startDepth);

    _textured = false;
    _alphaMap = false;

    _tclFP = NULL;


    _bbspExpand = true;       // automatically preserve rotation/scale of billboards and sprites
    _texEnabled = true;       // enable texture maps
    _writeMaterials = true;   // output material library in .tcl file
    _alphaTestRef = 0.7f;     // default ".setAlphaRef" value for alpha test
    _debugInScript = false;   // insert additional debugging info in .tcl file

    _writeMeshes = true;      // output mesh files
    _relativePaths = false;   // makes the matlib refs relative, useful for avatars
    _alphaTest = true;        // adds a default alpha test to alpha-mapped materials
}

void ExportNebula::InitMenu(SMENU menu)
{
    _writeMeshes = true;      // output mesh files
    _relativePaths = false;   // makes the matlib refs relative, useful for avatars
    _alphaTest = true;        // adds a default alpha test to alpha-mapped materials

    SMENU sandMenu = swAppendMenu(menu, "SAND");
    swAppendMenuItem(sandMenu, "Export Scene", ID_SAND_EXPORT);
    swAppendSeparator(sandMenu);
    swAppendMenuItem(sandMenu, "Write Meshes", ID_SAND_WRITEMESHES);
    swAppendMenuItem(sandMenu, "Alpha Test Enabled", ID_SAND_ALPHATEST);
    swAppendMenuItem(sandMenu, "Use Relative Paths", ID_SAND_RELPATHS);
    swMenuSetFlags(menu, ID_SAND_WRITEMESHES, SW_MENU_CHECKED, SW_MENU_CHECKED);
    swMenuSetFlags(menu, ID_SAND_ALPHATEST, SW_MENU_CHECKED, SW_MENU_CHECKED);

}

void ExportNebula::OnToggleWriteMeshes(SMENU menu)
{
    if (_writeMeshes)
    {
        _writeMeshes = false;
        swMenuSetFlags(menu, ID_SAND_WRITEMESHES, SW_MENU_CHECKED, 0);
    }
    else
    {
        _writeMeshes = true;
        swMenuSetFlags(menu, ID_SAND_WRITEMESHES, SW_MENU_CHECKED, SW_MENU_CHECKED);
    }
}

void ExportNebula::OnToggleUseRelativePaths(SMENU menu)
{
    if (_relativePaths)
    {
        _relativePaths = false;
        swMenuSetFlags(menu, ID_SAND_RELPATHS, SW_MENU_CHECKED, 0);
    }
    else
    {
        _relativePaths = true;
        swMenuSetFlags(menu, ID_SAND_RELPATHS, SW_MENU_CHECKED, SW_MENU_CHECKED);
    }
}

void ExportNebula::OnToggleAlphaTest(SMENU menu)
{
    if (_alphaTest)
    {
        _alphaTest = false;
        swMenuSetFlags(menu, ID_SAND_ALPHATEST, SW_MENU_CHECKED, 0);
    }
    else
    {
        _alphaTest = true;
        swMenuSetFlags(menu, ID_SAND_ALPHATEST, SW_MENU_CHECKED, SW_MENU_CHECKED);
    }
}


bool ExportNebula::ExportScene(char *path, Scene *scene, StatusBar *statusBar)
{

    _statusBar = statusBar;
    strcpy(_tclPath, path);
    if (!strstr(_tclPath, ".tcl"))   // TODO:  make case-insensitive
    {
        strcat(_tclPath, ".tcl");
    }

    _scene = scene;

    mysnprintf(_buf, 4096, "Exporting  %s...", _tclPath);
    _statusBar->SetText(_buf);


    // setup paths and directories

    strcpy(_tclDir, _tclPath);
    char *endOfPath = strrchr(_tclDir, '\\');
    if (endOfPath != NULL)
    {
        endOfPath++;
        *endOfPath='\0';
    }

//    mysnprintf(_buf, 4096, "\n  tclDir:  %s\n", _tclDir);
//    swDebugf(_buf);


    mysnprintf(_meshDir, 4096, "%smeshes\\", _tclDir);
    mysnprintf(_mapDir, 4096, "%smaps\\", _tclDir);

#ifdef WIN32
    mkdir(_meshDir);
    mkdir(_mapDir);
#else
    mkdir(_meshDir, S_IRWXU);
    mkdir(_mapDir, S_IRWXU);
#endif

    WriteSetupScript();

    _tclFP = fopen(_tclPath, "w");
    if (!_tclFP)
    {
        swMessageBox(TheApp->mainWnd(), "Failed to open TCL file!", "Export Error:", SW_MB_OK, SW_MB_ERROR);
        return false;
    }

    if (_debugInScript)
    {
        fprintf(_tclFP, "#####  debug mode on  #####\n");
        fprintf(_tclFP, "set debugFile [open \"debug.txt\" \"w\"]\n\n");
    }


    if (_relativePaths)
    {
        fprintf(_tclFP, "new n3dnode matlib\n");
        fprintf(_tclFP, "sel matlib\n");
    }
    else
    {
        fprintf(_tclFP, "sel /usr/local/matlib\n");
    }


    WalkScenegraph(_scene->getRoot(), _startDepth);

    if (_debugInScript)
    {
        fprintf(_tclFP, "close $debugFile\n");
    }


    fclose(_tclFP);

    _statusBar->SetText("Done.");
    return true;
}



void ExportNebula::WriteSetupScript(void)
{

    char setupPath[4096];
    strcpy(setupPath, _tclPath);

    // extract filename from path
    char scriptName[512];
    int spLen = strlen(setupPath);
    int i;
    int fNameLoc = 0;
    for (i=spLen; i>0; i--)
    {
        if (setupPath[i]=='\\' || setupPath[i]=='/') break;
    }
    if (i < spLen)
    {
        fNameLoc = i+1;
        char* temp = setupPath+fNameLoc;
        strcpy(scriptName, temp);
    }
    else return;


    // change file name to "world.tcl"
    setupPath[fNameLoc] = '\0';  
    strcat(setupPath, "world.tcl");


    // check to see if "world.tcl" already exists.
    FILE* exists = fopen(setupPath, "r");
    if (exists) 
    {
        fclose(exists);
        // file is already there, we'll append onto it.
        _tclFP = fopen(setupPath, "a+");
        if (!_tclFP)
        {
            swMessageBox(TheApp->mainWnd(), "Failed to append onto setup file!", "Export Error:", SW_MB_OK, SW_MB_ERROR);
        }
    }
    else
    {
        // no file exists, so we'll start over from scratch
        _tclFP = fopen(setupPath, "w");
        if (!_tclFP)
        {
            swMessageBox(TheApp->mainWnd(), "Failed to create a new setup file!", "Export Error:", SW_MB_OK, SW_MB_ERROR);
        }

        if (_writeMaterials)
        {
            fprintf(_tclFP, "sel /usr/local\n");
            fprintf(_tclFP, "new nroot matlib%s\n",_spaces);
        }

        fprintf(_tclFP, "sel /usr/local/scene\n");
        fprintf(_tclFP, "new n3dnode world\n");
        fprintf(_tclFP, "sel world\n");
        fprintf(_tclFP, "new n3dnode lights\n");   /// begin lights   TODO:   make optional??
        fprintf(_tclFP, "sel lights\n"); 
        fprintf(_tclFP, "  new nlightnode ambient\n");
        fprintf(_tclFP, "  ambient.settype ambient\n");
        fprintf(_tclFP, "  ambient.setcolor 0.5 0.5 0.5 1.0\n");
        fprintf(_tclFP, "sel ..\n\n");
    }

    fprintf(_tclFP, "source %s\n", scriptName); // source the scene file

    if (_tclFP) fclose(_tclFP);
}


void ExportNebula::SetIndent(int numSpaces)
{
    _currentDepth = numSpaces;
    int tab = 2;
    memset(_spaces, ' ', 100);
    if (numSpaces*tab > 100) _spaces[100]='\0';
    else if (numSpaces < 0) _spaces[0]='\0';
    else _spaces[numSpaces*tab]='\0'; 
}



//  recursive function that walks the entire VRML scenegraph, 
//  saving the required script commands and meshes as it goes
void ExportNebula::WalkScenegraph(Node *node, int depth)
{

    if (!_tclFP) return;

//    mysnprintf(_buf, 4096, "Depth: %i  Type: %i  Name: %s  \n", depth, node->getType(), (const char *) (node->getName()));
//    swDebugf(_buf);

    _inSwitch = false;

    NodeList *children=NULL;
    FieldValue *value=NULL;
    const char *name;
    int numChildren = 0;
    int i=0;
    int tmp=0;

    const float *fpVector;
    Vec3f Vector;

    char *flag;
    bool billboard = false;
    bool sprite = false;

    switch (node->getType())
    {
    case NODE_GROUP:
        children = ((MFNode*)node->getField(0))->getValues();
        numChildren = children->size();
//        mysnprintf(_buf, 4096, "\n  numChildren:  %i\n", numChildren);
//        swDebugf(_buf);


        for (i=0; i<numChildren; i++)
        {
            WalkScenegraph(children->get(i), depth);
            if (children->get(i)->getType() == NODE_TRANSFORM)
            {
                SetIndent(depth);
                fprintf(_tclFP, "%ssel ..\n", _spaces);
//                fprintf(_tclFP, "#####   NODE_GROUP\n");
            }
        }
    break;
//    case NODE_DIRECTIONAL_LIGHT:
    case NODE_POINT_LIGHT:
        name = node->getName();
        fprintf(_tclFP, "%snew n3dnode lights/%s\n",_spaces, name);
        fprintf(_tclFP, "%ssel lights/%s\n", _spaces, name);

        fpVector = ((SFVec3f *) node->getField(4))->getValue();
        fprintf(_tclFP, "%s  .txyz %f %f %f\n", _spaces, fpVector[0], fpVector[1], fpVector[2]);

        fprintf(_tclFP, "%s  new nlightnode light\n",_spaces);
        fprintf(_tclFP, "%s  sel light\n",_spaces);

        fprintf(_tclFP, "%s    .settype point\n",_spaces);


        fpVector = ((SFColor *) node->getField(2))->getValue();
        fprintf(_tclFP, "%s    .setcolor %f %f %f 1\n",_spaces, fpVector[0], fpVector[1], fpVector[2]);   ///
        fprintf(_tclFP, "%s    .setattenuation 0.7 0 0\n",_spaces);

        fprintf(_tclFP, "%s  sel /usr/local/scene/world\n",_spaces);


    break;
    case NODE_TRANSFORM:

        SetIndent(depth);

        name = node->getName();
        fprintf(_tclFP, "%snew n3dnode %s\n", _spaces, name);
        fprintf(_tclFP, "%ssel %s\n", _spaces, name);

        SetIndent(++depth);


        //  parse render flags
        strcpy(_buf, name);
        flag = strtok(_buf, "_");
        while( flag != NULL )
        {
            if (_stricmp(flag, "bb") == 0) billboard = true;
            if (_stricmp(flag, "sp") == 0) sprite = true;
            flag = strtok(NULL, "_");
        }


        if (sprite)
        {
            fprintf(_tclFP, "%s.setsprite true\n", _spaces);
        }
        else if (billboard)
        {
            fprintf(_tclFP, "%s.setbillboard true\n", _spaces);
        }



        //  output translation
        value = node->getField(5);
        if (!value->equals(node->getProto()->getField(5)->getDefault()))  // only if not default
        {
            fpVector = ((SFVec3f*)value)->getValue();
            fprintf(_tclFP, "%s.txyz %f %f %f\n", _spaces, fpVector[0], fpVector[1], fpVector[2]);
        }



        if (_bbspExpand && (sprite || billboard))
        {
            fprintf(_tclFP, "%snew n3dnode transform\n", _spaces);
            fprintf(_tclFP, "%ssel transform\n", _spaces);
            SetIndent(++depth);
        }


        //  output rotation
        value = node->getField(2);
        Vector = ((SFRotation*)value)->getEulerAngles(0);
        if (Vector.x!=0.0 || Vector.y!=0.0 || Vector.z!=0.0)  // only if not default
        {
            fprintf(_tclFP, "%s.rxyz %f %f %f\n", _spaces, Vector.x, Vector.y, Vector.z);
        }    


        //  output scale
        value = node->getField(3);
        if (!value->equals(node->getProto()->getField(3)->getDefault()))  // only if not default
        {
            fpVector = ((SFVec3f*)value)->getValue();
            fprintf(_tclFP, "%s.sxyz %f %f %f\n", _spaces, fpVector[0], fpVector[1], fpVector[2]);
        }    



        children = ((MFNode*)node->getField(1))->getValues();
        numChildren = children->size();
        for (i=0; i<numChildren; i++)
        {
            if (children->get(i)->getType() == NODE_SHAPE)
            {
                WriteShape(children->get(i));

                if (_bbspExpand && (sprite || billboard))    //  go back one more for billboards/sprites
                {
                    SetIndent(_currentDepth-1);
                    fprintf(_tclFP, "%ssel ..\n", _spaces);
                }
            }
            else
            {
                WalkScenegraph(children->get(i), depth);
                SetIndent(depth);
                fprintf(_tclFP, "%ssel ..\n", _spaces);
//                fprintf(_tclFP, "#####   NODE_TRANSFORM (if)\n");
            }
        }

//        SetIndent(depth);
//        fprintf(_tclFP, "%ssel ..\n", _spaces);
//        fprintf(_tclFP, "#####   NODE_TRANSFORM\n");
    break;

    case NODE_SWITCH:
        _inSwitch = true;
        children = ((MFNode*)node->getField(0))->getValues();
        numChildren = children->size();
        for (i=0; i<numChildren; i++)
        {
            if (children->get(i)->getType() == NODE_SHAPE)
            {
                WriteShape(children->get(i));
            }
            else
            {
                //  ???
            }
        }

        if (_relativePaths)
        {
            fprintf(_tclFP, "sel ..\n");
        }
        else
        {
            fprintf(_tclFP, "sel /usr/local/scene/world\n");
        }

/*
        fprintf(_tclFP, "new n3dnode world\n");
        fprintf(_tclFP, "sel world\n");
        fprintf(_tclFP, "new n3dnode lights\n");   /// begin lights   TODO:   make optional  ********
        fprintf(_tclFP, "sel lights\n"); 
        fprintf(_tclFP, "  new nlightnode ambient\n");
        fprintf(_tclFP, "  ambient.settype ambient\n");
        fprintf(_tclFP, "  ambient.setcolor 0.5 0.5 0.5 1.0\n");
        fprintf(_tclFP, "sel ..\n");
*/
        _inSwitch = false;
    break;
    }

}


void ExportNebula::WriteShape(Node *node)
{
    if (!_tclFP) return;

//    int type = node->getProto()->getField(0)->getType();
//    mysnprintf(_buf, 4096, "%sNODE_SHAPE  %i\n", _spaces, type);
//    swDebugf(_buf);


    Node *geoNode = ((SFNode*)node->getField(1))->getValue();
    if (geoNode != NULL)
    {
        if (geoNode->getType() != NODE_INDEXED_FACE_SET)
        {
            static bool hasBeenWarned = false;
            if (!hasBeenWarned)
            {
                swMessageBox(TheApp->mainWnd(), "Nebula exporter only understands Geometry of type IndexedFaceSet.",
                    "Export Warning:", SW_MB_OK, SW_MB_WARNING);
                hasBeenWarned = true;
            }
            return;
        }

        if (!_inSwitch)
        {
            const char *name = geoNode->getName();

            if (_debugInScript)
            {
                fprintf(_tclFP, "puts $debugFile \"%s\"\n", name); 
            }


            fprintf(_tclFP, "%snew n3dnode %s\n", _spaces, name);
            fprintf(_tclFP, "%ssel %s\n", _spaces, name);
            SetIndent(_currentDepth+1);
        }
        WriteGeometry(geoNode);
    }


    Node *appNode = ((SFNode*)node->getField(0))->getValue();
    if (appNode != NULL)
    {
        if (_writeMaterials)
        {
            WriteAppearance(appNode);
        }
    }

    if (!_inSwitch)
    {
        SetIndent(_currentDepth-1);
        fprintf(_tclFP, "%ssel ..\n", _spaces);
//        fprintf(_tclFP, "#####   WriteShape\n");

    }

}


void ExportNebula::WriteAppearance(Node *node)
{

    if (!_tclFP) return;

//    int type = node->getProto()->getField(0)->getType();
//    mysnprintf(_buf, 4096, "%sNODE_Appearance  %i\n", _spaces, type);
//    swDebugf(_buf);

    _alphaMap = false;
    _textured = false;



    Node *texNode = ((SFNode*)node->getField(1))->getValue();
    if (texNode != NULL)
    {
        if (_texEnabled)
        {
            WriteTexture(texNode);
            _textured = true;
        }
    }
    Node *matNode = ((SFNode*)node->getField(0))->getValue();
    if (matNode != NULL)
    {
        WriteMaterial(matNode);
    }
}

const char* ExportNebula::matlibPath(void)
{

    static char path[128];

    if (_relativePaths)
    {
        path[0]='\0';
        for (int i=0; i<_currentDepth; i++)
        {
            strcat(path, "../");
        }
        strcat(path, "matlib/");
    }
    else
    {
        strcpy(path, "/usr/local/matlib/");
    }

    return path;
}


void ExportNebula::WriteTexture(Node *node)
{

//    String url(((MFString *) value)->getValue(0));

    const char *name = node->getName();
    if (node->needsDEF() && !_inSwitch)  // link node
    {
        fprintf(_tclFP, "%snew nlinknode texmap\n", _spaces);
        fprintf(_tclFP, "%stexmap.settarget %s%s\n", _spaces, matlibPath(), name);
    }
    else                                // real node
    {
        FieldValue  *value = node->getField(0);
        const char *url_path = (((MFString *) value)->getValue(0));

        char *file_name = strrchr(url_path, '/');
        if (file_name == NULL) file_name=(char*)url_path;
        else file_name++;

        _alphaMap = false;
        if (((NodeImageTexture*)node)->getNumComponents() == 4) _alphaMap = true;

        if (((NodeImageTexture*)node)->getWasScaleRequired())
        {
            mysnprintf(_buf, 4096, "Texture \"%s\" is not sized in powers of two.", file_name);
            swMessageBox(TheApp->mainWnd(), _buf, "Export Warning:", SW_MB_OK, SW_MB_WARNING);
        }

        fprintf(_tclFP, "%snew ntexarraynode %s\n", _spaces, name);
        fprintf(_tclFP, "%ssel %s\n", _spaces, name);
        SetIndent(_startDepth+1);
        fprintf(_tclFP, "%s.setgenmipmaps 0 true\n", _spaces, name);
        fprintf(_tclFP, "%s.settexture 0 maps/%s none\n", _spaces, file_name);
        fprintf(_tclFP, "%ssel ..\n", _spaces);
        SetIndent(_startDepth);
    }
}



void ExportNebula::WriteMaterial(Node *node)
{
    if (!_tclFP) return;

    const char *name = node->getName();

    if (node->needsDEF() && !_inSwitch)  // link node
    {
        fprintf(_tclFP, "%snew nlinknode shader\n", _spaces);
        fprintf(_tclFP, "%sshader.settarget %s%s\n", _spaces, matlibPath(), name);
    }
    else                                // real node
    {
        fprintf(_tclFP, "%snew nshadernode %s\n", _spaces, name);
        fprintf(_tclFP, "%ssel %s\n", _spaces, name);
        SetIndent(_startDepth+1);

        //  parse render flags
        bool cullDisable = false;
        bool cullReverse = false;
        bool sphereMap = false;
        int vertexColorMode = 0;
        strcpy(_buf, name);
        char *flag = strtok(_buf, "_");
        while( flag != NULL )
        {
            if (_stricmp(flag, "sm") == 0) sphereMap = true;
            if (_stricmp(flag, "cd") == 0) cullDisable = true;
            if (_stricmp(flag, "cr") == 0) cullReverse = true;
            if (_stricmp(flag, "vd") == 0) vertexColorMode = 1;
            if (_stricmp(flag, "ve") == 0) vertexColorMode = 2;
            if (_stricmp(flag, "va") == 0) vertexColorMode = 3;
            flag = strtok(NULL, "_");
        }

        fprintf(_tclFP, "%s.setnumstages 1\n",_spaces);
        fprintf(_tclFP, "%s.setlightenable true\n",_spaces);

        float ambient = ((SFFloat *)node->getField(0))->getValue();
        fprintf(_tclFP, "%s.setambient  %f %f %f 1\n", _spaces, ambient, ambient, ambient);


//        float difAlpha = 1.0;
        float difAlpha = 1.0f - ((SFFloat *)node->getField(5))->getValue();


        if (_textured)
        {
            fprintf(_tclFP, "%s.setdiffuse  1 1 1 %f\n", _spaces, difAlpha);

            fprintf(_tclFP, "%s.setcolorop 0 \"mul tex prev\"\n",_spaces);
            fprintf(_tclFP, "%s.begintunit 0\n",_spaces);
            SetIndent(_startDepth+2);
            fprintf(_tclFP, "%s.setaddress         wrap   wrap\n",_spaces);
            fprintf(_tclFP, "%s.setminmagfilter    linear_mipmap_nearest linear\n",_spaces);
            if (sphereMap)
            {
                fprintf(_tclFP, "%s.settexcoordsrc     spheremap\n",_spaces);
                fprintf(_tclFP, "%s.setenabletransform true\n",_spaces);
            }
            else
            {
                fprintf(_tclFP, "%s.settexcoordsrc     uv0\n",_spaces);
                fprintf(_tclFP, "%s.setenabletransform false\n",_spaces);
            }
            SetIndent(_startDepth+1);        
            fprintf(_tclFP, "%s.endtunit\n",_spaces);
        }
        else
        {
            const float *diffuse = ((SFVec3f *)node->getField(1))->getValue();
            fprintf(_tclFP, "%s.setdiffuse  %f %f %f %f\n", _spaces, diffuse[0], diffuse[1], diffuse[2], difAlpha);
        }


        if (_alphaMap || difAlpha < 1.0)
        {
            fprintf(_tclFP, "%s.setalphaenable true\n",_spaces);
            fprintf(_tclFP, "%s.setalphablend srcalpha invsrcalpha\n",_spaces);
//            fprintf(_tclFP, "%s.setalphablend invsrcalpha srcalpha\n",_spaces);
            if (_textured && _alphaTest)
            {
                fprintf(_tclFP, "%s.setalphatestenable true\n",_spaces);
                fprintf(_tclFP, "%s.setalpharef %f\n",_spaces, _alphaTestRef);
            }

        }

        if (cullDisable)
        {
            fprintf(_tclFP, "%s.setcullmode none\n",_spaces);
        }
        else if (cullReverse)
        {
            fprintf(_tclFP, "%s.setcullmode cw\n",_spaces);
        }

        switch (vertexColorMode)
        {
            case 1: fprintf(_tclFP, "%s.setcolormaterial diffuse\n",_spaces);  break;
            case 2: fprintf(_tclFP, "%s.setcolormaterial emissive\n",_spaces);  break;
            case 3: fprintf(_tclFP, "%s.setcolormaterial ambient\n",_spaces);  break;
        }

        fprintf(_tclFP, "%ssel ..\n",_spaces);
        SetIndent(_startDepth);
    }


//    mysnprintf(_buf, 4096, "%sambient  %f\n", _spaces, ambient);
//    swDebugf(_buf);

}


void ExportNebula::WriteGeometry(Node *node)
{

    if (!_tclFP) return;

    const char *name = node->getName();
    if (node->needsDEF() && !_inSwitch)  // link node  (don't think this will work with all VRML files...)
    {
        fprintf(_tclFP, "%snew nmeshnode mesh\n", _spaces);
        fprintf(_tclFP, "%smesh.setfilename meshes/%s.n3d\n", _spaces, name);
    }
    else if (_writeMeshes)                // real node
    {
        WriteMeshfile(node);
    }

}


//  safe array access
int ExportNebula::geti(const int *array, int size, int index)
{
    if (size == 0) return -1;
    if (index >= size) return -1;
    return array[index];
}


void ExportNebula::WriteMeshfile(Node *node)
{

    
    const char *name = node->getName();

    mysnprintf(_buf, 4096, "Processing geometry:  %s", name);
    _statusBar->SetText(_buf);

//    fprintf(_tclFP, "\n\n***  %s\n", _buf);  ///


    wfObject *mesh = new wfObject;


    //  verticies
    Node *vtxNode = ((SFNode*)node->getField(1))->getValue();
    if (vtxNode != NULL)
    {
        FieldValue *value=vtxNode->getField(0);

        int size = ((MFVec3f *)value)->getSize();
        int stride = ((MFVec3f *)value)->getStride();
        const float *vertex;
        for (int i=0; i<size/stride; i++)
        {
            vertex = ((MFVec3f*)value)->getValue(i);
            wfCoord v(vertex[0],vertex[1],vertex[2]);
            mesh->v_array.push_back(v);
//            fprintf(_tclFP, "v %f %f %f\n", vertex[0], vertex[1], vertex[2]);   ///
        }
    }


    //  texture UVs
    Node *tuvNode = ((SFNode*)node->getField(3))->getValue();
    if (tuvNode != NULL)
    {
        FieldValue *value=tuvNode->getField(0);

        int size = ((MFVec2f *)value)->getSize();
        int stride = ((MFVec2f *)value)->getStride();
        const float *uv;
        for (int i=0; i<size/stride; i++)
        {
            uv = ((MFVec2f*)value)->getValue(i);
            vector2 v(uv[0],uv[1]);
            mesh->vt_array.push_back(v);
//            fprintf(_tclFP, "vt %f %f\n", uv[0], uv[1]);  ///
        }
    }


    //  normals
    Node *nmlNode = ((SFNode*)node->getField(2))->getValue();
    if (nmlNode != NULL)
    {
        FieldValue *value=nmlNode->getField(0);

        int size = ((MFVec3f *)value)->getSize();
        int stride = ((MFVec3f *)value)->getStride();
        const float *normal;
        for (int i=0; i<size/stride; i++)
        {
            normal = ((MFVec3f*)value)->getValue(i);
            vector3 v(normal[0],normal[1],normal[2]);
            mesh->vn_array.push_back(v);
//            fprintf(_tclFP, "n %f %f %f\n", normal[0], normal[1], normal[2]);  ///
        }
    }


    //  vertex colors
    Node *vtxColorNode = ((SFNode*)node->getField(0))->getValue();
    if (vtxColorNode != NULL)
    {
        FieldValue *value=vtxColorNode->getField(0);

        int size = ((MFVec3f *)value)->getSize();
        int stride = ((MFVec3f *)value)->getStride();
        const float *vertex_color;
        for (int i=0; i<size/stride; i++)
        {
            vertex_color = ((MFVec3f*)value)->getValue(i);
            vector4 vc(vertex_color[0],vertex_color[1],vertex_color[2], 1.0f);
            mesh->c_array.push_back(vc);
//            fprintf(_tclFP, "vc %f %f %f\n", vertex_color[0], vertex_color[1], vertex_color[2]);   ///
        }
    }



//        int type = node->getProto()->getField(8)->getType();
//        mysnprintf(_buf, 4096, "****   Coord_Index   %i\n", type);
//        swDebugf(_buf);



    ////  indecies

    FieldValue *vvalue, *tvalue, *nvalue, *cvalue;
    int vsize, tsize, nsize, csize;

    vvalue = node->getField(8);
    vsize = ((MFInt32 *)vvalue)->getSize();

    tvalue = node->getField(13);
    tsize = ((MFInt32 *)tvalue)->getSize();

    nvalue = node->getField(10);
    nsize = ((MFInt32 *)nvalue)->getSize();

    cvalue = node->getField(5);
    csize = ((MFInt32 *)cvalue)->getSize();


//    mysnprintf(_buf, 4096, "*  vsize: %i   tsize: %i   nsize: %i\n", vsize, tsize, nsize);
//    swDebugf(_buf);


    const int *vi = ((MFInt32 *)vvalue)->getValues();
    const int *ti = ((MFInt32 *)tvalue)->getValues();
    const int *ni = ((MFInt32 *)nvalue)->getValues();
    const int *ci = ((MFInt32 *)cvalue)->getValues();

    for (int i=0; i<vsize;)
    {
        wfFace f;
        while (geti(vi, vsize, i)>=0)
        {
            wfPoint p(geti(vi,vsize,i), geti(ti,tsize,i), geti(ni,nsize,i), geti(ci,csize,i));
            f.points.push_back(p);
            i++;
        }
        mesh->f_array.push_back(f);
        i++;
    }


    char meshPath[4096];

    mysnprintf(meshPath, 4096,"%s%s.n3d", _meshDir, name);

//    mysnprintf(_buf, 4096, "Saving to file:  %s\n\n", meshPath);
//    swDebugf(_buf);



    // save file
    FILE *meshFP;

    meshFP = fopen(meshPath, "w");
    if (!meshFP)
    {
//        puts("Couldn't open .n3d file for output...");
        return;
    }

//////////   save original  (for debugging)
//  mesh->save(meshFP);
//    fclose(meshFP);
//    delete mesh;
//  return;
///////////////


    wfToolbox toolbox;
    
    wfObject *mesh2;

    mesh2 = new wfObject;
//    swDebugf("Cleaning...");
    toolbox.clean(*mesh,*mesh2,0.0000001f,0.0000001f,0.0000001f,0.0000001f);
    delete mesh;

    mesh = new wfObject;
//    swDebugf("Triangulating...");
    toolbox.triangulate(*mesh2,*mesh);
    delete mesh2;

    mesh2 = new wfObject;
//    swDebugf("Flattening...");
    toolbox.flatten(*mesh,*mesh2);
    delete mesh;

    mesh2->save(meshFP);
    fclose(meshFP);
    delete mesh2;


}        /////////////////     WriteMeshfile


#endif   ///  #ifdef HAVE_SAND

/*
 * ExportNebula.h
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

#include <stdio.h>
#include "swt.h"

class StatusBar;
class Scene;
class Node;

class ExportNebula
{

public:

     ExportNebula(void);
     bool ExportScene(char *path, Scene *scene, StatusBar *statusBar);
     void InitMenu(SMENU menu);
     void OnToggleWriteMeshes(SMENU menu);
     void OnToggleUseRelativePaths(SMENU menu);
     void OnToggleAlphaTest(SMENU menu);

private:
     void WalkScenegraph(Node *node, int depth);
     void WriteSetupScript(void);
     void SetIndent(int numSpaces);
     void WriteShape(Node *node);
     void WriteAppearance(Node *node);
     void WriteTexture(Node *node);
     void WriteMaterial(Node *node);
     void WriteGeometry(Node *node);
     void WriteMeshfile(Node *node);

     int geti(const int *array, int size, int index);

     const char* matlibPath(void);


     char _tclDir[4096];
     char _meshDir[4096];
     char _mapDir[4096];
     char _tclPath[4096];
     Scene *_scene;
     StatusBar *_statusBar;
     int _startDepth, _currentDepth;
     char _buf[4096];
     char _spaces[128];
     bool _inSwitch;

     FILE *_tclFP;

     bool _bbspExpand;
     bool _texEnabled;
     bool _textured;
     bool _alphaMap;
     bool _alphaTest;
     float _alphaTestRef;

     bool _writeMeshes;
     bool _writeMaterials;
     bool _debugInScript;

     bool _relativePaths;

};

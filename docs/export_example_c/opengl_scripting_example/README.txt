This file is free software; the Free Software Foundation
gives unlimited permission to copy, distribute and modify it.

This example shows how to use code from a VRML/X3D "Script" node in the 
C source export. 
In a VRML/X3D Script node, the "url" field has a MFString type, a array
of strings (a string may contain newline characters). 
A VRML/X3D browser tries to execute the first of the strings in the "url" 
field, if it fails it tries the next and so on. A string in the "url" field 
may contain a URL of a file (e.g. a java class file), but also inline code 
marked by a header (like "javascript:" or "ecmascript").
Therefore it is legal, to define additional headers (like "c:",
"c++:" or "java:") for matching inlined source code in a Script node.
To avoid name clashes of the header name, it can be redefined by a
white_dune commandline argument (see man page for more information).

To use this example on Linux/UNIX/MacOSX, just type

$ make

The resulting executable is named "./render"

To use this example on other systems, load the VRML/X3D file 
ScriptingExample.wrl into white_dune, export it as a C(mesh) file named 
CExport.c, compile and link it together with ../libCRWD.h, main.c and the 
glut library


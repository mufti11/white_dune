This file is free software; the Free Software Foundation
gives unlimited permission to copy, distribute and modify it.

When you copy this directory e.g. to $HOME, you have to remove "../../../bin/"
from the Makefile

To use this example on Linux/UNIX/MacOSX, modify the X3DV file, save it and type

$ make

The resulting executable is named "./render"

To use this example on other systems, modify the X3DV file, export   
it with white_dune as a C++(mesh) file, compile and link it together with
../libC++RWD.h, main.c and the glut library


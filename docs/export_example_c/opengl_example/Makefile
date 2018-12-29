# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

render: ../libCRWD.h main.c CExport.c
	cc -ansi -pedantic -g -o $@ -I. -I.. main.c -lglut -lGLU -lGL -lXmu -lX11 -ljpeg -lpng -lm

clean:
	rm -f CExport.c render core

CExport.c: Untitled.wrl ../../../bin/dune
	../../../bin/dune -c Untitled.wrl > CExport.c 


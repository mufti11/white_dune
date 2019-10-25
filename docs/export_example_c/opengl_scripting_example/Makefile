# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

render: main.c CExport.c
	cc -ansi -pedantic -g -o $@ -I. -I../../../include/ main.c -lglut -lGLU -lGL -lXmu -lX11 -ljpeg -lpng -lm

clean:
	rm -f CExport.c render core

CExport.c: ScriptingExample.wrl ../../../bin/dune
	echo '#include <stdio.h>' > CExport.c
	( dune -c ScriptingExample.wrl >> CExport.c ) || \
	../../../bin/dune -c ScriptingExample.wrl >> CExport.c


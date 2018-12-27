# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

render: ../libC++RWD.h main.cpp C++Export.cc
	g++ -g -o $@ -I. -I.. main.cpp -lpng -ljpeg -lglut -lGLU -lGL -lXmu -lX11

clean:
	rm -f C++Export.cc render core

C++Export.cc: ScriptingExample.wrl ../../../bin/dune
	../../../bin/dune -c++ ScriptingExample.wrl > C++Export.cc


# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

render: main.cpp C++Export.cc
	g++ -g -o $@ -I. -I../../../include main.cpp -lpng -ljpeg -lglut -lGLU -lGL -lXmu -lX11

clean:
	rm -f C++Export.cc render core

C++Export.cc: ScriptingExample.wrl
	dune -c++ ScriptingExample.wrl > C++Export.cc


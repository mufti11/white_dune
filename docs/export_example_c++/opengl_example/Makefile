# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

render: ../libC++RWD.h C++Export.cc main.cpp 
	g++ -g -o $@ -I. -I.. -Wno-narrowing main.cpp \
            -lglut -lGLU -lGL -lXmu -lX11 -ljpeg -lpng

# set special picture path for Fedora OpenGL example binary
render_with_picture_path: ../libC++RWD.h C++Export.cc main.cpp
	g++ -g $(CXXFLAGS) -Wno-unused-variable $(LDFLAGS) -o $@ \
            -DPICTURE_PATH=\"/usr/share/white_dune/opengl_example/\" \
            -I. -I.. -Wno-narrowing main.cpp -lglut -lGLU -lGL -lXmu -lX11 \
            -ljpeg -lpng


clean:
	rm -f C++Export.cc render render_with_picture_path core

C++Export.cc: robot.x3dv
	(../../../bin/dune -c++ robot.x3dv > C++Export.cc) || \
	dune -c++ robot.x3dv > C++Export.cc


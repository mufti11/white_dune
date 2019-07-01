# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

INCLUDE_DIR=../docs/export_example_c++/

warbird: $(INCLUDE_DIR)/libC++RWD.h C++Export.cc warbird.cpp 
	g++ -g -o $@ -I. -I$(INCLUDE_DIR) -Wno-narrowing warbird.cpp \
        -DPICTURE_PATH=\"`echo $$PWD/`\" -lglut -lGLU -lGL \
        -ljpeg -lpng

# set special picture path for Fedora OpenGL example binary
warbird_with_picture_path: $(INCLUDE_DIR)/libC++RWD.h C++Export.cc warbird.cpp
	g++ -g $(CXXFLAGS) -Wno-unused-variable -Wno-dangling-else \
            -Wno-sign-compare -Wno-narrowing -Wno-unused-but-set-variable \
            $(LDFLAGS) -o $@ -I. -I$(INCLUDE_DIR) warbird.cpp \
            -DPICTURE_PATH=\"/usr/share/white_dune/opengl_example/\" \
            -lglut -lGLU -lGL -lXmu -lX11 -ljpeg -lpng

clean:
	rm -f C++Export.cc warbird warbird_with_picture_path core

C++Export.cc: warbird.wrl
	(../bin/dune -c++ warbird.wrl > C++Export.cc) || \
	dune -c++ warbird.wrl > C++Export.cc


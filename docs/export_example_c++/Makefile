# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

all:
	(cd simple_example && make)
	(cd opengl_example && make)
	(cd opengl_scripting_example && make)
	(cd qt_opengl_example && qmake && make)

clean:
	(cd simple_example && make clean)
	(cd opengl_example && make clean)
	(cd opengl_scripting_example && make clean)
	rm -f qt_opengl_example/C++Export.cc
	rm -f qt_opengl_example/qt_opengl_example
	rm -rf build-qt_opengl_example-Desktop-Debug
	rm -f qt_opengl_example/.qmake.stash
	if test -x /usr/lib/qt5/bin/qmake ; then (cd qt_opengl_example && make clean); fi
	rm -f build-qt_opengl_example-Desktop-Debug/*


# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

prototyp: C++Export.cc ../libC++RWD.h callbacksToVrml.cxx GUIMain.cxx GUIElements.cxx OpenGLWindow.cxx OpenGLWindow.h
	g++ -g -o $@ GUIMain.cxx GUIElements.cxx OpenGLWindow.cxx -lfltk_gl -lglut -lGLU -lGL -lfltk -lX11 

GUIElements.cxx: GUIElements.fl
	fluid -c GUIElements.fl

C++Export.cc : Untitled.wrl
	dune -meshc++ Untitled.wrl > C++Export.cc

clean:
	rm -f C++Export.cc GUIElements.h GUIElements.cxx prototyp 

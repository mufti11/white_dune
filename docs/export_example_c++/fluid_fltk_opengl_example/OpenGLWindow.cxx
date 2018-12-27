// OpenGLWindow.cxx based on
//
// "$Id: CubeView.cxx 5519 2006-10-11 03:12:15Z mike $"
//
// CubeView class implementation for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include "stdio.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "C++Export.cc"
#include "../libC++RWD.h"
#include "callbacksToVrml.cxx"

#include "OpenGLWindow.h"
#include <math.h>

OpenGLWindow::OpenGLWindow(int x,int y,int w,int h,const char *l)
            : Fl_Gl_Window(x,y,w,h,l)
{
    Fl::add_timeout(1.0f/FRAMES, timerCallback, (void*)this);
}

int OpenGLWindow::handle(int event) {
    return Fl_Gl_Window::handle(event);
}

void OpenGLWindow::draw() {
    static bool initialized = false;
    if (!initialized) {
        CPPRWD::init();
        initialized = true;
    }
    CPPRWD::processEvents();
    CPPRWD::draw();
}

//
// End of "$Id: CubeView.cxx 5519 2006-10-11 03:12:15Z mike $".
//

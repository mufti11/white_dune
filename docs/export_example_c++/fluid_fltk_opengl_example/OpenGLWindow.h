// OpenGLWindow.h based on
//
// "$Id: CubeView.h 5519 2006-10-11 03:12:15Z mike $"
//
// CubeView class definitions for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#  include <FL/Fl_Gl_Window.H>
#  include <FL/gl.h>

#include <stdlib.h>

#define FRAMES 24

class OpenGLWindow : public Fl_Gl_Window {
public:
    // this value determines the scaling factor used to draw the cube.
    double size;

    static void timerCallback(void *userdata) {
         OpenGLWindow *gl = (OpenGLWindow*)userdata;
         gl->redraw();
         Fl::repeat_timeout(1.0f/FRAMES, timerCallback, userdata);
    }


    OpenGLWindow(int x,int y,int w,int h,const char *l=0);

    /*The widget class draw() override.
     *
     *The draw() function initialize Gl for another round o f drawing
     * then calls specialized functions for drawing each of the
     * entities displayed in the cube view.
     *
     */
    void draw();    
    int handle(int);
};

//
// End of "$Id: CubeView.h 5519 2006-10-11 03:12:15Z mike $".
//

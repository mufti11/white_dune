/* Copyright (c) Mark J. Kilgard, 1997. */

/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */


/* This is an example for the libCRWD library*/

/* libCRWD Library based on*/
/* libC++RWD Library for C++ Rendering of White_dune Data*/
/* Copyright (c) Stefan Wolf, 2010. */
/* Copyright (c) J. "MUFTI" Scheurich, 2010. */

#include <GL/glut.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
extern void reInitSensor(void *);
#include "CExport.c"
#include "white_dune/libCRWD.h"
#include <math.h>

float dist = 10.0f;
static int distInit = 0;

void display()
{
    if (distInitialised())
        if (!distInit) {
            distInit = -1;
            dist = getInitialDist();
        }
    CRWDdraw(1);
    glutSwapBuffers();
}

void animation()
{
    CRWDdraw(0);
    CRWDprocessEvents();
    display();
    usleep(10);
}

int left_button = 0;
int middle_button = 0;
int right_button = 0;

int moved_x = 0;
int moved_y = 0;

int dist_y = 0;

void onMouseClick(int button, int state, int x, int y)
{
    left_button = 0;
    middle_button = 0;
    right_button = 0;
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
    { 
        left_button = 1;
        moved_x = x;
        moved_y = y;        
        setMouseClick(x, y);
    }	
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
    { 
        setMouseRelease(x, y);
        dist = navigate(0, 0, dist);
    }	
    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
    { 
        middle_button = 1;
        moved_x = x;
        moved_y = y;
    }	
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
    { 
        right_button = 1;
        moved_x = x;
        moved_y = y;
    }	
}

void onMouseMove(int x, int y)
{
    if (left_button)
    {
        static int init = -1;
        if (init) {
            setMouseMove(x, x, y, y);
            init = 0;
        } else
            setMouseMove(x, moved_x, y, moved_y);
        if (!hasHit()) {
            navigate(moved_x - x, moved_y - y, dist);
        }
        moved_x = x;
        moved_y = y;
    } 
    else if (middle_button || right_button )
    {
        dist += (y - dist_y) / 5.0;
        dist_y = y;
        dist = navigate(0, 0, dist);
    } else
        setMousePosition(x, y);
}

void onMouseMovePassive(int x, int y)
{
    setMousePosition(x, y);
    moved_x = x;
    moved_y = y;
    dist_y = y;
}


void onSpecialKeyClick(int key, int x, int y)
{
}

void onReshape(int width, int height)
{
    setWidthHeight(width, height);
}

void onWalkOn(int time)
{
    CRWDwalkOn();    
    glutTimerFunc(40, onWalkOn, 0);
}

int main(int argc, char **argv)
{
    glutInitWindowSize(600, 600);
    setWidthHeight(600, 600);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("white_dune C viewer");
    CRWDinit();
    CRWDsetWalkOn();
    glutReshapeFunc(onReshape);
    glutMouseFunc(onMouseClick); 
    glutMotionFunc(onMouseMove);
    glutPassiveMotionFunc(onMouseMovePassive);
    glutSpecialFunc(onSpecialKeyClick);
    glutIdleFunc(animation);
    glutTimerFunc(40, onWalkOn, 0);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

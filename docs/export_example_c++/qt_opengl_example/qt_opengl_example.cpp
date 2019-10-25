/* Copyright (c) J. "MUFTI" Scheurich, 2019. */

/*
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "qt_opengl_example.h"
#include "ui_qt_opengl_example.h"
#include <QMouseEvent>
#include <QGLWidget>
#include <QtCore/QCoreApplication>
#include <QTimer>
#include <sys/time.h>
#include <GL/glu.h>
#include "white_dune/libC++RWD_namespace.h"

extern void startAnimation(void);

Qt_opengl_example::Qt_opengl_example(QGLWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Qt_opengl_example)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Qt_opengl_example::walkOn);
    timer->start(10);
    CPPRWD::setWalkOn();
}

Qt_opengl_example::~Qt_opengl_example()
{
    delete ui;
}

void Qt_opengl_example::on_actionexit_triggered()
{
    exit(1);
}

float dist = 10.0f;
static bool distInit = false;

int left_button = 0;
int middle_button = 0;
int right_button = 0;

int moved_x = 0;
int moved_y = 0;

int dist_y = 0;

void Qt_opengl_example::mousePressEvent(QMouseEvent *event)
{
    if (CPPRWD::distInitialised())
        if (!distInit) {
            distInit = true;
            dist = CPPRWD::getInitialDist();
        }

    left_button = 0;
    middle_button = 0;
    right_button = 0;
    
    m_lastPos = event->pos();
    int x = event->x(); 
    int y = event->y(); 
    if (event->buttons() & Qt::LeftButton) {
        left_button = 1;
        moved_x = x;
        moved_y = y;
        dist_y = y;
        CPPRWD::setMouseClick(x, y);
    } else if (event->buttons() & Qt::MiddleButton) {
        middle_button = 1;
        moved_x = x;
        moved_y = y;
        dist_y = y;
    } else if (event->buttons() & Qt::RightButton) {
        right_button = 1;
        moved_x = x;
        moved_y = y;
        dist_y = y;
    }
}

void Qt_opengl_example::mouseReleaseEvent(QMouseEvent *event)
{
    int x = event->x(); 
    int y = event->y(); 
    CPPRWD::setMouseRelease(x, y);

    dist = CPPRWD::navigate(0, 0, dist);
}

void Qt_opengl_example::mouseMoveEvent(QMouseEvent *event)
{
    if (CPPRWD::distInitialised())
        if (!distInit) {
            distInit = true;
            dist = CPPRWD::getInitialDist();
        }

    int x = event->x();
    int y = event->y();

    if (left_button)
    {
        static bool init = true;
        if (init) {
            CPPRWD::setMouseMove(x, x, y, y);
            init = false;
        } else
            CPPRWD::setMouseMove(x, moved_x, y, moved_y);
        if (!CPPRWD::hasHit()) {
            CPPRWD::navigate(moved_x - x, moved_y - y, dist);
        }
        moved_x = x;
        moved_y = y;
    } 
    else if (middle_button || right_button )
    {
        dist += (y - dist_y) / 5.0;
        dist_y = y;
        dist = CPPRWD::navigate(0, 0, dist);
    } else
        CPPRWD::setMousePosition(x, y);

    m_lastPos = event->pos();
}

void Qt_opengl_example::on_actionstart_animation_triggered()
{
    startAnimation();
}

void Qt_opengl_example::on_pushButton_clicked()
{
    startAnimation();
}

void Qt_opengl_example::walkOn(void)
{
    CPPRWD::walkOn();
}

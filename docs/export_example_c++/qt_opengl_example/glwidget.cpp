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

#include "glwidget.h"
#include <GL/glu.h>
class X3dNode;
extern void reInitSensor(void *node);
#include "C++Export.cc"
#include "../../../include/libC++RWD.h"
#include "callbacksToVrml.cpp"
#include <QOpenGLFunctions>
#include <QTimer>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    m_context = NULL;

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GLWidget::animate);
    timer->start(10);
}

void GLWidget::initializeGL()
{
    glClearColor(1, 0, 0, 1);
    CPPRWD::init();
}

void GLWidget::paintGL()
{
    CPPRWD::draw(true);
    swapBuffers();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWidget::animate()
{
    CPPRWD::draw(false);
    CPPRWD::processEvents();
    paintGL();
}


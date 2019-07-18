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

#ifndef QT_OPENGL_EXAMPLE_H
#define QT_OPENGL_EXAMPLE_H

#include <QMainWindow>
#include <QGLWidget>

namespace Ui {
class Qt_opengl_example;
}

class Qt_opengl_example : public QMainWindow {
    Q_OBJECT

public:
    explicit Qt_opengl_example(QGLWidget *parent = nullptr);
    ~Qt_opengl_example();

private slots:
    void on_actionexit_triggered();

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void on_actionstart_animation_triggered();

    void on_pushButton_clicked();

    void walkOn(void);

private:
    Ui::Qt_opengl_example *ui;
    QPoint m_lastPos;
};

#endif // QT_OPENGL_EXAMPLE_H

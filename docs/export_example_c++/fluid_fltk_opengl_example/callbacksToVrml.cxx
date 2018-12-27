/* callbacks for fluid/fltk OpenGL rendering example */

/* Copyright (c) J. "MUFTI" Scheurich, 2012. */

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

static double getCurrentTime()
{
#ifdef WIN32
    struct _timeb t;
    _ftime(&t);
    return (double) t.time + t.millitm / 1000.0;
#else
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double) t.tv_sec + (double) t.tv_usec / 1000000.0;
#endif
}

void startJump() {
    struct timeval now;
    gettimeofday(&now, NULL);
    double time = (double) now.tv_sec + (double) now.tv_usec / 1000000.0;

    scenegraph.TimeSensor1.startTime = time;
}

void startTalk() {
    struct timeval now;
    gettimeofday(&now, NULL);
    double time = (double) now.tv_sec + (double) now.tv_usec / 1000000.0;

    scenegraph.TimeSensor2.startTime = time;
}

void slider_changed(float f)
{
    scenegraph.Material1.diffuseColor[0] = f;
    scenegraph.Material1.diffuseColor[1] = 1 - f;
    scenegraph.Material1.diffuseColor[2] = 0;
}

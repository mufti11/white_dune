/*
 * NodeCattExportRec.cpp
 *
 * Copyright (C) 2007 J. "MUFTI" Scheurich
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file "COPYING" for details); if 
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave, 
 * Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include "stdafx.h"

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
# include <fcntl.h>
#endif

#include "NodeCattExportRec.h"
#include "Scene.h"
#include "Proto.h"
#include "RenderState.h"
#include "Util.h"
#include "swt.h"
#include "NodeNavigationInfo.h"

enum {
    RECEIVER_LOCATION
};

ProtoCattExportRec::ProtoCattExportRec(Scene *scene)
  : Proto(scene, "CattExportRec")
{
    id.set(
         addField(SFINT32, "id", new SFInt32(1)));

    receiverLocation.set(
         addExposedField(SFVEC3F, "receiverLocation", new SFVec3f(0, 0, 0)));

    furtherParameters.set(
         addField(SFSTRING, "furtherParameters", new SFString("")));
    
    addURLs(URL_EXPORT_CONTAINER);
}

Node *
ProtoCattExportRec::create(Scene *scene)
{ 
    return new NodeCattExportRec(scene, this); 
}

NodeCattExportRec::NodeCattExportRec(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

int
NodeCattExportRec::writeProto(int f)
{
    return ((Node *)this)->writeProto(f, "", "exportContainers"
#ifdef HAVE_EXPORT_CONTAINER_PROTO_URL
                                      , HAVE_EXPORT_CONTAINER_PROTO_URL
#endif
                                     );
}

void
NodeCattExportRec::drawHandles()
{
    glPushMatrix();
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    Vec3f loc(receiverLocation()->getValue());

    glPushName(RECEIVER_LOCATION);
    glLoadName(RECEIVER_LOCATION);

    RenderState state;

    Util::myGlColor3f(1.0f, 1.0f, 1.0f);
    state.startDrawHandles();
    state.drawHandle(loc);
    state.endDrawHandles();
    glPopName();
    glPopAttrib();

    glPopMatrix();
}

Vec3f
NodeCattExportRec::getHandle(int handle, int *constraint, int *field)
{
    if (handle == RECEIVER_LOCATION) {
        *field = receiverLocation_Field();
        return receiverLocation()->getValue();
    } 
    *field = getProto()->metadata_Field();
    return Vec3f(0.0f, 0.0f, 0.0f);
}

void
NodeCattExportRec::setHandle(int handle, const Vec3f &v)
{
    if (handle == RECEIVER_LOCATION)
        m_scene->setField(this, receiverLocation_Field(), new SFVec3f(v));
}

void
NodeCattExportRec::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    update();
}

void    
NodeCattExportRec::preDraw()
{
}

void
NodeCattExportRec::draw(int pass)
{
    if (pass == RENDER_PASS_TRANSPARENT)
        return;

    const float *loc =  receiverLocation()->getValue();

    float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glPushMatrix();
    glTranslatef(loc[0], loc[1], loc[2]);

    float scale = TheApp->GetHandleScale() *
                   m_scene->getNavigationInfoNode()->speed()->getValue(); 
    glScalef(scale, scale, scale);

    glPushName(RECEIVER_LOCATION);

    glEnable(GL_CULL_FACE);

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);

    GLUquadricObj *obj = gluNewQuadric();

    glLoadName(RECEIVER_LOCATION);
    float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);

    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -0.1);    
    gluQuadricOrientation(obj, GLU_INSIDE);
    gluDisk(obj, 0.0, 0.07, 10, 1);
    gluQuadricOrientation(obj, GLU_OUTSIDE);
    gluCylinder(obj, 0.07, 0.07, 0.2, 10, 10);
    glTranslatef(0.0f, 0.0f, 0.2);        
    gluDisk(obj, 0.0, 0.07, 10, 1);
    glPopMatrix();

    gluDeleteQuadric(obj);
    glPopName();

    glDisable(GL_CULL_FACE);

    glPopMatrix();

}

int
NodeCattExportRec::writeCattGeo(int filedes, int indent)
{ 
    if (!TheApp->GetCattExportSrcRec()) {
         const char *message = "Warning: CattExportRec not written, see Options -> output settings...\n";
         TheApp->PrintMessageWindows(message);
         swDebugf("%s", message);
         return 0;
    }
    const char *path = TheApp->GetCattExportPath();
    int len = strlen(path)+256;
    char *filename = new char[len];
    mysnprintf(filename, len - 1, "%sRec.loc", path);
    int openflags = O_WRONLY | O_CREAT;
    if (m_scene->getCattRecIsWritten())
        openflags = O_RDWR | O_APPEND;
    int f = open(filename, openflags, 00666);
    if (f == -1)
        return -1;
    delete [] filename;
    if (m_scene->getCattRecIsWritten())
        lseek(f, 0, SEEK_END); // O_APPEND seams not to work under M$Windows...

    if (!m_scene->getCattRecIsWritten()) {
        RET_ONERROR( mywritestr(f, "RECEIVERS") )
        RET_ONERROR( mywritestr(f, swGetLinefeedString()) )
    }

    RET_ONERROR( mywritef(f, "%d ", id()->getValue()) )
    const float *loc =  receiverLocation()->getValue();
    glPushMatrix();
    Matrix matrix;
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) matrix);
    glPopMatrix();
    Vec3f v = matrix * Vec3f(loc);
    RET_ONERROR( mywritef(f, "%g %g %g", v.x, v.z, v.y) )

    const char* rest = furtherParameters()->getValue();
    RET_ONERROR( mywritef(f, " %s", rest) )

    RET_ONERROR( mywritestr(f, swGetLinefeedString()) )

    m_scene->setCattRecIsWritten();
    return swTruncateClose(f);
}


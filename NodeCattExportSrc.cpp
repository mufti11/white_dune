/*
 * NodeCattExportSrc.cpp
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
#include "swt.h"

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
# include <fcntl.h>
#endif

#include "NodeCattExportSrc.h"
#include "Scene.h"
#include "Proto.h"
#include "RenderState.h"
#include "Util.h"
#include "NodeNavigationInfo.h"

enum {
    SOURCE_CENTER_LOCATION,
    COORDINATE_TO_AIM_THE_SOURCE
};


ProtoCattExportSrc::ProtoCattExportSrc(Scene *scene)
  : Proto(scene, "CattExportSrc")
{
    id.set(
         addField(SFSTRING, "id", new SFString("A0")));

    sourceCenterLocation.set(
         addExposedField(SFVEC3F, "sourceCenterLocation", new SFVec3f(0, 0, 0)));

    sourceDirectivityName.set(
         addField(SFSTRING, "sourceDirectivityName", new SFString("OMNI")));

    coordinateToAimTheSource.set(
         addExposedField(SFVEC3F, "coordinateToAimTheSource", 
                         new SFVec3f(0, 0, 0)));

    roll.set(
         addExposedField(SFFLOAT, "roll", new SFFloat(0)));

    furtherParameters.set(
         addField(MFSTRING, "furtherParameters", new MFString("Lp1m_a = Lp_nominal ; Lp_voice_normal")));

    addURLs(URL_EXPORT_CONTAINER);
}

Node *
ProtoCattExportSrc::create(Scene *scene)
{ 
    return new NodeCattExportSrc(scene, this); 
}

NodeCattExportSrc::NodeCattExportSrc(Scene *scene, Proto *def)
  : Node(scene, def)
{
}

int
NodeCattExportSrc::writeProto(int f)
{
    return ((Node *)this)->writeProto(f, "", "exportContainers"
#ifdef HAVE_EXPORT_CONTAINER_PROTO_URL
                                            , HAVE_EXPORT_CONTAINER_PROTO_URL
#endif
                                            );
}

void
NodeCattExportSrc::setField(int index, FieldValue *value, int cf)
{
    Node::setField(index, value, cf);
    update();
}

void
NodeCattExportSrc::drawHandles()
{
    glPushMatrix();
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    Vec3f aimLocation(coordinateToAimTheSource()->getValue());

    float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    const float *loc = sourceCenterLocation()->getValue();
    const float *aim = coordinateToAimTheSource()->getValue();

    glLineWidth(1.0f);
    glEnable(GL_LINE_SMOOTH);
    Util::myGlColor3fv(white);
    glBegin(GL_LINES);
    glVertex3f(loc[0], loc[1], loc[2]);
    glVertex3f(aim[0], aim[1], aim[2]);
    glEnd();

    glPushName(COORDINATE_TO_AIM_THE_SOURCE);
    glLoadName(COORDINATE_TO_AIM_THE_SOURCE);

    RenderState state;

    Util::myGlColor3f(1.0f, 1.0f, 1.0f);
    state.startDrawHandles();
    state.drawHandle(aimLocation);
    state.endDrawHandles();
    glPopName();

    glPopAttrib();

    glPopMatrix();

    glPushMatrix();
    const float *srcLocation = sourceCenterLocation()->getValue();
    glTranslatef(srcLocation[0], srcLocation[1], srcLocation[2]);

    glPushName(SOURCE_CENTER_LOCATION);
    glLoadName(SOURCE_CENTER_LOCATION);

    GLUquadricObj *obj = gluNewQuadric();

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);
    gluSphere(obj, 0.05f, 8, 4);
    glPopName();
    gluDeleteQuadric(obj);

    glPopMatrix();
}

Vec3f
NodeCattExportSrc::getHandle(int handle, int *constraint, int *field)
{
    if (handle == COORDINATE_TO_AIM_THE_SOURCE) {
        *field = coordinateToAimTheSource_Field();
        return coordinateToAimTheSource()->getValue();
    } 
    *field = sourceCenterLocation_Field();
    return sourceCenterLocation()->getValue();
}

void
NodeCattExportSrc::setHandle(int handle, const Vec3f &v)
{
    if (handle == COORDINATE_TO_AIM_THE_SOURCE)
        m_scene->setField(this, coordinateToAimTheSource_Field(), new SFVec3f(v));
    else
        m_scene->setField(this, sourceCenterLocation_Field(), new SFVec3f(v));
}

void    
NodeCattExportSrc::preDraw()
{
}

void
NodeCattExportSrc::draw(int pass)
{
    if (pass == RENDER_PASS_TRANSPARENT)
        return;
    float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

    glPushMatrix();

    glPushAttrib(GL_LIGHTING | GL_LINE_SMOOTH);
    glDisable(GL_LIGHTING);
    glDisable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    const float *loc = sourceCenterLocation()->getValue();
    const float *aim = coordinateToAimTheSource()->getValue();
    Util::myGlColor3fv(green);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex3f(loc[0], loc[1], loc[2]);
    glVertex3f(aim[0], aim[1], aim[2]);
    glEnd();

    glDisable(GL_CULL_FACE);
    glPopAttrib();

    glPopMatrix();


    float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glPushMatrix();
    glTranslatef(loc[0], loc[1], loc[2]);

    glScalef(1.0f, 1.0f, 1.0f);

    glPushName(SOURCE_CENTER_LOCATION);

    glEnable(GL_CULL_FACE);

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);

    GLUquadricObj *obj = gluNewQuadric();

    glLoadName(SOURCE_CENTER_LOCATION);
    float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    Util::myGlMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);

    glPushMatrix();

    float scale = TheApp->GetHandleScale() *
                   m_scene->getNavigationInfoNode()->speed()->getValue(); 
    glScalef(scale, scale, scale);

    gluSphere(obj, 0.07, 10, 10);

    glPopMatrix();

    gluDeleteQuadric(obj);
    glPopName();

    glDisable(GL_CULL_FACE);

    glPopMatrix();

    glPushMatrix();

    glPushAttrib(GL_LIGHTING | GL_LINE_SMOOTH);
    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glTranslatef(aim[0], aim[1], aim[2]);

    glScalef(scale, scale, scale);

    Util::myGlColor3fv(white);

    glBegin(GL_LINES);
    glVertex3f(-0.1f, 0.0f, 0.0f);
    glVertex3f( 0.1f, 0.0f, 0.0f);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0.0f, -0.1f, 0.0f);
    glVertex3f(0.0f,  0.1f, 0.0f);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -0.1f);
    glVertex3f(0.0f, 0.0f,  0.1f);
    glEnd();

    glDisable(GL_CULL_FACE);
    glPopAttrib();

    glPopMatrix();
}

int
NodeCattExportSrc::writeCattGeo(int filedes, int indent)
{ 
    if (!TheApp->GetCattExportSrcRec()) {
         const char *message = "Warning: CattExportSrc not written, see Options -> output settings...\n";
         TheApp->PrintMessageWindows(message);
         swDebugf("%s", message);
         return 0;
    }
    const char *path = TheApp->GetCattExportPath();
    int len = strlen(path)+256;
    char *filename = new char[len];
    mysnprintf(filename, len - 1, "%sSrc.loc", path);
    int openflags = O_WRONLY;
    if (m_scene->getCattSrcIsWritten())
        openflags |= O_APPEND;
    else 
        openflags |= O_CREAT;
    int f = open(filename, openflags, 00666);
    if (f == -1)
        return -1;
    delete [] filename;
   
    if (!m_scene->getCattSrcIsWritten())
        RET_ONERROR( mywritestr(f, "SOURCEDEFS") )
    RET_ONERROR( mywritestr(f, swGetLinefeedString()) )

    const char* s_id = id()->getValue();
    RET_ONERROR( mywritef(f, "%s ", s_id) )
    const float *location = sourceCenterLocation()->getValue();
    glPushMatrix();
    Matrix matrix;
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) matrix);
    glPopMatrix();
    Vec3f loc = matrix * Vec3f(location);
    const char* name = sourceDirectivityName()->getValue();
    RET_ONERROR( mywritef(f, " %g %g %g %s", loc.x, loc.z, loc.y, name) )
    const float *coordinateAim = coordinateToAimTheSource()->getValue();
    glPushMatrix();
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) matrix);
    glPopMatrix();
    Vec3f aim = matrix * Vec3f(coordinateAim);
    RET_ONERROR( mywritef(f, " %g %g %g", aim.x, aim.z, aim.y) )
    
    if (roll()->getValue() != 0.0f)
        RET_ONERROR( mywritef(f, " %f", roll()->getValue()) )
    RET_ONERROR( mywritestr(f, swGetLinefeedString()) )
    for (int i = 0; i < furtherParameters()->getSize(); i++) {
        const char* parameter = furtherParameters()->getValue(i);
        RET_ONERROR( mywritestr(f, parameter) )
        RET_ONERROR( mywritestr(f, swGetLinefeedString()) )
    }

    m_scene->setCattSrcIsWritten();
    return swTruncateClose(f);
}


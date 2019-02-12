/*
 * NodeLOD.cpp
 *
 * Copyright (C) 1999 Stephen F. White
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

#include "NodeLOD.h"
#include "Proto.h"
#include "Scene.h"
#include "FieldValue.h"
#include "MFNode.h"
#include "SFVec3f.h"
#include "MFFloat.h"


ProtoLOD::ProtoLOD(Scene *scene)
  : WonderlandExportProto(scene, "LOD")
{
    addElements();
}

ProtoLOD::ProtoLOD(Scene *scene, const char* name)
  : WonderlandExportProto(scene, name)
{
    addElements();
}

void
ProtoLOD::addElements(void)
{
    level.set(
          addExposedField(MFNODE, "level", new MFNode(), CHILD_NODE, 
                          "children"));
    bboxCenter.set(
          addField(SFVEC3F, "bboxCenter", new SFVec3f(0.0f, 0.0f, 0.0f)));
    setFieldFlags(bboxCenter, FF_X3D_ONLY);
    bboxSize.set(
          addField(SFVEC3F, "bboxSize", new SFVec3f(-1.0f, -1.0f, -1.0f), 
                   new SFFloat(-1.0f)));
    setFieldFlags(bboxSize, FF_X3D_ONLY);
    center.set(
          addField(SFVEC3F, "center", new SFVec3f(0.0f, 0.0f, 0.0f)));
    range.set(
          addField(MFFLOAT, "range", new MFFloat(), new SFFloat(0.0f)));
    forceTransitions.set(
          addField(SFBOOL, "forceTransitions", new SFBool(false)));

    render.set(
        addExposedField(SFBOOL, "render", new SFBool(true)));
    setFieldFlags(render, FF_X3DOM_ONLY);

    addEventIn(MFNODE, "addChildren", FF_X3D_ONLY);
    addEventIn(MFNODE, "removeChildren", FF_X3D_ONLY);
}

Node *
ProtoLOD::create(Scene *scene)
{ 
    return new NodeLOD(scene, this); 
}

NodeLOD::NodeLOD(Scene *scene, Proto *def)
  : Node(scene, def)
{
    m_nodeToDrawIndex = -1;
}

void
NodeLOD::draw(int pass)
{
    NodeList *childList = level()->getValues();

    if ((childList->size() > 0) && (m_nodeToDrawIndex >= 0)) {

        glPushName(level_Field());  // field offset

        childList->get(m_nodeToDrawIndex)->bind();

        glPushName(0);

        glLoadName(m_nodeToDrawIndex);
        childList->get(m_nodeToDrawIndex)->draw(pass);

        glPopName();

        childList->get(m_nodeToDrawIndex)->unbind();

        glPopName();
    }
}

void
NodeLOD::preDraw()
{
    accountNodeToDrawIndex();

    NodeList *childList = level()->getValues();

    if ((childList->size() > 0) && (m_nodeToDrawIndex >= 0))
        childList->get(m_nodeToDrawIndex)->preDraw();
}

void
NodeLOD::accountNodeToDrawIndex()
{
    NodeList *childList = level()->getValues();
    MyArray<int> indexList;
    NodeList tmpChildList;

    for (int i = 0; i < childList->size(); i++)
        if (childList->get(i)->getType() != VRML_COMMENT) {
            tmpChildList.append(childList->get(i));
            indexList.append(i);
        }

    int tmpNodeToDrawIndex = -1;

    if (tmpChildList.size() > 0) {
        tmpNodeToDrawIndex = tmpChildList.size() - 1;
        Matrix matrix;
        const float *fcenter = center()->getValue();

        glPushMatrix();
        glTranslatef(fcenter[0], fcenter[1], fcenter[2]);
        glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
        glPopMatrix();
        
        Vec3f v(matrix[12], matrix[13], matrix[14]);
        MFFloat *mfrange = range();
        if (tmpNodeToDrawIndex > mfrange->getSize())
            tmpNodeToDrawIndex = mfrange->getSize();
        // vrml97/part1/nodesRef.html#LOD
        // | An empty range field ... a hint to the browser that it may
        // | choose a level automatically to maintain a constant display rate.
        // we choose the first level 8-(
        if (mfrange->getSize() > 0)
            for (int i = tmpNodeToDrawIndex; i >= 0; i--)
                if ((i < tmpChildList.size()) && (i < mfrange->getSize())) 
                    if (v.length() < mfrange->getValues()[i])
                        tmpNodeToDrawIndex = i;

        m_nodeToDrawIndex = -1;
        if (tmpNodeToDrawIndex >= 0)
            m_nodeToDrawIndex = indexList.get(tmpNodeToDrawIndex);
    }
}

void
NodeLOD::flip(int index)
{
    if (m_scene->isX3d())
        bboxCenter()->flip(index);
    center()->flip(index);
    level()->flip(index);
}

void
NodeLOD::swap(int fromTo)
{
    if (m_scene->isX3d()) {
        bboxCenter()->swap(fromTo);
        bboxSize()->swap(fromTo);
    }
    center()->swap(fromTo);
    level()->swap(fromTo);
}

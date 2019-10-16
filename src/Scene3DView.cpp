/*
 * Scene3DView.cpp
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

#include "stdafx.h"

#include "swt.h"

#include "Scene3DView.h"
#include "Scene.h"
#include "Path.h"
#include "Node.h"
#include "RenderState.h"
#include "Matrix.h"
#include "SFRotation.h"
#include "SFVec3f.h"
#include "EulerAngles.h"
#include "InputDeviceTime.h"
#include "Util.h"
#include "VertexModifier.h"
#include "MyMesh.h"
#include "NodeViewpoint.h"
#include "NodeTransform.h"
#include "NodeNurbsSurface.h"
#include "NodeNurbsCurve.h"
#include "NodeNurbsGroup.h"
#include "NodeCoordinate.h"
#include "NodeViewport.h"
#include "NodeLayout.h"
#include "NodeLayoutLayer.h"
#include "NodeLayoutGroup.h"

#define PART_BOX 0.1

static int 
inputDeviceTimerCallback(void *data)
{
    return((Scene3DView *) data)->readInputDevice();
}

static int 
walkTimerCallback(void *data)
{
    return((Scene3DView *) data)->walk();
}

Scene3DView::Scene3DView(Scene *scene, SWND parent)
  : SceneView(scene, parent)
{
    m_dc = NULL;
    m_glc = NULL;
    m_trackMouse = false;
    m_cursorArrow = swLoadCursor(SW_CURSOR_ARROW);
    m_cursorZoom = swLoadCursor(SW_CURSOR_DBL_ARROW_VERT);
    m_cursorRotate = swLoadCursor(SW_CURSOR_CIRCLE);
    m_cursorRoll = swLoadCursor(SW_CURSOR_ROLL);
    m_cursorMove = swLoadCursor(SW_CURSOR_CROSS);
    m_cursorNone = swLoadCursor(SW_CURSOR_NONE);
    m_button1down = false;
    m_button2down = false;
    m_button3down = false;
    m_fieldEdit = false;
    m_inputDeviceTimer = NULL;
    m_walkTimer = NULL;
    m_mouseX = INT_MIN;
    m_mouseY = INT_MIN;
    m_hitHandle = false;
    m_rubberBandX = -1;
    m_rubberBandY = -1;
    m_offset.x = 0;
    m_offset.y = 0;
    m_offset.z = 0;
    m_walk.x = 0;
    m_walk.y = 0;
    m_walk.z = 0;
    m_walking = false;
    m_width = 0;
    m_height = 0;
    m_rubberBandX = 0;
    m_rubberBandY = 0;
}

Scene3DView::~Scene3DView()
{
    if (TheApp->getNumberInputDevices() != 0)
        if (m_inputDeviceTimer != NULL)
            swKillTimer(m_inputDeviceTimer);
    if (m_glc != NULL)
        swDestroyGLContext(m_glc);
    m_glc = NULL;
    swDestroyDC(m_dc);
    // early/double destroy of window is protected by destroySelf()
    swDestroyWindow(m_wnd);

    swDestroyCursor(m_cursorArrow);
    swDestroyCursor(m_cursorZoom);
    swDestroyCursor(m_cursorRotate);
    swDestroyCursor(m_cursorRoll);
    swDestroyCursor(m_cursorMove);
    swDestroyCursor(m_cursorNone);
}

void Scene3DView::OnUpdate(SceneView *sender, int type, Hint *hint)
{
    // inputdevice reading routine need information if about field editing
    switch (type) {
      CASE_UPDATE(UPDATE_START_FIELD_EDIT)
        m_fieldEdit = true;
        break;
      CASE_UPDATE(UPDATE_STOP_FIELD_EDIT) 
        if (m_fieldEdit) {
            m_fieldEdit = false;
            // restart inputdevice reading timer 
            if (TheApp->getNumberInputDevices() != 0)
                m_inputDeviceTimer = swSetTimer(m_wnd, INPUTDEVICE_TIME,
                                               inputDeviceTimerCallback, this);
        }
        swInvalidateWindow(m_wnd);
        break;
      CASE_UPDATE(UPDATE_FIELD)
        {
        NodeUpdate *nodeUpdate = (NodeUpdate *) hint;
        if (nodeUpdate->node != NULL)
            nodeUpdate->node->update();
        }
        swInvalidateWindow(m_wnd);
        break;
      CASE_UPDATE(UPDATE_MODE)
      CASE_UPDATE(UPDATE_REDRAW)
      CASE_UPDATE(UPDATE_REDRAW_3D)
        swInvalidateWindow(m_wnd);
        break;
      CASE_UPDATE(UPDATE_SELECTION)
        swInvalidateWindow(m_wnd);
        break;
      CASE_UPDATE(UPDATE_TIME)
        OnDraw(0, 0, 0, 0, true);
    }
    if ((m_inputDeviceTimer == 0) && (TheApp->getNumberInputDevices() != 0))
        m_inputDeviceTimer = swSetTimer(m_wnd, INPUTDEVICE_TIME, 
                                       inputDeviceTimerCallback, this);
}

void 
Scene3DView::drawViewPort(Node *root, int count, bool update)
{
    int width, height;
    if (!m_dc) m_dc = swCreateDC(m_wnd);
    if (!m_glc) m_glc = swCreateGLContext(m_dc);
    swMakeCurrent(m_dc, m_glc);
    swGetSize(m_wnd, &width, &height);
    float scaleX = 1;
    float scaleY = 1;
    if (root == NULL)
        glViewport(0, 0, width, height);
    else if (root->getType() == X3D_VIEWPORT) {
        NodeViewport *viewport = (NodeViewport *)root;
        const float *data = viewport->clipBoundary()->getValues();
        glViewport(width * data[0], height * data[2], 
                   width * data[1], height * data[3]);
    } else if (root->getType() == X3D_LAYOUT_LAYER) {
        NodeLayoutLayer *layer = (NodeLayoutLayer *)root;
        NodeViewport *viewport = (NodeViewport *)layer->viewport()->getValue();
        if (viewport) { 
            const float *data = viewport->clipBoundary()->getValues();
            int dataSize = viewport->clipBoundary()->getSize();
            bool sizeChangeAbleX = (dataSize < 1);
            bool sizeChangeAbleY = (dataSize < 3);
            float datacp[4];
            datacp[0] = (dataSize > 0) ? data[0] : 0; 
            datacp[1] = (dataSize > 1) ? data[1] : 1; 
            datacp[2] = (dataSize > 2) ? data[2] : 0; 
            datacp[3] = (dataSize > 3) ? data[3] : 1;
            NodeLayout *layout = (NodeLayout *)layer->layout()->getValue();
            if (layout)
                layout->modifyViewportData(datacp, 
                                           sizeChangeAbleX, sizeChangeAbleY, 
                                           width, height, &scaleX, &scaleY);
            glViewport(width * datacp[0], height * datacp[2], 
                       width * datacp[1], height * datacp[3]);
        }           
    } else if (root->getType() == X3D_LAYOUT_GROUP) {
        NodeLayoutGroup *group = (NodeLayoutGroup *)root;
        NodeViewport *viewport = (NodeViewport *)group->viewport()->getValue();
        if (viewport) { 
            const float *data = viewport->clipBoundary()->getValues();
            int dataSize = viewport->clipBoundary()->getSize();
            bool sizeChangeAbleX = (dataSize < 1);
            bool sizeChangeAbleY = (dataSize < 3);
            float datacp[4];
            datacp[0] = (dataSize > 0) ? data[0] : 0; 
            datacp[1] = (dataSize > 1) ? data[1] : 1; 
            datacp[2] = (dataSize > 2) ? data[2] : 0; 
            datacp[3] = (dataSize > 3) ? data[3] : 1;
            NodeLayout *layout = (NodeLayout *)group->layout()->getValue();
            float scaleX;
            float scaleY;
            if (layout)
                layout->modifyViewportData(datacp, 
                                           sizeChangeAbleX, sizeChangeAbleY, 
                                           width, height, &scaleX, &scaleY);
            glViewport(width * data[0], height * data[2], 
                       width * data[1], height * data[3]);
        }
    }
    if (root == NULL)
        root = m_scene->getRoot();
    TheApp->setEyeMode(EM_NONE);
    if (TheApp->isAnaglyphStereo()) {
        glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
        glClearAccum(0.0,0.0,0.0,0.0); 
        glClearColor(0.0,0.0,0.0,0.0);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }
    if ((TheApp->canQuadBufferStereo()) || (TheApp->useStereo())) {
        if (TheApp->useStereo())
            TheApp->setEyeMode(EM_LEFT);
        if (TheApp->canQuadBufferStereo())
             glDrawBuffer(GL_BACK_LEFT);
        if (TheApp->isAnaglyphStereo()) {
            glClear(GL_ACCUM_BUFFER_BIT);
            if (TheApp->getStereoType() == RED_GREEN_ANAGLYPH_STEREO)
                glColorMask(GL_FALSE, GL_TRUE, GL_FALSE, GL_TRUE);
            else if (TheApp->getStereoType() == RED_BLUE_ANAGLYPH_STEREO)
                glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
            else if (TheApp->getStereoType() == RED_CYAN_ANAGLYPH_STEREO)
                glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
            else 
                glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
        }
        m_scene->drawScene(false, 0, 0, width, height, root, update, 
                           scaleX, scaleY);
        if (TheApp->useStereo())
            if ((m_mouseX != INT_MIN) && (m_mouseY != INT_MIN))
                m_scene->draw3dCursor(m_mouseX, m_mouseY);
        if (m_button3down)
            if ((m_mouseX != INT_MIN) && (m_mouseY != INT_MIN))
                if (m_scene->isVerticesOrNurbs())
                    m_scene->draw3dBoundingBox(m_mouseX, m_mouseY, 
                                               m_rubberBandX, m_rubberBandY);
        if (TheApp->isAnaglyphStereo())
            glAccum(GL_ACCUM, 1.0f);

        if (TheApp->useStereo())
            TheApp->setEyeMode(EM_RIGHT);

        if (TheApp->canQuadBufferStereo())
            glDrawBuffer(GL_BACK_RIGHT);
        if (TheApp->isAnaglyphStereo()) {
            glClear(GL_ACCUM_BUFFER_BIT);
            if (TheApp->getStereoType() == GREEN_RED_ANAGLYPH_STEREO)
                glColorMask(GL_FALSE, GL_TRUE, GL_FALSE, GL_TRUE);
            else if (TheApp->getStereoType() == BLUE_RED_ANAGLYPH_STEREO)
                glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
            else if (TheApp->getStereoType() == CYAN_RED_ANAGLYPH_STEREO)
                glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
            else
               glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);        
        } 
        m_scene->drawScene(false, 0, 0, width, height, root, update, 
                           scaleX, scaleY);
        if (TheApp->useStereo())
            if ((m_mouseX != INT_MIN) && (m_mouseY != INT_MIN)) {
                m_scene->draw3dCursor(m_mouseX, m_mouseY);
                if (m_button3down)
                    if (m_scene->isVerticesOrNurbs())
                        m_scene->draw3dBoundingBox(m_mouseX, m_mouseY, 
                                                   m_rubberBandX, m_rubberBandY);
            }
        if (TheApp->isAnaglyphStereo())
            glAccum(GL_ACCUM, 1.0f);
        }
    else
       {
       m_scene->drawScene(false, 0, 0, width, height, root, update,
                          scaleX, scaleY);
       if (m_button3down)
           if ((m_mouseX != INT_MIN) && (m_mouseY != INT_MIN))
               if (m_scene->isVerticesOrNurbs())
                   m_scene->draw3dBoundingBox(m_mouseX, m_mouseY, 
                                              m_rubberBandX, m_rubberBandY);
       }
    if (m_scene->getVertexModifier()) {
         m_scene->draw3dBoundingBox(0, 0, width * PART_BOX, height * PART_BOX);
         m_scene->draw3dBoundingBox(width * (1.0f - PART_BOX),
                                    height * (1.0f - PART_BOX), width, height);
         m_scene->draw3dBoundingBox(0, height * (1.0f - PART_BOX), 
                                    width * PART_BOX, height);
         m_scene->draw3dBoundingBox(width * (1.0f - PART_BOX), 0, 
                                    width, height * PART_BOX);
    }
}

void Scene3DView::OnDraw(int /* x */, int /* y */,
                         int /* width */, int /* height */, bool update) 
{
    MyArray<Node *> *viewports = m_scene->getViewPorts();
    drawViewPort(NULL, 0, update);
    if (viewports->size() > 0)
        for (int i = 0; i < (*viewports).size(); i++)
            drawViewPort(viewports->get(i), i);
    if (viewports->size() > 0) {
        int width, height;
        if (!m_dc) m_dc = swCreateDC(m_wnd);
        if (!m_glc) m_glc = swCreateGLContext(m_dc);
            swMakeCurrent(m_dc, m_glc);
        swGetSize(m_wnd, &width, &height);
        glViewport(0, 0, width, height);

        m_scene->resetPerspective();
    }
    swSwapBuffers(m_dc, m_glc);
#ifndef _WIN32
    swInvalidateWindow(m_wnd);
#endif
}

void Scene3DView::OnSize(int width, int height) 
{
    m_width = width;
    m_height = height;
    swInvalidateWindow(m_wnd);
}

void Scene3DView::OnKeyDown(int key, int x, int y, int modifiers) 
{
    switch (key) {
      case SW_UP:
        m_scene->moveCamera(0.0f, 0.0f, -1.0f);
        m_scene->applyCamera();
        swInvalidateWindow(m_wnd);
        break;
      case SW_DOWN:
        m_scene->moveCamera(0.0f, 0.0f, 1.0f);
        m_scene->applyCamera();
        swInvalidateWindow(m_wnd);
        break;
      case SW_KEY_LEFT:
        m_scene->turnCamera(0.0f, 1.0f, 0.0f, 0.10f);
        m_scene->applyCamera();
        swInvalidateWindow(m_wnd);
        break;
      case SW_KEY_RIGHT:
        m_scene->turnCamera(0.0f, 1.0f, 0.0f, -0.10f);
        m_scene->applyCamera();
        swInvalidateWindow(m_wnd);
        break;
      case SW_MOUSE4:
        m_scene->moveCamera(0.0f, 0.0f, 0.1f);
        m_scene->applyCamera();
        swInvalidateWindow(m_wnd);
        break;
      case SW_MOUSE5:
        m_scene->moveCamera(0.0f, 0.0f, -0.1f);
        m_scene->applyCamera();
        swInvalidateWindow(m_wnd);
        break;       
      default:
        SceneView::OnKeyDown(key, x, y, modifiers);
        break;
    }
    TheApp->interact();
}

void Scene3DView::OnKeyUp(int key, int x, int y, int modifiers) 
{
    m_trackMouse = false;
}

#define PICK_BUFFER_SIZE 65536
#define PICK_REGION_SIZE 0.0001

unsigned int
Scene3DView::getHit(int x, int y)
{
    unsigned int ret = 0;
    int width, height;

    swGetSize(m_wnd, &width, &height);

    float xwidth = PICK_REGION_SIZE; 
    float yheight = PICK_REGION_SIZE;
    GLuint pBuffer[PICK_BUFFER_SIZE];
    GLuint *pickBuffer = pBuffer;
    glSelectBuffer(PICK_BUFFER_SIZE, pickBuffer);
    glRenderMode(GL_SELECT);
    glInitNames();

    TheApp->setEyeMode(EM_NONE);
    m_scene->drawScene(true, x, y, xwidth, yheight);

    GLint hits = glRenderMode(GL_RENDER);
    Path *path = NULL;
    if (hits < 0) // overflow flag has been set, ignore
        hits = - hits;
    if (hits) {
        unsigned depth = 0;
        int hit = -1;
        for (int i = 0; i < hits; i++) {
            unsigned numNames = *pickBuffer++;
            unsigned minDepth = *pickBuffer++;
            unsigned maxDepth = *pickBuffer++;
            for (int i = 0; i < numNames; i++) {
               int buffer = *pickBuffer++;
               if (minDepth > depth) {
                   depth = minDepth; 
                   hit = buffer;
               }
            } 
        }    
        if (hit > -1)
            return depth; 
    }
    return ret;
}


void Scene3DView::OnLButtonDown(int x, int y, int modifiers) 
{
    Path *path;
    m_backedUp = false;
    int width, height;

    swGetSize(m_wnd, &width, &height);

    swMakeCurrent(m_dc, m_glc);
    swSetFocus(m_wnd);

    m_hitHandle = false;
    bool cursorChanged = false;

    if ((modifiers & SW_CONTROL)) {
        cursorChanged = true;
        if ((modifiers & SW_SHIFT) || (TheApp->GetMouseMode() == MOUSE_ROLL))
            swSetCursor(m_wnd, m_cursorMove);
        else if (TheApp->GetMouseMode() == MOUSE_ROLL)
            swSetCursor(m_wnd, m_cursorRoll);
        else {
            m_scene->setTurnPoint();
            if (TheApp->GetMouseMode() == MOUSE_WALK)
                startWalking(true);
            swSetCursor(m_wnd, m_cursorRotate);
        }
    } else if (modifiers & SW_ALT) {
        cursorChanged = true;
        if (m_button2down)
           swSetCursor(m_wnd, m_cursorZoom);
        else if (TheApp->GetMouseMode() == MOUSE_ROLL)
            swSetCursor(m_wnd, m_cursorRoll);
        else if (TheApp->GetMouseMode() == MOUSE_FOLLOW)
            swSetCursor(m_wnd, m_cursorMove);
        else {
            m_scene->setTurnPoint();
            swSetCursor(m_wnd, m_cursorRotate);
        }
    } else if (modifiers & SW_SHIFT) {
        cursorChanged = true;
        swSetCursor(m_wnd, m_cursorZoom);
    } else if (!m_scene->getMouseNavigationMode()) {
        path = m_scene->pick(x, height - y);
        int newHandle = -1;
        if (path) {
            if (path->getPathLen() == 0)
                m_scene->removeSelectedHandles();                
            else  {
                newHandle = m_scene->getLastSelectedHandle();
                if (m_scene->isNewSelectedHandle())
                    m_scene->removeSelectedHandles();
            }
            Node *node = m_scene->getSelection()->getNode();
            if ((node != path->getNode()) && !m_scene->getMouseNavigationMode())
                if ((!(node->getNodeClass() & VIEWPOINT_NODE)) || 
                    (path->getPathLen() != 0)) {
                    m_scene->setSelection(path);
                    m_scene->UpdateViews(this, UPDATE_SELECTION);
                    node = m_scene->getSelection()->getNode();
                }

            if (m_scene->getfirstSelectionRangeHandle() != -1)
                m_scene->addSelectedHandle(newHandle);
            else if ((newHandle != -1) && m_scene->getSelectedHandlesSize()<2)
                m_scene->setSelectedHandle(newHandle);

            swInvalidateWindow(m_wnd);

            if (newHandle == -1)
                m_hitHandle = false;
            else {
                int handle = newHandle;
    
                m_hitHandle = true;
    
                int constrain = m_scene->getConstrain();
                int field;
                Vec3f v = node->getHandle(handle, &constrain, &field);
    
                m_backedUp = false;
    
                // now calculate an offset in object space from the mouse 
                // to the actual handle position
    
                glPushMatrix();
                Vec3f o;
                float px, py, pz;
                m_scene->transform(m_scene->getSelection());
                node->transformForHandle(handle);
                m_scene->projectPoint(v.x, v.y, v.z, &px, &py, &pz);
                m_scene->unProjectPoint((float) x, (float) height - (float) y, 
                                        pz, &o.x, &o.y, &o.z);
                glPopMatrix();
    
                m_offset = v - o;
            }
        } else 
            m_scene->removeSelectedHandles();
    }
    if (!m_hitHandle) {
        if (!m_button2down) {
            m_scene->setTurnPoint();
            if (TheApp->GetMouseMode() == MOUSE_WALK)
                startWalking(true);
        }
        if (!cursorChanged) {
            if ((m_button2down) || (TheApp->GetMouseMode() == MOUSE_FOLLOW))
                swSetCursor(m_wnd, m_cursorMove);
            if (TheApp->GetMouseMode() == MOUSE_ROLL)
                swSetCursor(m_wnd, m_cursorRoll);
            else
                swSetCursor(m_wnd, m_cursorRotate);
        }
    }
    bool insideBoxes = false;
    if ((x < PART_BOX * width) && (y < PART_BOX * height))
        insideBoxes = true;
    if ((x > width - PART_BOX * width) && (y > height - PART_BOX * height))
        insideBoxes = true;
    if ((x > width - PART_BOX * width) && (y < PART_BOX * height))
        insideBoxes = true;
    if ((x < PART_BOX * width) && (y < height - PART_BOX * height))
        insideBoxes = true;
    if (m_scene->getVertexModifier() && !insideBoxes) {
        Node *node = m_scene->getVertexModifier()->getNode();
        int field = m_scene->getVertexModifier()->getField();
        MFVec3f *vertices = NULL;
        if (node != m_scene->getSelection()->getNode())
            node = m_scene->getSelection()->getNode();
        if (node->getType() == VRML_COORDINATE) {
            NodeCoordinate *coord = (NodeCoordinate *)node;
            vertices = (MFVec3f *)coord->point()->copy();
            field = coord->point_Field();
        } else if (node->getType() == VRML_NURBS_SURFACE) {
            NodeNurbsSurface *nurbs = (NodeNurbsSurface *)node;
            vertices = (MFVec3f *)nurbs->getControlPoints()->copy();
            field = nurbs->controlPoint_Field();
        }
        if (vertices) {
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
            Matrix transformMatrix;
            transformMatrix = Matrix::identity();
            Node *trans = m_scene->searchTransform()->getNode();
            trans->getMatrix(transformMatrix);
            glPopMatrix();

            Quaternion viewQuat = m_scene->getCamera()->getOrientation();
            Vec3d viewPos = m_scene->getCamera()->getPosition();

            Vec3f hit;

            unsigned int depth = getHit(x, y);
            glPushMatrix();
            glLoadIdentity();
            m_scene->unProjectPoint((float) x, (float) height - (float) y, 
                                    (float) depth / UINT_MAX, 
                                    &hit.x, &hit.y, &hit.z);
            glPopMatrix();

            Vec3f view(0, 0, viewPos.length());
            view = view * viewQuat;
            hit.y = -hit.y; // ???
            Vec3f compareVec = (hit * viewQuat + view);
            compareVec = transformMatrix.invert() * compareVec;
            float eps = TheApp->GetHandleEpsilon();
            float amount = m_scene->getVertexModifier()->getAmount();
            float radius = m_scene->getVertexModifier()->getRadius();
            int mode = m_scene->getVertexModifier()->getMode();
            bool plus = m_scene->getVertexModifier()->getPlus();
            Vec3f direction;
            bool hasDirection = !(m_scene->getVertexModifier()->getNormal());
            if (hasDirection) {
                direction.x = m_scene->getVertexModifier()->getX();
                direction.y = m_scene->getVertexModifier()->getY();
                direction.z = m_scene->getVertexModifier()->getZ();
            }

            if (depth != 0) {
                for (int i = 0; i < vertices->getSFSize(); i++) {
                    Vec3f vec = vertices->getVec(i);
                    Vec3f oldVec = vec;
                    if ((vec - compareVec).length() < radius) {
                        Vec3f normal = vec;
                        if (hasDirection)
                            normal = direction;
                        normal.normalize();
                        if (mode == VERTEX_MODIFIER_MODE_JUMP)
                            if (plus)
                                vec = vec + normal * amount;
                             else
                                vec = vec - normal * amount;
                        else if (mode == VERTEX_MODIFIER_MODE_PEAK) {
                            float mult = (vec - compareVec).length() / radius;
                            float add = amount * (1 - mult);
                            if (plus)
                                vec = vec + normal * add;
                             else
                                vec = vec - normal * add;
                        }
                        else if (mode == VERTEX_MODIFIER_MODE_QUAD) {
                            float mult = (vec - compareVec).length() / radius;
                            float add = amount * 
                                        (1 - mult * mult * mult * mult);
                            if (plus)
                                vec = vec + normal * add;
                            else
                                vec = vec - normal * add;
                        }
                        vertices->setVec(i, vec);
                        if (m_scene->getXSymetricMode())
                            for (int j = 0; j < vertices->getSFSize(); j++) {
                                if (i == j)
                                    continue;
                                Vec3f symVec = vertices->getVec(j);
                                if ((fabs(symVec.x + oldVec.x) < eps) &&
                                    (fabs(symVec.y - oldVec.y) < eps) &&
                                    (fabs(symVec.z - oldVec.z) < eps)) {
                                    vec.x *= -1.0f;
                                    vertices->setVec(j, vec);
                                }
                            }
                    }
                }
                if (node->getType() == VRML_NURBS_SURFACE) {
                    m_scene->addNextCommand();
                    NodeNurbsSurface *nurbs = (NodeNurbsSurface *)node;
                    if ((m_scene->isX3d()) && nurbs->controlPointX3D()) {
                        NodeCoordinate *coord = (NodeCoordinate *)
                           nurbs->controlPointX3D()->getValue();
                        if (coord)
                            m_scene->backupField(coord, coord->point_Field());
                    } else
                        m_scene->backupField(nurbs, 
                                             nurbs->controlPoint_Field());
                    nurbs->setControlPoints(vertices);
                    m_scene->deleteLastNextCommand();
                } else {
                    m_scene->addNextCommand();
                    m_scene->backupField(node, field);
                    m_scene->setField(node, field, vertices);
                    m_scene->deleteLastNextCommand();
                }
                m_scene->UpdateViews(NULL, UPDATE_REDRAW_3D);
            }
        }
    }
    m_trackMouse = true;
    swSetCapture(m_wnd);
    m_trackX = x;
    m_trackY = y;
    m_button1down = true;
    swInvalidateWindow(m_wnd);
    TheApp->interact();
}

void Scene3DView::OnLButtonUp(int x, int y, int modifiers) 
{
    if (!m_button2down) {
        m_trackMouse = false;
        m_scene->setTurnPoint();
        if (TheApp->useStereo())
            swSetCursor(m_wnd, m_cursorNone);
        else 
            swSetCursor(m_wnd, m_cursorArrow);
        swReleaseCapture(m_wnd);
    }
    m_button1down = false;
    stopWalking();
    if (m_button2down) {
       swMakeCurrent(m_dc, m_glc);
       swSetFocus(m_wnd);

       if (modifiers & SW_ALT)
          swSetCursor(m_wnd, m_cursorMove);

       m_trackMouse = true;
       swSetCapture(m_wnd);
       m_trackX = x;
       m_trackY = y;
    }
    if (m_scene->getSelectedHandlesSize() > 0)
        m_scene->UpdateViews(this, UPDATE_TOOLBAR);
}

void Scene3DView::OnMButtonDown(int x, int y, int modifiers) 
{
    m_backedUp = false;
    int width, height;
    swGetSize(m_wnd, &width, &height);

    swMakeCurrent(m_dc, m_glc);
    swSetFocus(m_wnd);
    if (modifiers & SW_ALT) 
        if (m_button1down)
            swSetCursor(m_wnd, m_cursorZoom);
        else
            swSetCursor(m_wnd, m_cursorMove);
    else if (TheApp->GetMouseMode() == MOUSE_WALK) {
        startWalking(false);
        swSetCursor(m_wnd, m_cursorMove);
    } else {
        if (TheApp->GetMouseMode() == MOUSE_WALK)
            swSetCursor(m_wnd, m_cursorRotate);
        else
            swSetCursor(m_wnd, m_cursorZoom);
        if (!m_scene->getMouseNavigationMode()) {
            Path *path = m_scene->pick(x, height - y);
            m_scene->setSingleSelectedHandle();
            m_scene->UpdateViews(this, UPDATE_SELECTION);
            if (path) {
                if (path->getPathLen() == 0)
                    m_scene->removeSelectedHandles();
                Node *selection = m_scene->getSelection()->getNode();
                int handle = m_scene->getLastSelectedHandle();
                if (!selection->validHandle(handle)) {
                    m_scene->removeSelectedHandle(handle);
                }
                delete path;
                swInvalidateWindow(m_wnd);
            }
        }
    }

    m_trackMouse = true;
    swSetCapture(m_wnd);
    m_trackX = x;
    m_trackY = y;

    m_button2down = true;
    swInvalidateWindow(m_wnd);
    TheApp->interact();
}

void Scene3DView::OnMButtonUp(int x, int y, int modifiers) 
{
    if (!m_button1down) {
        m_trackMouse = false;
        if (TheApp->useStereo())
            swSetCursor(m_wnd, m_cursorNone);
        else 
            swSetCursor(m_wnd, m_cursorArrow);
        swReleaseCapture(m_wnd);
    }
    m_trackMouse = false;
    if (TheApp->useStereo())
        swSetCursor(m_wnd, m_cursorNone);
    else
        swSetCursor(m_wnd, m_cursorArrow);
    swReleaseCapture(m_wnd);
    m_button2down = false;
    stopWalking();
}


void Scene3DView::OnRButtonDown(int x, int y, int modifiers) 
{
    m_trackMouse = true;
    m_button3down = true;
    m_rubberBandX = x;
    m_rubberBandY = y;
    m_mouseX = x;
    m_mouseY = y;
}

void Scene3DView::OnRButtonUp(int x, int y, int modifiers) 
{
    m_trackMouse = false;
    m_button3down = false;
    if ((m_rubberBandX > -1) && (m_rubberBandY > -1)) {
        int width, height;
        swGetSize(m_wnd, &width, &height);

        float midX = (x + m_rubberBandX) / 2;
        float midY = (y + m_rubberBandY) / 2;
        float sizeX = fabs((float)(x - m_rubberBandX));
        float sizeY = fabs((float)(y - m_rubberBandY));

        m_rubberBandX = -1;
        m_rubberBandY = -1;

        Path *path = m_scene->pick(midX, height - midY, sizeX, sizeY);
        m_scene->setMultipleSelectedHandles();
        if (path) {
            Node *node = m_scene->getSelection()->getNode();
            if (node) {
                int startHandle = 0;
                bool removed = true;
                while (removed) {
                    removed = false;
                    for (int i = startHandle; 
                         i < m_scene->getSelectedHandlesSize(); i++) {
                        int handle = m_scene->getSelectedHandle(i);
                        if (!node->validHandle(handle)) {
                            m_scene->removeSelectedHandle(handle);
                            startHandle = i + 1;
                            removed = true;
                            break;
                        }
                    }
                }
            }
            delete path;
        }
        swInvalidateWindow(m_wnd);
    }
    if (m_scene->getSelectedHandlesSize() > 0)
        m_scene->UpdateViews(this, UPDATE_TOOLBAR);
}

void Scene3DView::OnMouseEnter()
{
    m_scene->setHasFocus();
    m_scene->use3dCursor(true);
    if (TheApp->useStereo())
        swSetCursor(m_wnd, m_cursorNone);
    swInvalidateWindow(m_wnd);
} 

void Scene3DView::OnMouseLeave()
{
    m_scene->deleteHasFocus();
    m_scene->use3dCursor(false);
// M$Windows generates MouseLeave events constantly (every frame ?)
#ifndef _WIN32
    if (TheApp->useStereo())
        swSetCursor(m_wnd, m_cursorArrow);
#endif
    swInvalidateWindow(m_wnd);
} 

void Scene3DView::OnMouseMove(int x, int y, int modifiers) 
{
#ifdef _WIN32
    if (!(m_trackMouse) || (m_hitHandle))
        if (TheApp->useStereo())
            swSetCursor(m_wnd, m_cursorNone);
#endif

    int width, height;

    swMakeCurrent(m_dc, m_glc);
    swGetSize(m_wnd, &width, &height);

    if (TheApp->useStereo() && m_scene->use3dCursor()) {
        m_mouseX = x;
        m_mouseY = y;
        swInvalidateWindow(m_wnd);
    }

    if (m_trackMouse) {
        m_mouseX = x;
        m_mouseY = y;

        TheApp->interact();
        float dx = (float) x - m_trackX;
        float dy = (float) y - m_trackY;
        if (modifiers & SW_CONTROL) {
            if ((modifiers & SW_SHIFT) || 
                (TheApp->GetMouseMode() == MOUSE_FOLLOW)) {
                swSetCursor(m_wnd, m_cursorMove);
                m_scene->moveCamera(-dx * 0.05f, dy * 0.05f, 0.0f);
                m_trackX = x;
                m_trackY = y;
                m_scene->applyCamera();
                swInvalidateWindow(m_wnd);
            } else if (TheApp->GetMouseMode() == MOUSE_ROLL) {
                swSetCursor(m_wnd, m_cursorRoll);
                m_scene->rollCamera(-DEG2RAD(dx * 0.5f));
                m_trackX = x;
                m_trackY = y;
                m_scene->applyCamera();
                swInvalidateWindow(m_wnd);
            } else {
                swSetCursor(m_wnd, m_cursorRotate);
                m_scene->orbitCamera(-DEG2RAD(dx * 0.5f), -DEG2RAD(dy * 0.5f));
                m_trackX = x;
                m_trackY = y;
                m_scene->applyCamera();
                swInvalidateWindow(m_wnd);
            }
        } else if (modifiers & SW_SHIFT) {
            swSetCursor(m_wnd, m_cursorZoom);
            m_scene->moveCamera(0.0f, 0.0f, dy * 0.25f);
            m_trackX = x;
            m_trackY = y;
            swInvalidateWindow(m_wnd);
        } else if (modifiers & SW_ALT) {
            /* SGI style ALT key navigation */
            if (m_button1down && m_button2down) {
                swSetCursor(m_wnd, m_cursorZoom);
                m_scene->moveCamera(0.0f, 0.0f, dy * 0.25f);
                m_trackX = x;
                m_trackY = y;
                swInvalidateWindow(m_wnd);
            } else if (m_button1down) {
                swSetCursor(m_wnd, m_cursorRotate);
                m_scene->orbitCamera(-DEG2RAD(dx * 0.5f), -DEG2RAD(dy * 0.5f));
                m_trackX = x;
                m_trackY = y;
                swInvalidateWindow(m_wnd);
            } else if (m_button2down) {
                swSetCursor(m_wnd, m_cursorMove);
                m_scene->moveCamera(-dx * 0.05f, dy * 0.05f, 0.0f);
                m_trackX = x;
                m_trackY = y;
                swInvalidateWindow(m_wnd);
            }
        } else if ((m_scene->getSelectedHandlesSize() != 0) && 
                   !m_button2down && !m_button3down && 
                   (!m_scene->getMouseNavigationMode())) {
            Node *node = m_scene->getSelection()->getNode();
            int constrain = m_scene->getConstrain();
            int field;
            int handle = m_scene->getLastSelectedHandle();
            Vec3f old = node->getHandle(handle, &constrain, &field);
            if (field == -1) {
                 m_scene->removeSelectedHandles();
                 return;
            }
            if (handle == -1) {
                 return;
            }
            Vec3f v = old;
            if (constrain == CONSTRAIN_NONE) {
                float px, py, pz;
                glPushMatrix();
                glLoadIdentity();
                m_scene->transform(m_scene->getSelection());
                node->transformForHandle(handle);
                m_scene->projectPoint(old.x, old.y, old.z, &px, &py, &pz);
                m_scene->unProjectPoint((float) x, (float) height - y, pz, 
                                       &v.x, &v.y, &v.z);
                glPopMatrix();
                v += m_offset;
            } else {
                Vec3f vec;
                float px, py, pz;
                glPushMatrix();
                glLoadIdentity();
                m_scene->transform(m_scene->getSelection());
                node->transformForHandle(handle);
                m_scene->projectPoint(old.x, old.y, old.z, &px, &py, &pz);
                m_scene->unProjectPoint((float) x, (float) height - y, pz, 
                                       &vec.x, &vec.y, &vec.z);
                glPopMatrix();

                float x1, y1, z1;
                float x2, y2, z2;
                Matrix mat;
                glPushMatrix();
                glLoadIdentity();
                m_scene->transform(m_scene->getSelection());
                glGetFloatv(GL_MODELVIEW_MATRIX, mat);
                node->transformForHandle(handle);
                m_scene->unProjectPoint((float) x, (float) height - y, 0.0f, 
                                       &x1, &y1, &z1);
                m_scene->unProjectPoint((float) x, (float) height - y, 1.0f, 
                                       &x2, &y2, &z2);

                glPopMatrix();

                x1 += m_offset.x;
                y1 += m_offset.y;
                z1 += m_offset.z;
                x2 += m_offset.x;
                y2 += m_offset.y;
                z2 += m_offset.z;

                switch (constrain) {
                  case CONSTRAIN_X:
                    v.x = vec.x + m_offset.x;
                    break;
                  case CONSTRAIN_Y:
                    v.y = vec.y + m_offset.y;
                    break;
                  case CONSTRAIN_Z:
                    v.z = vec.z + m_offset.z;
                    break;
                  case CONSTRAIN_XY:
                    v.x = constrainLine(y1 - v.y, z1 - v.z, x1, 
                                        y2 - v.y, z2 - v.z, x2);
                    v.y = constrainLine(x1 - v.x, z1 - v.z, y1, 
                                        x2 - v.x, z2 - v.z, y2);

                    break;
                  case CONSTRAIN_ZX:
                    v.x = constrainLine(y1 - v.y, z1 - v.z, x1, 
                                        y2 - v.y, z2 - v.z, x2);
                    v.z = constrainLine(x1 - v.x, y1 - v.y, z1, 
                                        x2 - v.x, y2 - v.y, z2);
                    break;
                  case CONSTRAIN_YZ:
                    v.y = constrainLine(x1 - v.x, z1 - v.z, y1, 
                                        x2 - v.x, z2 - v.z, y2);
                    v.z = constrainLine(x1 - v.x, y1 - v.y, z1, 
                                        x2 - v.x, y2 - v.y, z2);
                    break;
                  case CONSTRAIN_SPHERE:
                    v = constrainSphere(v, dx, dy, mat);
                    m_trackX = x;
                    m_trackY = y;
                    break;
                }
            }
            Vec3f diff = v - old;
            if (m_scene->getSelectedHandlesSize() > 1)
                node->startSetMultiHandles();
            if (node->getType() == VRML_COORDINATE)
                ((NodeCoordinate *)node)->setBeginHandles();

            MyArray<Vec3f> alreadyHandledVertices;
            for (int i = 0; i < m_scene->getSelectedHandlesSize(); i++) {
                int handle = m_scene->getSelectedHandle(i);
                if (v != old) {
                    Vec3f oldv = node->getHandle(handle, &constrain, &field);
                    if (!m_backedUp) {
                        m_scene->addNextCommand();
                        m_scene->backupFieldsStart();
                        if ((node->getType() == VRML_NURBS_SURFACE) &&
                            (field > -1)) {
                            NodeNurbsSurface *surf = (NodeNurbsSurface *) node;
                            surf->backupFieldsAppend(field);
                            if (m_scene->getXSymetricMode()) {
                                NodeNurbsGroup *nurbsGroup = 
                                      surf->findNurbsGroup();
                                if (nurbsGroup != NULL)
                                    nurbsGroup->backupFieldsAppend(node, field);
                            }
                        } else if ((node->getType() == VRML_NURBS_CURVE) &&
                                   (field > -1)) {
                            NodeNurbsCurve *curve = (NodeNurbsCurve *) node;
                            curve->backupFieldsAppend(field);
                        } else if (field > -1)
                            m_scene->backupFieldsAppend(node, field);
                        m_scene->backupFieldsDone();
                        m_backedUp = true;
                    }
                    if (m_scene->getXSymetricMode()) {
                        float eps = TheApp->GetHandleEpsilon();
                        bool alreadyHandled = false;
                        for (int i = 0; i < alreadyHandledVertices.size(); 
                             i++) {
                            Vec3f vec = alreadyHandledVertices[i];
                            if ((fabs(oldv.x - vec.x) < eps) &&
                                (fabs(oldv.y - vec.y) < eps) &&
                                (fabs(oldv.z - vec.z) < eps)) {
                                alreadyHandled = true;
                                break;
                            }
                        }                    
                        if (!alreadyHandled) {
                            node->setHandle(handle, oldv + diff);
                            Vec3f diffCorrectX = diff;
                            if (oldv.x < TheApp->GetHandleEpsilon())
                                diffCorrectX.x = 0;
                            alreadyHandledVertices.append(oldv + diffCorrectX);
                        }
                    } else 
                        node->setHandle(handle, oldv + diff);
                }
                if (m_scene->getSelectedHandlesSize() > 1)
                    node->startSetMultiHandles();
            }
            if (node->getType() == VRML_COORDINATE)
                ((NodeCoordinate *)node)->setEndHandles();
            if (m_scene->getSelectedHandlesSize() > 0)
                swInvalidateWindow(m_wnd);
        } else if ((m_button1down && m_button2down) ||
                   (m_button1down && (TheApp->GetMouseMode() == MOUSE_FOLLOW))) {
            swSetCursor(m_wnd, m_cursorMove);
            m_scene->moveCamera(-dx * 0.05f, dy * 0.05f, 0.0f);
            m_trackX = x;
            m_trackY = y;
            swInvalidateWindow(m_wnd);
        } else if (m_button1down && (TheApp->GetMouseMode() == MOUSE_ROLL)) {
            swSetCursor(m_wnd, m_cursorRoll);
            m_scene->rollCamera(-DEG2RAD(dx * 0.5f));
            m_trackX = x;
            m_trackY = y;
            swInvalidateWindow(m_wnd);
        } else if (m_button1down)
            {
            if (TheApp->GetMouseMode() == MOUSE_WALK) {
                if (m_width != 0)
                    m_walk.x = dx / m_width;
                m_walk.y = 0;
                if (m_height != 0)
                    m_walk.z = dy / m_height;
                swSetCursor(m_wnd, m_cursorRotate);
            } else if (TheApp->GetMouseMode() == MOUSE_ROLL) {
                swSetCursor(m_wnd, m_cursorRoll);
            } else if (TheApp->GetMouseMode() == MOUSE_FOLLOW) {
                swSetCursor(m_wnd, m_cursorMove);
            } else {
                swSetCursor(m_wnd, m_cursorRotate);
                m_scene->orbitCamera(-DEG2RAD(dx * 0.5f), -DEG2RAD(dy * 0.5f));
                m_scene->applyCamera();
                m_trackX = x;
                m_trackY = y;
            }
            swInvalidateWindow(m_wnd);
        } else if (m_button2down) {
            if (TheApp->GetMouseMode() == MOUSE_WALK) {
                if (m_width != 0)
                    m_walk.x = dx / m_width;
                if (m_height != 0)
                    m_walk.y = - dy / m_height;
                m_walk.z = 0;
                swSetCursor(m_wnd, m_cursorMove);
            } else {
                swSetCursor(m_wnd, m_cursorZoom);
                m_scene->moveCamera(0.0f, 0.0f, -dy * 0.25f);
                m_scene->applyCamera();
                m_trackX = x;
                m_trackY = y;
            }
            swInvalidateWindow(m_wnd);
        } else if (m_button3down) {
            Path *path = m_scene->pick(x, height - y);
            int newHandle = -1;
            if (path) {
                if (path->getPathLen() != 0)
                    newHandle = m_scene->getLastSelectedHandle();
                Node *node = m_scene->getSelection()->getNode();
                if (m_scene->getSelectedHandlesSize() > 1)
                    node->startSetMultiHandles();
                if ((newHandle != -1) && m_scene->getSelectedHandlesSize()<2)
                    m_scene->setSelectedHandle(newHandle);
                delete path;
            }
            swInvalidateWindow(m_wnd);
        }
    }
}

Quaternion &Scene3DView::getQuat(const Path* path)
   {
   static Quaternion ret; 
   // initialise with unit one quaternion
   ret = Quaternion(0.0, 0.0, 0.0, 1.0);
   const NodeList *nodes = path->getNodes();

   for (int i = nodes->size() - 2; i >= 0; i--) {
       if (nodes->get(i)->getType() == VRML_TRANSFORM) {
          NodeTransform *transform = (NodeTransform*)nodes->get(i);
          Quaternion q = transform->getQuat();
          q.normalize();
          ret=ret * q;
       }
   }
   return ret;
}

void
Scene3DView::startWalking(bool walkForward)
{
    m_walkForward = walkForward;
    startWalking();
}

void
Scene3DView::startWalking(void)
{
    m_walkTimer = swSetTimer(m_wnd, 20, walkTimerCallback, this);
    m_walking = true;
    m_scene->startWalking();
}
 
void
Scene3DView::stopWalking(void) {
    if (m_walking) {
        if (m_walkTimer != NULL)
            swKillTimer(m_walkTimer);
        m_walkTimer = NULL;
        m_walking = false;
    }
}

int
Scene3DView::walk()
{
    m_scene->walkCamera(m_walk, m_walkForward);
    startWalking();
    return 0;
}

void Scene3DView::Transform3D(const Path* path,InputDevice* inputDevice)
{
    glPushMatrix();
    glLoadIdentity();
    Quaternion viewrot=m_scene->getCamera()->getOrientation();
    viewrot.normalize();
    NodeTransform* transform=(NodeTransform*)path->getNode();
    TransformMode* tm=m_scene->getTransformMode();
    Quaternion oldrot=transform->getQuat();
    float norm_oldrot=oldrot.norm();
    oldrot.normalize();
    Quaternion scenerot=getQuat(path);
    Quaternion newrot;
    bool rotation_changed=false;
    bool translation_changed=false;
    bool scale_changed=false;
    bool center_changed=false;
    if (tm->hasRotation()) {
       rotation_changed=true; 
       if (tm->isLocal()) {
          Quaternion inputrot=inputDevice->get_localQuaternion(tm);
          newrot=inputrot*oldrot;
       } else {
          Quaternion inputrot=inputDevice->get_quaternion(tm);
          newrot=oldrot*
                 (scenerot*viewrot.conj()*inputrot*viewrot*scenerot.conj());
       }
       transform->setQuat(newrot*norm_oldrot);
       m_scene->setField(transform, transform->rotation_Field(), 
                        new SFRotation(newrot*norm_oldrot));
    }
    if (tm->hasTranslation()) {
       Vec3f v=inputDevice->get_vector(tm);
       if ((v[0]!=0) && (v[1]!=0) && (v[2]!=0))
          translation_changed=true;
       Vec3f xyz;
       if (!tm->isLocal()) {
          xyz=viewrot*scenerot.conj()*oldrot.conj()*v;
          v[0]= xyz.x;
          v[1]= xyz.y;
          v[2]= xyz.z;
       } else {
          xyz=(scenerot*v)*scenerot.conj();
          v[0]=-xyz.x;
          v[1]= xyz.y;
          v[2]=-xyz.z;
       }
       transform->setHandle(TRANSLATION,v);
    } else if (tm->tmode==TM_SCALE) {
       Vec3f oldscale=transform->getScale();
       Vec3f v=m_scene->constrainVec(inputDevice->get_vector(tm));
       if ((v[0]!=0) || (v[1]!=0) || (v[2]!=0))
          scale_changed=true;
       transform->setHandle(SCALE,v+oldscale);
    } else if (tm->tmode==TM_UNIFORM_SCALE) {
       Vec3f oldscale=transform->getScale();
       Vec3f v=inputDevice->get_vector(tm);
       float mult = 1.0f;
       if ((v[0]!=0) || (v[1]!=0) || (v[2]!=0)) {
          scale_changed=true;
          if ((v[0] !=0) && (oldscale[0]!=0))
              mult = (oldscale[0]+v[0])/oldscale[0];       
          else if ((v[1] !=0) && (oldscale[1]!=0))
              mult = (oldscale[1]+v[1])/oldscale[1];       
          else 
              mult = (oldscale[2]+v[2])/oldscale[2];       
       }
       transform->setHandle(SCALE,oldscale*mult);
    } else if (tm->tmode==TM_CENTER) {
       Vec3f v=m_scene->constrainVec(inputDevice->get_vector(tm));
       if ((v[0]!=0) && (v[1]!=0) && (v[2]!=0))
          center_changed=true;
       transform->setHandle(CENTER,(viewrot*oldrot.conj())*v);
    }
    // Backup of inputdevice data is maybe memory murder 8-(
    m_scene->addNextCommand();
    if (translation_changed)
       m_scene->backupField(path->getNode(), transform->rotation_Field());
    if (rotation_changed)
       m_scene->backupField(path->getNode(), transform->translation_Field());
    if (scale_changed)
       m_scene->backupField(path->getNode(), transform->scale_Field());
    if (center_changed)
       m_scene->backupField(path->getNode(), transform->center_Field());
    if (   (translation_changed) || (rotation_changed) 
        || (scale_changed) || (center_changed))
       swInvalidateWindow(m_wnd);
    glPopMatrix();
}

void Scene3DView::Handle3D(const Path* path,InputDevice* inputDevice, 
                           int handle)
{
    glPushMatrix();
    glLoadIdentity();
    int constrain = CONSTRAIN_NONE;
    int field;
    Node* node = path->getNode();
    Vec3f old = node->getHandle(handle, &constrain, &field);
    if (field==-1) {
        return;
    }
    Vec3f vNew = old;
    m_scene->transform(m_scene->getSelection());
    node->transformForHandle(handle);
    TransformMode* tm=m_scene->getTransformMode();
    Quaternion viewrot=m_scene->getCamera()->getOrientation();
    Quaternion oldrot(0,0,0,1);
    oldrot.normalize();
    Quaternion scenerot=getQuat(path);
    Quaternion newrot;
    if (tm->hasTranslation()) {
        Vec3f v=inputDevice->get_vector(tm);
        Vec3f xyz;
        if (!tm->isLocal()) {
            xyz=viewrot*scenerot.conj()*oldrot.conj()*v;
            v[0]= xyz.x;
            v[1]= xyz.y;
            v[2]= xyz.z;
        } else {
            xyz=(scenerot*v)*scenerot.conj();
            v[0]=-xyz.x;
            v[1]= xyz.y;
            v[2]=-xyz.z;
        }
        if (m_scene->getXonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            vNew.x+=v[0];
        if (m_scene->getYonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            vNew.y+=v[1];
        if (m_scene->getZonly() || (m_scene->getConstrain() == CONSTRAIN_NONE))
            vNew.z+=v[2];
    }
    if (vNew != old) {
        if (!m_backedUp) {
            m_scene->addNextCommand();
            m_scene->backupField(node, field);
            m_backedUp = true;
        }
        node->setHandle(handle, vNew);
    } 
    glPopMatrix();
}

void Scene3DView::Navigate3D(InputDevice* inputDevice)
{
    glPushMatrix();
    glLoadIdentity();
    Node *camera=m_scene->getCamera();
    Quaternion viewrot=camera->getOrientation();
    Quaternion newrot;
    TransformMode* tm=m_scene->getTransformMode();
    if (!inputDevice->getHeadNavigation()) 
       {
       TransformMode tmTracker(TM_TRANSLATE,TM_3D,TM_NEAR_FAR);
       tm=&tmTracker;
       }
    if (tm->hasRotation()) {
       Quaternion inputrot=inputDevice->get_quaternion(tm);
       inputrot.z=-inputrot.z;
       newrot=inputrot*viewrot; 
       camera->setOrientation(newrot);
    }
    if (tm->hasTranslation()) {
       Vec3f v = inputDevice->get_vector(tm);
       Vec3f xyz;
       if ((inputDevice->isTracker()) || (inputDevice->isWand())) {
           xyz=newrot.conj()*(viewrot*v);
       } else
           xyz=viewrot*v;
       v[0]=xyz.x;
       v[1]=xyz.y;
       v[2]=xyz.z;
       Vec3d vcamera=camera->getPosition();
       v[0]=v[0]+vcamera.x;
       v[1]=v[1]+vcamera.y;
       v[2]=v[2]+vcamera.z;
       camera->setPosition(Vec3d(v.x, v.y, v.z));
    }
    m_scene->applyCamera();
    swInvalidateWindow(m_wnd);
    glPopMatrix();
}

// read from a array of InputDevice devices and move/rotate selected thing

int 
Scene3DView::readInputDevice(void)
{
    if ((TheApp->getNumberInputDevices()>0) && (!m_fieldEdit)) {
      if (m_scene->getHasFocus()) {
        for (int i=0;i<TheApp->getNumberInputDevices();i++)
          if (TheApp->getInputDevice(i)->hasReadDelay())
            TheApp->getInputDevice(i)->prepareRead();
        for (int i=0;i<TheApp->getNumberInputDevices();i++) {
          InputDevice* inputDevice=TheApp->getInputDevice(i);
          if (inputDevice==NULL)
             fprintf(stderr,"internal error in Scene3DView::readInputDevice\n");
          if (inputDevice->readInputDevice() && !(inputDevice->allzero())) {
             TheApp->interact();
             const Path *path = m_scene->getSelection();
             Node *node = path->getNode();
             Path *transform = NULL;
             if (m_scene->getInputDeviceNavigationMode() &&
                  !inputDevice->isWand())
                Navigate3D(inputDevice);
             else if ((node->getType() == VRML_TRANSFORM) && 
                      !inputDevice->isHead())
                Transform3D(path, inputDevice);
             else if ((m_scene->getSelectedHandlesSize() != 0) && 
                      !inputDevice->isHead()) {
                if (node->getType() == VRML_COORDINATE)
                   ((NodeCoordinate *)node)->setBeginHandles();
                for (int j = 0; j < m_scene->getSelectedHandlesSize(); j++) {
                   int handle = m_scene->getSelectedHandle(j);
                   Handle3D(path,inputDevice, handle);
                if (node->getType() == VRML_COORDINATE)
                   ((NodeCoordinate *)node)->setEndHandles();
                }
             } else if ((node==m_scene->getRoot()) && !inputDevice->isWand())
                Navigate3D(inputDevice);
             else if ((node->getType()==VRML_VIEWPOINT) && 
                      !inputDevice->isWand())
                Navigate3D(inputDevice);
             else if (( (transform = m_scene->searchTransform()) != NULL)
                      &&
                      (!inputDevice->sendalways())) {
                m_scene->setSelection(transform);
                m_scene->UpdateViews(this, UPDATE_SELECTION);
                m_scene->removeSelectedHandles();
                Transform3D(m_scene->getSelection(),inputDevice);
                transform=NULL;
            } else if (inputDevice->isHead())
                Navigate3D(inputDevice);
            }
         }
      }
#ifndef _WIN32  
      if (m_inputDeviceTimer) {
          swKillTimer(m_inputDeviceTimer);
          m_inputDeviceTimer=NULL;
      }
#endif
      m_inputDeviceTimer = swSetTimer(m_wnd, INPUTDEVICE_TIME, 
                                      inputDeviceTimerCallback, this);

    }
    return(0);
}

float Scene3DView::constrainLine(float x1, float y1, float z1, 
                                 float x2, float y2, float z2) const
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    if (dx == 0.0f && dy == 0.0f) 
        return 0;
//        return z2;
    float alpha = (x1 * dx + y1 * dy) / (dx * dx + dy * dy);

    return (1.0f - alpha) * z1 + alpha * z2;
}

void Scene3DView::constrainPlane(float x1, float y1, float z1, 
                                 float x2, float y2, float z2, 
                                 float *x, float *y) const
{
    if(EQUALF(z1, z2)) {
        *x = *y = 0.0f;
        return;
    }

    float alpha = -z1 / (z2 - z1);
    
    *x = (1.0f - alpha) * x1 + alpha * x2;
    *y = (1.0f - alpha) * y1 + alpha * y2;
}

Vec3f Scene3DView::constrainSphere(const Vec3f &euler, float dx, float dy,
                                   const Matrix &mat)
{
    SFRotation sfr(euler, 0);
    Quaternion cam = m_scene->getCamera()->getOrientation();

    Quaternion ry(Vec3f(0.0f, 1.0f, 0.0f), DEG2RAD(dx));
    Quaternion rx(Vec3f(1.0f, 0.0f, 0.0f), DEG2RAD(dy));
    Quaternion r = sfr.getQuat() * cam.conj() * ry * rx * cam;
    return SFRotation(r).getEulerAngles(0);
}


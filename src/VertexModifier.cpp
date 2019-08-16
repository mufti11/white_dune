/*
 * VertexModifier.cpp
 *
 * Copyright (C) 2002 Christian Hanisch, 2019 J. "MUFTI" Scheurich
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

#include "VertexModifier.h"
#include "resource.h"
#include "NodeCoordinate.h"
#include "NodeNurbsSurface.h"

static void radiusScrollCB(void *data, int type, int value)
{
    ((VertexModifier *) data)->radiusScrollCallback(type,value);
}

static void amountScrollCB(void *data, int type, int value)
{
    ((VertexModifier *) data)->amountScrollCallback(type,value);
}

static void xScrollCB(void *data, int type, int value)
{
    ((VertexModifier *) data)->xScrollCallback(type,value);
}
   
static void yScrollCB(void *data, int type, int value)
{
    ((VertexModifier *) data)->yScrollCallback(type,value);
}
   
static void zScrollCB(void *data, int type, int value)
{
    ((VertexModifier *) data)->zScrollCallback(type,value);
}
   
static void radiusTextEditInputCB(void *data, void *command)
{
    ((VertexModifier *) data)->radiusTextCallback();
}
   
static void amountTextEditInputCB(void *data, void *command)
{
    ((VertexModifier *) data)->amountTextCallback();
}

static void xTextEditInputCB(void *data, void *command)
{
    ((VertexModifier *) data)->xTextCallback();
}
   
static void yTextEditInputCB(void *data, void *command)
{
    ((VertexModifier *) data)->yTextCallback();
}
   
static void zTextEditInputCB(void *data, void *command)
{
    ((VertexModifier *) data)->zTextCallback();
}
   
static void okeyButtonCB(void *data, void *id)
{
    ((VertexModifier *) data)->okeyButtonCallback(id);
}

static void normalCheckBoxCB(void *data, void *id)
{
    ((VertexModifier *) data)->normalCheckBoxCallback(id);
}


static void keyCB(void *data, int key, int state, int x, int y, int modifiers)
{
    ((VertexModifier *) data)->keyCallback(key, state, x, y, modifiers);
}

static void exposeCB(void *data, int x, int y, int width, int height)
{
    ((VertexModifier *) data)->exposeCallback(x, y, width, height);
}

static void mouseCB(void *data, int x, int y, int modifiers)
{
}

#define X_radiusScrollBar 30
#define Y_radiusScrollBar 150
#define X_amountScrollBar 30
#define Y_amountScrollBar 190
#define X_xScrollBar 50
#define Y_xScrollBar 250
#define X_radiusTEXTEDIT 
#define ScrollBarVisit 10

VertexModifier::VertexModifier(Scene *scene, SWND parent, 
                               FieldUpdate *fieldUpdate)
                              : SceneView(scene, parent)
{
    m_canvas = parent;
    m_dc = swCreateDC(GetWindow());
    m_fieldUpdate = fieldUpdate;

    swSetClientData(m_canvas, this);
    swSetExposeCallback(m_canvas, exposeCB);
    swSetKeyCallback(m_canvas, keyCB);
    swSetMouseCallback(m_canvas, mouseCB);

    int ScrollBarWidth = 200;
    int ScrollBarHeight = 15;
    int TextEditWidth = 45;
    int TextEditHeight = 15;
    int ButtonWidth = 80;

    char buffer[256];

    swLoadString(IDS_QUAD, buffer, 255);
    m_quadButton = swCreateRadioButton(buffer, 20, 20, 100, 20, m_canvas);

    swLoadString(IDS_JUMP, buffer, 255);
    m_jumpButton = swCreateRadioButton(buffer, 20, 50, 100, 20, m_canvas);

    swLoadString(IDS_PEAK, buffer, 255);
    m_peakButton = swCreateRadioButton(buffer, 20, 80, 100, 20, m_canvas);

    swSetCheck(m_quadButton, 1);

    m_plusCheckBox = swCreateCheckBox("+", 20, 110, 100, 20, m_canvas);
    swSetClientData(m_plusCheckBox, this);
    swSetCheck(m_plusCheckBox, 1);

    m_okeyButton = swCreateButton("OK", 20,  
                                  Y_xScrollBar + 80, 
                                  ButtonWidth, 20, m_canvas);
    swSetClientData(m_okeyButton,this);
    swSetCommandCallback(m_okeyButton, okeyButtonCB);

    m_radius = 0;
    
    Node *node = m_fieldUpdate->node;

    MFVec3f *points = NULL;
    if (node->getType() == VRML_COORDINATE) {
        NodeCoordinate *coord = (NodeCoordinate *)node;
        points = coord->point();
    } else if (node->getType() == VRML_NURBS_SURFACE) {
        NodeNurbsSurface *nurbs = (NodeNurbsSurface *)node;
        points = nurbs->getControlPoints();
    }

    if (points) 
        for (int i = 0; i < points->getSFSize(); i++) {
            float radius = points->getVec(i).length();
            if (radius > m_radius)
                m_radius = radius;
        }
    
    m_radius /= 5.0f;

    swCreateLabel("radius", 20, Y_radiusScrollBar - 20, ButtonWidth, 20, 
                  m_canvas);

    m_radiusScrollBar = swCreateScrollBar(SW_HORIZONTAL, 
                                          X_radiusScrollBar, 
                                          Y_radiusScrollBar, 
                                          ScrollBarWidth, ScrollBarHeight, 
                                          0, 100 + ScrollBarVisit, 
                                          0, ScrollBarVisit,
                                          m_canvas);
    swSetClientData(m_radiusScrollBar, this);
    swSetScrollBarCallback(m_radiusScrollBar, radiusScrollCB);
    swScrollBarSetValue(m_radiusScrollBar, (int) (m_radius * 100));

    m_radiusTextEdit = swCreateTextEdit(SW_SINGLE_LINE, 
                                        X_radiusScrollBar + ScrollBarWidth + 5, 
                                        Y_radiusScrollBar, 
                                        TextEditWidth, TextEditHeight, 
                                        m_canvas); 
    swTextEditSetSelection(m_radiusTextEdit, 1, 4);
    swSetClientData(m_radiusTextEdit,this);
    swSetCommandCallback(m_radiusTextEdit, radiusTextEditInputCB);          

    mysnprintf(buffer, 128, " %1.2f", m_radius);
    swSetText(m_radiusTextEdit, buffer);

    m_amount = m_radius / 2.0f;

    swLoadString(IDS_AMOUNT, buffer, 255);
    swCreateLabel(buffer, 20, Y_amountScrollBar - 20, ButtonWidth, 20, 
                  m_canvas);

    m_amountScrollBar = swCreateScrollBar(SW_HORIZONTAL, 
                                          X_amountScrollBar, 
                                          Y_amountScrollBar, 
                                          ScrollBarWidth, ScrollBarHeight, 
                                          0, 100 + ScrollBarVisit, 
                                          0, ScrollBarVisit,
                                          m_canvas);
    swSetClientData(m_amountScrollBar, this);
    swSetScrollBarCallback(m_amountScrollBar, amountScrollCB);
    swScrollBarSetValue(m_amountScrollBar, (int) (m_amount * 100));

    m_amountTextEdit = swCreateTextEdit(SW_SINGLE_LINE, 
                                        X_amountScrollBar + ScrollBarWidth + 5, 
                                        Y_amountScrollBar, 
                                        TextEditWidth, TextEditHeight, 
                                        m_canvas); 
    swTextEditSetSelection(m_amountTextEdit, 1, 4);
    swSetClientData(m_amountTextEdit,this);
    swSetCommandCallback(m_amountTextEdit, amountTextEditInputCB);          

    mysnprintf(buffer, 128, " %1.2f", m_amount);
    swSetText(m_amountTextEdit, buffer);

    m_normalCheckBox = swCreateCheckBox("normal", 20, 220, 100, 20, m_canvas);
    swSetClientData(m_normalCheckBox, this);
    swSetCheck(m_normalCheckBox, 1);
    swSetCommandCallback(m_normalCheckBox, normalCheckBoxCB);

    m_x = 0;
    m_y = -1;
    m_z = 0;

    m_xScrollBar = swCreateScrollBar(SW_HORIZONTAL, 
                                     X_xScrollBar, 
                                     Y_xScrollBar, 
                                     ScrollBarWidth, ScrollBarHeight, 
                                     0, 100 + ScrollBarVisit, 
                                     0, ScrollBarVisit,
                                     m_canvas);
    swSetClientData(m_xScrollBar, this);
    swSetScrollBarCallback(m_xScrollBar, xScrollCB);
    swScrollBarSetValue(m_xScrollBar, (int)((m_x / 2.0f + 0.5f) * 100));

    m_xTextEdit = swCreateTextEdit(SW_SINGLE_LINE, 
                                   X_xScrollBar + ScrollBarWidth + 5, 
                                   Y_xScrollBar, 
                                   TextEditWidth, TextEditHeight, 
                                   m_canvas); 
    swTextEditSetSelection(m_xTextEdit, 1, 4);
    swSetClientData(m_xTextEdit,this);
    swSetCommandCallback(m_xTextEdit, xTextEditInputCB);          

    mysnprintf(buffer, 128, " %1.2f", m_x);
    swSetText(m_xTextEdit, buffer);

    m_yScrollBar = swCreateScrollBar(SW_HORIZONTAL, 
                                     X_xScrollBar, 
                                     Y_xScrollBar + ScrollBarHeight + 5, 
                                     ScrollBarWidth, ScrollBarHeight, 
                                     0, 100 + ScrollBarVisit, 
                                     0, ScrollBarVisit,
                                     m_canvas);
    swSetClientData(m_yScrollBar, this);
    swSetScrollBarCallback(m_yScrollBar, yScrollCB);
    swScrollBarSetValue(m_yScrollBar, (int)((m_y / 2.0f + 0.5f) * 100));

    m_yTextEdit = swCreateTextEdit(SW_SINGLE_LINE, 
                                   X_xScrollBar + ScrollBarWidth + 5, 
                                   Y_xScrollBar + ScrollBarHeight + 5, 
                                   TextEditWidth, TextEditHeight, 
                                   m_canvas); 
    swTextEditSetSelection(m_yTextEdit, 1, 4);
    swSetClientData(m_yTextEdit,this);
    swSetCommandCallback(m_yTextEdit, yTextEditInputCB);          

    mysnprintf(buffer, 128, " %1.2f", m_y);
    swSetText(m_yTextEdit, buffer);

    m_zScrollBar = swCreateScrollBar(SW_HORIZONTAL, 
                                     X_xScrollBar, 
                                     Y_xScrollBar + 2 * ScrollBarHeight + 10, 
                                     ScrollBarWidth, ScrollBarHeight, 
                                     0, 100 + ScrollBarVisit, 
                                     0, ScrollBarVisit,
                                     m_canvas);
    swSetClientData(m_zScrollBar, this);
    swSetScrollBarCallback(m_zScrollBar, zScrollCB);
    swScrollBarSetValue(m_zScrollBar, (int)((m_z / 2.0f + 0.5f) * 100));

    m_zTextEdit = swCreateTextEdit(SW_SINGLE_LINE, 
                                   X_xScrollBar + ScrollBarWidth + 5, 
                                   Y_xScrollBar + 2 * ScrollBarHeight + 10, 
                                   TextEditWidth, TextEditHeight, 
                                   m_canvas); 
    swTextEditSetSelection(m_zTextEdit, 1, 4);
    swSetClientData(m_zTextEdit,this);
    swSetCommandCallback(m_zTextEdit, zTextEditInputCB);          

    mysnprintf(buffer, 128, " %1.2f", m_z);
    swSetText(m_zTextEdit, buffer);

    m_xLabel = swCreateLabel("x", 20, Y_xScrollBar, 20, 20, m_canvas);
    m_yLabel = swCreateLabel("y", 20, Y_xScrollBar + ScrollBarHeight + 5, 
                             20, 20, m_canvas);
    m_zLabel = swCreateLabel("z", 20, Y_xScrollBar+ 2 * ScrollBarHeight + 10, 
                             20, 20, m_canvas);

    swHideWindow(m_xScrollBar);
    swHideWindow(m_yScrollBar);
    swHideWindow(m_zScrollBar);

    swHideWindow(m_xTextEdit);
    swHideWindow(m_yTextEdit);
    swHideWindow(m_zTextEdit);

    swHideWindow(m_xLabel);
    swHideWindow(m_yLabel);
    swHideWindow(m_zLabel);
}

VertexModifier::~VertexModifier()
{
} 

void 
VertexModifier::exposeCallback(int x, int y, int width, int height)
{
    swSetFGColor(m_dc, swGetWindowColor(m_canvas, SW_COLOR_WINDOW_BG));
    swFillRect(m_dc, x, y, width, height);

    setRadius(m_radius);
    setAmount(m_amount);
}

void 
VertexModifier::DeleteView()
{
    swDeleteCallbacks(m_radiusScrollBar);
    swDestroyWindow(m_radiusScrollBar);
    swDeleteCallbacks(m_amountScrollBar);
    swDestroyWindow(m_amountScrollBar);

    swDeleteCallbacks(m_radiusTextEdit);
    swDestroyWindow(m_radiusTextEdit);
    swDeleteCallbacks(m_amountTextEdit);
    swDestroyWindow(m_amountTextEdit);

    swDeleteCallbacks(m_okeyButton);
    swDestroyWindow(m_okeyButton);
    swDeleteCallbacks(m_quadButton);
    swDestroyWindow(m_quadButton);
    swDeleteCallbacks(m_jumpButton);
    swDestroyWindow(m_jumpButton);
    swDeleteCallbacks(m_peakButton);
    swDestroyWindow(m_peakButton);
    swDeleteCallbacks(m_plusCheckBox);
    swDestroyWindow(m_plusCheckBox);

    swDeleteCallbacks(m_normalCheckBox);
    swDestroyWindow(m_normalCheckBox);

    swDeleteCallbacks(m_xScrollBar);
    swDestroyWindow(m_xScrollBar);
    swDeleteCallbacks(m_yScrollBar);
    swDestroyWindow(m_yScrollBar);
    swDeleteCallbacks(m_zScrollBar);
    swDestroyWindow(m_zScrollBar);

    swDeleteCallbacks(m_xTextEdit);
    swDestroyWindow(m_xTextEdit);
    swDeleteCallbacks(m_yTextEdit);
    swDestroyWindow(m_yTextEdit);
    swDeleteCallbacks(m_zTextEdit);
    swDestroyWindow(m_zTextEdit);

    swDestroyWindow(m_xLabel);
    swDestroyWindow(m_yLabel);
    swDestroyWindow(m_zLabel);

    swDestroyDC(m_dc);
    m_dc=NULL;

    swDeleteCallbacks(m_canvas);
}

void 
VertexModifier::okeyButtonCallback(void *id)
{
    m_scene->UpdateViews(this, UPDATE_REDRAW_3D);
    m_scene->UpdateViews(this, UPDATE_CLOSE_VERTEX_MODIFIER);
}

void 
VertexModifier::normalCheckBoxCallback(void *id)
{
    if (swGetCheck(m_normalCheckBox)) {
        swHideWindow(m_xScrollBar);
        swHideWindow(m_yScrollBar);
        swHideWindow(m_zScrollBar);

        swHideWindow(m_xTextEdit);
        swHideWindow(m_yTextEdit);
        swHideWindow(m_zTextEdit);

        swHideWindow(m_xLabel);
        swHideWindow(m_yLabel);
        swHideWindow(m_zLabel);
    } else {
        swShowWindow(m_xScrollBar);
        swShowWindow(m_yScrollBar);
        swShowWindow(m_zScrollBar);

        swShowWindow(m_xTextEdit);
        swShowWindow(m_yTextEdit);
        swShowWindow(m_zTextEdit);

        swShowWindow(m_xLabel);
        swShowWindow(m_yLabel);
        swShowWindow(m_zLabel);
    }
}

void 
VertexModifier::radiusScrollCallback(int type, int value)
{
    setRadius(value / 100.0f);
}

void 
VertexModifier::amountScrollCallback(int type, int value)
{
    setAmount(value / 100.0f);
}

void 
VertexModifier::xScrollCallback(int type, int value)
{
    m_x = (value / 100.0f - 0.5f) * 2.0f;
    setX(m_x);
}

void 
VertexModifier::yScrollCallback(int type, int value)
{
    m_y = (value / 100.0f - 0.5f) * 2.0f;
    setY(m_y);
}

void 
VertexModifier::zScrollCallback(int type, int value)
{
    m_z = (value / 100.0f - 0.5f) * 2.0f;
    setZ(m_z);
}

void 
VertexModifier::radiusTextCallback(void)
{
    char textInput[12];
          
    swGetText(m_radiusTextEdit, textInput, 11);
    setRadius(atof(textInput));
}

void 
VertexModifier::amountTextCallback(void)
{
    char textInput[12];
          
    swGetText(m_amountTextEdit, textInput, 11);
    setAmount(atof(textInput));
}

void 
VertexModifier::xTextCallback(void)
{
    char textInput[12];
          
    swGetText(m_xTextEdit, textInput, 11);
    setX(atof(textInput));
}

void 
VertexModifier::yTextCallback(void)
{
    char textInput[12];
          
    swGetText(m_yTextEdit, textInput, 11);
    setY(atof(textInput));
}

void 
VertexModifier::zTextCallback(void)
{
    char textInput[12];
          
    swGetText(m_zTextEdit, textInput, 11);
    setZ(atof(textInput));
}

void 
VertexModifier::mouseCallback(int x, int y, int modifiers)
{
}

void 
VertexModifier::keyCallback(int key, int state, int x, int y, int modifiers)
{
    if (key == SW_MOUSE1) {
    }
}
   
void 
VertexModifier::resizeCallback(int x, int y)
{
}


void 
VertexModifier::setRadius(float radius)
{
    char buffer[128];

    m_radius = radius;
    mysnprintf(buffer, 127, " %1.2f", m_radius);
    swSetText(m_radiusTextEdit, buffer);

    swScrollBarSetValue(m_radiusScrollBar, (int) (m_radius * 100));
}

void 
VertexModifier::setAmount(float amount)
{
    char buffer[128];

    m_amount = amount;
    mysnprintf(buffer, 127, " %1.2f", m_amount);
    swSetText(m_amountTextEdit, buffer);

    swScrollBarSetValue(m_amountScrollBar, (int) (m_amount * 100));
}

void 
VertexModifier::setX(float value)
{
    char buffer[128];

    m_x = value;
    mysnprintf(buffer, 127, " %1.2f", m_x);
    swSetText(m_xTextEdit, buffer);

    swScrollBarSetValue(m_xScrollBar, (int) ((m_x / 2.0f + 0.5f) * 100));
}

void 
VertexModifier::setY(float value)
{
    char buffer[128];

    m_y = value;
    mysnprintf(buffer, 127, " %1.2f", m_y);
    swSetText(m_yTextEdit, buffer);

    swScrollBarSetValue(m_yScrollBar, (int) ((m_y / 2.0f + 0.5f) * 100));
}

void 
VertexModifier::setZ(float value)
{
    char buffer[128];

    m_z = value;
    mysnprintf(buffer, 127, " %1.2f", m_z);
    swSetText(m_zTextEdit, buffer);

    swScrollBarSetValue(m_zScrollBar, (int) ((m_z / 2.0f + 0.5f) * 100));
}

int
VertexModifier::getMode(void)
{
    if (swGetCheck(m_quadButton))
        return VERTEX_MODIFIER_MODE_QUAD;

    if (swGetCheck(m_jumpButton))
        return VERTEX_MODIFIER_MODE_JUMP; 

    if (swGetCheck(m_peakButton))
        return VERTEX_MODIFIER_MODE_PEAK; 

    return -1;
}

bool
VertexModifier::getPlus(void)
{
    return swGetCheck(m_plusCheckBox);
}

bool
VertexModifier::getNormal(void)
{
    return swGetCheck(m_normalCheckBox);
}


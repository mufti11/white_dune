/*
 * VertexModifier.h
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

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef FLT_MAX
# include <float.h>
#endif
#include "swt.h"

#include "stdafx.h"
#include "Scene.h"
#include "SceneView.h"
#include "Node.h"
#include "FieldValue.h"
#include "MFieldCommand.h"

enum VertexModifierMode {
    VERTEX_MODIFIER_MODE_QUAD,
    VERTEX_MODIFIER_MODE_JUMP,
    VERTEX_MODIFIER_MODE_PEAK
};

class VertexModifier : public SceneView
   {
public:   
   VertexModifier(Scene *scene, SWND parent, FieldUpdate *fieldUpdate);
   ~VertexModifier(); 
   float getRadius(void) { return m_radius; }
   float getAmount(void) { return m_amount; }
   int getMode(void);
   Node *getNode(void) { return m_fieldUpdate->node; }
   int getField(void) { return m_fieldUpdate->field; }
   bool getPlus(void);
   bool getNormal(void);
   float getX(void) { return m_x; }
   float getY(void) { return m_y; }
   float getZ(void) { return m_z; }

   const char *getName(void) const { return "VertexModifier"; }

   void DeleteView();

   void okeyButtonCallback(void *id);

   void normalCheckBoxCallback(void *id);
   
   void radiusScrollCallback(int type, int value);
   void amountScrollCallback(int type, int value);

   void radiusTextCallback(void);
   void amountTextCallback(void);

   void xScrollCallback(int type, int value);
   void yScrollCallback(int type, int value);
   void zScrollCallback(int type, int value);

   void xTextCallback(void);
   void yTextCallback(void);
   void zTextCallback(void);

   void mouseCallback(int x, int y, int modifiers);
   void keyCallback(int key, int state, int x, int y, int modifiers);
   void resizeCallback(int x, int y);
   void exposeCallback(int x, int y, int width, int height);

   void setRadius(float radius);
   void setAmount(float amount);

   void setX(float value);
   void setY(float value);
   void setZ(float value);

private:
   SWND m_canvas; 
   SWND m_radiusScrollBar;
   SWND m_amountScrollBar;

   SWND m_radiusTextEdit;
   SWND m_amountTextEdit;

   SWND m_okeyButton;
   SWND m_quadButton;
   SWND m_jumpButton;
   SWND m_peakButton;
   SWND m_plusCheckBox;

   SWND m_normalCheckBox;

   SWND m_xScrollBar;
   SWND m_yScrollBar;
   SWND m_zScrollBar;

   SWND m_xTextEdit;
   SWND m_yTextEdit;
   SWND m_zTextEdit;

   SWND m_xLabel;
   SWND m_yLabel;
   SWND m_zLabel;

   float m_radius;
   float m_amount;

   float m_x;
   float m_y;
   float m_z;

protected:
        SDC m_dc;
        FieldUpdate *m_fieldUpdate;
};

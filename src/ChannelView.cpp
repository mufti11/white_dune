/*
 * ChannelView.cpp
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

#include <stdlib.h>

#include "stdafx.h"

#include "ChannelView.h"
#include "swt.h"
#include "Scene.h"
#include "Node.h"
#include "Interpolator.h"
#include "Path.h"

#define GRID_MIN        15    // minimum distance between grid lines
#define BORDER_WIDTH    20    // border at right and bottom edges
#define X_RULER_WIDTH   20    // width of horizontal ruler
#define Y_RULER_WIDTH   55    // width of vertical ruler
#define TICK_SIZE        3     // size of "ticks" in ruler

#define TO_WORLD_X(x)  (((x) - m_rect.left) / (float) m_rect.Width())
#define TO_WORLD_Y(y)  ((m_rect.bottom - (y)) / (float) m_rect.Height())
#define TO_VALUE(y)    ((TO_WORLD_Y(y) / m_yScale) + m_yMin)
#define TO_SCREEN_X(x) (m_rect.left + (int) ((x) * m_rect.Width() + 0.5f))
#define TO_SCREEN_Y(y) (m_rect.bottom - (int) ((y) * m_rect.Height() + 0.5f))

#define AUTOSCROLL_MARGIN   10
#define AUTOSCROLL_AMOUNT   20

static int      currentX, currentY;

// maximum size for drawing to bitmap
#define MAX_BACK_WIDTH 500
#define MAX_BACK_HEIGHT 500

#define MoveTo(dc, x, y)        { currentX = x; currentY = y; }
#define LineTo(dc, x, y)        { swDrawLine(dc, currentX, currentY, x, y); \
                                  currentX = x; currentY = y; }
#ifdef BLINKTIMER
static int
timerCallback(void *data)
{
    return ((ChannelView *) data)->OnBlinkTimer();
}
#endif

static int
autoScrollCallback(void *data)
{
    return ((ChannelView *) data)->OnAutoScrollTimer();
}

ChannelView::ChannelView(Scene *scene, SWND wnd)
  : SceneView(scene, wnd)
{
    int width, height;
    swGetSize(m_wnd, &width, &height);

    m_state = NORMAL;
    m_interpolator = NULL;
    m_selectedChannel = -1;
    m_selectedKey = -1;

    m_yMin = m_yMax = 0.0f;
    m_yScale = m_xScale = 1.0f;
    m_anchor = 0;
    m_selMin = m_selMax = 0;

#ifdef BLINKTIMER
    m_timer = swSetTimer(m_wnd, 500, timerCallback, this);
#endif
    m_autoScrollTimer = NULL;
    m_cursorOn = true;
    m_autoScrolling = false;

    m_rect.left = Y_RULER_WIDTH;
    m_rect.top = X_RULER_WIDTH;
    m_copiedChannels = -1;
}

ChannelView::~ChannelView()
{
#ifdef BLINKTIMER
    if (m_timer) swKillTimer(m_timer);
#endif
    if (m_autoScrollTimer) swKillTimer(m_autoScrollTimer);
}

void ChannelView::OnSize(int width, int height)
{
    swInvalidateWindow(m_wnd);
}

void ChannelView::OnDraw(int x, int y, int width, int height, bool update)
{
    SDC dc = swCreateDC(m_wnd);
//    swSetClipRect(dc, x, y, width, height);
    swSetFGColor(dc, swGetWindowColor(m_wnd, SW_COLOR_WINDOW_BG));
    swFillRect(dc, x, y, width, height);

    int w, h;
    swGetSize(m_wnd, &w, &h);

    m_rect.right = w - BORDER_WIDTH;
    m_rect.bottom = h - BORDER_WIDTH;

    if (m_interpolator && m_rect.Width() > 0 && m_rect.Height() > 0) {
        // draw selection
        DrawSelection(dc);
        DrawRulers(dc, 0.0f, 1.0f, m_yMin, m_yMax);
        DrawKeys(dc);
    }
    swDestroyDC(dc);
}

void
ChannelView::AutoScale()
{
    if (!m_interpolator) return;

    int numChannels = m_interpolator->getNumChannels();
    int numKeys = m_interpolator->getNumKeys();

    m_yMin = 0.0f;
    m_yMax = 1.0f;

    for (int chan = 0; chan < numChannels; chan++) {
        for (int k = 0; k < numKeys; k++) {
            float value = m_interpolator->getKeyValue(chan, k);
            if (value < m_yMin) m_yMin = value;
            if (value > m_yMax) m_yMax = value;
        }
    }

    m_yScale = 1.0f / (m_yMax - m_yMin);
}

// reduce drawing/use of MFVec?f Interpolators
// only draw/use channels with different keyValues

bool                
ChannelView::isDrawableChannel(int chan)
{
    if (m_interpolator->getNumChannels() > 4) {
        if (m_multipleValuesInChannel[chan])
            return true;
        else
            return false;
    }
    return true;
}

void
ChannelView::findDrawableChannels(void)
{
    int numChannels = m_interpolator->getNumChannels();
    int numKeys = m_interpolator->getNumKeys();

    m_multipleValuesInChannel.resize(numChannels);
    int maxNumChannels = TheApp->GetMaxKeysInChannelView();
    int channelCount = 0;

    for (int chan = 0; chan < numChannels; chan++) {
        m_multipleValuesInChannel[chan] = false;
        if (maxNumChannels > 0)
            if (channelCount > maxNumChannels)
                continue; 
        float firstKey;
        if (numKeys > 0)
            firstKey = m_interpolator->getKeyValue(chan, 0);
        else 
            continue;
        for (int j = 1; j < numKeys; j++)
            if (m_interpolator->getKeyValue(chan, j) != firstKey) {
                m_multipleValuesInChannel[chan] = true;
                channelCount++;
                break;
            }
    }
    // if no channel is changed, use first channel
    if ((numChannels > 0) && (channelCount == 0))
        m_multipleValuesInChannel[0] = true;
}

void
ChannelView::DrawKeys(SDC dc)
{
    int numChannels = m_interpolator->getNumChannels();
    int numKeys = m_interpolator->getNumKeys();

    for (int chan = 0; chan < numChannels; chan++) {
        if (!isDrawableChannel(chan))
            continue;

        int y = m_rect.bottom;
        float key;

        if ((numChannels == 4) && (chan == 3))
            swSetFGColor(dc, 0x00FF00FF); // only for SFRotation
        else
            swSetFGColor(dc, 0x000000FF << ((chan % 3) * 8));
        MoveTo(dc, m_rect.left, y);
        for (int k = 0; k < numKeys; k++) {
            key = m_interpolator->getKey(k);
            float value = m_interpolator->getKeyValue(chan, k);
            int x = TO_SCREEN_X(key);

            // normalize value into range 0..1
            float sv = (value - m_yMin) * m_yScale;

            y = TO_SCREEN_Y(sv);
            if (k == 0) MoveTo(dc, m_rect.left, y);
            LineTo(dc, x, y);
            swFillRect(dc, x - 2, y - 2, 5, 5);
        }
        LineTo(dc, m_rect.right, y);
    }
}

static float getStep(float ratio)
{
    float step = 1.0f;

    while ((ratio != 0) && (ratio > GRID_MIN)) {
        ratio *= 0.1f;
        step *= 0.1f;
    }

    while ((ratio != 0) && (ratio < GRID_MIN)) {
        ratio *= 10.0f;
        step *= 10.0f;
    }

    return step;
}

void
ChannelView::DrawRulers(SDC dc, float xMin, float xMax, float yMin, float yMax)
{
    float xRatio = (float) m_rect.Width() / (xMax - xMin);
    float yRatio = (float) m_rect.Height() / (yMax - yMin);

    // find nearest power of 10 

    float xStep = getStep(xRatio);
    float yStep = getStep(yRatio);

    int grey = swGetWindowColor(m_wnd, SW_COLOR_BSHADOW);
    char buf[128];
    int theight = swGetFontHeight(swGetDefaultFont());

    swSetFGColor(dc, grey);
    swFillRect(dc, 0, 0, m_rect.left, m_rect.bottom +BORDER_WIDTH);
    swFillRect(dc, m_rect.left, 0, m_rect.right+BORDER_WIDTH, m_rect.top);
    swDrawLine(dc, m_rect.left, m_rect.top-1, m_rect.right, m_rect.top-1);
    for (float x = xStep * (float) floor(xMin / xStep); x < xMax + 0.001;
         x += xStep) {
        int scrx = (int) (m_rect.left + x * xRatio + 0.5f);
        swSetFGColor(dc, 0);
        swDrawLine(dc, scrx, m_rect.top, scrx, m_rect.top - TICK_SIZE);
        mysnprintf(buf, 128, "%g", x);
        int twidth = swGetStringWidth(swGetDefaultFont(), buf);
        swDrawText(dc, scrx - twidth / 2, m_rect.top - TICK_SIZE,
                   buf);
    }
    MoveTo(dc, m_rect.left-1, m_rect.top);
    LineTo(dc, m_rect.left-1, m_rect.bottom);
    for (float y = yStep * (float) floor(yMin / yStep); y < yMax + 0.001;
         y += yStep) {
        int scry = (int) (m_rect.bottom - (y - yMin) * yRatio + 0.5f);
        swSetFGColor(dc, 0);
        swDrawLine(dc, m_rect.left, scry, m_rect.left - TICK_SIZE, scry);
        mysnprintf(buf, 128, "%g", y);
        int twidth = swGetStringWidth(swGetDefaultFont(), buf);
        int point = m_rect.left - TICK_SIZE - twidth;
        swDrawText(dc, point > 0 ? point : 0, scry + theight / 2,
                   buf);
    }
}

void
ChannelView::DrawSelection(SDC dc)
{
    if (m_selMin != m_selMax || m_cursorOn) {
        swSetFGColor(dc, 0);
        swFillRect(dc, m_rect.left + m_selMin, m_rect.top,
                   m_selMax - m_selMin+1, m_rect.Height());
    }
}

void ChannelView::OnLButtonDown(int px, int py, int modifiers) 
{
    const Interpolator *node = m_interpolator;

    if (node == NULL) return;

    int numChannels = node->getNumChannels();
    int numKeys = node->getNumKeys();

    for (int i = 0; i < numKeys; i++) {
        float key = m_interpolator->getKey(i);
        int x = TO_SCREEN_X(key);
        if (px >= x - 3 && px <= x + 3) {
            for (int chan = numChannels - 1; chan >= 0; chan--) {
                if (!isDrawableChannel(chan))
                    continue;

                float value = node->getKeyValue(chan, i);

                // normalize value into range 0..1
                float sv = (value - m_yMin) * m_yScale;
                int y = TO_SCREEN_Y(sv);

                if (py >= y - 3 && py <= y + 3) {
                    m_selectedChannel = chan;
                    m_selectedKey = i;
                    m_state = DRAGGING;
                    swSetCapture(m_wnd);
                    m_interpolator->backupKey(m_selectedKey);
                    return;
                }
            }
        }
    }

    // no keys picked; check for line picking
    for (int chan = numChannels - 1 ; chan >= 0; chan--) {
        if (!isDrawableChannel(chan))
            continue;
        int x1 = m_rect.left, y1 = TO_SCREEN_Y(0.0f);
        int k;

        for (k = 0; k < numKeys; k++) {
            float key = node->getKey(k);
            float value = node->getKeyValue(chan, k);
            int x2 = TO_SCREEN_X(key);

            // normalize value into range 0..1
            float sv = (value - m_yMin) * m_yScale;

            int y2 = TO_SCREEN_Y(sv);

            if (k == 0) y1 = y2;

            if (PointNearLine(px, py, x1, y1, x2, y2)) {
                AddKey(chan, k, px);
                return;
            }
            x1 = x2;  y1 = y2;
        }

        // check m_last line segment
        if (PointNearLine(px, py, x1, y1, m_rect.right, y1)) {
            AddKey(chan, k, px);
            return;
        }
    }

    // no keys picked; do a drag-select

    m_anchor = m_lastX = (px - m_rect.left);
    SetSelection(m_anchor);
    m_interpolator->sendInterpolatedValue(0.0, m_selMin / (float) (m_rect.Width() - 1));
    m_state = SELECTING;
    m_scene->setViewOfLastSelection(this);
    swSetCapture(m_wnd);
}

void ChannelView::AddKey(int chan, int key, int x)
{
    m_scene->addNextCommand();
    // picked a line; create a new key at that point
    float newKey = TO_WORLD_X(x);
    float *values = new float[m_interpolator->getNumChannels()];
    m_interpolator->interpolate(newKey, values);
    m_interpolator->insertKey(key, newKey, values);
    delete [] values;
    m_selectedChannel = chan;
    m_selectedKey = key;
    m_state = DRAGGING;
    swSetCapture(m_wnd);
}

void ChannelView::OnLButtonUp(int x, int y, int modifiers) 
{
    if (m_autoScrolling) {   // stop scrolling
        swKillTimer(m_autoScrollTimer);
        m_autoScrollTimer = NULL;
        m_autoScrolling = false;
        AutoScale();
        swInvalidateWindow(m_wnd);
    }
    if (m_state == DRAGGING || m_state == SELECTING) {
        swReleaseCapture(m_wnd);
        m_state = NORMAL;
    }
}

void ChannelView::OnMouseMove(int x, int y, int modifiers) 
{
    if (m_state == DRAGGING || m_state == SELECTING) {
        CheckAutoScroll(x, y);
    }
    DoMouseMove(x, y);
}

void ChannelView::DoMouseMove(int px, int py)
{
    if (m_state == DRAGGING) {
        float key = TO_WORLD_X(px);
        float value = TO_VALUE(py);
        if (m_selectedKey > 0) {
            float prevKey = m_interpolator->getKey(m_selectedKey - 1);
            if (key < prevKey) key = prevKey;
        }
        if (m_selectedKey < m_interpolator->getNumKeys() - 1) {
            float nextKey = m_interpolator->getKey(m_selectedKey + 1);
            if (key > nextKey) key = nextKey;
        }
        m_interpolator->setKey(m_selectedKey, key);
        m_interpolator->setKeyValue(m_selectedChannel, m_selectedKey, value);
        m_interpolator->sendInterpolatedValue(0.0, m_interpolator->getFraction());
    } else if (m_state == SELECTING) {
        int x = px - m_rect.left;
        x = CLAMP(x, 0, m_rect.Width() -1);
        int min = MIN(m_lastX, x);
        int max = MAX(m_lastX, x);
        swInvalidateRect(m_wnd, m_rect.left + min, m_rect.top,
                         max - min + 1, m_rect.Height());
        if (x < m_anchor) {
            m_selMin = x;
            m_selMax = m_anchor;
        } else {
            m_selMin = m_anchor;
            m_selMax = x;
        }
        m_lastX = x;
        m_interpolator->sendInterpolatedValue(0.0, x / (float) (m_rect.Width() - 1));
    }
}

void
ChannelView::InvalidateSelection()
{
    swInvalidateRect(m_wnd, m_rect.left + m_selMin, m_rect.top,
                     m_selMax - m_selMin + 1, m_rect.Height());
}

#ifdef BLINKTIMER
int ChannelView::OnBlinkTimer() 
{
    if (m_selMin == m_selMax && !m_scene->isRunning()) {
        m_cursorOn = !m_cursorOn;
        InvalidateSelection();
    }
    return TRUE;
}
#endif

int ChannelView::OnAutoScrollTimer()
{
    AutoScale();
    DoMouseMove(m_autoScrollPX, m_autoScrollPY);
    return TRUE;
}

void ChannelView::OnUpdate(SceneView* sender, int type, Hint *hint) 
{
    const Path *sel = m_scene->getSelection();
    Node *node = sel ? sel->getNode() : NULL;
    if (node == NULL)
        return;
    Interpolator *interp = NULL;
    if (node->isInterpolator())
        interp = dynamic_cast_Interpolator(node);
    FieldUpdate *fieldUpdate;
    NodeUpdate *nodeUpdate;

    switch (type) {
      CASE_UPDATE(UPDATE_ALL)
        swInvalidateWindow(m_wnd);
        break;
      CASE_UPDATE(UPDATE_SELECTION)
        if (interp != NULL && m_interpolator != interp) {
            m_interpolator = interp;
            AutoScale();
            findDrawableChannels();
            swInvalidateWindow(m_wnd);
        } 
        break;
      CASE_UPDATE(UPDATE_FIELD)
        fieldUpdate = (FieldUpdate *) hint;
        if (fieldUpdate->node == m_interpolator) {
            AutoScale();
            findDrawableChannels();
            swInvalidateWindow(m_wnd);
        }
        break;
      CASE_UPDATE(UPDATE_REMOVE_NODE)
        nodeUpdate = (NodeUpdate *) hint;
        if (nodeUpdate->node == m_interpolator) {
            m_interpolator = NULL;
            swInvalidateWindow(m_wnd);
        }
        break;
      CASE_UPDATE(UPDATE_TIME)
        if (m_interpolator) {
            SetSelection((int) (m_interpolator->getFraction() * 
                                (m_rect.Width() - 1)));
        }
        break;
    }
}

bool ChannelView::PointNearLine(int x, int y, int x1, int y1, int x2, int y2) const
{
    if (x1 != x2) {
        float slope = (y2 - y1) / (float) (x2 - x1);
        float v = y1 + slope * (x - x1);

        if (x >= x1 && x <= x2 && y >= v - 3.0f && y <= v + 3.0f) {
            return true;
        }
    }
    return false;
}

void ChannelView::OnFastForward() 
{
    if (m_interpolator) {
        m_interpolator->sendInterpolatedValue(0.0, 1.0f);
        SetSelection(m_rect.Width()-1);
    }
}

void ChannelView::OnRewind() 
{
    if (m_interpolator) {
        m_interpolator->sendInterpolatedValue(0.0, 0.0f);
        SetSelection(0);
    }
}

void ChannelView::SetSelection(int pos)
{
    InvalidateSelection();
    m_selMin = m_selMax = pos;
    m_cursorOn = true;
    InvalidateSelection();
}

void ChannelView::OnEditDelete() 
{
    if (m_interpolator) {
        int min = m_interpolator->findKey(m_selMin / (float) (m_rect.Width() - 1));
        int max = m_interpolator->findLessKey(m_selMax / (float) (m_rect.Width() - 1));
        if (max == m_interpolator->key()->getSize())
            max--;
        m_interpolator->deleteKeys(min, max);
    }
}

#if 0
void ChannelView::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
    if (m_interpolator) {
        int min = m_interpolator->findKeyInclusive(m_selMin / (float) (m_rect.Width() - 1));
        int max = m_interpolator->findKey(m_selMax / (float) (m_rect.Width() - 1));
        if (max == m_interpolator->key()->getSize())
            max--;

        pCmdUI->Enable(min < max);
    } else {
        pCmdUI->Enable(FALSE);
    }
}
#endif

void ChannelView::CheckAutoScroll(int px, int py)
{
    int width, height;
    bool autoScroll = true;

    swGetSize(m_wnd, &width, &height);
    m_autoScrollPX = px;
    m_autoScrollPY = py;

    if (px < AUTOSCROLL_MARGIN) {
        autoScroll = true;
    } else if (px > width - AUTOSCROLL_MARGIN) {
        autoScroll = true;
    } else if (py < AUTOSCROLL_MARGIN) {
        autoScroll = true;
    } else if (py > height - AUTOSCROLL_MARGIN) {
        autoScroll = true;
    } else {
        autoScroll = false;
    }

    if (autoScroll && !m_autoScrolling) {   // start scrolling
        m_autoScrollTimer = swSetTimer(m_wnd, 100, autoScrollCallback, this);
        m_autoScrolling = true;
    }
    if (!autoScroll && m_autoScrolling) {   // stop scrolling
        swKillTimer(m_autoScrollTimer);
        m_autoScrollTimer = NULL;
        m_autoScrolling = false;
    }
}

void ChannelView::DeleteLastSelection(void)
{
    OnEditDelete();
}

void ChannelView::GoToLastKey(void)
{
    if (m_interpolator) {
        m_selectedKey = m_interpolator->findKeyExclusive(m_selMin / (float) 
                                                        (m_rect.Width() - 1));
        if (m_selectedKey > 0)
            m_selectedKey--;
        float key = m_interpolator->getKey(m_selectedKey);
        SetSelection(key * (m_rect.Width() - 1));
        m_interpolator->sendInterpolatedValue(0.0, key);
        m_scene->UpdateViews(NULL, UPDATE_REDRAW);
    }
}

void ChannelView::GoToNextKey(void)
{
    if (m_interpolator) {
        int selectedKey = m_interpolator->findKeyInclusive(m_selMin / (float) 
                                                           (m_rect.Width() - 1));
        if (selectedKey == m_selectedKey)
            m_selectedKey = selectedKey + 1;
        else
            m_selectedKey = selectedKey;
        float key = m_interpolator->getKey(m_selectedKey);
        SetSelection(key * (m_rect.Width() - 1));
        m_interpolator->sendInterpolatedValue(0.0, key);
        m_scene->UpdateViews(NULL, UPDATE_REDRAW);
    }
}

void            
ChannelView::CopyLastSelection(void)
{
    m_copiedValues.resize(0);
    if (m_interpolator) {
        m_copiedChannels = m_interpolator->getNumChannels();
        float keyStart = m_selMin / (float) (m_rect.Width() - 1);
        int min = m_interpolator->findKeyInclusive(m_selMin / (float) (m_rect.Width() - 1));
        int max = m_interpolator->findKey(m_selMax / (float) (m_rect.Width() - 1));
        for (int i = min; i < max; i++) {
            m_copiedValues.append(m_interpolator->key()->getValue(i) - keyStart);
            for (int j = 0; j < m_interpolator->getNumChannels(); j++)
                m_copiedValues.append(m_interpolator->getKeyValue(j, i));
        }
    }
}

void            
ChannelView::PasteLastSelection(void)
{
    if (m_interpolator) {
        if (m_copiedChannels != m_interpolator->getNumChannels())
            return;
        float keyStart = m_selMin / (float) (m_rect.Width() - 1);
        int keys = 0;
        long i = 0;
        while (i < m_copiedValues.size()) {
            float key = keyStart + m_copiedValues[keys++ * 
                                                  (m_copiedChannels + 1)];
            if (key > 1.0f) {
                m_copiedChannels = -1;
                return;
            }
            int foundKey = m_interpolator->findKeyInclusive(key);
            i++;
            m_interpolator->insertKey(foundKey, key, &m_copiedValues[i]);
            i += m_interpolator->getNumChannels();
        }
        m_copiedChannels = -1;
    }    
}

void            
ChannelView::PasteSymetricLastSelection(int direction)
{
    if (m_interpolator) {
        if (m_copiedChannels != m_interpolator->getNumChannels())
            return;
        float keyStart = m_selMin / (float) (m_rect.Width() - 1);
        int stride = m_interpolator->getStride();
        int keys = 0;
        long i = 0;
        while (i < m_copiedValues.size()) {
            float key = keyStart + m_copiedValues[keys++ * 
                                                  (m_copiedChannels + 1)];
            if (key > 1.0f) {
                m_copiedChannels = -1;
                return;
            }
            int foundKey = m_interpolator->findKeyInclusive(key);
            i++;
            for (int j = 0; j <  m_interpolator->getNumChannels(); j++)
                if ((j % stride) == direction)
                    m_copiedValues[i + j] *= -1;
            m_interpolator->insertKey(foundKey, key, &m_copiedValues[i]);
            i += m_interpolator->getNumChannels();
        }
        m_copiedChannels = -1;
    }    
}


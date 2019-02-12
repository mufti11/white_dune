/*
 * ChannelView.h
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

#ifndef _CHANNEL_VIEW_H
#define _CHANNEL_VIEW_H

// undefine if you do not need the blinking channelview cursor
#define BLINKTIMER 1 

#ifndef _RECT_H
#include "Rect.h"
#endif
#ifndef _POINT_H
#include "Point.h"
#endif
#ifndef _SCENE_VIEW_H
#include "SceneView.h"
#endif

class Node;
class DuneDoc;

enum {
    NORMAL,
    DRAGGING,
    SELECTING
};

#include "swttypedef.h"

class Interpolator;

class ChannelView : public SceneView
{
public:
                        ChannelView(Scene *scene, SWND parent);
    virtual            ~ChannelView();

    virtual void        OnDraw(int x, int y, int width, int height);
    virtual void        OnUpdate(SceneView *sender, int type, Hint *hint);

    virtual void        OnLButtonDown(int x, int y, int modifiers);
    virtual void        OnLButtonUp(int x, int y, int modifiers);
    virtual void        OnMouseMove(int x, int y, int modifiers);
    virtual void        OnFastForward();
    virtual void        OnRewind();
    virtual void        OnEditDelete();
    virtual void        OnSize(int width, int height);
//    virtual void      OnUpdateEditDelete(CCmdUI* pCmdUI);

    const char         *getName(void) const { return "ChannelView"; }

    virtual void        DeleteLastSelection(void);
    virtual void        GoToNextKey(void);
    virtual void        GoToLastKey(void);

#ifdef BLINKTIMER
    int                 OnBlinkTimer();
#endif
    int                 OnAutoScrollTimer();

private:
    void                AutoScale();
    void                DoMouseMove(int x, int y);
    void                CheckAutoScroll(int x, int y);
    void                DrawRulers(SDC dc, float xMin, float xMax,
                                           float yMin, float yMax);
    void                DrawKeys(SDC dc);
    void                DrawSelection(SDC dc);
    void                SetSelection(int pos);
    void                InvalidateSelection();
    bool                PointNearLine(int x, int y, int x1, int y1,
                                                    int x2, int y2) const;
    void                AddKey(int chan, int key, int x);
    bool                isDrawableChannel(int chan);
    void                findDrawableChannels(void);

private:
    Interpolator       *m_interpolator;
    int                 m_selectedChannel;
    int                 m_selectedKey;
    float               m_yMin, m_yMax;
    float               m_xScale;
    float               m_yScale;
    int                 m_state;
    Rect                m_rect;
    int                 m_anchor;
    int                 m_lastX;
    int                 m_selMin;
    int                 m_selMax;
#ifdef BLINKTIMER
    STIMER              m_timer;
#endif
    STIMER              m_autoScrollTimer;
    bool                m_cursorOn;
    bool                m_autoScrolling;
    int                 m_autoScrollPX, m_autoScrollPY;
    MyArray<bool>       m_multipleValuesInChannel;
};
#endif

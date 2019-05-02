/*
 * RouteViewApp.h
 *
 * Copyright (C) 2003 J. "MUFTI" Scheurich
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

#ifndef _ROUTE_VIEW_APP_H
#define _ROUTE_VIEW_APP_H

class RouteViewApp {
public:
                        RouteViewApp();

    void                SetRouteViewQueueLength(int value) 
                           { m_queueLength = value; }
    int                 GetRouteViewQueueLength(void) 
                           { return m_queueLength; }

    void                SetRoutedNodeIndent(int value) 
                           { m_routedNodeIndent = value; }
    int                 GetRoutedNodeIndent(void) 
                           { return m_routedNodeIndent; }

    bool                GetRouteViewShowAll() const { return m_showAll; }
    void                SetRouteViewShowAll(bool flag) { m_showAll = flag; }

    bool                GetShowRoutesAtBegin() const 
                           { return m_showRoutesAtBegin; }
    void                SetShowRoutesAtBegin(bool flag) 
                           { m_showRoutesAtBegin = flag; }

    void                RouteViewSetDefaults();

    void                RouteViewLoadPreferences();
    void                RouteViewSavePreferences();


private:
    bool                m_showAll;
    bool                m_showRoutesAtBegin;
    int                 m_queueLength;
    int                 m_routedNodeIndent;
};

#endif


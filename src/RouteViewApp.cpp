/*
 * RouteViewApp.cpp
 *
 * Copyright (C) 2005 J. "MUFTI" Scheurich
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

#include <errno.h>
#include "stdafx.h"
#include <math.h>

#include "DuneApp.h"
#include "ExternTheApp.h"
#include "RouteViewApp.h"
#include "PreferencesApp.h"

#define DEFAULT_QUEUE_LENGTH 20
#define DEFAULT_ROUTED_NODE_INDENT 6

RouteViewApp::RouteViewApp()
   {
   RouteViewSetDefaults();
   }

void
RouteViewApp::RouteViewSetDefaults()
   {
   m_showAll = false;   
   m_showRoutesAtBegin = true;
   m_queueLength = DEFAULT_QUEUE_LENGTH;
   m_routedNodeIndent = DEFAULT_ROUTED_NODE_INDENT;
   }

void
RouteViewApp::RouteViewLoadPreferences()
   {
   assert(TheApp != NULL);

   m_showAll = TheApp->GetBoolPreference("RouteViewShowAll", false);    
   m_showRoutesAtBegin = TheApp->GetBoolPreference("ShowRoutesAtBegin", true);

   char buf[128];
   const char *buf2;

   mysnprintf(buf, 128, "%d", DEFAULT_QUEUE_LENGTH);   
   buf2 = TheApp->GetPreference("RouteViewQueueLength", buf); 
   m_queueLength = atoi(buf2);

   mysnprintf(buf, 128, "%d", DEFAULT_ROUTED_NODE_INDENT);   
   buf2 = TheApp->GetPreference("RoutedNodeIndent", buf); 
   m_routedNodeIndent = atoi(buf2);
   }

void
RouteViewApp::RouteViewSavePreferences()
   {
   assert(TheApp != NULL);

   char buf[128];

   TheApp->SetBoolPreference("RouteViewShowAll", m_showAll);
   TheApp->SetBoolPreference("ShowRoutesAtBegin", m_showRoutesAtBegin);


   mysnprintf(buf, 128, "%d", m_queueLength);
   TheApp->SetPreference("RouteViewQueueLength", buf); 

   mysnprintf(buf, 128, "%d", m_routedNodeIndent);
   TheApp->SetPreference("RoutedNodeIndent", buf); 
   }


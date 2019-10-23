/*
 * StartWithApp.cpp
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

#include <errno.h>
#include "stdafx.h"
#include <math.h>

#include "DuneApp.h"
#include "StartWithApp.h"
#include "PreferencesApp.h"

StartWithApp::StartWithApp()
   {
   m_startVariant = NULL;
   m_startLanguage = NULL;
   }

void
StartWithApp::StartWithSetDefaults()
   {
   if (m_startVariant != NULL)
       free(m_startVariant);
   m_startVariant = mystrdup("dune");
   if (m_startLanguage != NULL)
       free(m_startLanguage);
   m_startLanguage = mystrdup("en");
   }

void
StartWithApp::StartWithLoadPreferences()
   {
   assert(TheApp != NULL);

   StartWithSetDefaults();

   if (m_startVariant != NULL)
       free(m_startVariant);
#ifdef _WIN32
   m_startVariant = mystrdup(TheApp->GetPreference("StartWith", "4kids"));
#elifdef DARWIN
   m_startVariant = mystrdup(TheApp->GetPreference("StartWith", "4kids"));
#else
   m_startVariant = mystrdup(TheApp->GetPreference("StartWith", "dune"));
#endif
   if (m_startLanguage != NULL)
       free(m_startLanguage);
   m_startLanguage = mystrdup(TheApp->GetPreference("StartLanguage", "en"));
   }

void
StartWithApp::StartWithSavePreferences()
   {
   assert(TheApp != NULL);

   if (m_startVariant != NULL)
       TheApp->SetPreference("StartWith", m_startVariant);
   if (m_startLanguage != NULL)
       TheApp->SetPreference("StartLanguage", m_startLanguage);
   }


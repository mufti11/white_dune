/*
 * ExportNamesApp.cpp
 *
 * Copyright (C) 2009 J. "MUFTI" Scheurich
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

#include "DuneApp.h"
#include "ExportNamesApp.h"


ExportNamesApp:: ExportNamesApp()
{
    m_prefix = ""; 
    m_cNodeName = "Node";
    m_cNodeNamePtr = "Node*";
    m_cNodeNameArray = "Node []";
    m_cSceneGraphName = "SceneGraph";
}

const char *
ExportNamesApp::getCPrefix(void) 
{
    if ((m_prefix == NULL) || (strlen(m_prefix) == 0))
       return TheApp->GetWonderlandModuleExportPrefix();        
    return m_prefix;
}

void
ExportNamesApp::buildCSceneGraphName(void) 
{ 
     m_cSceneGraphName = "";
     m_cSceneGraphName += getCPrefix();
     m_cSceneGraphName += "SceneGraph";
}

void
ExportNamesApp::buildCProcessEventsName(void) 
{ 
     m_cProcessEventsName = "";
     m_cProcessEventsName += getCPrefix();
     m_cProcessEventsName += "ProcessEvents";
}

void
ExportNamesApp::buildCNodeName(void) 
{ 
     m_cNodeName = "";
     m_cNodeName += getCPrefix();
     m_cNodeName += "Node";

     m_cNodeNamePtr = "";
     m_cNodeNamePtr += m_cNodeName;
     m_cNodeNamePtr += "*";
    
     m_cNodeNameArray = "";
     m_cNodeNameArray += m_cNodeName;
     m_cNodeNameArray += " []";
}

void
ExportNamesApp::updatePrefix(void)
{
     buildCNodeName();
     buildCSceneGraphName();
     buildCProcessEventsName();
}

void
ExportNamesApp::setPrefix(const char* prefix) 
{ 
     m_prefix = prefix; 
     updatePrefix();
}



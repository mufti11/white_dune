/*
 * StereoViewApp.cpp
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

#include <errno.h>
#include "stdafx.h"
#include "math.h"

#include "DuneApp.h"
#include "ExternTheApp.h"
#include "StereoViewApp.h"

StereoViewApp::StereoViewApp()
   {
   m_eyeMode = EM_NONE;
   m_useStereo = false;
   m_canStereo = false;
   m_canQuadBufferStereo = false;
   m_canAnaglyphStereo = false;
   m_stereoType = NO_STEREO;
   m_wantStereo = false;
   StereoViewSetDefaults();
   }

void
StereoViewApp::StereoViewSetDefaults()
   {
   m_eyeHalfDist = 0.06/2.0;
   m_eyeScreenDist = 0.8;
   /* inexact, repair when fieldofview in Viewpoint is working */
   /* 18.0 fieldofview of human eye */
   /* 45.0 default fieldofview of VRML97 */
   m_fixFieldOfView = -1;
   m_eyeAngleFactor = 18.0/45.0;   
   accountEyeAngle();
   m_stereoHandleSizeMult = 2.0;
   m_cursor3dMode = CM_3DCURSOR_ALWAYS;
   m_cursor3dWidth = 1.0;
   m_cursor3dLength = 0.001;
   }

static bool isAnaglyph(int stereoType)
   {
   switch (stereoType) 
      {
      case RED_GREEN_ANAGLYPH_STEREO:
      case GREEN_RED_ANAGLYPH_STEREO:
      case RED_BLUE_ANAGLYPH_STEREO:
      case BLUE_RED_ANAGLYPH_STEREO:
      case RED_CYAN_ANAGLYPH_STEREO:
      case CYAN_RED_ANAGLYPH_STEREO:
         return true;
      }
   return false;
   }

bool 
StereoViewApp::canStereo(int stereoType)
   {
   if (stereoType == QUAD_BUFFER_STEREO)
       return m_canQuadBufferStereo;
   if (isAnaglyph(stereoType))
       return m_canAnaglyphStereo;
   return false;
   }


bool
StereoViewApp::isAnaglyphStereo(void)
   {
   if (isAnaglyph(m_stereoType))
       return true;
   return false;
   }


void
StereoViewApp::setStereoType(int value)
   { 
   m_stereoType = value; 
   if (value != NO_STEREO) 
       {
       m_canStereo = true;
       setWantStereo(true);
       }
   if (isAnaglyph(value))
      m_canAnaglyphStereo = true;
   else if (value == QUAD_BUFFER_STEREO)
      m_canQuadBufferStereo = true;
}

void
StereoViewApp::accountEyeAngle()
   {
   m_eyeAngle=RAD2DEG(atan2(m_eyeHalfDist,m_eyeScreenDist))*m_eyeAngleFactor;
   }

void
StereoViewApp::StereoViewLoadPreferences()
   {
   assert(TheApp != NULL);

   char buf[128];
   const char* buf2;

   StereoViewSetDefaults();

   m_stereoType = TheApp->GetIntPreference("StereoType", QUAD_BUFFER_STEREO);
   m_wantStereo = TheApp->GetBoolPreference("WantStereo", false);

   mysnprintf(buf, 128, "%f", 2 * m_eyeHalfDist);
   buf2 = TheApp->GetPreference("EyeDistance", buf); 
   m_eyeHalfDist = atof(buf2) * 0.5;

   mysnprintf(buf, 128, "%f", m_eyeScreenDist);
   buf2 = TheApp->GetPreference("EyeScreenDistance", buf); 
   m_eyeScreenDist = atof(buf2);

   mysnprintf(buf, 128, "%f", m_fixFieldOfView);
   buf2 = TheApp->GetPreference("FixFieldOfView", buf); 
   m_fixFieldOfView = atof(buf2);


   mysnprintf(buf, 128, "%f", m_eyeAngleFactor);
   buf2 = TheApp->GetPreference("EyeAngleFactor", buf); 
   m_eyeAngleFactor = atof(buf2);

   mysnprintf(buf, 128, "%f", m_stereoHandleSizeMult);
   buf2 = TheApp->GetPreference("StereoViewHandleSizeMult", buf); 
   m_stereoHandleSizeMult = atof(buf2);


   m_cursor3dMode = (Cursor3dMode) TheApp->GetIntPreference("Cursor3Mode", 
                                                            m_cursor3dMode);

   mysnprintf(buf, 128, "%f", m_cursor3dWidth);
   buf2 = TheApp->GetPreference("Cursor3dWidth", buf); 
   m_cursor3dWidth = atof(buf2);

   mysnprintf(buf, 128, "%f", m_cursor3dLength);
   buf2 = TheApp->GetPreference("Cursor3dLength", buf); 
   m_cursor3dLength = atof(buf2);


   accountEyeAngle();   
   }

void
StereoViewApp::StereoViewSavePreferences()
   {
   assert(TheApp != NULL);

   char buf[128];

   TheApp->SetIntPreference("StereoType", m_stereoType);
   TheApp->SetBoolPreference("WantStereo", m_wantStereo);

   mysnprintf(buf, 128, "%f", 2 * m_eyeHalfDist);
   TheApp->SetPreference("EyeDistance", buf); 

   mysnprintf(buf, 128, "%f", m_eyeScreenDist);
   TheApp->SetPreference("EyeScreenDistance", buf); 

   mysnprintf(buf, 128, "%f", m_fixFieldOfView);
   TheApp->SetPreference("FixFieldOfView", buf); 


   mysnprintf(buf, 128, "%f", m_eyeAngleFactor);
   TheApp->SetPreference("EyeAngleFactor", buf); 

   mysnprintf(buf, 128, "%f", m_stereoHandleSizeMult);
   TheApp->SetPreference("StereoViewHandleSizeMult", buf); 


   TheApp->SetIntPreference("Cursor3Mode", m_cursor3dMode);

   mysnprintf(buf, 128, "%f", m_cursor3dWidth);
   TheApp->SetPreference("Cursor3dWidth", buf); 

   mysnprintf(buf, 128, "%f", m_cursor3dLength);
   TheApp->SetPreference("Cursor3dLength", buf); 
   }

bool parseCommandlineArgumentStereoView(int & i,int argc, char** argv)
   {
   bool found = true;
   if (strcmp(argv[i],"-nostereo")==0)
      return found;
   else if (strcmp(argv[i],"-eyedist")==0) 
      {
      float eyedist;
      if (i++>=argc) return found;
      if (sscanf(argv[i],"%f",&eyedist)==1)
         TheApp->setEyeDist(eyedist);
      } 
   else if (strcmp(argv[i],"-screendist")==0) 
      {
      float screendist;
      if (i++>=argc) return found;
      if (sscanf(argv[i],"%f",&screendist)==1)
         TheApp->setEyeScreenDist(screendist);
      }
   else if (strcmp(argv[i],"-anaglyph")==0) 
      {
      if (i++>=argc) return found;
      }
   else
      return false;
   return found;
}

float               
StereoViewApp::getEyePosition(void)
{
    float eyeposition=0; 

    if (TheApp->useStereo())
       {
       // inexact "toe in" stereo method 
       if (TheApp->getEyeMode()==EM_RIGHT)
          eyeposition= - TheApp->getEyeHalfDist();
       else if (TheApp->getEyeMode()==EM_LEFT)
          eyeposition= + TheApp->getEyeHalfDist();
       }
    return eyeposition;
}


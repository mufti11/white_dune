/*
 * StereoViewApp.h
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

#ifndef _STEREO_VIEW_APP_H
#define _STEREO_VIEW_APP_H

#include "swt/include/stereoview.h"

typedef enum {
    EM_LEFT,
    EM_RIGHT,
    EM_NONE
} EyeMode;

typedef enum {
    CM_3DCURSOR_NONE,
    CM_3DCURSOR_RECORDING,
    CM_3DCURSOR_NOT_RUN,
    CM_3DCURSOR_ALWAYS
} Cursor3dMode;

extern bool parseCommandlineArgumentStereoView(int & i,int argc, char** argv);

class StereoViewApp {
public:
                        StereoViewApp();

    bool                canStereo(int stereoType);

    bool                canQuadBufferStereo(void) 
                           { return m_canQuadBufferStereo; }

    void                setUseStereo(bool value) 
                           {
                           if (m_canStereo) 
                               m_useStereo=value;
                           else
                               m_useStereo=false;
                           }
    bool                useStereo(void) 
                           {return m_useStereo;}

    void                setWantStereo(bool value) {m_wantStereo = value;}
    bool                wantStereo(void) {return m_wantStereo;}

    void                setStereoType(int value);

    int                 getStereoType(void)
                           {
                           if (useStereo())
                               return m_stereoType;
                           else
                               return NO_STEREO;
                           }

    bool                isAnaglyphStereo(void);

    void                setEyeDist(float value) 
                           {
                           m_eyeHalfDist = value / 2.0; 
                           accountEyeAngle();
                           }

    void                setEyeScreenDist(float value) 
                           {
                           m_eyeScreenDist = value; 
                           accountEyeAngle();
                           }

    float               getEyeHalfDist(void)  {return m_eyeHalfDist;}
    float               getEyeAngle(void) {return m_eyeAngle;}
    float               getEyeScreenDist(void) {return m_eyeScreenDist;}

    EyeMode             getEyeMode(void)  {return m_eyeMode;}
    void                setEyeMode(EyeMode value) {m_eyeMode = value;}

    float               getEyePosition(void);

    // good stereoviewing may need to ignore the FieldOfView field 
    // of a viewpoint (FieldOfView of the human eye is about 18 Degree)
    void                setFixFieldOfView(float fov) { m_fixFieldOfView = fov; }
    float               getFixFieldOfView(void) { return m_fixFieldOfView; }
    bool                hasFixFieldOfView(void)
                           { 
                           if (m_fixFieldOfView > 0) 
                               return true;
                           else
                               return false;
                           }
    float               GetStereoHandleSizeMult(void) 
                           { return m_stereoHandleSizeMult; }
    void                SetStereoHandleSizeMult(float m) 
                           { m_stereoHandleSizeMult = m; }

    Cursor3dMode        Get3dCursorMode(void) {return m_cursor3dMode; }
    void                Set3dCursorMode(Cursor3dMode cursor3dMode)
                           {
                           m_cursor3dMode = cursor3dMode;
                           }
    float               Get3dCursorWidth(void) { return m_cursor3dWidth; }
    void                Set3dCursorWidth(float w) { m_cursor3dWidth = w; }
    float               Get3dCursorLength(void) { return m_cursor3dLength; }
    void                Set3dCursorLength(float l) { m_cursor3dLength = l; }

    float               GetEyeAngleFactor(void) { return m_eyeAngleFactor; }
    void                SetEyeAngleFactor(float factor) 
                           { 
                           m_eyeAngleFactor = factor; 
                           accountEyeAngle();
                           }

    void                StereoViewSetDefaults();

    void                StereoViewLoadPreferences();
    void                StereoViewSavePreferences();

private:
    void                accountEyeAngle(void);

private:
    bool                m_canStereo;
    bool                m_canQuadBufferStereo;
    bool                m_canAnaglyphStereo;
    bool                m_useStereo;
    bool                m_wantStereo;
    int                 m_stereoType;
    EyeMode             m_eyeMode;
    float               m_eyeHalfDist;
    float               m_eyeAngle;
    float               m_eyeAngleFactor;
    float               m_eyeScreenDist;
    float               m_fixFieldOfView;
    float               m_stereoHandleSizeMult;
    Cursor3dMode        m_cursor3dMode;
    float               m_cursor3dWidth;
    float               m_cursor3dLength;
};

#endif


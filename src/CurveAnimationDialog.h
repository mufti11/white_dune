/*
 * CurveAnimationDialog.h
 *
 * Copyright (C) 2000 Stephen F. White, 2003 Th. Rothermel
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

#pragma once

#include "Dialog.h"
#include "swttypedef.h"

class NodeTimeSensor;
class Scene;

class CurveAnimationDialog : public Dialog {
public:
                        CurveAnimationDialog(SWND parent, int nPoints, 
                                             int degree, int direction, 
                                             Scene *scene, float animationTime,
                                             bool keepDegree = false);
    virtual            ~CurveAnimationDialog();

    void                LoadData();
    virtual void        SaveData();
    virtual bool        Validate();
    int                 getnPoints() {return m_nPoints;}
    int                 getDegree() {return m_degree;}
    int                 getDirection() {return m_direction; }
    NodeTimeSensor     *getTimeSensor()        { return m_timeSensor; }
    float               getTimeSensorSeconds() { return m_newTimeSensorSeconds; }
private:
    int                 m_nPoints;
    int                 m_degree;
    int                 m_direction;
    Scene              *m_scene;
    NodeTimeSensor     *m_timeSensor;
    float               m_newTimeSensorSeconds;
    bool                m_keepDegree;
};



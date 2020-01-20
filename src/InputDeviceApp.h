/*
 * InputDeviceApp.h
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

#pragma once

#include <string.h>

#include "Array.h"
#include "TransformMode.h"

class InputDevice;
#ifdef HAVE_AFLOCK
class AflockDevice;
#endif 

class InputDeviceApp {
public:

                        InputDeviceApp() 
                           {
                           m_dontCareFocus = false;
                           m_maxNumberAxes=0;    
                           }


    bool                dontCareFocus(void) { return m_dontCareFocus; }
    void                setDontCareFocus(void) { m_dontCareFocus = true; }

    InputDevice*        getInputDevice(int i);
    int                 getNumberInputDevices(void) 
                           { return m_inputDevices.size(); }
    int                 setInputDevice(InputDevice* inputdevice);
    int                 getMaxNumberAxesInputDevices(void)
                           { return m_maxNumberAxes; }
    void                accoutMaxNumberAxesInputDevices(void);

                        // sort inputdevices, so inputdevices with a readdelay
                        // can prepare the read, while other inputdevices 
                        // can use the time to read data
    void                sortInputDevices(void);

    bool                hasInputDevices(void);
    bool                has2AxesInputDevices(void);
    bool                has3AxesInputDevices(void);
    int                 searchInputDevice(const char *option, 
                                         const char *deviceName);
    int                 replaceOrAddInputDevice(InputDevice* newDevice,
                                                const char *option, 
                                                const char *deviceName);
    void                setInputDeviceSetting(InputDevice *device, 
                                              const char *setting);
    void                deleteInputDevice(InputDevice* device);


#ifdef HAVE_AFLOCK
    AflockDevice*       getAflockDevice(int i); 
    int                 getNumberAflockDevices(void) 
                           {return m_aflockDevices.size();}
    int                 setAflockDevice(AflockDevice* aflockDevice);
    int                 searchAflockDevice(const char *deviceName);
    int                 replaceOrAddAflockDevice(AflockDevice* newDevice,
                                                 const char *deviceName);
    void                deleteAflockDevice(AflockDevice* device);
    bool                returnTracker(void);

    void                stopTrackers(void);
    void                restartTrackers(void);
#else
    bool                returnTracker(void) { return false; }
#endif
    void                calibrateInputDevices(void);

    void                increaseInputDevice(TransformMode* tm);
    void                decreaseInputDevice(TransformMode* tm);

    void                InputDeviceLoadPreferences();
    void                InputDeviceSavePreferences();


private:
    bool                m_dontCareFocus;
    int                 m_maxNumberAxes;
    MyArray<InputDevice*> m_inputDevices;
#ifdef HAVE_AFLOCK
    MyArray<AflockDevice*>m_aflockDevices;
#endif
};



/*
 * InputDeviceApp.cpp
 *
 * Copyright (C) 2003, 2019 J. "MUFTI" Scheurich
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

#include "DuneApp.h"
#include "InputDeviceApp.h"
#include "InputDevice.h"

InputDevice*        
InputDeviceApp::getInputDevice(int i) 
{
   if ((i<((int)m_inputDevices.size())) && (i>=0))
      return m_inputDevices[i];
   else
      return NULL;    
}

int                 
InputDeviceApp::setInputDevice(InputDevice* inputdevice)
{
   return replaceOrAddInputDevice(inputdevice,
         inputdevice->getDeviceOption(),
         inputdevice->getDeviceName());
}

void                
InputDeviceApp::accoutMaxNumberAxesInputDevices(void)
{
   int max=0;       
   for (size_t i=0;i<m_inputDevices.size();i++)
      if (m_inputDevices[i]->get_number_axes()>max)
         max=m_inputDevices[i]->get_number_axes();
   m_maxNumberAxes=max;
}

bool                
InputDeviceApp::hasInputDevices(void)
{
   if (m_inputDevices.size()>0)
      {
      for (size_t i=0; i < m_inputDevices.size(); i++)
         if (m_inputDevices[i]->isValid()) 
            return true;
      return false;
      }
   else
      return false;
}

bool                
InputDeviceApp::has2AxesInputDevices(void)
{
   for (size_t i=0;i<m_inputDevices.size();i++)
      if (m_inputDevices[i]->get_number_axes()==2)
         return true;
   return false;
}

bool                
InputDeviceApp::has3AxesInputDevices(void)
{
   for (size_t i=0;i<m_inputDevices.size();i++)
      if (m_inputDevices[i]->get_number_axes()==3)
         return true;
   return false;
}

int                
InputDeviceApp::searchInputDevice(const char *option, const char *deviceName)
{
   for (size_t i=0;i<m_inputDevices.size();i++)
      if ((strcmp(m_inputDevices[i]->getDeviceOption(),
                  option) == 0) &&
          (strcmp(m_inputDevices[i]->getDeviceName(),
                  deviceName) == 0))
         return i;
   return -1;
}

int                
InputDeviceApp::replaceOrAddInputDevice(InputDevice* newDevice, 
                                        const char *option, 
                                        const char *deviceName)
{
   int device = searchInputDevice(option, deviceName);
   if (device > -1) {
       if (m_inputDevices[device] != newDevice) {
           delete m_inputDevices[device];
           m_inputDevices[device] = newDevice;
       }
       return device; 
   } else if (newDevice != NULL) {
       m_inputDevices.append(newDevice); 
       return m_inputDevices.size()-1;
   }
   return -1;
}

void                
InputDeviceApp::deleteInputDevice(InputDevice* device)
{
   for (size_t i=0;i<m_inputDevices.size();i++)
      if (m_inputDevices[i] == device)
           m_inputDevices.remove(i);
}


#ifdef HAVE_AFLOCK
AflockDevice*       
InputDeviceApp::getAflockDevice(int i) 
{
   if ((i<((int)m_aflockDevices.size())) && (i>=0))
      return m_aflockDevices[i];
   else
      return NULL;    
}

int                 
InputDeviceApp::setAflockDevice(AflockDevice* aflockDevice)
{
   return replaceOrAddAflockDevice(aflockDevice, aflockDevice->getDeviceName());
}

int                
InputDeviceApp::searchAflockDevice(const char *deviceName)
{
   for (size_t i=0;i<m_aflockDevices.size();i++)
      if (strcmp(m_aflockDevices[i]->getDeviceName(),
                 deviceName) == 0)
         return i;
   return -1;
}

int                 
InputDeviceApp::replaceOrAddAflockDevice(AflockDevice* newDevice,
                                         const char *deviceName)
{
   int device = searchAflockDevice(deviceName);
   if (device > -1) {
       if (m_aflockDevices[device] != newDevice) {
           delete m_aflockDevices[device];
           m_aflockDevices[device] = newDevice;
       }
       return device; 
   } else
       m_aflockDevices.append(newDevice); 
   return m_aflockDevices.size()-1;
}

bool                
InputDeviceApp::returnTracker(void)
{
   if (m_aflockDevices.size()==0)
      return(false);
   else
      for (size_t i=0;i<m_aflockDevices.size();i++)
         delete m_aflockDevices[i];
   return(true);
} 

void                
InputDeviceApp::stopTrackers(void)
{
   for (size_t i=0;i<m_aflockDevices.size();i++)
      m_aflockDevices[i]->stop();
} 

void                
InputDeviceApp::restartTrackers(void)
{
   for (size_t i=0;i<m_aflockDevices.size();i++)
      m_aflockDevices[i]->start();
} 

void                
InputDeviceApp::deleteAflockDevice(AflockDevice* device)
{
   for (size_t i=0;i<m_aflockDevices.size();i++)
      if (m_aflockDevices[i] == device)
           m_aflockDevices.remove(i);    
}

#endif

void                
InputDeviceApp::calibrateInputDevices(void)
{
   for (size_t i=0;i<m_inputDevices.size();i++)
      m_inputDevices[i]->set_firstflag();
}

void                
InputDeviceApp::increaseInputDevice(TransformMode* tm)
{
   for (size_t i=0;i<m_inputDevices.size();i++) 
      {
      if (m_inputDevices[i]->isTracker() || 
          m_inputDevices[i]->isWand()) continue;
      if (tm->hasTranslation())
         {
         float factor=m_inputDevices[i]->getxyzfactor();
         m_inputDevices[i]->setxyzfactor(2*factor);
         }
      if (tm->hasRotation())
         {
         float factor=m_inputDevices[i]->getrotfactor();
         m_inputDevices[i]->setrotfactor(2*factor);
         }
      }
}

void                
InputDeviceApp::decreaseInputDevice(TransformMode* tm)
{
   for (size_t i=0;i<m_inputDevices.size();i++) 
      {
      if (m_inputDevices[i]->isTracker() || 
          m_inputDevices[i]->isWand()) continue;
      if (tm->hasTranslation())
         {
         float factor=m_inputDevices[i]->getxyzfactor();
         m_inputDevices[i]->setxyzfactor(0.5*factor);
         }
     if (tm->hasRotation())
         {
         float factor=m_inputDevices[i]->getrotfactor();
         m_inputDevices[i]->setrotfactor(0.5*factor);
         }
      }
}



// sort inputdevices
//
// make inputdevices without readdelay comes first in array
//
// so inputdevices with a readdelay can prepare the read, 
// while other inputdevices can use the time to read data

void
InputDeviceApp::sortInputDevices(void)
{
    // only a few devices, it is ok to use bubblesort 
    bool changedflag=false;
    do {
       InputDevice* tmp;
       changedflag=false;
       for (size_t i=0;i<m_inputDevices.size()-1;i++)
          if (!m_inputDevices[i]->hasReadDelay() && 
               m_inputDevices[i+1]->hasReadDelay()) {
             tmp=m_inputDevices[i];
             m_inputDevices[i]=m_inputDevices[i+1];
             m_inputDevices[i+1]=tmp;
             changedflag=true;
          }
    } while (changedflag==true);
}

void 
InputDeviceApp::setInputDeviceSetting(InputDevice *device, const char *setting)
   {
   if ((stringncmp(setting,"-x")==0) ||
       (stringncmp(setting,"-y")==0) ||
       (stringncmp(setting,"-z")==0) ||
       (stringncmp(setting,"-all")==0) ||
       (stringncmp(setting,"-none")==0))
      device->setAxisInformation(setting);
   else if (stringncmp(setting,"-axes")==0)
      device->setNumberAxes(setting);
   else if (strcmp(setting,"-sendalways")==0)
      device->setSendAlways(true);
   else if (strcmp(setting,"-dontcarefocus")==0)
      setDontCareFocus();
   }


void
InputDeviceApp::InputDeviceLoadPreferences()
{
    assert(TheApp != NULL);

    MyString inputDeviceOption = "";
    MyString inputDeviceName = "";
    int i = 0;
    bool hasAnotherDevice=false;
    do {
         inputDeviceOption = "";
         inputDeviceOption += TheApp->GetArrayPreference("InputDeviceOption",
                                                         i);
         inputDeviceName = "";
         inputDeviceName += TheApp->GetArrayPreference("InputDeviceName",
                                                       i);
         hasAnotherDevice=false;
         if ((inputDeviceOption.length() > 0) && (inputDeviceName.length() > 0))
             {
             hasAnotherDevice=true;
             InputDevice *device = InputDevice::createInputDevice(
                                          inputDeviceOption, inputDeviceName);
             replaceOrAddInputDevice(device, 
                                     inputDeviceOption, inputDeviceName);
             if (device != NULL) {
                 int j = 0;
                 const char *setting = "";
                 do {
                     MyString settingsKey = "";
                     settingsKey += "InputDeviceSettings";
                     settingsKey.catInt(i);
                     setting = TheApp->GetArrayPreference(settingsKey, j);
                     if (strlen(setting) > 0)
                         setInputDeviceSetting(device, setting);
                     j++;
                 } while (strlen(setting) > 0);
             }
         }         
         i++;
    } while (hasAnotherDevice);
}

void
InputDeviceApp::InputDeviceSavePreferences()
{
    assert(TheApp != NULL);
 
    for (int i = 0; i < getNumberInputDevices(); i++) {
        InputDevice *device = getInputDevice(i);
        TheApp->SetArrayPreference("InputDeviceOption", i, 
                                   device->getDeviceOption());
        TheApp->SetArrayPreference("InputDeviceName", i, 
                                   device->getDeviceName());
/*
        char *axes_name[6];
        for (int j = 0; j < 6; j++)
             axes_name[j] = device->getAxesInfo(j);
*/

        for (int j = 0; j < 6; j++) {
             int axis = device->getNumAxis(j);
             MyString settings = "";
             settings += "-";
             if (device->get_ignore(axis))
                 settings += "none=";
             else { 
                 settings += device->getAxesInfo(j);
                 settings += "=";
                     if (device->get_sign(axis) < 0)
                         settings += "-";                     
             }
             settings.catInt(device->getNumAxis(j));
             if (!device->get_ignore(axis)) {
                 settings += ",";
                 settings.catInt(device->get_factor(axis));
                 settings += ",";
                 settings.catFloat(device->get_acceleration(axis));
                 settings += ",";
                 if (!device->get_zero_on_release(axis))
                     settings += "wheel";
                 settings += ",";
                 settings.catFloat(device->get_zero_size_fraction(axis));
             }
             MyString settingsKey = "";
             settingsKey += "InputDeviceSettings";
             settingsKey.catInt(i);
             TheApp->SetArrayPreference(settingsKey, j, settings);
        }
    }
    TheApp->SetArrayPreference("InputDeviceOption", getNumberInputDevices(), ""
                              ); 
    TheApp->SetArrayPreference("InputDeviceName", getNumberInputDevices(), ""); 
}



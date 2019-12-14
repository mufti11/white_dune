/*
 * InputDevice.h
 *
 * Copyright (C) 2001 J. "MUFTI" Scheurich
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

#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"
#include "MyString.h"
#include "StringArray.h"
#include "swt.h"
#include "TransformMode.h"
#include "EulerAngles.h"
#include "Quaternion.h"
#include "Map.h"

class InputDeviceApp;

// class to access several joystick like devices via a unified interface

// Example for such devices are spaceballs, magellan-devices, 
// thumbsticks of gamepads and of course joysticks 8-)   

// This can also be used for Xinput devices, that not deliver zero, 
// if you take your hand away from the device like dialbox, mouse or 
// graphics tablett.

class InputDevice
   {
public:
   // true if there is a new event from the joystick
   virtual bool readInputDevice(void) = 0;
   
   virtual bool isValid(void) { return valid; }

   Quaternion& get_quaternion(TransformMode* tm, bool local, bool check_only);

   virtual Quaternion& get_quaternion(TransformMode* tm=NULL,
                                      bool check_only=false);
   virtual Quaternion& get_localQuaternion(TransformMode* tm=NULL,
                                           bool check_only=false);
   EulerAngles& get_eulerAngles(TransformMode* tm,bool check_only,bool current);

   Vec3f& get_vector(TransformMode* tm);

   int get_number_axes(void) { return number_max_axis - number_ignored_axes; }
   int get_number_max_axis(void) { return number_max_axis; } 

   int get_number_buttons(void) { return number_buttons; }

   virtual const char *getDeviceOption(void) const = 0;
   const char *getDeviceName(void) { return device_name; }

   bool isWand(void) { return wandflag; }
   bool isHead(void) { return headflag; }
   virtual bool sendalways(void) { return alwaysflag; }
 
   virtual bool allzero(void);

   virtual bool isTracker(void) { return false; }

   virtual bool hasReadDelay(void) { return false; }

   virtual void prepareRead(void) { return; }

   virtual float maxvalue(int) const=0;

   virtual bool useCurrent(void) { return false; }

   virtual void setHeadNavigation(void) { }
   virtual bool getHeadNavigation(void) { return true; }

   float getxyzfactor(void) { return xyzfactor; }
   float getrotfactor(void) { return rotfactor; }

   void setxyzfactor(float f) { xyzfactor = f; }
   void setrotfactor(float f) { rotfactor = f; }

   void set_firstflag(void);

   virtual void guessInputDeviceNames(StringArray *names) const = 0;

   static InputDevice *createInputDevice(const char* device_option, 
                                         const char* device_name);
 
   InputDevice(const char *device) 
      {
      initialize();
      valid=true;
      if (device==NULL)
          device_name=NULL;
      else
          device_name=strdup(device);
      }

   void initialize(void) 
      {
      valid=false;
      // initialise joystick class default data
      button_pressed=-1;
      button_released=-1;
      number_buttons=0;
      number_max_axis=0;
      number_max_axes=7;
      number_ignored_axes=0;
      for (int i=0;i<number_max_axes;i++)
         value[i]=0;
      headflag=false;
      wandflag=false;
      alwaysflag=false;
      zero_on_release=NULL;
      sign_swap=NULL;
      factor=NULL;
      acceleration=NULL;
      zero_size_fraction=NULL;
      num_axis_x=0;
      num_axis_y=1;
      num_axis_z=2;
      num_axis_xrot=3;
      num_axis_yrot=4;
      num_axis_zrot=5;
      num_axis_angle=6;
      static const char* axesinfodata[6]= { 
         "xrot", 
         "yrot", 
         "zrot", 
         "x", 
         "y", 
         "z"
      };
      for (int i=0;i<6;i++)
         axesinfo[i]=(char *)axesinfodata[i];
      for (int i=0;i<7;i++)
         firstflag[i]=true;
      for (int i=0;i<7;i++)
         oldvalue[i]=0;
      for (int i=0;i<7;i++)
         ignore[i]=false;
      zero_on_release=new bool[number_max_axes];   
      for (int i=0;i<number_max_axes;i++)
         zero_on_release[i]=true;
      alwaysflag=false;
      xyzfactor=1;
      rotfactor=1;
      device_name=NULL;
      error_message="";
      }
   virtual ~InputDevice() 
      {
      if (zero_on_release!=NULL)
         delete zero_on_release;
      if (acceleration!=NULL)
         delete acceleration;
      }
   void setAxisInformation(const char* info_string);
   void setNumberAxes(const char* number);
   void setSendAlways(bool flag) {alwaysflag=flag;}

   bool get_ignore(int axis) { return ignore[axis]; }
   int get_sign(int axis_number);
   int getAxisFromInformation(const char* axesinfo);
   float get_factor(int axis_number);
   float get_acceleration(int axis_number);
   bool get_zero_on_release(int axis_number);
   float get_zero_size_fraction(int axis_number); 

   char *getAxesInfo(int i) { return axesinfo[i >= 3 ? i - 3 : i + 3]; } 

   const char *getErrorMessage(void) { return error_message; }

   int getNumAxis(int i) {
      switch(i) {
        case 0:
          return num_axis_x;
        case 1:
          return num_axis_y;
        case 2:
          return num_axis_z;
        case 3:
          return num_axis_xrot;
        case 4:
          return num_axis_yrot;
        case 5:
          return num_axis_zrot;
      }
      return -1;
   }     
   
protected:
   bool valid;

   float get_x(TransformMode* tm=NULL,bool check_only=false,bool curr=false);
   float get_y(TransformMode* tm=NULL,bool check_only=false,bool curr=false);
   float get_z(TransformMode* tm=NULL,bool check_only=false,bool curr=false);
   float get_xrot(TransformMode* tm=NULL,bool check_only=false,bool curr=false);
   float get_yrot(TransformMode* tm=NULL,bool check_only=false,bool curr=false);
   float get_zrot(TransformMode* tm=NULL,bool check_only=false,bool curr=false);
   float get_angle(TransformMode* tm=NULL,bool check_only=false,bool curr=false);

   // is device a headtracker ?
   bool headflag;
   // is device a wand ?
   bool wandflag;
   // do device send (amost) always values ?
   bool alwaysflag;

   // axes of device
   int number_max_axis;
   // number of axes used in account (7 (including angle axis))
   int number_max_axes;
   // number of ignored axes of device
   int number_ignored_axes;

   // 0-2 offset and 3-7 rotation values from joystick (if available)
   float value[7];

   // buttons of joystick
   int number_buttons;
   bool *button;
   // last pressed/released button
   int button_pressed;
   int button_released;

   // number of axis for values
   int num_axis_x;
   int num_axis_y;
   int num_axis_z;
   int num_axis_xrot;
   int num_axis_yrot;
   int num_axis_zrot;
   int num_axis_angle;   

   // true if axis number deliver zero when device released
   void set_zero_on_release(int axis_number,bool value);
   void set_zero_on_release(const char* axis_string,bool value);

   // formula to accelerate values from axis
   float accelerate(float value,int num_axis);

   // signswap of value from axis
   void set_sign_swap(int axis_number,bool value);

   // factor: multiplcated to value from axis
   void set_factor(int axis_number,float value);
   void set_factor(const char* axis_string,float value);

   // acceleration: additional acceleration parameter
   void set_acceleration(int axis_number,float value);
   void set_acceleration(const char* axis_string,float value);

   // percent (relative to max) of value which will be ignored
   // "insensitivity of device"
   void set_zero_size_fraction(int axis_number,float value);
   void set_zero_size_fraction(const char* string,float value);

   MyString error_message;

protected:

   // function to read/set configuration data like
   //    axisnumbers, zero_on_release, acceleration
   void setAxisOfInformation(const char* axesinfo,int axis_number);

   bool max_value(int index,TransformMode* tm);
   float get_value(int index,bool check_only,int num_axis);
   float get_value(TransformMode* tm, 
                   int index, bool check_only, int num_axis, bool current);

   float get_current_value(int index,int num_axis);

   // array of flags for first get from device
   bool firstflag[7];

   // flags if the value of a axis should be ignored
   bool ignore[7];

   // array of old values from device
   float oldvalue[7];

   // array of flags if axis number arrayindex deliver zero when device is 
   // released
   bool* zero_on_release;

   // array of nullsize in percent
   float* zero_size_fraction;

   // array of flags, if sign of result is swapped
   bool* sign_swap;
  
   // array of factor to multiply with value of axis
   float* factor;

   // array of acceleration per axis
   float* acceleration;

   // helper functions for setAxisInformation
   bool scan_argument(char *string,const char* argument);
   bool argument_is_axis(const char* string);
   char* axesinfo[6]; 

   float xyzfactor;
   float rotfactor;

   Quaternion old_quat;
   Quaternion old_local_quat;
  
   const char *device_name;
   };

# ifdef LINUX_JOYSTICK

// using Linux Joystick interface

#  define NAME_LENGTH 128
#  include <linux/joystick.h>

class linux_joystick : public InputDevice 
   {
public: 
   linux_joystick(const char* device);
   ~linux_joystick();
   bool readInputDevice(void);
   const char *getDeviceOption(void) const { return "-joystick"; }
   void guessInputDeviceNames(StringArray *names) const;
private:
   int fd;
   int version;
   char name[NAME_LENGTH];
   struct js_event js;
   float maxvalue(int i) const {return(32767.0);}
   float linux_js_factor;
   };

# endif

# ifdef DIRECTX_JOYSTICK

// using M$Windows Joystick interface from DirectInput API

#  include <Windows.h>
#include <commctrl.h>
#include <basetsd.h>
#include <dinput.h>

class windows_joystick : public InputDevice 
   {
public: 
   windows_joystick(const char* device);
   windows_joystick() : InputDevice() {}
   ~windows_joystick();
   bool readInputDevice(void);
   const char *getDeviceOption(void) const { return "-joystick"; }
   void guessInputDeviceNames(StringArray *names) const;
private:
   int number_of_joystick;
   int version;
   char name[MAXPNAMELEN];
   DIJOYSTATE2 js;
   DIPROPRANGE diprg;
   int *max_value;
   int *null_value;
   float maxvalue(int i) const {return max_value[i];}
   float factor;

   HRESULT InitDirectInput();
   LPDIRECTINPUT8 g_pDI;         
   LPDIRECTINPUTDEVICE8 g_pJoystick;
   };

# endif

# ifdef WINDOWS_JOYSTICK

// using older M$Windows Joystick API interface ("JOYCAPS" etc.)

#  include <Windows.h>

class windows_joystick : public InputDevice 
   {
public: 
   windows_joystick(const char* device);
   ~windows_joystick();
   bool readInputDevice(void);
   const char *getDeviceOption(void) const { return "-joystick"; }
   void guessInputDeviceNames(StringArray *names) const;
private:
   int number_of_joystick;
   int version;
   char name[MAXPNAMELEN];
   JOYCAPS joycapabilities;
   JOYINFOEX joyinfo_ex;
   int *maximum;
   int *null_value;
   float maxvalue(int i) const {return maximum[i];}
   float windows_js_factor;
   };

# endif

# ifdef HAVE_SDL_JOYSTICK

// using modified Simple DirectMedia Layer joystick interface
# include "SDLjoystick/SDL_joystick.h"

class SDL_joystick : public InputDevice 
   {
public: 
   SDL_joystick(const char* device);
   ~SDL_joystick();
   bool readInputDevice(void);
   const char *getDeviceOption(void) const { return "-SDLjoystick"; }
   void guessInputDeviceNames(StringArray *names) const;
private:
   SDL_Joystick* js;
   int number_of_joystick;
   int *maximum;
   int *null_value;
   float maxvalue(int i) const {return maximum[i];}
   float sdl_factor;
   };

# endif

# ifdef HAVE_XINPUT

// Xi X11 XInput devices (e.g. spaceball, magellan) 

#  include <X11/extensions/XInput.h>
extern "C" 
   {
#  include "swtxinput.h"
   }

extern Display* swGetDisplay(void);
extern Window* swGetWindow(void);

class xinput : public InputDevice 
   {
public: 
   xinput(const char* device);
   ~xinput();
   bool readInputDevice(void);
   const char *getDeviceOption(void) const { return "-xinput"; }
   void guessInputDeviceNames(StringArray *names) const;
private:
   char* name;
   swXinput* swxinput;
   float xinput_factor;
   float *maximum;
   float maxvalue(int i) const {return maximum[i];}
   int nullsize;
   };

# endif

# ifdef HAVE_LIBSBALL

// using libsball library for SpaceTec/LabTec Spaceball devices

#  include <sball.h>

class spaceball : public InputDevice 
   {
public: 
   spaceball(const char* device);
   ~spaceball();
   bool readInputDevice(void);
   const char *getDeviceOption(void) const { return "-spaceball"; }
   virtual bool isValid(void) { return valid && (sball != NULL); }
   void guessInputDeviceNames(StringArray *names) const;
private:
   const char* name;
   SBallHandle sball;
   float maxvalue(int i) const {return maximum;}
   float maximum;
   float sball_factor;
   int nullsize;
   };

# endif

#ifdef HAVE_NXT_DIALS

#define USB_ID_VENDOR_LEGO 0x0694
#define USB_ID_PRODUCT_NXT 0x0002
#define USB_OUT_ENDPOINT   0x01
#define USB_IN_ENDPOINT    0x82
#define USB_TIMEOUT        1000
#define USB_INTERFACE      0
#define USB_NXT_CONFIG     1

class nxtDials : public InputDevice 
   {
public: 
   nxtDials(const char* device);
   virtual ~nxtDials();
   bool readInputDevice(void);
   const char *getDeviceOption(void) const { return "-nxtdials"; }
   void guessInputDeviceNames(StringArray *names) const;
private:
   const char* name;
   float maxvalue(int i) const {return maximum;}
   float maximum;
   float factor;
   const char* errorString;
   int getRotationCount(int motorport);

   // USB functions in this class from on libnxtusb.cpp of 
   // http://jgraef.bplaced.de/libnxtusb-0.1b.zip
   /****************************************************************************
    *                               LibNXTUSB                                  *
    ****************************************************************************
    *                                                                          *
    *  Name:     LibNXTUSB 0.1                                                 *
    *  Auhor:    Janosch Gräf <jgraef@users.sf.ne t>                          *
    *  License:  GNU/GPL                                                           *
    *                                                                              *
    ********************************************************************************
    *                                                                              *
    *  LibNXTUSB - Use your NXT with USB under Linux                               *
    *  Copyright (C) 2007  Janosch Gräf                                           *
    *                                                                              *
    *  This program is free software: you can redistribute it and/or modify        *
    *  it under the terms of the GNU General Public License as published by        *
    *  the Free Software Foundation, either version 3 of the License, or           *
    *  (at your option) any later version.                                         *
    *                                                                              *
    *  This program is distributed in the hope that it will be useful,             *
    *  but WITHOUT ANY WARRANTY; without even the implied warranty of              *
    *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
    *  GNU General Public License for more details.                                *
    *                                                                              *
    *  You should have received a copy of the GNU General Public License           *
    *  along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
    *                                                                              *
    ********************************************************************************/

   struct usb_dev_handle *nxt_handle;

   void shutdown();
   usb_dev_handle *locate_nxt(int index);
   int usb_sendpacket(char *buffer,int size);
   int usb_recvpacket(char *buffer,int size);
   void abortprog();
   };

#endif

# ifdef HAVE_WINDOWS_P5

// using P5 library for P5 devices on M$Windows

#include "p5dll.h"
#include "p5bend.h"
#include "P5Motion.h"

class windowsp5 : public InputDevice 
   {
public:
   windowsp5(const char* device);
   ~windowsp5();
   bool sendalways(void) { return true; }  
   bool readInputDevice(void);
   const char *getDeviceOption(void) const { return "-p5"; }
   void guessInputDeviceNames(StringArray *names) const;
private:
   CP5DLL P5;
   bool bP5Present;
   char* name;
   int number_p5;
   float maxvalue(int i) const {return maximum;}
   float maximum;
   float p5_factor;
   int nullsize;
   };

# endif

# ifdef HAVE_WINDOWS_SPACEBALL

// using modern driver for SpaceNavigator devices on M$Windows

class windowsspaceball : public InputDevice 
   {
public: 
   windowsspaceball(const char* device);
   ~windowsspaceball();
   const char *getDeviceOption(void) const { return "-spaceball"; }
   bool readInputDevice(void);
   void guessInputDeviceNames(StringArray *names) const;
private:
   bool initialized;
   char* name;
   float maxvalue(int i) const {return maximum;}
   float maximum;
   float windows_sball_factor;
   int nullsize;
   };

# endif

# ifdef HAVE_AFLOCK

// using aFlock program from VR Juggler library for 
// Ascention Flock of birds devices

#  include "Aflock.h"

class AflockDevice
   {
public: 
   AflockDevice(const char* device);
   ~AflockDevice();
   bool isValid() { return opened; }
   Aflock* getAflock(void);
   const char *getDeviceName(void) { return name; } 
   void close();
   const char *getErrorMessage(void) { return errorMessage; }
   // parameters
   void setBaud(char* baudrate_string);
   void setSync(char* sync_string);
   void setBlock(char* block_string);
   void setNumBrds(char* numBrds_string);
   void setTransmit(char* transmit_string);
   void setHemi(char* hemi_string);
   void setFilt(char* filt_string);
   void setSuddenChangeLock(char* sudden_string);
   void setReport(char* report_string);
   void setCalfile(char* calfile_string);
   void setIgnoreSize(const char* ignoresize_string);

   void start(void) { if (flock) flock->start(); }
   void stop(void) { if (flock) flock->stop(); }

   int getBaudrate(void) { return baudrate; }
   int getNumBrds(void) { return numBrds; }
   int getBlock(void) { return block; }
   int getTransmit(void) { return transmit; }
   const char* getHemi(void);
   const char* getFilt(void);
   const char* getCalfile(void) { return calfile; }
   bool getSudden_change_lock(void) { return sudden_change_lock; }
   char getReport(void) { return report; }
   int getSync(void) { return sync; }
   float getIgnoreSize() { return ignoreSize; }

private:
   char* name;
   Aflock* flock;
   bool opened;   
   MyString errorMessage;
  
   const char* device;
   int baudrate;
   int sync;
   int block;
   int numBrds;
   int transmit;
   BIRD_HEMI hemi;
   BIRD_FILT filt;
   bool sudden_change_lock;
   char report;
   char* calfile;
   float ignoreSize;
   };

class aflock : public InputDevice 
   {
public: 
   aflock(AflockDevice* device,char* receiver_string,bool headflag);
   aflock(const char *checkDev) : InputDevice("")
       { 
       aflockDevice = NULL;
       checkDevice = checkDev;
       } 
   ~aflock();
   bool readInputDevice(void);
   bool isValid();
   bool sendalways(void) { return true; }
   bool hasReadDelay(void) { return true; }
   void prepareRead(void);         
   void setHeadNavigation(void) { headNavigation=true; }
   bool getHeadNavigation(void) { return isTracker() && headNavigation; }
   bool useCurrent(void) { return true; }
   const char *getDeviceOption(void) const 
      { 
      if (head) 
         return "-tracker";
      else
         return "-wand";
      }
   void guessInputDeviceNames(StringArray *names) const;
private:
   int receiver;   
   char* name;
   bool head;
   bool wand;
   bool headNavigation;
   bool isTracker(void) { return head; }
   bool isWand(void) { return wand; }
   Aflock* flock;
   AflockDevice* aflockDevice;
   const char *checkDevice;
   float maxvalue(int i) const {return maximum;}
   float maximum;
   float aflock_factor;
   float degreefactor;
   bool aflockFirstFlag;
   float old_value[3];
   };
# endif

class dummyInputDevice : public InputDevice 
   {
public: 
   dummyInputDevice(const char* device) : InputDevice(device) {};
   ~dummyInputDevice() {};
   bool readInputDevice(void) { return false; }
   const char *getDeviceOption(void) const { return "-dummy"; }
   void guessInputDeviceNames(StringArray *names) const;
private:
   float maxvalue(int axis) const { return 0.0; }
   };

/*
 * InputDevice.cpp 
 * 2001-2008 J. "MUFTI" Scheurich 
 */

/*
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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "InputDevice.h"
#include <string.h>
#include <math.h>
#include "SFRotation.h"

#include <sys/types.h>
#include <sys/stat.h>
#ifndef _WIN32
# include <unistd.h>
#endif

#ifdef _WIN32
#if _MSC_VER < 1800
double copysign (double x, double y)
   {
   if (y>=0)
      return fabs(x);
   else
      return -fabs(x);
   }
#endif
#endif

float InputDevice::accelerate(float value,int num_axis)
   {
   float result=0;
   if (get_acceleration(num_axis)!=1.0)
      {
#  ifdef HAVE_POWF
      result=copysign(powf(get_acceleration(num_axis),fabs(value))-1,value);
#  else
      result=copysign(pow((double)get_acceleration(num_axis),fabs(value))-1,
                      value);
#  endif
      }
   result+=value*get_factor(num_axis);
   return result*get_sign(num_axis);
   }

static int max_position(float f1,float f2,float f3)
   {
   if (fabs(f1) >= fabs(f2)) 
      if (fabs(f1) >= fabs(f3))
         return 0;
   if (fabs(f2) >= fabs(f3)) 
      if (fabs(f2) >= fabs(f1))
         return 1;
   if (fabs(f3) >= fabs(f2))
      if (fabs(f3) >= fabs(f1))
         return 2;
   assert(0);
   return -1;
   }

static int min_position(float f1,float f2,float f3)
   {
   if (fabs(f1) <= fabs(f2) )
      if (fabs(f1) <= fabs(f3))
         return 0;
   if (fabs(f2) <= fabs(f3)) 
      if (fabs(f2) <= fabs(f1))
         return 1;
   if (fabs(f3) <= fabs(f2)) 
      if (fabs(f3) <= fabs(f1))
         return 2;
   assert(0);
   return -1;
   }

// test if value belongs to the greater values in 1, 2 (or 3) dimensions

bool InputDevice::max_value(int index,TransformMode* tm)
   {
   if (tm==NULL)
      return true;
   if (tm->tdimension==TM_3D)
      return(true);
   if (tm->tdimension==TM_2D) {
      if (index<3)
         {
         if (index!=min_position(value[0],value[1],value[2]))
            return true;
         }
      else
         if (index!=(3+min_position(value[3],value[4],value[5])))
            return true;
   }
   if (tm->tdimension==TM_1D) {
      if (index<3)
         {
         if (index==max_position(value[0],value[1],value[2]))
            return true;
         }
      else
         if (index==(3+max_position(value[3],value[4],value[5])))
            return true;
   }
   return false;
   }   

void InputDevice::set_firstflag(void)
   {
   for (int i=0;i<7;i++)
      if (!get_zero_on_release(i))
         firstflag[i]=true;
   }


// return value, if device deliver zero if you release it,
// otherwise return difference to last value

float InputDevice::get_value(int index,bool check_only,int num_axis)
   {
   // value > zerosize ?
   float zerosize=get_zero_size_fraction(num_axis)*maxvalue(num_axis);
   if (fabs(value[index]) < zerosize)
      return 0;
   float val;
   // subtract nullsize from value
   if (value[index]>0)
      val=value[index]-zerosize;
   else
      val=value[index]+zerosize;
   if (get_zero_on_release(num_axis))
      return(accelerate(val,num_axis));
   else
      {
      float ret;
      if (firstflag[index])
         {
         ret=0.0;
         if (check_only)
            {
            if (val!=0)
               {
               firstflag[index]=false;
               oldvalue[index]=val;
               }
            return(0);
            }
         firstflag[index]=false;
         }
      else
         ret=accelerate(val,num_axis)-
             accelerate(oldvalue[index],num_axis);
      if (!check_only)
         oldvalue[index]=val;
      return ret;
      }    
   }

float InputDevice::get_value(TransformMode* tm, int index, bool check_only,
                             int num_axis, bool current)
   {
   if (ignore[num_axis])
         return 0;
   if (current) 
      {
      // value > zerosize ?
      float zerosize=get_zero_size_fraction(num_axis)*maxvalue(num_axis);
      if (fabs(value[index]) < zerosize)
         return 0;
      float val;
      // subtract nullsize from value
      if (value[index]>0)
         val=value[index]-zerosize;
      else
         val=value[index]+zerosize;
      return val;
      }
   else 
      {
      if (!max_value(index,tm))
         return 0.0;
      return get_value(index,check_only,num_axis);
      }
   }

#define INPUT_DEVICE_GET(functionname,index,num_axis) \
float functionname (TransformMode* tm,bool check_only, bool current) \
   { return get_value(tm, index, check_only, num_axis, current); }

INPUT_DEVICE_GET(InputDevice::get_x,0,num_axis_x)
INPUT_DEVICE_GET(InputDevice::get_y,1,num_axis_y)
INPUT_DEVICE_GET(InputDevice::get_z,2,num_axis_z)
INPUT_DEVICE_GET(InputDevice::get_xrot,3,num_axis_xrot)
INPUT_DEVICE_GET(InputDevice::get_yrot,4,num_axis_yrot)
INPUT_DEVICE_GET(InputDevice::get_zrot,5,num_axis_zrot)
INPUT_DEVICE_GET(InputDevice::get_angle,6,num_axis_angle)

Vec3f& InputDevice::get_vector(TransformMode* tm)
   {
   static Vec3f v;
   v.x=get_x(tm);
   v.y=get_y(tm);
   v.z=get_z(tm);
   if (get_number_axes()==4)
      v[2]=-v[2];
   TMode mode=tm->tmode;
   if (mode==TM_TRANSLATE) 
      {
      if (get_number_axes()==2)
         if (tm->t2axes==TM_NEAR_FAR) 
            {
            float temp=v[2];
            v[2]=-v[1];
            v[1]=temp;
            } 
      } 
   else if (mode==TM_ROCKET) 
      {
      v[0]=0;
      v[1]=0;
      } 
   else if (mode==TM_HOVER) 
      {
      if (get_number_axes()==3) 
         {
         v[0]=0;
         } 
      else if (get_number_axes()==2) 
         {
         if (tm->t2axes==TM_NEAR_FAR) 
            {
            v[2]=-v[1];
            v[0]=0;
            v[1]=0;
            }
         } 
      else if (tm->t2axes==TM_UP_DOWN) 
         {
         v[1]=v[1];
         v[0]=0;
         v[2]=0;
         }             
      } 
   else if (mode==TM_SCALE)
      {
      v[0]=-v[0];
      v[2]=-v[2];
      if (get_number_axes()==2) 
         if (tm->t2axes==TM_NEAR_FAR) 
            {
            float temp=v[2];
            v[2]=-v[1];
            v[1]=temp;
            }             
      } 
   else if (mode==TM_UNIFORM_SCALE)
      {
      v[0]=-v[0];
      v[2]=-v[2];
      if (get_number_axes()==2) 
         if (tm->t2axes==TM_NEAR_FAR) 
            {
            float temp=v[2];
            v[2]=-v[1];
            v[1]=temp;
            }             
      int max_index = max_position(v[0],v[1],v[2]);
      if (max_index == 0) 
         {
         v[1] = 0.0f;
         v[2] = 0.0f;
         }
      else if (max_index == 1) 
         {
         v[0] = 0.0f;
         v[2] = 0.0f;
         }
      else
         {
         v[0] = 0.0f;
         v[1] = 0.0f;
         }
      } 
   else if (mode==TM_CENTER)
      {
      if (get_number_axes()==2) 
         if (tm->t2axes==TM_NEAR_FAR) 
            {
            float temp=v[2];
            v[2]=-v[1];
            v[1]=temp;
            }             
      } 
   for (int i=0;i<3;i++)
      v[i] *= xyzfactor;
   return v;
   }

EulerAngles& InputDevice::get_eulerAngles(TransformMode* tm, bool check_only,
                                          bool current)
   {
   bool rotationOnly=false;
   bool rocketOnly=false;
   bool hoverOnly=false;
   static EulerAngles euler;   
   // check for constraints on 2 axes or 3 axes or 4 axes devices
   if (get_number_axes()<=4)
      if (tm!=NULL) {
         if (tm->tmode==TM_ROTATE)
            rotationOnly=true;
         else if (tm->tmode==TM_ROCKET)
            rocketOnly=true;
      }
   if (tm!=NULL)
      if (tm->tmode==TM_HOVER)
         hoverOnly=true;
   if (rotationOnly)
      { 
      if (get_number_axes()==4)
         {
         euler.x=/*RAD2DEG*/(-get_y(tm, check_only, current));
         euler.y=/*RAD2DEG*/( get_xrot(tm, check_only, current));
         euler.z=/*RAD2DEG*/(-get_x(tm, check_only, current));
         }
      else if (get_number_axes()==3)
         {
         euler.x=/*RAD2DEG*/(-get_y(tm, check_only, current));
         euler.y=/*RAD2DEG*/( get_z(tm, check_only, current));
         euler.z=/*RAD2DEG*/(-get_x(tm, check_only, current));
         }
      else if (get_number_axes()==2)
         {
         if (tm->t2axes==TM_NEAR_FAR)
            {
            euler.x=/*RAD2DEG*/(-get_y(tm, check_only, current));
            euler.y=/*RAD2DEG*/(get_x(tm, check_only, current));
            euler.z=0;
            }
         else 
            {
            euler.x=/*RAD2DEG*/(-get_y(tm, check_only, current));
            euler.y=0;
            euler.z=/*RAD2DEG*/(-get_x(tm, check_only, current));
            }
         }
      }
   else if (rocketOnly)
      {
      if (get_number_axes()==4)
         {
         euler.x=/*RAD2DEG*/(-get_y(tm, check_only, current));
         euler.y=/*RAD2DEG*/(-get_xrot(tm, check_only, current));
         euler.z=/*RAD2DEG*/(-get_x(tm, check_only, current));
         }
      else if (get_number_axes()==3)
         {
         if (tm->t2axes==TM_NEAR_FAR)
            { 
            euler.x=/*RAD2DEG*/(-get_y(tm, check_only, current));
            euler.y=/*RAD2DEG*/(-get_x(tm, check_only, current));
            euler.z=0;
            }
         else
            { 
            euler.x=/*RAD2DEG*/(-get_y(tm, check_only, current));
            euler.y=0;
            euler.z=/*RAD2DEG*/(-get_x(tm, check_only, current));
            }
         }
      }
   else if (hoverOnly)
      {
      if (get_number_axes()==4)
         {
         euler.x=0;
         euler.y=/*RAD2DEG*/(-get_xrot(tm, check_only, current));
         euler.z=0;
         }
      else if (get_number_axes()==3)
         {
         euler.x=0;
         euler.y=/*RAD2DEG*/(-get_x(tm, check_only, current));
         euler.z=0;
         }
      else if (get_number_axes()==2)
         {
         euler.x=0;
         euler.y=/*RAD2DEG*/(-get_x(tm, check_only, current));
         euler.z=0;
         }
      else
         {
         euler.x=0;
         euler.y=/*RAD2DEG*/( get_yrot(tm, check_only, current));
         euler.z=0;
         }
      }
   else
      {
      euler.x=/*RAD2DEG*/( get_xrot(tm, check_only, current));
      euler.y=/*RAD2DEG*/( get_yrot(tm, check_only, current));
      euler.z=/*RAD2DEG*/(-get_zrot(tm, check_only, current));
      }
 
   if (!current)
      {
      euler.x *= rotfactor;
      euler.y *= rotfactor;
      euler.z *= rotfactor;
      }
   euler.w=EulOrdXYZs;
   return euler;
   }

Quaternion& InputDevice::get_quaternion(TransformMode* tm, 
                                        bool local, bool check_only)
   {
   Quaternion inputrot;
   EulerAngles euler=get_eulerAngles(tm, check_only, useCurrent());
   if (local)    
      {
      euler.x=-euler.x;   
      euler.z=-euler.z;   
      }
   inputrot=Eul_ToQuat(euler);
   inputrot.normalize();
   static Quaternion ret;
   if (useCurrent())
      {   
      SFRotation rot(inputrot);
      const float *quat=rot.getValue();
      float arc=quat[3]*rotfactor;
      SFRotation rot2(quat[0],quat[1],quat[2],arc);
      inputrot=rot2.getQuat();
      inputrot.normalize();
      if (get_zero_on_release(num_axis_angle))
         ret=inputrot;
      else 
         {
         if (firstflag[num_axis_angle]==true) 
            firstflag[num_axis_angle]=false;
         else
            if (local)
               ret=old_quat.conj()*inputrot;
            else 
               ret=inputrot*old_quat.conj();
         if (!check_only) 
            old_quat=inputrot;
         }
      SFRotation rot3(ret);
      const float *v=rot3.getValue();
      float value[4] = { v[0], v[1], v[2], v[3] };
      // limitation of axis make no sense for "wheel" type devices 
      if (!isTracker() && get_zero_on_release(num_axis_angle))
         {  
         if (tm->tdimension==TM_2D)
            {
            int index=min_position(value[0],value[1],value[2]);
            value[index]=0;
            }
         else if (tm->tdimension==TM_1D)
            {
            int index=max_position(value[0],value[1],value[2]);
            for (int i=0;i<3;i++)
               if (i!=index)
                  value[i]=0;
            }
         }
      SFRotation rot4(-value[0],value[1],value[2],value[3]);
      ret=rot4.getQuat();
      ret.normalize();
      }
   else 
      ret=inputrot;
   return ret;
   }

Quaternion& InputDevice::get_quaternion(TransformMode* tm, bool check_only)
   {
   return get_quaternion(tm, false, check_only);
   }

Quaternion& InputDevice::get_localQuaternion(TransformMode* tm, 
                                                    bool check_only)
   {
   return get_quaternion(tm, true, check_only);
   }

bool InputDevice::allzero(void)
   {
   if (useCurrent())
      return false;
   if ((get_x(NULL,true)==0) && (get_y(NULL,true)==0) && (get_z(NULL,true)==0)
       && (get_xrot(NULL,true)==0) 
       && (get_yrot(NULL,true)==0) 
       && (get_zrot(NULL,true)==0)
      )
      return true;
   else
      return false;
   }

// signswap of value from axis

int InputDevice::get_sign(int axis_number)
   {
   if ((sign_swap==NULL) || (axis_number>=number_max_axis))
      return 1;
   else
      if (sign_swap[axis_number])
         return -1;
      else
         return 1;
   }

void InputDevice::set_sign_swap(int axis_number,bool value)
   {
   if (axis_number<number_max_axis)
      {
      if (sign_swap==NULL)
         {
         sign_swap=new bool[number_max_axis];
         for (int i=0;i<number_max_axis;i++)
            sign_swap[i]=false;
         }
      sign_swap[axis_number]=value;
      }
   else
      swDebugf("axis number %d do not exist \n",axis_number);
   }

// factor: multiplied to the axis value returned from device
// deliver 1.0 if factor is not set

float InputDevice::get_factor(int axis_number)
   {
   if ((factor==NULL) || (axis_number>=number_max_axis))
      return 1.0;
   else
      return factor[axis_number];
   }

void InputDevice::set_factor(int axis_number,float value)
   {
   if (axis_number<number_max_axis)
      {
      if (factor==NULL)
         {
         factor=new float[number_max_axis];
         for (int i=0;i<number_max_axis;i++)
            factor[i]=1.0;
         }
      factor[axis_number]=value;
      }
   else
      swDebugf("axis number %d do not exist \n",axis_number);
   }

void InputDevice::set_factor(const char* string,float value)
   {
   for (int i=0;i<sizeof(axesinfo)/sizeof(char*);i++)
      if (stringncmp(string,axesinfo[i])==0)
         {
         set_factor(getAxisFromInformation(string),value);
         break;
         }
   if (stringncmp(string,"all")==0)
      {
      if (stringncmp(string,"allrot")!=0)
         xyzfactor=value;
      if (stringncmp(string,"allxyz")!=0)
         rotfactor=value;
      }
   }

// acceleration: additional acceleration for devices without acceleration
// deliver 1.0 (no acceleration) if acceleration is not set

float InputDevice::get_acceleration(int axis_number)
   {
   if ((acceleration==NULL) || (axis_number>=number_max_axis))
      return 1.0;
   else
      return acceleration[axis_number];
   }

void InputDevice::set_acceleration(int axis_number,float value)
   {
   if (axis_number<number_max_axis)
      {
      if (acceleration==NULL)
         {
         acceleration=new float[number_max_axis];
         for (int i=0;i<number_max_axis;i++)
            acceleration[i]=1.0;
         }
      acceleration[axis_number]=value;
      }
   else
      swDebugf("axis number %d do not exist \n",axis_number);
   }

void InputDevice::set_acceleration(const char* string,float value)
   {
   for (int i=0;i<sizeof(axesinfo)/sizeof(char*);i++)
      if (stringncmp(string,axesinfo[i])==0)
         {
         set_acceleration(getAxisFromInformation(string),value);
         break;
         }
   if (stringncmp(string,"all")==0)
      {
      if (stringncmp(string,"allrot")!=0)
         {
         set_acceleration(num_axis_x,value);
         set_acceleration(num_axis_y,value);
         set_acceleration(num_axis_z,value);
         }
      if (stringncmp(string,"allxyz")!=0)
         {
         set_acceleration(num_axis_xrot,value);
         set_acceleration(num_axis_yrot,value);
         set_acceleration(num_axis_zrot,value);    
         }
      }
   }

// return true if axis number deliver zero when device is released

bool InputDevice::get_zero_on_release(int axis_number)
   {
   if ((zero_on_release==NULL) || (axis_number>=number_max_axes))
      return false;
   else
      return zero_on_release[axis_number];
   }

void InputDevice::set_zero_on_release(int axis_number,bool value)
   {
   if (axis_number<number_max_axes)
      {
      if (zero_on_release==NULL)
         {
         zero_on_release=new bool[number_max_axes];
         for (int i=0;i<number_max_axes;i++)
            zero_on_release[i]=true;
         }
      zero_on_release[axis_number]=value;
      }
   else
      swDebugf("axis number %d do not exist \n",axis_number);
   }

void InputDevice::set_zero_on_release(const char* string,bool value)
   {
   for (int i=0;i<sizeof(axesinfo)/sizeof(char*);i++)
      if (stringncmp(string,axesinfo[i])==0)
         {
         set_zero_on_release(getAxisFromInformation(string),value);
         break;
         }
   if (stringncmp(string,"all")==0)
      {
      if (stringncmp(string,"allrot")!=0)
         {
         set_zero_on_release(num_axis_x,value);
         set_zero_on_release(num_axis_y,value);
         set_zero_on_release(num_axis_z,value);
         }
      if (stringncmp(string,"allxyz")!=0)
         {
         set_zero_on_release(num_axis_xrot,value);
         set_zero_on_release(num_axis_yrot,value);
         set_zero_on_release(num_axis_zrot,value);    
         set_zero_on_release(num_axis_angle,value);    
         }
      }
   }


// return value (relative to max) which will be ignored

float InputDevice::get_zero_size_fraction(int axis_number)
   {
   if ((zero_size_fraction==NULL) || (axis_number>=number_max_axis))
      return 0;
   else
      return zero_size_fraction[axis_number];
   }

void InputDevice::set_zero_size_fraction(int axis_number,float value)
   {
   if (axis_number<number_max_axis)
      {
      if (zero_size_fraction==NULL)
         {
         zero_size_fraction=new float[number_max_axis];
         for (int i=0;i<number_max_axis;i++)
            zero_size_fraction[i]=0;
         }
      zero_size_fraction[axis_number]=value;
      }
   else
      swDebugf("axis number %d do not exist \n",axis_number);
   }

void InputDevice::set_zero_size_fraction(const char* string,float value)
   {
   for (int i=0;i<sizeof(axesinfo)/sizeof(char*);i++)
      if (stringncmp(string,axesinfo[i])==0)
         {
         set_zero_size_fraction(getAxisFromInformation(string),value);
         break;
         }
   if (stringncmp(string,"all")==0)
      {
      if (stringncmp(string,"allrot")!=0)
         {
         if (num_axis_x < get_number_max_axis())
            set_zero_size_fraction(num_axis_x,value);
         if (num_axis_y < get_number_max_axis())
            set_zero_size_fraction(num_axis_y,value);
         if (num_axis_z < get_number_max_axis())
            set_zero_size_fraction(num_axis_z,value);
         }
      if (stringncmp(string,"allxyz")!=0)
         {
         if (num_axis_xrot < get_number_max_axis())
            set_zero_size_fraction(num_axis_xrot,value);
         if (num_axis_yrot < get_number_max_axis())
            set_zero_size_fraction(num_axis_yrot,value);
         if (num_axis_zrot < get_number_max_axis())
            set_zero_size_fraction(num_axis_zrot,value);    
         }
      }
   }

int InputDevice::getAxisFromInformation(const char* axesinfo)
   {
   int axis_number;

   if      (strcmp(axesinfo,"xrot")==0)
      axis_number=num_axis_xrot;
   else if (strcmp(axesinfo,"yrot")==0)
      axis_number=num_axis_yrot;
   else if (strcmp(axesinfo,"zrot")==0)
      axis_number=num_axis_zrot;
   else if (strcmp(axesinfo,"x")==0)
      axis_number=num_axis_x;
   else if (strcmp(axesinfo,"y")==0)
      axis_number=num_axis_y;
   else if (strcmp(axesinfo,"z")==0)
      axis_number=num_axis_z;
   else 
      {
      swDebugf("axesinfo %s not understood, using axis 0\n",axesinfo);
      axis_number=0;
      }
   return axis_number;
   }

void InputDevice::setAxisOfInformation(const char* axesinfo,int axis_number)
   {
   if      (strcmp(axesinfo,"xrot")==0)
      num_axis_xrot=axis_number;
   else if (strcmp(axesinfo,"yrot")==0)
      num_axis_yrot=axis_number;
   else if (strcmp(axesinfo,"zrot")==0)
      num_axis_zrot=axis_number;
   else if (strcmp(axesinfo,"x")==0)
      num_axis_x=axis_number;
   else if (strcmp(axesinfo,"y")==0)
      num_axis_y=axis_number;
   else if (strcmp(axesinfo,"z")==0)
      num_axis_z=axis_number;
   else if (strcmp(axesinfo,"none")==0) 
      {
      ignore[axis_number]=true;
      number_ignored_axes++;
      }
   }

// destruktive way to cut out the string till next comma
// return index to the comma position or -1 if no comma position available

static int cut_out_till_next_comma(char* string)
   {
   char* comma;
   // search for comma
   comma=strchr(string,',');
   if (comma==NULL)
      return -1;
   else
      {
      // end of string is placed at comma position          
      comma[0]=(char)0;
      return(comma-string+1);
      }
   }

static bool string2int(int &value,const char* string)
   {
   int characters_read;
   
   sscanf(string,"%d%n",&value,&characters_read);
   if (characters_read!=strlen(string))
      return(false);
   return(true);
   }

static bool string2float(float &value,const char* string)
   {
   int characters_read;
   
   sscanf(string,"%f%n",&value,&characters_read);
   if (characters_read!=strlen(string))
      return(false);
   return(true);
   }

bool InputDevice::argument_is_axis(const char* string)
   {
   for (int i=0;i<sizeof(axesinfo)/sizeof(char*);i++)
      if (stringncmp(string,axesinfo[i])==0)
         return true;
   if (stringncmp(string,"none")==0)
      return true;
   return false;
   }   

/*
 * parse argumentstring 
 * valid syntax is
 *
 * -x|-y|-z|-xrot|-yrot|-zrot=[-][integer_axisnumber][,[factor][,[accel][,[wheel][,ignore]]]]
 * -all|-allxyz|-allrot=[factor][,[accel][,[wheel][,ignore]]]
 * -none=integer_axisnumber
 *
 * -                  sign of value from axis is swapped
 *
 * integer_axisnumber is a integer with the number of the axis, that should 
 *                    be used for the x y z xrot yrot zrot directions.
 *                    This number may not be greater than the number of
 *                    axes of the inputdevice
 *
 * factor is a float with a multiplicator for the axes
 *
 * accel is a float with a expotential accelerator for the axes
 *
 * wheel is the string "wheel"
 *       wheel means this axis of the inputdevice will not deliver zero 
 *       if released
 *
 * ignore is a float with the value (relative to the maximal value
 *        from the device) which will be ignored (insensitivity)
 *  
 * function returns always true    
 */

bool InputDevice::scan_argument(char *string,const char* argument) 
   {
   int next;
   bool sign_swap=false;
   // skip argument
   string=string+strlen(argument);

   // skip '='
   if (string[0]=='=')
      string++;

   // read sign of axis
   if (string[0]=='-')
      {
      sign_swap=true;
      string++;
      }

   if (argument_is_axis(argument))
      {
      // read axisnumber
      next=cut_out_till_next_comma(string);
      if (next!=1)
         {
         int axis_number;
         if (!string2int(axis_number,string))
            {
            swDebugf("axisnumber %s is not a integer\n",string);
            return true;
            }
         setAxisOfInformation(argument,axis_number);
         if (strcmp(argument,"none")==0)
             return true;
         set_sign_swap(axis_number,sign_swap);
         if (next==-1)
            return true;
         }
      // position after comma
      string=string+next;
      }

   // read factor
   next=cut_out_till_next_comma(string);
   if (next!=1)
      {
      int characters_read;
      float fact;
      sscanf(string,"%f%n",&fact,&characters_read);
      if (characters_read!=strlen(string))
         {
         swDebugf("factor %s is not a float\n",string);
         return true;
         }
      else
         {
         set_factor(argument,fact);
         if (next==-1)
            return true;
         }
      }
   // position after comma
   string=string+next;

   // read acceleration
   next=cut_out_till_next_comma(string);
   if (next!=1)
      {
      int characters_read;
      float accel;
      sscanf(string,"%f%n",&accel,&characters_read);
      if (characters_read!=strlen(string))
         {
         swDebugf("acceleration %s is not a float\n",string);
         return true;
         }
      else
         {
         set_acceleration(argument,accel);
         if (next==-1)
            return true;
         }
      }
   // position after comma
   string=string+next;

   // wheel/zero_on_release information
   next=cut_out_till_next_comma(string);
   if (next!=1)
      {
      if (strcmp(string,"wheel")==0)
         set_zero_on_release(argument,false);
      else
         {
         swDebugf("%s is not the string \"wheel\"\n",string);
         return true;
         }
      if (next==-1)
         return true;
      } else
         set_zero_on_release(argument,true);

   // position after comma
   string=string+next;

   // read acceleration
   next=cut_out_till_next_comma(string);
   if (next==-1)
       return true;
   if (next!=1)
      {
      int characters_read;
      float ignore;
      sscanf(string,"%f%n",&ignore,&characters_read);
      if (characters_read!=strlen(string))
         {
         swDebugf("zero_size_fraction %s is not a float\n",string);
         return true;
         }
      else
         {
         set_zero_size_fraction(argument,ignore);
         if (next==-1)
            return true;
         }
      }
   // position after comma
   string=string+next;

   swDebugf("argument %s not understood\n",string);
   return true;
   }

void InputDevice::setAxisInformation(const char* info_string)
   {
   char *string2free=strdup(info_string);
   char *string=string2free;
   if (string[0]=='-') 
      string++;

   bool found=false;
   for (int i=0;i<sizeof(axesinfo)/sizeof(char*);i++)
      {
      if (stringncmp(string,axesinfo[i])==0)
         {
         found=scan_argument(string,axesinfo[i]);
         break;
         }
      }
   if (stringncmp(string,"all")==0) 
      {
      if (stringncmp(string,"allrot")==0)
         found=scan_argument(string,"allrot");
      else if (stringncmp(string,"allxyz")==0)
         found=scan_argument(string,"allxyz");
      else 
         found=scan_argument(string,"all");
      } 
   else if (stringncmp(string,"none")==0) 
      found=scan_argument(string,"none");

   if (!found)
      {   
      swDebugf("%s do not contain",info_string);
      for (int i=0;i<sizeof(axesinfo)/sizeof(char*);i++)
         swDebugf(" -%s or",axesinfo[i]);
      swDebugf(" -all=wheel\n");
      }
   free(string2free);
   return;
   }

// set number of axes
// only valid to reduce axes from a bad designed joystick

void InputDevice::setNumberAxes(const char* argument)
   {
   const char* string=argument;

   if (stringncmp(string,"-axes")!=0)
      {
      swDebugf("internal error in InputDevice::SetNumberAxes\n");
      return;
      }
   string=string+strlen("-axes");

   if (string[0]=='=') 
      string++;

   int new_number_axes;
   if (string2int(new_number_axes,string)) 
      {  
      if (new_number_axes<=number_max_axis)
         number_max_axis=new_number_axes;
      else
         swDebugf("only %d axes found, can not decrease to %d\n",
                 number_max_axis,new_number_axes);
      }
   return;      
   }


static bool checkUnixDevice(const char *device)
   {
   struct stat inode_info;
   if (stat(device,&inode_info)==0)
#ifndef _WIN32
       if (S_ISCHR(inode_info.st_mode) || S_ISBLK(inode_info.st_mode))
#endif
           return true;
   return false;
   }

static void loopAddUnixDevices(StringArray *names, const char *device_base,
                               int device_offset=0)
   {
   char device[128];
   int offset=device_offset;
   do 
      {
      mysnprintf(device,127,"%s%d",device_base,offset);
      if (checkUnixDevice(device)) 
         {
         MyString append;
         append+=device;
         names->append(append);
         }
      offset++;        
      } while (checkUnixDevice(device));
   }

static void appendNumbers(StringArray *names, int one2three=3)
   {
   names->resize(0);

   MyString append;
   append="";
   append+="0";
   names->append(append);

   if (one2three > 1) 
      {       
      append="";
      append+="1";
      names->append(append);
      }

   if (one2three > 2) 
      {       
      append="";
      append+="2";
      names->append(append);
      }
}

// Drivers for joystick-like devices

// Linux Joystick driver of Linux 2.2 or Linux 2.4

/*
 * based on jstest.c  Version 1.2
 *
 * Copyright (c) 1996-1999 Vojtech Pavlik
 *
 * Sponsored by SuSE
 *
 * modified by J. "MUFTI" Scheurich 2001-2002 for white_dune
 * This program can be used to test [..] the features of the Linux
 * joystick API, including non-blocking [..] access [..]. 
 * It is also intended to
 * serve as an example implementation for those who wish to learn
 * how to write their own joystick using applications.
 */

#ifdef LINUX_JOYSTICK
 
# include <sys/ioctl.h>
# include <sys/time.h>
# include <sys/types.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>  

/*
 * initialise joystick 
 * argument "device" is the joystickdevice, eg. "/dev/input/js0" or "/dev/js0" 
 */
 
linux_joystick::linux_joystick(const char *device) : InputDevice(device)
   {
   if (device == NULL)
       return;

   if ((fd = open(device, O_RDONLY)) < 0) 
      {
      valid = false;
      char msg[1024]; 
      mysnprintf(msg, 1024, 
                 "joystickdevice %s joystick initialisation failed: %s", 
                 device, strerror(errno));
      error_message += msg;
      }
   valid=true;   

   linux_js_factor=0.1;
   unsigned char axes;
   unsigned char buttons;
   number_max_axis = 2;
   number_buttons = 2;
   version = 0x000800;
   strcpy(name,"Unknown");

   if (valid) {
       ioctl(fd, JSIOCGVERSION, &version);
       ioctl(fd, JSIOCGAXES, &axes);
       number_max_axis = axes;
       ioctl(fd, JSIOCGBUTTONS, &buttons);
       number_buttons = buttons;
       ioctl(fd, JSIOCGNAME(NAME_LENGTH), name);
       fcntl(fd, F_SETFL, O_NONBLOCK);
   }

   if (number_buttons>0)
      { 
      button=new bool[number_buttons];
      for (int i=0;i<number_buttons;i++)
         button[i]=false;
      }
 
# ifdef DEBUG
   swDebugf("Joystick (%s) has %d axes and %d buttons. Driver version is %d.%d.%d.\n",
            name, number_max_axis, number_buttons, version >> 16, (version >> 8) & 0xff, 
            version & 0xff);
# endif
   }

linux_joystick::~linux_joystick()
   {
   if (valid)
      close(fd);
   }

void 
linux_joystick::guessInputDeviceNames(StringArray *names) const
   {
   names->resize(0);
   loopAddUnixDevices(names,"/dev/input/js");
   loopAddUnixDevices(names,"/dev/js");
   }

bool linux_joystick::readInputDevice(void)
   {
   if (!valid)
       return false;
   bool ret=false;
   button_pressed=-1;
   button_released=-1;
   while (read(fd, &js, sizeof(struct js_event)) == sizeof(struct js_event))  
      {
# ifdef DEBUG
       swDebugf("Event: type %d, time %d, number %d, value %d\n",
                js.type, js.time, js.number, js.value);
# endif
      switch(js.type & ~JS_EVENT_INIT) 
         {
         case JS_EVENT_BUTTON:
            ret=true;
            if (js.value)
               {
               button[js.number] = true;
               button_pressed=js.number;
               }
            else
               {
               button[js.number] = false;
               button_released=js.number;
               }
            break;
         case JS_EVENT_AXIS:
            ret=true;
            if (js.number==num_axis_x)
               value[0] =  js.value/maxvalue(js.number)*linux_js_factor;
            if (js.number==num_axis_y)
               value[1] = -js.value/maxvalue(js.number)*linux_js_factor;
            if (js.number==num_axis_z)
               value[2] = -js.value/maxvalue(js.number)*linux_js_factor;
            if (js.number==num_axis_xrot)
               value[3] =  js.value/maxvalue(js.number)*linux_js_factor;
            if (js.number==num_axis_yrot)
               value[4] =  js.value/maxvalue(js.number)*linux_js_factor;
            if (js.number==num_axis_zrot)
               value[5] =  js.value/maxvalue(js.number)*linux_js_factor;
            break;
         }
      if ((button_pressed!=-1) || (button_released!=-1))
         break;
      }
   if (ret==false)
      if ((value[0]!=0) || (value[1]!=0) || (value[2]!=0) || 
          (value[3]!=0) || (value[4]!=0) || (value[5]!=0))
         {
         // deliver constantly values
         ret=true;      
         }
   return ret;
   }
 
# ifdef TEST_JOYSTICK
int main(int argc,char** argv)
   {
   linux_joystick js=linux_joystick("/dev/input/js0");
   while(1)
      if (js.readInputDevice())
         printf("%f %f %f\n",js.get_x(),js.get_y(),js.get_z());
   }
# endif
#endif


#ifdef DIRECTX_JOYSTICK
 
# include <Windows.h>  

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, 
                                    VOID* pContext );

/*
 * initialise joystick 
 * argument "device" is the number of the joystick, eg. "0", "1" or "2"
 */

windows_joystick::windows_joystick(const char *device) : InputDevice(device)
   {
   if (device == NULL)
       return;

   int characters_read;
   sscanf(device,"%d%n",&number_of_joystick,&characters_read);
   
   if ((characters_read!=strlen(device)) || 
       (number_of_joystick>15) || (number_of_joystick<0)) 
      {
      valid = false
      char msg[1024]; 
      mysnprintf(msg, 1024, "no joystick number \"%s\"",device);
      error_message += msg;
      }
 
   if ( FAILED( InitDirectInput() ) ) 
      {
      valid = false
      char msg[1024]; 
      mysnprintf(msg, 1024, 
                 "joystick number \"%s\" not present or error", device);
      error_message += msg;
      }

   DIDEVCAPS lpDIDevCaps;
   lpDIDevCaps.dwSize=sizeof(DIDEVCAPS);
   g_pJoystick->GetCapabilities(&lpDIDevCaps);

   factor=1;
   rotfactor *= 0.1;
   number_max_axis=6;
   if (number_max_axis>0) 
      { 
      max_value=new int[6];
      null_value=new int[6];

      for (int i=0;i<6;i++) 
         {                   
         max_value[i]=65535;
         null_value[i]=32768;
         }
      }

   number_buttons = lpDIDevCaps.dwButtons;

   DIDEVICEINSTANCE pdidi;
   pdidi.dwSize=sizeof(DIDEVICEINSTANCE);
   g_pJoystick->GetDeviceInfo(&pdidi);
   
   strcpy(name,pdidi.tszProductName);

   DIPROPDWORD dipdw;
   dipdw.diph.dwSize = sizeof(DIPROPDWORD);
   dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
   dipdw.diph.dwObj = 0;
   dipdw.diph.dwHow = DIPH_DEVICE;

   g_pJoystick->GetProperty(DIPROP_VIDPID,&dipdw.diph);

   version = HIWORD(dipdw.dwData);

///////////////////////////////////////////////////////////////////////////77

//   version = joycapabilities.wPid;//?????????????????????????????????????????

/////////////////////////////////////////////////////////////////////////////

   if (number_buttons>0)
      { 
      button=new bool[number_buttons];
      for (int i=0;i<number_buttons;i++)
         button[i]=false;
      }
 
   // ignore first read of joystick....
   //  result=joyGetPosEx(number_of_joystick, &joyinfo_ex);

# ifdef DEBUG
   swDebugf("Joystick (%s) has %d axes and %d buttons. Driver version is %d.%d.%d.\n",
            name, number_max_axis, number_buttons, version >> 16, (version >> 8) & 0xff, 
            version & 0xff);
# endif
   }

HRESULT windows_joystick::InitDirectInput()
{
    HRESULT hr;
    if ( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), 
                                          DIRECTINPUT_VERSION, 
                                          IID_IDirectInput8, 
                                          (VOID**)&g_pDI, NULL ) ) )
        return hr;

    // Look for a simple joystick we can use for this sample program.
    if ( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
                                          EnumJoysticksCallback,
                                          NULL, DIEDFL_ATTACHEDONLY ) ) )
        return hr;

    // Make sure we got a joystick
    if ( NULL == g_pJoystick )
    {
        MessageBox( NULL, TEXT("Joystick not found."),
                    TEXT("DirectInput error"), SW_MB_ICONERROR | SW_MB_OK );
       
        return S_OK;
    }
    // Set the data format to "simple joystick" 
    if ( FAILED( hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick2 ) ) )
        return hr;

    // Would // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
    //   if( FAILED( hr = g_pJoystick->SetCooperativeLevel( NULL/*hier hwnd*/, DISCL_EXCLUSIVE | 
    //                                                            DISCL_FOREGROUND ) ) )
    //       return hr;
  
    DIPROPRANGE diprg; 
    diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
    diprg.diph.dwHow        = DIPH_DEVICE;//DIPH_BYID; 
    diprg.diph.dwObj        = 0;
    //pdidoi->dwType; // Specify the enumerated axis
    diprg.lMin              = 0; 
    diprg.lMax              = +65535; 
    
    if ( FAILED( hr = g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
        return hr;

    return S_OK;
}


windows_joystick::~windows_joystick()
   {
   }

void 
windows_joystick::guessInputDeviceNames(StringArray *names) const
   {
   appendNumbers(names);
   }

BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{
    HRESULT hr;

    // Obtain an interface to the enumerated joystick.
    hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, NULL );

    // If it failed, then we can't use this joystick. (Maybe the user unplugged
    // it while we were in the middle of enumerating it.)
    if( FAILED(hr) ) 
        return DIENUM_CONTINUE;

    // Stop enumeration. Note: we're just taking the first joystick we get. You
    // could store all the enumerated joysticks and let the user pick.
    return DIENUM_STOP;
}


bool windows_joystick::readInputDevice(void)
   {
   if (NULL == g_pJoystick) 
      return false;
   
   if (FAILED(g_pJoystick->Poll()))
      {
      HRESULT hr;hr = g_pJoystick->Acquire();
      while ( hr == DIERR_INPUTLOST ) 
          hr = g_pJoystick->Acquire();
      return false;
      }
   if (! FAILED( g_pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &js ) ))
      {
      if (number_buttons>0)
         for (int i = 0; i < 128; i++ )
            {
            if ( js.rgbButtons[i] & 0x80 )
                button[i] = true;
            }
            
      for (int i=0;i<6;i++)
         value[i]=0;
      value[0] =  (float)((int)js.lX-null_value[0])/maxvalue(0)*factor;
      value[1] = -(float)((int)js.lY-null_value[1])/maxvalue(1)*factor;
      value[2] =  (float)((int)js.lZ-null_value[2])/maxvalue(2)*factor;
      value[3] =  (float)((int)js.rglSlider[0]-null_value[3])/maxvalue(3)*factor;
      value[4] =  (float)((int)js.lRz-null_value[4])/maxvalue(4)*factor;
      value[5] =  (float)((int)js.rgdwPOV[0]-null_value[5])/maxvalue(5)*factor;
   }
   else
      return false;

   if ((value[0]!=0) || (value[1]!=0) || (value[2]!=0) || 
       (value[3]!=0) || (value[4]!=0) || (value[5]!=0))
      return true;      
   return false;
   }
 
# ifdef TEST_JOYSTICK
int main(int argc,char** argv)
   {
   windows_joystick js=windows_joystick("0");
   while(1)
      if (js.readInputDevice())
         printf("%f %f %f\n",js.get_x(),js.get_y(),js.get_z());
   }
# endif
#endif


#ifdef WINDOWS_JOYSTICK
 
# include <Windows.h>  
# include "swt.h"
/*
 * initialise joystick 
 * argument "device" is the number of the joystick, eg. "0", "1" or "2"
 */

windows_joystick::windows_joystick(const char *device) : InputDevice(device)
   {
   if (device==NULL)
       return;

   valid=true;
   device_name=device;
   int characters_read;
   sscanf(device,"%d%n",&number_of_joystick,&characters_read);
   if ((characters_read!=strlen(device)) || 
       (number_of_joystick>15) || (number_of_joystick<0))
      {
      valid=false;
      char msg[1024]; 
      mysnprintf(msg,1024,"no joystick number \"%s\"",device);
      error_message+=msg;
      return;
      }
   MMRESULT result;
   result=joyGetDevCaps(number_of_joystick,&joycapabilities,sizeof(JOYCAPS));
   if (result!=JOYERR_NOERROR)
      {
      valid=false;
      char msg[1024]; 
      mysnprintf(msg,1024,"joystick number \"%s\" not present or error",device);
      error_message+=msg;
      return;
      }

   windows_js_factor=1;
   rotfactor*=0.1;
   if (valid) 
       number_max_axis=joycapabilities.wNumAxes;
   else
       number_max_axis=0;
   if (number_max_axis>0)
      { 
      maximum=new int[number_max_axis];
      null_value=new int[number_max_axis];
      for (int i=0;i<number_max_axis;i++)
         {
         switch(i)
            {
            case 0:           
               maximum[i]=joycapabilities.wXmax;
               null_value[i]=(maximum[i]-joycapabilities.wXmin)/2;
               break;
            case 1:           
               maximum[i]=joycapabilities.wYmax;
               null_value[i]=(maximum[i]-joycapabilities.wYmin)/2;
               break;
            case 2:           
               maximum[i]=joycapabilities.wZmax;
               null_value[i]=(maximum[i]-joycapabilities.wZmin)/2;
               break;
            case 3:           
               maximum[i]=joycapabilities.wRmax;
               null_value[i]=(maximum[i]-joycapabilities.wRmin)/2;
               break;
            case 4:           
               maximum[i]=joycapabilities.wUmax;
               null_value[i]=(maximum[i]-joycapabilities.wUmin)/2;
               break;
            case 5:           
               maximum[i]=joycapabilities.wVmax;
               null_value[i]=(maximum[i]-joycapabilities.wVmin)/2;
               break;
            }
         }
      }

   if (valid) {
       number_buttons = joycapabilities.wMaxButtons;
       version = joycapabilities.wPid;
       strcpy(name,joycapabilities.szPname);
   }

   if (number_buttons>0)
      { 
      button=new bool[number_buttons];
      for (int i=0;i<number_buttons;i++)
         button[i]=false;
      }
 
   // ignore first read of joystick....
   result=joyGetPosEx(number_of_joystick, &joyinfo_ex);


# ifdef DEBUG
   swDebugf("Joystick (%s) has %d axes and %d buttons. Driver version is %d.%d.%d.\n",
            name, number_max_axis, number_buttons, version >> 16, (version >> 8) & 0xff, 
            version & 0xff);
# endif
   }

windows_joystick::~windows_joystick()
   {
   }

void 
windows_joystick::guessInputDeviceNames(StringArray *names) const
   {
   appendNumbers(names);
   }

bool windows_joystick::readInputDevice(void)
   {
   joyinfo_ex.dwSize=sizeof(JOYINFOEX);
//   joyinfo_ex.dwFlags=JOY_RETURNCENTERED ;// | JOY_RETURNBUTTONS; 
   joyinfo_ex.dwFlags=JOY_RETURNALL | JOY_RETURNCENTERED; // | JOY_RETURNBUTTONS; 
  
   MMRESULT result=joyGetPosEx(number_of_joystick, &joyinfo_ex);

   if (result == JOYERR_NOERROR)
      {
      if (number_buttons>0)
         for (int i=0;i<number_buttons;i++)
            button[i]=false;
      if (joyinfo_ex.dwFlags && JOY_RETURNBUTTONS)
         if (joyinfo_ex.dwButtons != 0)
            button[joyinfo_ex.dwButtonNumber] = true;
      for (int i=0;i<6;i++)
          value[i]=0;
      value[0] =   (float)((int)joyinfo_ex.dwXpos-null_value[0])/maxvalue(0)*
                   windows_js_factor;
      value[1] = - (float)((int)joyinfo_ex.dwYpos-null_value[1])/maxvalue(1)*
                   windows_js_factor;
      if (joycapabilities.wCaps & JOYCAPS_HASZ)
         value[2] = - (float)((int)joyinfo_ex.dwZpos-null_value[2])/maxvalue(2)*
                      windows_js_factor;
      if (joycapabilities.wCaps & JOYCAPS_HASR)
         value[3] =   (float)((int)joyinfo_ex.dwRpos-null_value[3])/maxvalue(3)*
                      windows_js_factor;
      if (joycapabilities.wCaps & JOYCAPS_HASU)
         value[4] =   (float)((int)joyinfo_ex.dwUpos-null_value[4])/maxvalue(4)*
                      windows_js_factor;
      if (joycapabilities.wCaps & JOYCAPS_HASV)
         value[5] = - (float)((int)joyinfo_ex.dwVpos-null_value[5])/maxvalue(5)*
                      windows_js_factor;
      }
   else
      return false;
   if ((value[0]!=0) || (value[1]!=0) || (value[2]!=0) || 
       (value[3]!=0) || (value[4]!=0) || (value[5]!=0))
      return true;      
   return false;
   }
 
# ifdef TEST_JOYSTICK
int main(int argc,char** argv)
   {
   windows_joystick js=windows_joystick("0");
   while(1)
      if (js.readInputDevice())
         printf("%f %f %f\n",js.get_x(),js.get_y(),js.get_z());
   }
# endif
#endif

#ifdef HAVE_SDL_JOYSTICK

/*
 * initialise joystick 
 * argument "device" is the number of the joystick, eg. "0", "1", "2", ...
 */
 
SDL_joystick::SDL_joystick(const char *device) : InputDevice(device)
   {
   if (device == NULL)
       return;

   int characters_read;
   sscanf(device,"%d%n",&number_of_joystick,&characters_read);
   js = NULL;
   if (SDL_JoystickInit() != 0)
      {
      valid = false;
      error_message += "joystick initialisation failed";
      return;
      }
   if (number_of_joystick >= SDL_NumJoysticks())
      {
      valid = false;
      char msg[1024]; 
      mysnprintf(msg, 1024, "joystickdevice %s only %d joysticks", device,
                 SDL_NumJoysticks());
      error_message += msg;
      return;
      }

   js = NULL;
   if (valid)
       js = SDL_JoystickOpen(number_of_joystick);
   sdl_factor = 1;
   rotfactor *= 0.1;
   if (valid)
       number_max_axis = SDL_JoystickNumAxes(js);
   else
       number_max_axis = 2;
   int array_size = number_max_axis < 6 ? 6 : number_max_axis;
   maximum=new int[array_size];
   null_value=new int[array_size];
   for (int i=0;i<array_size;i++)
      {
      maximum[i]=256*256;
      null_value[i]=0;
      }
   number_buttons = 0;
   }

SDL_joystick::~SDL_joystick()
   {
   if (js != NULL)
      SDL_JoystickClose(js);
   }

void 
SDL_joystick::guessInputDeviceNames(StringArray *names) const
   {
   appendNumbers(names);
   }

bool SDL_joystick::readInputDevice(void)
   {
   bool ret=false;

   SDL_JoystickUpdate();
   if (num_axis_x < number_max_axis)
      value[0] = ((float)SDL_JoystickGetAxis(js, num_axis_x)) /
                 maxvalue(num_axis_x) * sdl_factor;
   if (num_axis_y < number_max_axis)
      value[1] = ((float)SDL_JoystickGetAxis(js, num_axis_y)) / 
                 maxvalue(num_axis_y) * sdl_factor;
   if (num_axis_z < number_max_axis)
      value[2] = ((float)SDL_JoystickGetAxis(js, num_axis_z)) /
                 maxvalue(num_axis_z) * sdl_factor;

   if (num_axis_xrot < number_max_axis)
      value[3] =  ((float)SDL_JoystickGetAxis(js, num_axis_xrot)) /
                  maxvalue(num_axis_xrot) * sdl_factor;
   if (num_axis_yrot < number_max_axis)
      value[4] =  ((float)SDL_JoystickGetAxis(js, num_axis_yrot)) / 
                  maxvalue(num_axis_yrot) * sdl_factor;
   if (num_axis_zrot < number_max_axis)
      value[5] =  ((float)SDL_JoystickGetAxis(js, num_axis_zrot)) /
                  maxvalue(num_axis_zrot) * sdl_factor;

   if ((value[0]!=0) || (value[1]!=0) || (value[2]!=0) || 
       (value[3]!=0) || (value[4]!=0) || (value[5]!=0))
      {
      // deliver constantly values
      ret=true;      
      }
   return ret;
   }
#endif

// Xinput driver
 
#ifdef HAVE_XINPUT

 /*
  * initialise XInput 
  * argument "device" is the name of the device, eg. "magellan" 
  */

xinput::xinput(const char *device) : InputDevice(device)
   {
   if (device==NULL)
       return;

   number_buttons=0;
   number_max_axis=0;
   xinput_factor=1;
   maximum=NULL;

   swxinput=(swXinput*) malloc(sizeof(swXinput));
   swInitXinputDevice(swxinput,device);

   if (swxinput->xinput==NULL)
      {
      char msg[1024]; 
      mysnprintf(msg,1024,"failed to initialize xinput device %s",device);
      error_message+=msg;
      free(swxinput);
      swxinput=NULL;
      valid=false;
      return;
      }

   if (swxinput)
       number_buttons=swxinput->number_buttons;
   else
       number_buttons = 0;
   if (number_buttons>0)
      { 
      button=new bool[number_buttons];
      for (int i=0;i<number_buttons;i++)
         button[i]=false;
      }

   if (swxinput)
       number_max_axis=swxinput->number_axes;
   else
       number_max_axis=0;

   if (number_max_axis>0)
      { 
      int array_size = number_max_axis < 7 ? 7 : number_max_axis;
      maximum=new float[array_size];
      for (int i=0;i<number_max_axis;i++)
         maximum[i]=swxinput->maxvalue[i];
      for (int i=number_max_axis;i<7;i++)
         maximum[i]=1;
      }

   name=(char *)"Unknown";
  
#  ifdef DEBUG
   swDebugf("Xinputdevice %s has %d axes and %d buttons.\n",
            device, number_max_axis, number_buttons);  
#  endif
   }
 
xinput::~xinput(void)
   {
   swCloseXinputDevice(swxinput);
   free(swxinput);
   }

void 
xinput::guessInputDeviceNames(StringArray *names) const
   {
   names->resize(0);
   MyString append;
   for (int i=0;i<swGetNumXinputDevice();i++)
      {
      append="";
      append+=swGetXinputDevice(i);
      names->append(append);
      }
   }

bool xinput::readInputDevice(void)
   {
   button_pressed=-1;
   button_released=-1;

   if (swxinput==NULL)
      return false;
   for (int i=0;i<number_buttons;i++)
      button[i]=false;
    
   swQueryXinputDevice(swxinput);
   if (swxinput->xinput==NULL)
      return false;

   for (int j=0;j<7;j++)   
      value[j] = 0;  

   if (num_axis_x < number_max_axis)
      value[0]= xinput_factor*float(swxinput->axes[num_axis_x])/
                              swxinput->maxvalue[num_axis_x];
   if (num_axis_y < number_max_axis)
      value[1]= xinput_factor*float(swxinput->axes[num_axis_y])/
                                    swxinput->maxvalue[num_axis_y];
   if (num_axis_z < number_max_axis)
      value[2]=-xinput_factor*float(swxinput->axes[num_axis_z])/
                              swxinput->maxvalue[num_axis_z];

   if (num_axis_xrot < number_max_axis)
      value[3]= xinput_factor*float(swxinput->axes[num_axis_xrot])/
                              swxinput->maxvalue[num_axis_xrot];
   if (num_axis_yrot < number_max_axis)
      value[4]= xinput_factor*float(swxinput->axes[num_axis_yrot])/
                              swxinput->maxvalue[num_axis_yrot];
   if (num_axis_zrot < number_max_axis)
      value[5]= xinput_factor*float(swxinput->axes[num_axis_zrot])/
                              swxinput->maxvalue[num_axis_zrot];
   if (num_axis_angle < number_max_axis)
      value[6]= xinput_factor*float(swxinput->axes[num_axis_angle])/
                              swxinput->maxvalue[num_axis_angle];

   return true;
   } 
#endif

// Driver for SpaceTec/LabTec Spaceball and clones.

#ifdef HAVE_LIBSBALL

/*
 * initialise spaceball 
 * argument "device" is the spaceballdevice, eg. "/dev/ttyd2" or "/dev/ttyS0" 
 */
 
spaceball::spaceball(const char *device) : InputDevice(device)
   {
   if (device == NULL)
       return;

   sball = NULL;
   if ((sball = sball_open((char *)device)) == NULL)
      {
      char msg[1024]; 
      mysnprintf(msg,1024,"spaceballdevice %s initialisation failed",device);
      error_message+=msg;
      valid=false;
      }
 
   if (valid)
       sball_init(sball);
   nullsize = 0;
   maximum = 32767.0;
   sball_factor = 1.0;

   if (valid)
       sball_set_nullregion(sball, nullsize, nullsize, nullsize, 
                                   nullsize, nullsize, nullsize);

   number_max_axis = 6;
   number_buttons = 9;
   name = "Unknown Spaceball";

   if (number_buttons>0)
      { 
      button = new bool[number_buttons];
      for (int i = 0; i < number_buttons; i++)
         button[i] = false;
      }
 
# ifdef DEBUG
   swDebugf("InputDevice (%s) has %d axes and %d buttons. Driver ???\n",
            name, number_max_axis, number_buttons);  
# endif
   }
 
spaceball::~spaceball(void)
   {
   sball_close(sball);
   }

void 
spaceball::guessInputDeviceNames(StringArray *names) const
   {
   names->resize(0);
#ifdef SERIAL_DEVICE
# ifdef OFFSET_SERIAL_DEVICE
   loopAddUnixDevices(names,SERIAL_DEVICE,OFFSET_SERIAL_DEVICE);
# endif
#endif
   loopAddUnixDevices(names,"/dev/ttyUSB");
   }

bool spaceball::readInputDevice(void)
   {
   bool ret=false;
   button_pressed=-1;
   button_released=-1;
   int buttons;
   int sx,sy,sz;
   int sxrot,syrot,szrot;
 
   if (sball==NULL)
      return false;
   for (int i=0;i<number_buttons;i++)
      button[i]=false;
   if (sball_getstatus(sball, &sx, &sy, &sz, &sxrot, &syrot, &szrot, &buttons))
      {
      value[0]= ((float)sx)/maxvalue(0)*sball_factor;
      value[1]= ((float)sy)/maxvalue(1)*sball_factor;
      value[2]=-((float)sz)/maxvalue(2)*sball_factor;
      value[3]= ((float)sxrot)/maxvalue(3)*sball_factor;
      value[4]= ((float)syrot)/maxvalue(4)*sball_factor;
      value[5]= ((float)szrot)/maxvalue(5)*sball_factor;
      ret=true;
      }
   return ret;
   }
 
# ifdef TEST_SPACEBALL
int main(int argc,char** argv)
   {
   spaceball device=spaceball("/dev/ttyd2");
   while(1)
      if (device.readInputDevice())
          printf("%f %f %f\n",device.get_x(),device.get_y(),device.get_z());
   }
# endif
#endif

#ifdef HAVE_NXT_DIALS
#include <usb.h>
// USB functions almost unmodified from libnxtusb.cpp of 
// http://jgraef.bplaced.de/libnxtusb-0.1b.zip
/********************************************************************************
 *                               LibNXTUSB                                      *
 ********************************************************************************
 *                                                                              *
 *  Name:     LibNXTUSB 0.1                                                     *
 *  Auhor:    Janosch Gräf <jgraef@users.sf.net>                               *
 *  License:  GNU/GPL                                                           *
 *                                                                              *
 ********************************************************************************
 *                                                                              *
 *  LibNXTUSB - Use your NXT with USB under Linux                               *
 *  Copyright (C) 2007  Janosch Gräf                                            *
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

static int usbUsageCount = 0;

nxtDials::nxtDials(const char* device) : InputDevice(device) {
  if (device == NULL)
     return;

  valid=true;   
  number_max_axis=3;
  number_max_axes=3;
  // set "wheel" property 
  for (int i=0;i<number_max_axes;i++)
     set_zero_on_release(i,false); 
  maximum=32767.0;
  factor=100.0;
  errorString="";
  if (usbUsageCount==0) {
    usb_init();
    usb_set_debug(0);
  }
  usbUsageCount++;
  int number_nxt = 0;
  if (device != NULL)
      number_nxt = atoi(device);
  nxt_handle=locate_nxt(number_nxt);
  if (!nxt_handle) {
      errorString="locate_nxt failed";
      return;
  }
  if (usb_reset(nxt_handle)) {
    errorString="Could not reset USB";
    valid=false;  
  } else if (usb_set_configuration(nxt_handle,USB_NXT_CONFIG)) {
    errorString="Could not set configuration to 1";
    valid=false;
  } else if (usb_claim_interface(nxt_handle,USB_INTERFACE)) {
    errorString="Could not claim interface to 0";
    valid=false;
  } else if (usb_set_altinterface(nxt_handle,USB_INTERFACE)) {
    errorString="Could not set alternative interface 0";
    valid=false;
  }
  if (!valid)
     error_message+=errorString;
}

nxtDials::~nxtDials() 
{
  usbUsageCount--;
  if (usbUsageCount==0)
    shutdown();
}

void 
nxtDials::guessInputDeviceNames(StringArray *names) const
   {
   names->resize(0);

   MyString append;
   append="";
   append+="NXT";
   names->append(append);
   }

bool nxtDials::readInputDevice(void) {
  if (!valid) 
     return false;
  value[0]=(getRotationCount(0)/maxvalue(0))*factor;
  value[1]=(getRotationCount(1)/maxvalue(1))*factor;
  value[2]=(getRotationCount(2)/maxvalue(2))*factor;
  return true;
}

// USB functions
void nxtDials::shutdown() {
  if (nxt_handle!=NULL) {
    usb_release_interface(nxt_handle,USB_INTERFACE);
    usb_close(nxt_handle);
  }
}

usb_dev_handle *nxtDials::locate_nxt(int index) {
  struct usb_bus *bus;
  struct usb_device *dev;

  usb_find_busses();
  usb_find_devices();
  for (bus = usb_busses; bus; bus = bus->next) {
    for (dev = bus->devices; dev; dev = dev->next) {
      if ((dev->descriptor.idVendor == USB_ID_VENDOR_LEGO) &&
          (dev->descriptor.idProduct == USB_ID_PRODUCT_NXT)) {
        if (index==0) return usb_open(dev);
        else index--;
      }
    }
  }

  swDebugf("[USB] No device found\n");
  valid = false;
  return NULL;
}

int nxtDials::usb_sendpacket(char *buffer,int size) {
  int result;
  result = usb_bulk_write(nxt_handle,USB_OUT_ENDPOINT,buffer,size,USB_TIMEOUT);
  if (result<0) {
    result *= -1;
    swDebugf("[USB] Send error [%i]: %s\r\n",result,strerror(result));
    valid = false;
  }
  return result;
}

int nxtDials::usb_recvpacket(char *buffer,int size) {
  int result;
  result = usb_bulk_read(nxt_handle,USB_IN_ENDPOINT,buffer,size,USB_TIMEOUT);
  if (result<0) {
    result *= -1;
    swDebugf("[USB] Receive error [%i]: %s\r\n",result,strerror(result));
    valid = false;
  }
  return result;
}

void nxtDials::abortprog() { /// TODO: recv response
  char buffer[64];
  buffer[0] = 0x80;
  buffer[1] = 0x01;
  usb_sendpacket(buffer,2);
}

int nxtDials::getRotationCount(int motorport) {  
  unsigned char buffer[64];
  buffer[0] = 0x00;
  buffer[1] = 0x06;
  buffer[2] = motorport;
  usb_sendpacket((char*)buffer,3);
  usb_recvpacket((char*)buffer,25);

  int count = buffer[24] * 0x1000000;
  count += buffer[23] * 0x10000;
  count += buffer[22] * 0x100;
  count += buffer[21];

  return count;
}
#endif

#ifdef HAVE_WINDOWS_P5

/*
 * initialise p5 
 * argument "device" is the number of the p5 device, eg. "0" or "1" 
 */

windowsp5::windowsp5(const char *device) : InputDevice(device)
   {
   if (device==NULL)
      return;

   device_name=device;
   valid=true;
   int characters_read;
   number_max_axis = 6;
   sscanf(device,"%d%n",&number_p5,&characters_read);
   if (!(bP5Present=P5.P5_Init()))
      {
      swDebugf("windows_p5_device %s ",device);
      swDebugf("initialisation failed\n");
      valid=false;
      return;
      }
   else
      {
      P5Bend_Init(&P5, number_p5);
      P5Motion_Init(&P5,number_p5);
      };

   nullsize=0;
   maximum=256.0;
   p5_factor=1.0;
   // the P5 is a "wheel" type device
   for (int i=0;i<number_max_axis;i++)
      set_zero_on_release(i,false);
   number_buttons = 9;
   name="Unknown windowsp5";

   if (number_buttons>0)
      { 
      button=new bool[number_buttons];
      for (int i=0;i<number_buttons;i++)
         button[i]=false;
      }
 
# ifdef DEBUG
   swDebugf("InputDevice (%s) has %d axes and %d buttons. Driver ???\n",
            name, number_max_axis, number_buttons);  
# endif
   }
 
windowsp5::~windowsp5(void)
   {void P5_Close();}

void 
windowsp5::guessInputDeviceNames(StringArray *names) const
   {
   appendNumbers(names,1);
   }

bool windowsp5::readInputDevice(void)
   {
   if (bP5Present)
      {
      int i=0;
      button[i++]=(((unsigned char)P5.m_P5Devices[number_p5].m_byBendSensor_Data[P5_THUMB]) > 33);
      button[i++]=(((unsigned char)P5.m_P5Devices[number_p5].m_byBendSensor_Data[P5_INDEX]) > 33);
      button[i++]=(((unsigned char)P5.m_P5Devices[number_p5].m_byBendSensor_Data[P5_MIDDLE]) > 33);
      button[i++]=(((unsigned char)P5.m_P5Devices[number_p5].m_byBendSensor_Data[P5_RING]) > 33);
      button[i++]=(((unsigned char)P5.m_P5Devices[number_p5].m_byBendSensor_Data[P5_PINKY]) > 33);
      button[i++]=P5.m_P5Devices[number_p5].m_byButtons[0];
      button[i++]=P5.m_P5Devices[number_p5].m_byButtons[1];
      button[i++]=P5.m_P5Devices[number_p5].m_byButtons[2];
      button[i++]=P5.m_P5Devices[number_p5].m_byButtons[3];

      value[0]= (float)P5.m_P5Devices[number_p5].m_fx/maxvalue(0)*p5_factor;
      value[1]= (float)P5.m_P5Devices[number_p5].m_fy/maxvalue(1)*p5_factor;
      value[2]=-(float)P5.m_P5Devices[number_p5].m_fz/maxvalue(2)*p5_factor;
      value[3]= (float)(P5.m_P5Devices[number_p5].m_froll)/maxvalue(3)*p5_factor;
      value[4]= (float)(P5.m_P5Devices[number_p5].m_fyaw)/maxvalue(4)*p5_factor;
      value[5]= (float)(P5.m_P5Devices[number_p5].m_fpitch)/maxvalue(5)*p5_factor;
      }
   return bP5Present;
   }
 
# ifdef TEST_P5
int main(int argc,char** argv)
   {
   windowsp5 device=windowsp5("/dev/ttyd2");
   while(1)
      if (device.readInputDevice())
          printf("%f %f %f\n",device.get_x(),device.get_y(),device.get_z());
   }
# endif
#endif

// M$Windows Driver for SpaceTec/LabTec Spaceball and clones.

#ifdef HAVE_WINDOWS_SPACEBALL
/*
 * initialise spaceball 
 * argument "device" is useless
 */
 
#include "swt.h"

windowsspaceball::windowsspaceball(const char *device) : InputDevice(device)
   {
   if (device == NULL)
      return; 

   device_name = device;
   valid = true;   
   initialized = swSpaceNavInit();
   if (!initialized)
      {
      char msg[1024]; 
      mysnprintf(msg, 1024, "windowsspaceballdevice %s initialisation failed", 
                 device);
      error_message += msg;
      valid = false;
      return;
      }

   nullsize=0;
   maximum=32767.0;
   windows_sball_factor=10.0;

   number_max_axis = 6;
   number_buttons = 9;
   name="Unknown windowsspaceball";

   if (number_buttons>0)
      { 
      button=new bool[number_buttons];
      for (int i=0;i<number_buttons;i++)
         button[i]=false;
      }
 
# ifdef DEBUG
   swDebugf("InputDevice (%s) has %d axes and %d buttons. Driver ???\n",
            name, number_max_axis, number_buttons);  
# endif
   }
 
windowsspaceball::~windowsspaceball(void)
   {
   swSpaceNavFree();
   }

void 
windowsspaceball::guessInputDeviceNames(StringArray *names) const
   {
   appendNumbers(names,1);
   }

bool windowsspaceball::readInputDevice(void)
   {
   bool ret=false;
   int sx,sy,sz;
   int sxrot,syrot,szrot;
 
   if (!initialized)
      return false;

   sx=swSpaceNavGetData(0);
   sz=swSpaceNavGetData(1);
   sy=swSpaceNavGetData(2);
   sxrot=swSpaceNavGetData(3);
   szrot=swSpaceNavGetData(4);
   syrot=swSpaceNavGetData(5);

   value[0]= ((float)sx)/maxvalue(0)*windows_sball_factor;
   value[1]= -((float)sy)/maxvalue(1)*windows_sball_factor;
   value[2]= ((float)sz)/maxvalue(2)*windows_sball_factor;
   value[3]= ((float)sxrot)/maxvalue(3)*windows_sball_factor;
   value[4]= -((float)syrot)/maxvalue(4)*windows_sball_factor;
   value[5]= -((float)szrot)/maxvalue(5)*windows_sball_factor;

   return true;
   }
 
# ifdef TEST_SPACEBALL
int main(int argc,char** argv)
   {
   windowsspaceball device=windowsspaceball("/dev/ttyd2");
   while(1)
      if (device.readInputDevice())
          printf("%f %f %f\n",device.get_x(),device.get_y(),device.get_z());
   }
# endif
#endif



// Driver for Ascention Flock of birds devices

#ifdef HAVE_AFLOCK
#include <unistd.h>

/*
 * initialise Aflock 
 */

AflockDevice::AflockDevice(const char* dev)
   {
   /* insert your default configuration here */
   device = dev;
   errorMessage="";
   if (device==NULL)
      return; 

   baudrate = 38400;     // baudrate of device
   sync = 0;          
   block = 0;         
   numBrds = 2;          // number of birds (without transmitter)
   transmit = 1;         // number of bird transmitter unit
   hemi = RIGHT_HEM;     // hemisphere (sit on the antenna block to see, 
                         //             what is left or right 8-)
   filt = FILTER_DEFAULT;// use default filter set by flock autoconfig
   sudden_change_lock=true;
   report = 'Q';
   calfile = (char *)""; // calibration file
   /* end of configuratable data */

   ignoreSize = 0;

   opened=false;
   flock=NULL;
   } 

AflockDevice::~AflockDevice(void)
   {
   if (flock!=NULL)
      if (opened)
         {
         flock->stop();
         opened=false;
         delete flock;
         flock=NULL;
         }
   }

void AflockDevice::setBaud(char* baudrate_string)
   {
   if (!string2int(baudrate,baudrate_string))
      swDebugf("argument %s is not a integer, ignored\n",baudrate_string);
   }

void AflockDevice::setSync(char* sync_string)
   {
   if (!string2int(sync,sync_string))
      swDebugf("argument %s is not a integer, ignored\n",sync_string);
   }

void AflockDevice::setBlock(char* block_string)
   {
   if (!string2int(block,block_string))
      swDebugf("argument %s is not a integer, ignored\n",block_string);
   }

void AflockDevice::setNumBrds(char* numBrds_string)
   {
   if (!string2int(numBrds,numBrds_string))
      swDebugf("argument %s is not a integer, ignored\n",numBrds_string);
   }

void AflockDevice::setTransmit(char* transmit_string)
   {
   if (!string2int(transmit,transmit_string))
      swDebugf("argument %s is not a integer, ignored\n",transmit_string);
   }

void AflockDevice::setHemi(char* hemi_string)
   {
   if (strcmp(hemi_string,"FRONT_HEM")==0)
      hemi=FRONT_HEM;
   else if (strcmp(hemi_string,"AFT_HEM")==0)
      hemi=AFT_HEM;
   else if (strcmp(hemi_string,"UPPER_HEM")==0)
      hemi=UPPER_HEM;
   else if (strcmp(hemi_string,"LOWER_HEM")==0)
      hemi=LOWER_HEM;
   else if (strcmp(hemi_string,"LEFT_HEM")==0)
      hemi=LEFT_HEM;
   else if (strcmp(hemi_string,"RIGHT_HEM")==0)
      hemi=RIGHT_HEM;
   else
      {
      swDebugf("argument %s is no of ",hemi_string);
      swDebugf("FRONT_HEM, AFT_HEM, UPPER_HEM, LOWER_HEM,");
      swDebugf(" LEFT_HEM, RIGHT_HEM, ignored\n");
      }
   }

const char* AflockDevice::getHemi(void)
   {
   if (hemi==FRONT_HEM)
       return("FRONT_HEM");
   else if (hemi==AFT_HEM)
       return("AFT_HEM");
   else if (hemi==UPPER_HEM)
       return("UPPER_HEM");
   else if (hemi==LOWER_HEM)
       return("LOWER_HEM");
   else if (hemi==LEFT_HEM)
       return("LEFT_HEM");
   else if (hemi==RIGHT_HEM)
       return("RIGHT_HEM");
   else
      {
      swDebugf("argument %d is no of ",hemi);
      swDebugf("FRONT_HEM, AFT_HEM, UPPER_HEM, LOWER_HEM,");
      swDebugf(" LEFT_HEM, RIGHT_HEM, ignored\n");
      return("");
      }
   }


void AflockDevice::setFilt(char* filt_string)
   {
   if (strcmp(filt_string,"AC_NARROW")==0)
      filt=filt | AC_NARROW;
   else if (strcmp(filt_string,"AC_WIDE")==0)
      filt=filt | AC_WIDE;
   else if (strcmp(filt_string,"DC_FILTER")==0)
      filt=filt | DC_FILTER;
   else 
      {
      swDebugf("argument %s is no of ",filt_string);
      swDebugf("AC_NARROW, AC_WIDE, DC_FILTER, ignored\n");
      }  
   }

const char* AflockDevice::getFilt(void)
   {
   if (filt & AC_NARROW)
      return("AC_NARROW");
   else if (filt & AC_WIDE)
      return("AC_WIDE");
   else if (filt & DC_FILTER)
      return("DC_FILTER");
   else 
      {
      swDebugf("argument %d is no of ",filt);
      swDebugf("AC_NARROW, AC_WIDE, DC_FILTER, ignored\n");
      return "";
      }  
   }

void AflockDevice::setSuddenChangeLock(char* sudden_string)
   {
   int sudden=1;
   if (!string2int(sudden,sudden_string))
      swDebugf("argument %s is not a integer, ignored\n",sudden_string);
   if (sudden==0)
      sudden_change_lock=false;
   else
      sudden_change_lock=true;
   }

void AflockDevice::setReport(char* report_string)
   {
   report=report_string[0];
   }

void AflockDevice::setCalfile(char* calfile_string)
   {
   calfile=calfile_string;
   }


/* set device and initialise flock of birds */

Aflock* AflockDevice::getAflock(void)
   {
   if (!opened)
      {
      flock=NULL;
      flock = new Aflock(device,baudrate,sync,block,
                         numBrds,transmit,hemi,filt,sudden_change_lock,
                         report,calfile);
      if (!flock->start())
         {
         flock=NULL;
         swDebugf("Aflock %s ",device);
         swDebugf("initialisation failed\n");
         return flock;
         }
      sleep(1);
      opened=true;
      }
   return flock;      
   }

void 
AflockDevice::setIgnoreSize(const char* ignoresize_string)
   {
   if (!string2float(ignoreSize,ignoresize_string))
      swDebugf("argument %s is not a float, ignored\n",ignoresize_string);
   }

 
aflock::aflock(AflockDevice* device,char* receiver_string,bool headflag) 
              : InputDevice("")
   {
   checkDevice = NULL;
   number_max_axis = 6;
   head=false;
   wand=false;
   headNavigation=false;
   aflockDevice=device;

   error_message="";

   if (headflag)
      {
      head=true;
      device_name=strdup("head");
      }
   else
      {
      wand=true;
      device_name=strdup("wand");
      }

   if (!string2int(receiver,receiver_string))
      {
      error_message="";
      error_message+="argument ";
      error_message+=receiver_string;
      error_message+=" is not a integer";
      swDebugf(error_message);
      flock=NULL;
      return;
      }

   if (device==NULL)
      {
      error_message="";
      error_message+="internal error, initialition failed";
      swDebugf(error_message);
      flock=NULL;
      return;
      }

   flock=device->getAflock();

   if (receiver==flock->getTransmitter())
      flock->setMasterUseReceiver();      

   number_buttons = 3;
   name=(char *)"Ascention flock of birds";
   maximum=1.0;
   aflock_factor=1.0;
   degreefactor=M_PI/180.0;
   aflockFirstFlag=true;

   for (int i=0;i<7;i++)
      set_zero_on_release(i,false);

   if (number_buttons>0)
      { 
      button=new bool[number_buttons];
      for (int i=0;i<number_buttons;i++)
         button[i]=false;
      }
# ifdef DEBUG
   swDebugf("InputDevice (%s) has %d axes and %d buttons. Driver ???\n",
            name, number_max_axis, number_buttons);  
# endif
   }

aflock::~aflock(void)
   {
   }

void 
aflock::guessInputDeviceNames(StringArray *names) const
   {
   names->resize(0);
#ifdef SERIAL_DEVICE
# ifdef OFFSET_SERIAL_DEVICE
   loopAddUnixDevices(names,SERIAL_DEVICE,OFFSET_SERIAL_DEVICE);
# endif
#endif
   loopAddUnixDevices(names,"/dev/ttyUSB");
   }

void aflock::prepareRead(void)
   {
   button_pressed=-1;
   button_released=-1;

   if (flock==NULL)
      return;
   if (!flock->isActive())
      {
      swDebugf("Bird not active\n");
      return;
      }
   flock->prepareSample();
   }

bool aflock::readInputDevice(void)
   {
   button_pressed=-1;
   button_released=-1;

   if (flock==NULL)
      return false;
   if (!flock->isActive())
      {
      swDebugf("Bird not active\n");
      return false;
      }
   flock->sample();
   for (int i=0;i<number_buttons;i++)
      button[i]=false;

   value[0]=-flock->yPos(receiver)/maxvalue(2)*aflock_factor;
   value[1]=-flock->zPos(receiver)/maxvalue(1)*aflock_factor;
   value[2]= flock->xPos(receiver)/maxvalue(0)*aflock_factor;
   float ignoreSize = aflockDevice->getIgnoreSize();
   if ((ignoreSize > 0) && !aflockFirstFlag)
      {
#ifdef HAVE_AFLOCK_DEBUG
#endif
      bool ret = true;
      if (fabs(old_value[0]-value[0])>ignoreSize)
           ret = false;
      if (fabs(old_value[1]-value[1])>ignoreSize)
           ret = false;
      if (fabs(old_value[2]-value[2])>ignoreSize)
           ret = false;
      if (ret == false) 
         { 
         swDebugf("ignoring jump flock %d: deltax=%f meter deltay=%f meter deltaz=%f meter\n",
                 receiver,
                 fabs(old_value[0]-value[0]),
                 fabs(old_value[1]-value[1]),
                 fabs(old_value[2]-value[2]));
         return false;
         }
      }
   old_value[0]=value[0];
   old_value[1]=value[1];
   old_value[2]=value[2];
   aflockFirstFlag=false;
#ifdef HAVE_AFLOCK_DEBUG
   if (ignoreSize == 0)
      printf("flock %d: x=%f meter y=%f meter z=%f meter\n",receiver,
             value[0],value[1],value[2]);
#endif
   value[3]=-flock->yRot(receiver)/maxvalue(3)*degreefactor;
   value[4]=-flock->zRot(receiver)/maxvalue(4)*degreefactor;
   value[5]=-flock->xRot(receiver)/maxvalue(5)*degreefactor;

   return true;
   }

bool aflock::isValid()
   { 
   if (aflockDevice) 
      return aflockDevice->isValid();
   if (checkDevice)
      return checkUnixDevice(checkDevice);
   return false;
   }

# ifdef TEST_AFLOCK
int main(int argc,char** argv)
   {
   AflockDevice* device=new AflockDevice();
   aflock device1(device,argv[1],true);
   for (int i=1;i<3000;i++)
      {
      if (device1.readInputDevice())
         printf("1 %g %g %g\n",device1.get_x(),device1.get_y(),device1.get_z());
         printf("1 rot %g %g %g\n",device1.get_xrot(),device1.get_yrot(),device1.get_zrot());
      }
   delete device;
   }
# endif
#endif

InputDevice *
InputDevice::createInputDevice(const char* device_option, 
                               const char* device_name) 
{
   InputDevice *ret = NULL;
#ifdef LINUX_JOYSTICK
    if (strcmp(device_option,"-joystick")==0)
        ret=new linux_joystick(device_name);
#endif
#ifdef WINDOWS_JOYSTICK
    if (strcmp(device_option,"-joystick")==0)
       ret=new windows_joystick(device_name);
#endif
#ifdef HAVE_SDL_JOYSTICK
    if (strcmp(device_option,"-SDLjoystick")==0)
       ret=new SDL_joystick(device_name);
#endif
#ifdef HAVE_LIBSBALL
    if (strcmp(device_option,"-spaceball")==0)
       ret=new spaceball(device_name);
#endif
#ifdef HAVE_WINDOWS_SPACEBALL
    if (strcmp(device_option,"-spaceball")==0)
       ret=new windowsspaceball(device_name);
#endif
#ifdef HAVE_WINDOWS_P5
    if (strcmp(device_option,"-p5")==0)
       ret=new windowsp5(device_name);
#endif
#ifdef HAVE_NXT_DIALS
    if (strcmp(device_option,"-nxtdials")==0)
       ret=new nxtDials(device_name);
#endif
#ifdef HAVE_XINPUT
    if (strcmp(device_option,"-xinput")==0) {
       xinput *device=new xinput(device_name);
       if (device_name != NULL) {
          if (device == NULL)
             return NULL;
          if (device->get_number_axes() == 0)
             return NULL;
       }
       ret=device;
    }
#endif
#ifdef HAVE_AFLOCK
    // aflock devices must be handled differently
    if (strcmp(device_option,"-aflock")==0) {
       ret=new aflock(device_name);
    }
#endif
    if (ret==NULL)
       swDebugf("internal program error: device option %s is missing here\n", 
                device_option);
    return ret;
}   


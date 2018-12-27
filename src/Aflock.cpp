/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          aFlock.cpp,v
 * Date modified: 2001/05/22 14:54:46
 * Version:       1.41.6.2
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

// Author:
//      Kevin Meinert
//      modified for white_dune by J. "MUFTI" Scheurich 
//      using things from HeadTracker/I3Stick API by J. Dean Brederson
//      using things from stty.c by David MacKenzie <djm@gnu.ai.mit.edu> 
//
//      the streaming mode of the orginal VRjuggler Ascention Flock of birds 
//      tracking class seams to be not a good idea for white_dune...
// 
// Last Modified: Wed May 29 2002

#include <config.h>
#ifdef HAVE_AFLOCK

#include <string.h>

#include <fcntl.h>         // for open
#include <termios.h>
#include <unistd.h>        // for sleep, and ioctl
#include <stdlib.h>        // for system (to be deleted...)
#ifdef __linux__
# include <sys/ioctl.h>
//# include <asm/termbits.h>
//# include <sys/termios.h>
#endif
#include <sys/types.h>     // for open
#include <sys/stat.h>      // for open
#include <stdio.h>         // for fopen,fprintf
#include "stdafx.h"        // for assert
#include "DuneApp.h"        

#if defined(__sun) || defined(__hpux)
#include <sys/file.h>
#endif

#include "Aflock.h"

#ifdef HAVE_AFLOCK_DEBUG
#define AFLOCK_ERROR_CHECK(port,bird,what) 
//#define AFLOCK_ERROR_CHECK(port,bird,what) aflock_error_check(port,what,bird)

static void aflock_error_check(int port,int bird,char* what)
{ 
    char geterror[3];
    geterror[0]=bird+0xF0;
    geterror[1]='O';
    geterror[2]=0x0A;
    if (write(port, geterror, 3) == -1)
        fprintf(stderr, "Error sending geterror command while %s\n",what);
    tcdrain(port);
    char flockerror[1];
    if (read(port, flockerror, 1) == -1)
        fprintf(stderr, "Error getting errorcode command while %s\n",what);
    else if (flockerror[0]!=0)
        fprintf(stderr, "Errorcode: %d from bird %d while %s\n",
                        flockerror[0],bird,what);
}
#else
#define AFLOCK_ERROR_CHECK(port,bird,what)
#endif

const int Aflock::MAXCHARSTRINGSIZE = 256;
const int Aflock::mSleepFactor = 3;

//: Configure Constructor
// Give:                                                 <BR>
//   port - such as "/dev/ttyd3"                         <BR>
//   baud - such as 38400, 19200, 9600, 14400, etc...    <BR>
//   sync - sync type.                                   <BR>
//   block - blocking                                    <BR>
//   numBrds - number of birds in flock,                 <BR>
//   transmit - transmitter unit number,                 <BR>
//   hemi - hemisphere to track from,                    <BR>
//   filt - filtering type,                              <BR>
//   report -                                            <BR>
//   calfile - a calibration file, if "", then use none. <BR>
//                                                       <BR>
// Result: configures internal data members,
//         doesn't actually talk to the FOB yet.
Aflock::Aflock(const char* const port,
               const int& baud,
               const int& sync,
               const int& block,
               const int& numBrds,
               const int& transmit,
               const BIRD_HEMI& hemi,
               const BIRD_FILT& filt,
               const bool& sudden_lock,
               const char& report,
               const char* const calfile) :
                  m_reportRate(report),
                  m_hemisphere(hemi),
                  m_filter(filt),
                  m_sudden_output_change_lock(sudden_lock),
                  m_portId(-1),
                  m_baud(baud),
                  m_syncStyle(sync),
                  m_blocking(block),
                  m_numBirds(numBrds),
                  m_xmitterUnitNumber(transmit),
                  m_usingCorrectionTable(false),
                  m_active(false)
{
  if (port != NULL)
      strncpy( m_port, port, Aflock::MAXCHARSTRINGSIZE );

  // fix the report rate if it makes no sense.
  if ((m_reportRate != 'Q') && (m_reportRate != 'R') &&
      (m_reportRate != 'S') && (m_reportRate != 'T'))
  {
     // illegal report rate, defaulting to "every other cycle" (R)
     assert(0);
     m_reportRate = 'R';
  }

  if (calfile != NULL && calfile[0] != '\0')
  {
     strncpy( m_calibrationFileName, calfile, Aflock::MAXCHARSTRINGSIZE );
     this->initCorrectionTable(m_calibrationFileName);
     m_usingCorrectionTable = true;
  }
  m_samplePrepared=false;
  m_masterUseReceiver=false;
}

//: Destructor
Aflock::~Aflock()
{
    this->stop();
}

//: deliver number of active devices  

int Aflock::getNumberDevices(void) const
{
    if (m_masterUseReceiver)
       return getNumBirds();
    else 
       return getNumBirds()+1;
}

bool Aflock::deliverData(int birdaddr)
{
    if (birdaddr==m_xmitterUnitNumber)
       if (m_masterUseReceiver)
          return(false);
    return(true);
}

//: see if the flock is active or not
const bool& Aflock::isActive() const
{
    return m_active;
}

//: set the port to use
//  this will be a string in the form of the native OS descriptor <BR>
//  ex: unix - "/dev/ttyd3", win32 - "COM3" <BR>
//  NOTE: flock.isActive() must be false to use this function
void Aflock::setPort(const char* const serialPort)
{
    if (m_active)
    {
       fprintf(stderr,"Aflock: Cannot change the serial Port while active\n");
       return;
    }
    if (serialPort != NULL)
       strncpy( m_port, serialPort, Aflock::MAXCHARSTRINGSIZE );
}

//: get the port used
//  this will be a string in the form of the native OS descriptor <BR>
//  ex: unix - "/dev/ttyd3", win32 - "COM3"
const char* Aflock::getPort() const
{
    return m_port;
}

//: set the baud rate
//  this is generally 38400, consult flock manual for other rates. <BR>
//  NOTE: flock.isActive() must be false to use this function
void Aflock::setBaudRate(const int& baud)
{
    if (m_active)
    {
      AFLOCK_PRINT("Aflock: Cannot change the baud rate while active\n");
      return;
    } else {
      m_baud = baud;
    }
}

//: call this to connect to the flock device.
//  NOTE: flock.isActive() must be false to use this function
int Aflock::start()
{
    if (!m_active)
    {
      AFLOCK_PRINT("Aflock: Getting ready....\n\n");

      AFLOCK_PRINT("Aflock: Opening port\n");
      Aflock::open_port( m_port, m_baud, m_portId );
      if (m_portId == -1)
      {
         fprintf(stderr,"unable to open port\n");
         return 0;
      }

      // number of bytes per flock in groupmode:
#ifdef USE_AFLOCK_QUATERNION
      // 14 Byte for position/quaternion
      // 1 Byte for what flock
      #define NUMBER_OF_BYTES_PER_FLOCK 15
#else
      // 12 Byte for position/angles
      // 1 Byte for what flock
      #define NUMBER_OF_BYTES_PER_FLOCK 13
#endif
      m_grouplength=m_numBirds*NUMBER_OF_BYTES_PER_FLOCK;
      m_groupbuffer=new unsigned char[m_grouplength];

      AFLOCK_PRINT("Aflock: throw away dirt from port\n");
      Aflock::destroyDirt();

      if (m_blocking==1)
         AFLOCK_PRINT("Aflock: Setting blocking\n");
      else if (m_blocking==0)
         AFLOCK_PRINT("Aflock: Setting non blocking\n");
      Aflock::set_blocking( m_portId, m_blocking );

      if (m_syncStyle!=0)
         AFLOCK_PRINT("Aflock: Setting sync\n");
      Aflock::set_sync( m_portId, m_syncStyle );


// Transmitter is at adress 1 (default)
      AFLOCK_PRINT("Aflock: Setting transmitter\n");
      Aflock::set_transmitter( m_portId, m_xmitterUnitNumber );

#ifdef USE_AFLOCK_QUATERNION
      AFLOCK_PRINT("Aflock: Setting pos_quaternion\n");
      Aflock::set_pos_quat( m_portId, m_xmitterUnitNumber );
#else
      AFLOCK_PRINT("Aflock: Setting pos_angles\n");
      Aflock::set_pos_angles( m_portId, m_xmitterUnitNumber );
#endif

      AFLOCK_PRINT("Aflock: Setting hemisphere\n");
      Aflock::set_hemisphere( m_portId, m_hemisphere, m_xmitterUnitNumber );

      AFLOCK_PRINT("Aflock: Setting autoconfig\n");
      Aflock::set_autoconfig( m_portId );

      if (m_filter!=FILTER_DEFAULT) 
      {
         AFLOCK_PRINT("Aflock: Setting filter\n");
         Aflock::set_filter( m_portId, m_filter );
      }
      else
         AFLOCK_PRINT("Aflock: keeping default filter\n");

      if (m_sudden_output_change_lock)
         AFLOCK_PRINT("Aflock: lock sudden output change\n");
      else
         AFLOCK_PRINT("Aflock: unlock sudden output change\n");  
      Aflock::set_sudden_output_change_lock(m_portId, 
                                            m_sudden_output_change_lock);

      AFLOCK_PRINT("Aflock: Setting group\n");
      Aflock::set_group( m_portId );

/// Currently not used (we are using point mode)
//      AFLOCK_PRINT("Aflock: Setting pickBird\n");
//      Aflock::pickBird( m_xmitterUnitNumber,m_portId );
//      AFLOCK_PRINT("Aflock: Setting rep_and_stream\n");
//      Aflock::set_rep_and_stream( m_portId, m_reportRate );

      AFLOCK_PRINT("Aflock: ready to go..\n");

      // flock is active.
      m_active = true;

      // return success
      return 1;
    } 
    
    else
       return 0; // already sampling
}

// call this repeatedly to request data from the birds
// get the data later with flock.sample()
// NOTE: flock.isActive() must be true to use this function
int Aflock::prepareSample()
{
     // can't ask for data when not active.
     assert(m_active == true);

     if (m_samplePrepared)
        return 1;

     // ask for data from master (1) 
     unsigned char getpoint[2];
     getpoint[0]=0xF0+1;
     getpoint[1]='B';
     if (write(m_portId, getpoint, 2) == -1) {
        fprintf(stderr, "Error sending polling command while reading 6DOF\n");
        tcdrain(m_portId);
        return 0;
     }
     tcdrain(m_portId);
     m_samplePrepared=true;
     return 1;
}


//: call this repeatedly to update the data from the birds.
//  NOTE: flock.isActive() must be true to use this function
int Aflock::sample()
{
     // can't sample when not active.
     assert(m_active == true);

     if (m_samplePrepared==false)
        if (prepareSample()!=0)
           return 0;

     /* get a data record from the Flock */
     int numbytes;
     int errorcounter=0;
     int i=0;
     do {
        numbytes=read(m_portId, m_groupbuffer, m_grouplength);
        if (numbytes<=0)
           {
           usleep(10);
           if (++errorcounter>MAX_READ_ERRORS)
              {
#ifdef HAVE_AFLOCK_DEBUG
              fprintf(stderr,"Aflock: cant get the bytes from getpoint command:");
              fprintf(stderr,"Aflock: got %d bytes of %d\n",i,m_grouplength);
#endif
              m_samplePrepared=false;
              destroyDirt();
              return 0;
              }
           if (numbytes<0)
              numbytes=0;
           }
        } while ((i+=numbytes)<m_grouplength);

     m_samplePrepared=false;

     int j=0;
     // for [1..n] birds, get their reading:
     for (i=1; i <= getNumberDevices() && i <= MAX_SENSORS; i++)
        {
        // do not read values from m_groupbuffer for Transmitter
        if (!deliverData(i))
            continue;

        // you can never go above the maximum number of sensors.
        assert(j <= MAX_SENSORS);

        int begin=NUMBER_OF_BYTES_PER_FLOCK*j;
        unsigned char* buff=m_groupbuffer;
        int flock=buff[begin+NUMBER_OF_BYTES_PER_FLOCK-1];

        if (flock > MAX_SENSORS)
           {
           fprintf(stderr,"Aflock: Invalid number of flock data in group: %d\n",
                   flock);
           return 0;
           }

        // Position
        xPos(flock) = rawToFloat(POSITION_RANGE, buff[1+begin],buff[0+begin]);
        yPos(flock) = rawToFloat(POSITION_RANGE, buff[3+begin],buff[2+begin]);
        zPos(flock) = rawToFloat(POSITION_RANGE, buff[5+begin],buff[4+begin]);

        // Orientation
#ifdef USE_AFLOCK_QUATERNION
        wQuat(flock) = rawToQuat(buff[7+begin],buff[6+begin]);
        xQuat(flock) = rawToQuat(buff[9+begin],buff[8+begin]);
        yQuat(flock) = rawToQuat(buff[11+begin],buff[10+begin]);
        zQuat(flock) = rawToQuat(buff[12+begin],buff[13+begin]);
#else  
        zRot(flock) = rawToFloat(ANGLE_RANGE, buff[7+begin],buff[6+begin]);
        yRot(flock) = rawToFloat(ANGLE_RANGE, buff[9+begin],buff[8+begin]);
        xRot(flock) = rawToFloat(ANGLE_RANGE, buff[11+begin],buff[10+begin]);
#endif

        if (m_usingCorrectionTable)
            this->positionCorrect( xPos(flock), yPos(flock), zPos(flock));
        j++;
        }

     return 1;
}

// error timeout to wait between drain and flush (in usleep units)
#define ERROR_TIMEOUT 1000

//// destroy remaining dirt on the port to recover from errors
void Aflock::destroyDirt()
{
     tcdrain(m_portId);
     usleep(ERROR_TIMEOUT);
     tcdrain(m_portId);
}


//: stop the flock
int Aflock::stop()
{
    char   bird_command[4];

    AFLOCK_PRINT("Aflock: Try stopping the flock...");

    bird_command[0] = 'B';
    while (write(m_portId, bird_command, 1) == -1);

    usleep( 500 * Aflock::mSleepFactor );
    bird_command[0] = 'G';
    while (write(m_portId, bird_command, 1) == -1);

    usleep( 200 * Aflock::mSleepFactor );

    tcdrain(m_portId);
    tcflush(m_portId, TCIOFLUSH);
    tcdrain(m_portId);

    close( m_portId );
    m_portId = -1;

    // flock is not active now.
    m_active = false;

    AFLOCK_PRINT(" STOPPED \n");

    return 1;
}

//: Set the hemisphere that the transmitter transmits from.
//  NOTE: flock.isActive() must be false to use this function
void Aflock::setHemisphere( const BIRD_HEMI& h )
{
    if (m_active)
    {
        fprintf(stderr,"Aflock: Cannot change the hemisphere\n");
        return;
    } else {
        // Set it.
        m_hemisphere = h;
    }
}

//: Set the type of filtering that the flock uses
//  NOTE: flock.isActive() must be false to use this function
void Aflock::setFilterType( const BIRD_FILT& f )
{
    if (m_active)
    {
        fprintf(stderr,"Aflock: Cannot change filter type while active\n");
        return;
    } else {
        // Set it.
        m_filter = f;
    }
}

//: Set if sudden output changes of a bird should be locked
//  NOTE: flock.isActive() must be false to use this function
void Aflock::setSuddenOutputChangeLock( const bool& b )
{
    if (m_active)
    {
        fprintf(stderr,"Aflock: Cannot change sudden lock while active\n");
        return;
    } else {
        // Set it.
        m_sudden_output_change_lock = b;
    }
}

//: Set the report rate that the flock uses
//  NOTE: flock.isActive() must be false to use this function
void Aflock::setReportRate( const char& rRate )
{
    if (m_active)
    {
        fprintf(stderr,"Aflock: Cannot change report rate while active\n");
        return;
    } else {
        // Set it.
        m_reportRate = rRate;
    }
}

//: Set the unit number of the transmitter
//  give - an integer that is the same as the dip switch
//         setting on the transmitter box (for the unit number) <BR>
//  NOTE: flock.isActive() must be false to use this function
void Aflock::setTransmitter( const int& Transmit )
{
  if (m_active)
  {
      fprintf(stderr,"Aflock: Cannot change transmitter while active\n");
      return;
  } else {
      // Set it.
      m_xmitterUnitNumber = Transmit;
  }
}

//: Set the number of birds to use in the flock.
//  give - an integer number not more than the number of
//         birds attached to the system <BR>
//  NOTE: flock.isActive() must be false to use this function
void Aflock::setNumBirds( const int& n )
{
    if (m_active)
    {
        fprintf(stderr,"Aflock: Cannot change num birds while active\n");
        return;
    } else {
        // Set it.
        m_numBirds = n;
    }
}

//: set the video sync type
//  this option allows the Flock to syncronize its pulses with
//  your video display.  This will eliminate most flicker caused
//  by the magnetic distortion. <BR>
//  - Refer to your flock manual for what number to use.
//  NOTE: flock.isActive() must be false to use this function
void Aflock::setSync(const int& sync)
{
  if (m_active)
  {
      fprintf(stderr,"Aflock: Cannot change report rate while active\n");
      return;
  } else {
      // Set it.
      m_syncStyle = sync;
  }
}

//: set blocking of flock
//  see flock manual for details.
//  NOTE: flock.isActive() must be false to use this function
void Aflock::setBlocking( const int& blVal )
{
  if (m_active)
  {
      fprintf(stderr,"Aflock: Cannot change blocking while active\n");
      return;
  } else {
      // Set it.
      m_blocking = blVal;
  }
}

//: with the calibration table info, correct a given position
//  give - a position in x, y, z euclidian coordinates
//  returns - a corrected position in x, y, z euclidian coordinates
void Aflock::positionCorrect( float& x, float& y, float& z )
{
    int xlo,ylo,zlo,xhi,yhi,zhi;
    float a,b,c,a1,b1,c1;

    // Find corners
    xlo = (int)(x-caltable.xmin);
    ylo = (int)(y-caltable.ymin);
    zlo = (int)(z-caltable.zmin);
    xhi = xlo+1;
    yhi = ylo+1;
    zhi = zlo+1;
    a1 = 1 - (a = caltable.xmin + xhi - x);
    b1 = 1 - (b = caltable.ymin + yhi - y);
    c1 = 1 - (c = caltable.zmin + zhi - z);

    x = a * b* c*caltable.xloc[xlo][ylo][zlo] +
        a * b*c1*caltable.xloc[xlo][ylo][zhi] +
        a *b1* c*caltable.xloc[xlo][yhi][zlo]+
        a *b1*c1*caltable.xloc[xlo][yhi][zhi]+
        a1* b* c*caltable.xloc[xhi][ylo][zlo]+
        a1* b*c1*caltable.xloc[xhi][ylo][zhi]+
        a1*b1* c*caltable.xloc[xhi][yhi][zlo]+
        a1*b1*c1*caltable.xloc[xhi][yhi][zhi];
    y = a * b* c*caltable.yloc[xlo][ylo][zlo] +
        a * b*c1*caltable.yloc[xlo][ylo][zhi] +
        a *b1* c*caltable.yloc[xlo][yhi][zlo]+
        a *b1*c1*caltable.yloc[xlo][yhi][zhi]+
        a1* b* c*caltable.yloc[xhi][ylo][zlo]+
        a1* b*c1*caltable.yloc[xhi][ylo][zhi]+
        a1*b1* c*caltable.yloc[xhi][yhi][zlo]+
        a1*b1*c1*caltable.yloc[xhi][yhi][zhi];
    z = a * b* c*caltable.zloc[xlo][ylo][zlo] +
        a * b*c1*caltable.zloc[xlo][ylo][zhi] +
        a *b1* c*caltable.zloc[xlo][yhi][zlo]+
        a *b1*c1*caltable.zloc[xlo][yhi][zhi]+
        a1* b* c*caltable.zloc[xhi][ylo][zlo]+
        a1* b*c1*caltable.zloc[xhi][ylo][zhi]+
        a1*b1* c*caltable.zloc[xhi][yhi][zlo]+
        a1*b1*c1*caltable.zloc[xhi][yhi][zhi];
    return;
}

/// Warning: initCorrectionTable is untested

//: init the correction table from a file
//  give - a file name of the calibration file.
//  result - initializes the correction table with the file's info
void Aflock::initCorrectionTable( const char* const fName )
{
  int i,j,k, xsize,ysize,zsize;
  float dump;
  FILE* inFile;
                          
  AFLOCK_PRINT("     Initializing calibration table ... ");
//  AFLOCK_PRINT(fName);
  inFile=fopen(fName,"r");
  if (!inFile)
  {
        fprintf(stderr,"Unable to open calibration.table\n");
        return;
  } else {
     AFLOCK_PRINT( "   Calibration table opened sucessfully.");
  }

  if (fscanf(inFile,"%f %f %f %f %f %f",
                    &caltable.xmin,&caltable.xmax,
                    &caltable.ymin,&caltable.ymax,
                    &caltable.zmin,&caltable.zmax)) {}

  xsize = (int) (caltable.xmax - caltable.xmin) + 1;
  ysize = (int) (caltable.ymax - caltable.ymin) + 1;
  zsize = (int) (caltable.zmax - caltable.zmin) + 1;

  for (i = 0; i < xsize; i++)
    for (j = 0; j < ysize; j++)
      for (k = 0; k < zsize; k++)
         {
            if (fscanf(inFile,"%f %f %f %f %f %f",
                       &dump , &dump , &dump,
                       &caltable.xloc[i][j][k],
                       &caltable.yloc[i][j][k],
                       &caltable.zloc[i][j][k])) {}
         }

   fclose(inFile);
}

 float& Aflock::xPos( const int& i )  { assert( i < MAX_SENSORS ); return m_position[i][0]; }
 float& Aflock::yPos( const int& i )  { assert( i < MAX_SENSORS ); return m_position[i][1]; }
 float& Aflock::zPos( const int& i )  { assert( i < MAX_SENSORS ); return m_position[i][2]; }
#ifdef USE_AFLOCK_QUATERNION
 float& Aflock::xQuat( const int& i )  { assert( i < MAX_SENSORS ); return m_orientation[i][0]; }
 float& Aflock::yQuat( const int& i )  { assert( i < MAX_SENSORS ); return m_orientation[i][1]; }
 float& Aflock::zQuat( const int& i )  { assert( i < MAX_SENSORS ); return m_orientation[i][2]; }
 float& Aflock::wQuat( const int& i )  { assert( i < MAX_SENSORS ); return m_orientation[i][3]; }
#else
 float& Aflock::zRot( const int& i )  { assert( i < MAX_SENSORS ); return m_orientation[i][0]; }
 float& Aflock::yRot( const int& i )  { assert( i < MAX_SENSORS ); return m_orientation[i][1]; }
 float& Aflock::xRot( const int& i )  { assert( i < MAX_SENSORS ); return m_orientation[i][2]; }
#endif

///////////////////////////////////////////////////////////////////
// Local functions to Aflock.cpp
//////////////////////////////////////////////////////////////////

// see
// "The flock of Birds, Installation and Operation Guide, Standalone
//  and Multiple Transmitter/Multiple Sensors Configurations", Page 103
//  (Capture "RS232 Commands") a number of a quaternion is in range
// bit 7 of each byte do not contain information, must be shifted out

float Aflock::rawToFloat(float factor, unsigned char& r1,unsigned char& r2 )
{
   int int16;

   int16 = (r2 & 0x7F) << 1;
   int16 = ((r1 & 0x7F) << 8) + int16;
   int16 = int16 << 1;
   return factor * int16 / 32767.0;
}

// see
// "The flock of Birds, Installation and Operation Guide, Standalone
//  and Multiple Transmitter/Multiple Sensors Configurations", Page 107
//  (Capture "RS232 Commands") a number of a quaternion is in range
// bit 7 of each byte do not contain information, must be shifted out

float Aflock::rawToQuat(unsigned char& r1, unsigned char& r2)
{
   int int16;

   int16 = (r2 & 0x7F) << 1;
   int16 = ((r1 & 0x7F) << 8) + int16;
   int16 = int16 << 1;
   return 1.0 / 32768.0 * int16;
}

//: RS232 To FBB Command 
//  adress next RS232 command to bird number "birdID"
void  Aflock::pickBird( const int& birdID, const int& port )
{
    char buff = 0xF0 + birdID;
    while (write(port, &buff, 1) == -1);

    usleep ( 100 * Aflock::mSleepFactor );
    tcdrain(m_portId);

    AFLOCK_ERROR_CHECK(port,"RS232 command",birdID);
}

//: Open the port.
//  give - a serial port
//  give - a baud rate
//  returns portId twice (created by the open function)
//  NOTE: portId is returned from both ends of this function.
//  if portId == -1 then function failed to open the port.
int Aflock::open_port( const char* const serialPort,
                       const int& baud,
                       int& portId )
{
    ///////////////////////////////////////////////////////////////////
    // Open and close the port to reset the tracker, then
    // Open the port
    ///////////////////////////////////////////////////////////////////
    portId = open( serialPort, O_RDWR | O_NDELAY);
    if (portId == -1)
    {
        fprintf(stderr,"  port reset failed (because port open failed)\n");
        return portId;
    } else {
        usleep(300000);
        close( portId );
        AFLOCK_PRINT( "  port reset successfully (port opened then closed)\n");
    }

    portId = open( serialPort, O_RDWR | O_NDELAY);

    if (portId == -1)
    {
        fprintf(stderr,"  port open failed\n");
        return portId;
    } else {
        AFLOCK_PRINT( "  port open successfully\n");
    }

    //////////////////////////////////////////////////////////////////
    // Setup the port, current setting is 38400 baud
    //
    //////////////////////////////////////////////////////////////////

    AFLOCK_PRINT( "  Getting current term setting\n");
    termios termIoPort;
    /* Initialize to all zeroes so there is no risk memcmp will report a
       spurious difference in an uninitialized portion of the structure.  */
    memset (&termIoPort, 0, sizeof(termIoPort));
    if (tcgetattr(portId, &termIoPort))
       TheApp->MessageBoxPerror(serialPort);
     
    // Set the flags to 0 to clear out any cruft and to ensure that we are
    // setting fresh values.
    termIoPort.c_iflag = termIoPort.c_oflag = termIoPort.c_lflag = 0;

#ifndef __sgi
# ifndef __sun
#  ifndef MACOSX
#   ifndef __FreeBSD__
#    ifndef __OpenBSD__
#     ifndef AIX
#      ifndef __hpux
#       ifndef __osf__
         termIoPort.c_lflag = N_TTY;
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif
# endif
#endif

    // get the B* format baud rate
    // i.e.: B38400 is baud = 38400
    int magicBaudRate = 0;
    switch (baud)
    {
        case 150: magicBaudRate = B150; break;
        case 200: magicBaudRate = B200; break;
        case 300: magicBaudRate = B300; break;
        case 600: magicBaudRate = B600; break;
        case 1200: magicBaudRate = B1200; break;
        case 1800: magicBaudRate = B1800; break;
        case 2400: magicBaudRate = B2400; break;
        case 4800: magicBaudRate = B4800; break;
        case 9600: magicBaudRate = B9600; break;
        case 19200: magicBaudRate = B19200; break;

#ifndef _POSIX_SOURCE
#   ifdef B57600
        case 57600: magicBaudRate = B57600; break;
#   endif
#   ifdef B76800
        case 76800: magicBaudRate = B76800; break;
#   endif
#   ifdef B115200
        case 115200: magicBaudRate = B115200; break;
#   endif
#endif

        case 38400:
        default:
          magicBaudRate = B38400;
          break;
    }

    termIoPort.c_cflag = CS8 | CLOCAL | CREAD;
//    termIoPort.c_iflag = IGNBRK;
    termIoPort.c_cc[0] = 0;                     /* no delete/interrupt */
    termIoPort.c_cc[VMIN] = 0;                  /* character or timeout
                                                   satisfies read */
    termIoPort.c_cc[VTIME] = 1;                 /* 100 millisecond timeout */

#ifdef __linux__
# define HAVE_CFSETISPEED 1
#endif
#ifdef __sun
# define HAVE_CFSETISPEED 1
#endif
#ifdef MACOSX
# define HAVE_CFSETISPEED 1
#endif
#ifndef HAVE_C_OSPEED
# define HAVE_CFSETISPEED 1
#endif

#ifdef HAVE_CFSETISPEED
    cfsetispeed (&termIoPort, magicBaudRate);
    cfsetospeed (&termIoPort, magicBaudRate);

    struct termios new_mode;

    if (tcsetattr (portId, TCSADRAIN, &termIoPort))
       TheApp->MessageBoxPerror(serialPort);

      /* POSIX (according to Zlotnick's book) tcsetattr returns zero if
         it performs *any* of the requested operations.  This means it
         can report `success' when it has actually failed to perform
         some proper subset of the requested operations.  To detect
         this partial failure, get the current terminal attributes and
         compare them to the requested ones. */

      /* Initialize to all zeroes so there is no risk memcmp will report a
          spurious difference in an uninitialized portion of the structure. */
      memset (&new_mode, 0, sizeof (new_mode));
      if (tcgetattr (portId, &new_mode))
         TheApp->MessageBoxPerror(serialPort);

      /* Normally, one shouldn't use memcmp to compare structures that
         may have `holes' containing uninitialized data, but we have been
         careful to initialize the storage of these two variables to all
         zeroes.  One might think it more efficient simply to compare the
         modified fields, but that would require enumerating those fields --
         and not all systems have the same fields in this structure.  */

      if (memcmp (&termIoPort, &new_mode, sizeof (termIoPort)) != 0)
          {
#ifdef CIBAUD
          /* SunOS 4.1.3 (at least) has the problem that after this sequence,
             tcgetattr (&m1); tcsetattr (&m1); tcgetattr (&m2);
             sometimes (m1 != m2).  The only difference is in the four bits
             of the c_cflag field corresponding to the baud rate.  To save
             Sun users a little confusion, don't report an error if this
             happens.  But suppress the error only if we haven't tried to
             set the baud rate explicitly -- otherwise we'd never give an
             error for a true failure to set the baud rate.  */

          new_mode.c_cflag &= (~CIBAUD);
          if (memcmp (&termIoPort, &new_mode, sizeof (termIoPort)) != 0)
#endif
          fprintf(stderr,
                  " unable to perform all requested operations %s",
                  serialPort);
#else
    /* set Ascension FOB port parameters */
    termIoPort.c_ospeed = magicBaudRate;        /* out */
    termIoPort.c_ispeed = magicBaudRate;        /* in : not supported 
                                                   (in = out) */
    if ((ioctl(portId, TCSETA, &termIoPort)) == -1) {
        fprintf(stderr, "Error setting attributes for serial port %s:\n", 
                serialPort);
#ifdef HAVE_OSERROR
        fprintf(stderr,"oserror %d\n",oserror());
#endif
        TheApp->MessageBoxPerror(serialPort);
#endif
    }

#ifdef USE_STTY_LINUX
// when termios do not work 8-(

   mysnprintf(command,1023,
              "stty 38400 cs8 cread clocal min 0 time 1 -F /dev/ttyS%c",
              serialPort[strlen(serialPort)-1]);
   system(command);
#endif
#ifdef HAVE_AFLOCK_DEBUG
   // only used for debugging 
   // run "stty -a 
# ifdef OFFSET_SERIAL_DEVICE
   mysnprintf(command,1023,"stty -a F %s%c", SERIAL_DEVICE,
              serialPort[strlen(serialPort)-1]);
   system(command);
#  endif
#endif

    // return the portID
    return portId;
}

void Aflock::set_blocking( const int& port, const int& blocking )
{
    //////////////////////////////////////////////////////////////////
    // Setup a non/blocked port & Flush port
    //////////////////////////////////////////////////////////////////
    static int blockf,nonblock;
    int flags;

    flags = fcntl( port,F_GETFL,0 );

    // Turn blocking on
    blockf   = flags & ~FNDELAY;

    // Turn blocking off
    nonblock = flags | FNDELAY;

    // 0 Non Blocked
    // 1 Blocked
    fcntl( port, F_SETFL, blocking ? blockf : nonblock );

    usleep( 1000 * Aflock::mSleepFactor );
//    tcflush(port, TCIOFLUSH);
}

void Aflock::set_sync( const int& port, const int& sync )
{
    /////////////////////////////////////////////////////////////////
    // Set CRT sync: (manual page 82)
    //   set crt sync
    //   nosync    -   0
    //   > 72Hz    -   1 (type 1)
    //                 2 (type 2)
    /////////////////////////////////////////////////////////////////
    unsigned char buff[4] = {'A', 1};
    buff[1] = sync;
    while (write(port,buff,2) == -1);
    tcdrain(port);
}


void Aflock::set_hemisphere( const int& port,
                             const BIRD_HEMI& hem,
                             const int& transmitter )
{
    /////////////////////////////////////////////////////////////////
    // Set Hemisphere for birds taking input
    //
    //  buff   [1]   [2]
    // Front: 0x00, 0x00
    // Aft  : 0x00, 0x01
    // Upper: 0x0C, 0x01
    // Lower: 0x0C, 0x00
    // Left : 0x06, 0x01
    // Right: 0x06, 0x00
    /////////////////////////////////////////////////////////////////
    char buff[3];
    buff[0] = 'L';
    for (int i = 1; i <= getNumberDevices(); i++)
    {
        if (!deliverData(i))  
           continue;
        pickBird( i,port );
        switch (hem)
        {
        case FRONT_HEM:
            buff[1] = 0x00;
            buff[2] = 0x00;
            break;
        case AFT_HEM:
            buff[1] = 0x00;
            buff[2] = 0x01;
            break;
        case UPPER_HEM:
            buff[1] = 0x0C;
            buff[2] = 0x01;
            break;
        case LOWER_HEM:
            buff[1] = 0x0C;
            buff[2] = 0x00;
            break;
        case LEFT_HEM:
            buff[1] = 0x06;
            buff[2] = 0x01;
            break;
        case RIGHT_HEM:
            buff[1] = 0x06;
            buff[2] = 0x00;
            break;
        }
        while (write(port, buff, 3) == -1);

        usleep( 500 * Aflock::mSleepFactor );
        tcdrain(port);

    }
}

// Streaming mode currently not used
void Aflock::set_rep_and_stream(const int& port, const char& reportRate)
{
    char buff[1];
    /////////////////////////////////////////////////////////////////
    // Set report rate
    //             Q  Every cycle
    //  buff[0] - 'R' Every other bird cycle
    //             S  every 8 cycles
    //             T  every 32 cycles
    /////////////////////////////////////////////////////////////////
    buff[0] = reportRate;
    while (write(port, buff, 1) == -1);

    usleep( 2000 * Aflock::mSleepFactor );
    tcdrain(port);

    ////////////////////////////////////////////////////////////////
    // set stream mode
    ////////////////////////////////////////////////////////////////
    buff[0] = '@';
    while (write(port, buff, 1) == -1);

    usleep( 500 * Aflock::mSleepFactor );
    tcdrain(port);

}

void Aflock::set_pos_quat(const int& port, const int& transmitter)
{
    //////////////////////////////////////////////////////////////////
    // Set Position Quaternion
    /////////////////////////////////////////////////////////////////
    char buff[1];
    for (int i = 1; i <= getNumberDevices(); i++)
    {
        if (!deliverData(i))
            continue;
        Aflock::pickBird( i,port );

        buff[0] = ']';
        while (write(port, buff, 1) == -1);

        usleep( 500 * Aflock::mSleepFactor );
        tcdrain(port);

      AFLOCK_ERROR_CHECK(port,"Position/Quaternion command",transmitter);
    }
}

void Aflock::set_pos_angles(const int& port, const int& transmitter)
{
    //////////////////////////////////////////////////////////////////
    // Set Position Angles
    /////////////////////////////////////////////////////////////////
    char buff[1];
    for (int i = 1; i <= getNumberDevices(); i++)
    {
        if (!deliverData(i))
            continue;
        Aflock::pickBird( i,port );

        buff[0] = 'Y';
        while (write(port, buff, 1) == 1);

        usleep(500 * Aflock::mSleepFactor);
        tcdrain(port);

      AFLOCK_ERROR_CHECK(port,"Position/Angles command",transmitter);
    }
}

void Aflock::set_filter(const int& port, const BIRD_FILT& filter)
{
    // a filter value of AC_NARROW | AC_WIDE | DC_FILTER 
    // turns all filters on
    ///////////////////////////////////////////////////////////////
    // Turn filters on (manual page 48)
    // 0s turn AC NARROW notch filter ON
    //         AC WIDE notch filter ON
    //         DC filter ON
    ///////////////////////////////////////////////////////////////
#ifdef AFLOCK_DEBUG
    fprintf(stderr,"Aflock: changing filters !\n");
#endif
    for (int i = 1; i <= getNumberDevices(); i++)
    {
       if (!deliverData(i))
          continue;
       pickBird( i,port );
       char buff[4];
       buff[0] = 'P';
       buff[1] = (~filter) & 7;
       buff[2] = 0x00;
       buff[3] = 0;
       while(write(port, buff, 4) == -1);

       //TODO: Do I need to sleep here?
       usleep(12000 * Aflock::mSleepFactor);
       tcdrain(port);
    }
}

void Aflock::set_transmitter(const int& port, const int& transmitter)
{
    ///////////////////////////////////////////////////////////////
    // Sets up the device for Transmitting (manual page 67)
    // Command (0x30) for Next Transmitter
    ///////////////////////////////////////////////////////////////
    char buff[2];
    buff[0] = (unsigned char) (0x30);
    buff[1] = (unsigned char) transmitter  << 4;
    while(write(port, buff, 2) == -1);

    //TODO: Do I need to sleep here?
    usleep(12000 * Aflock::mSleepFactor);
    tcdrain(port);
}


void Aflock::set_autoconfig(const int& port)
{
    ///////////////////////////////////////////////////////////////
    // FBB AUTO-CONFIGURATION (manual page 60)
    //
    // Must wait 300 milliseconds before and after this command
    ///////////////////////////////////////////////////////////////

    pickBird(1,port);

    usleep(100000);

    char buff[3];
    buff[0] = 'P';
    buff[1] = 0x32;
    buff[2] = getNumberDevices();
    while(write(port, buff,3) == -1);

    usleep(100000);

    tcdrain(port);

#ifdef HAVE_AFLOCK_DEBUG
    int i=0;

    pickBird(1,port);

    buff[0] = 'O';
    buff[1] = 0x32;
    while (write(port, buff,2);

    tcdrain(port);

    // TODO: we should know here, what's the current adressing mode     

    usleep(100000);

    // we are in normal adressing mode, autoconfig command will return 5 byte
    const int infolength=5;
    char info[infolength];

    int numbytes;
    do {
       numbytes=read(port, info, infolength-i);
       if (numbytes<=0)
          {
          fprintf(stderr,"Aflock: cant get 5 bytes from autoconfig command:");
          fprintf(stderr,"Aflock running in normal mode ?\n");
          fprintf(stderr,"aflock: got %d bytes instead of 5\n",i);
          break;
          }
       } while ((i+=numbytes)<infolength);
    if (i==5) 
       {
       if (info[0]==0)
          fprintf(stderr,"Aflock: in standalone mode\n");
       else if (info[0]==1)
          fprintf(stderr,"Aflock: in transmitter/multiple sensors mode\n");
       else
          fprintf(stderr,"Aflock: unexpected returnvalue from auto-config\n");
       for (i=1;i<8;i++)
          if ( (info[1]&&(1<<i)) == 0)
             fprintf(stderr,"Aflock: device %d not running\n",i);
          else
             fprintf(stderr,"Aflock: device %d is running\n",i);
       for (i=0;i<7;i++)
          if ( (info[2]&&(1<<i)) == 0)
             fprintf(stderr,"Aflock: device %d not running\n",i+8);
          else
             fprintf(stderr,"Aflock: device %d is running\n",i+8);
       }
    usleep(100000);
#endif
}

/// not used 
void Aflock::set_group(const int& port)
{
    ////////////////////////////////////////////////////////////////
    // Setup group mode: (manual page 59)
    // 'P' Change Parameter
    // Number 35 (hex 23),
    // Set flag to 1 enabling group mode.
    ////////////////////////////////////////////////////////////////
    char buff[3];
    buff[0] = 'P';
    buff[1] = 0x23;
    buff[2] = 0x01;
    while(write(port, buff, 3) == -2);
    tcdrain(port);
    usleep(100000);
}

void Aflock::set_sudden_output_change_lock(const int& port,bool lock)
{
    ////////////////////////////////////////////////////////////////
    // Set sudden output mode: (manual page ?)
    // 'P' Change Parameter
    // Number 14 (hex 0e),
    // Set flag to 1/0 to lock/unlock sudden output change
    ////////////////////////////////////////////////////////////////
    for (int i = 1; i <= getNumberDevices(); i++)
    {
       if (!deliverData(i))
          continue;
       pickBird( i,port );
       char buff[3];
       buff[0] = 'P';
       buff[1] = 0x0e;
       if (lock)
          buff[2] = 0x01;
       else
          buff[2] = 0x00;
       while(write(port, buff, 3)==-1);

       usleep(100000);
       tcdrain(port);
    }
}



#else
// dummy function, cause some compilers do not like empty inputfiles....
static void Aflock_dummy(void) {}
#endif

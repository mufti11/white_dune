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
 * File:          aFlock.h,v
 * Date modified: 2000/10/30 16:06:01
 * Version:       1.21
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


//===============================================================
// Aflock
//
// Purpose:
//      Ascention Flock of birds tracking class
//
// Author:
//      Kevin Meinert
//      modified for white_dune by J. "MUFTI" Scheurich 
//      using things from HeadTracker/I3Stick API by J. Dean Brederson
//
//      the streaming mode of the orginal VRjuggler Ascention Flock of birds 
//      tracking class seams to be not a good idea for white_dune...
//
//      TODO: maybe you should not trust calfile/initCorrectionTable....
// 
// Last Modified: Oct 11 2002

// white_dune assumes the following configuration:

// FOBs with single RS232 Interface to Host Computer 
// (see "The flock of Birds, Installation and Operation Guide, Standalone
//  and Multiple Transmitter/Multiple Sensors Configurations", Page 3
//  (Capture "Introduction"), Figure 2)

// +-------------+
// | Users Host  |
// | Computer    |
// |             |
// |  RS232      |
// +----+--------+
//      |           +----------------+----------------+------------
//      |           |                |                |
//      |           |                |                |
//  +---+-----------+------+   +-----+-----+    +-----+-----+
//  | RS232        FBB     |   |    FBB    |    |    FBB    |
//  |                      |   |           |    |           |
//  | Master (transmitter) |   |  Slave 1  |    |  Slave 2  |
//  |                      |   |           |    |           |
//  | 1. unit (fobadress)  |   |  2. unit  |    |  3. unit  | 
//  +----------------------+   +-----------+    +-----------+

//===============================================================
#ifndef _ASCENSION_FLOCKOFBIRD_H_
#define _ASCENSION_FLOCKOFBIRD_H_

//do not use
//#define USE_AFLOCK_QUATERNION 1

// standard range transmitter: 36 or 72 INCH (1 INCH = 0.0254 meter)
// #define POSITION_RANGE 36.0*0.0254
// #define POSITION_RANGE 72.0*0.0254

// extended range transmitter: 144 INCH (1 INCH = 0.0254 meter)
#define POSITION_RANGE 144.0*0.0254

#define ANGLE_RANGE 180.0f
#define MAX_SENSORS 128

// number of accectable READ (no data) errors 
#define MAX_READ_ERRORS 100

// error timeout to wait between drain and flush (in usleep units)
#define ERROR_TIMEOUT 1000

/*  #ifndef _BOOL */
/*  typedef bool int; */
/*  #define true 1 */
/*  #define false 0 */
/*  #endif */

typedef struct {
        float xmin, ymin, zmin;
        float xmax, ymax, zmax;
        float xloc[15][10][15],yloc[15][10][15],zloc[15][10][15];
} CalStruct;

enum BIRD_HEMI  {FRONT_HEM, AFT_HEM, UPPER_HEM, LOWER_HEM, LEFT_HEM, RIGHT_HEM};
typedef int BIRD_FILT;
#define AC_NARROW 4
#define AC_WIDE 2
#define DC_FILTER 1
#define FILTER_DEFAULT 0

//: class for running a Flock of Birds.
//  Aflock is a positional device driver for the Flock of Birds <br>
//!PUBLIC_API
class Aflock
{
public:
        //: Configure Constructor
        //! ARGS: port - such as "/dev/ttyd3"                         <BR>
        //! ARGS: baud - such as 38400, 19200, 9600, 14400, etc...    <BR>
        //! ARGS: sync - sync type.                                   <BR>
        //! ARGS: block - blocking                                    <BR>
        //! ARGS: numBrds - number of birds in flock                  <BR>
        //                  (without transmitter !)                   <BR>
        //! ARGS: transmit - transmitter unit number,                 <BR>
        //! ARGS: hemi - hemisphere to track from,                    <BR>
        //! ARGS: filt - filtering type,                              <BR>
        //! ARGS: report - flock report rate.                         <BR>
        //! ARGS: calfile - a calibration file, if "", then use none. <BR>
        //                                                       <BR>
        //! POST: configures internal data members,
        //+         doesn't actually talk to the FOB yet.
        Aflock(const char* const port = SERIAL_DEVICE,
                const int& baud = 38400,
                const int& sync = 0,
                const int& block = 0,
                const int& numBrds = 3,
                const int& transmit = 1,
                const BIRD_HEMI& hemi = RIGHT_HEM,
                const BIRD_FILT& filt = FILTER_DEFAULT,
                const bool& sudden_lock = true,
                const char& report = 'R',
                const char* const calfile = "");
        
        
        //: Destructor
        ~Aflock();

        
        //: call this to connect to the flock device.
        //  NOTE: flock.isActive() must be false to use this function
        int start();
        
        //: stop the flock
        int stop();
        
        //: call this repeatedly to request data from the birds
        //  get the data later with flock.sample()
        //  NOTE: flock.isActive() must be true to use this function
        int prepareSample();

        //: call this repeatedly to read the data from the birds.
        //  NOTE: flock.isActive() must be true to use this function
        int sample();
               
        //: destroy remaining dirt on the port to recover from errors
        void destroyDirt();
                
        //: see if the flock is active or not
        const bool& isActive() const;

        //: set the port to use
        //  this will be a string in the form of the native OS descriptor <BR>
        //  ex: unix - "/dev/ttyd3", win32 - "COM3" <BR>
        //  NOTE: flock.isActive() must be false to use this function
        void setPort( const char* const serialPort );
        
        //: get the port used
        //  this will be a string in the form of the native OS descriptor <BR>
        //  ex: unix - "/dev/ttyd3", win32 - "COM3"
        const char* getPort() const;
        
        
        //: set the baud rate
        //  this is generally 38400, consult flock manual for other rates. <BR>
        //  NOTE: flock.isActive() must be false to use this function
        void setBaudRate( const int& baud );

        //: get the baud rate
        //  this is generally 38400, consult flock manual for other rates. <BR>
        inline const int&  getBaudRate()  const { return m_baud;}

        //: Set the unit number of the transmitter
        //  give - an integer that is the same as the dip switch
        //         setting on the transmitter box (for the unit number) <BR>
        //  NOTE: flock.isActive() must be false to use this function
        void setTransmitter( const int& Transmit );
        
        //: Get the unit number of the transmitter
        //  returns - an integer that is the same as the dip switch
        //         setting on the transmitter box (for the unit number) <BR>
        inline const int&  getTransmitter() const { return m_xmitterUnitNumber; }


        //: Set the number of birds to use in the flock.
        //  give - an integer number not more than the number of
        //         birds attached to the system <BR>
        //  NOTE: flock.isActive() must be false to use this function
        void setNumBirds( const int& n );
        
        //: Get the number of birds to use in the flock.
        //  returns - an integer number not more than the number of
        //         birds attached to the system <BR>
        inline const int&  getNumBirds() const { return m_numBirds; }


        //: set the video sync type
        //  this option allows the Flock to syncronize its pulses with
        //  your video display.  This will eliminate most flicker caused
        //  by the magnetic distortion. <BR>
        //  - Refer to your flock manual for what number to use.
        //  NOTE: flock.isActive() must be false to use this function
        void setSync( const int& sync );
        
        //: Get the video sync type
        //  this option allows the Flock to syncronize its pulses with
        //  your video display.  This will eliminate most flicker caused
        //  by the magnetic distortion. <BR>
        //  - Refer to your flock manual for what the return number means
        inline const int&  getSync() const {return m_syncStyle; }


        //: Set blocking of flock
        //  see flock manual for details.
        //  NOTE: flock.isActive() must be false to use this function
        void setBlocking( const int& blVal );
        
        //: Get blocking flock's blocking type
        //  see flock manual for details.
        inline const int&  getBlocking() const { return m_blocking; }


        //: Set the type of filtering that the flock uses
        //  NOTE: flock.isActive() must be false to use this function
        void setFilterType( const BIRD_FILT& f );
        
        //: Set the type of filtering that the flock uses
        inline const BIRD_FILT&  getFilterType() const { return m_filter; }

        //: Set if sudden output changes of a bird are locked
        //  NOTE: flock.isActive() must be false to use this function
        void setSuddenOutputChangeLock( const bool& b );
        
        //: Set the type of filtering that the flock uses
        inline const bool&  getSuddenOutputChangeLock() const 
                    { return m_sudden_output_change_lock; }


        //: Set the hemisphere that the transmitter transmits from.
        //  NOTE: flock.isActive() must be false to use this function
        void setHemisphere( const BIRD_HEMI& h );
        
        //: Set the hemisphere that the transmitter transmits from.
        inline const BIRD_HEMI&  getHemisphere() const {return m_hemisphere; }


        //: Set the report rate that the flock uses
        //  NOTE: flock.isActive() must be false to use this function
        void setReportRate( const char& rRate );

        //: Set the report rate that the flock uses
        inline const char& getReportRate() const {return m_reportRate; }


        //: get the x position of the i'th bird
        float& xPos( const int& i );
        
        //: get the y position of the i'th bird
        float& yPos( const int& i );
        
        //: get the z position of the i'th bird
        float& zPos( const int& i );
        
#ifdef USE_AFLOCK_QUATERNION
        //: get the rotation of the i'th bird as quaternion
        float& xQuat( const int& i );
        float& yQuat( const int& i );
        float& zQuat( const int& i );
        float& wQuat( const int& i );
#else
        //: get the z rotation of the i'th bird
        float& zRot( const int& i );
        
        //: get the y rotation of the i'th bird
        float& yRot( const int& i );
        
        //: get the x rotation of the i'th bird
        float& xRot( const int& i );
#endif

// public methods
public:
        //: with the calibration table info, correct a given position
        //  give - a position in x, y, z euclidian coordinates
        //  returns - a corrected position in x, y, z euclidian coordinates
        void positionCorrect( float& x, float& y, float& z );
        
        //: init the correction table from a file
        //  give - a file name of the calibration file.
        //  result - initializes the correction table with the file's info
        void initCorrectionTable( const char* const );

        //: get number of devices 
        //  (including master, if master is use a receiver)
        int getNumberDevices(void) const;

        //: set that the master ("transmitter") deliver data
        void        setMasterUseReceiver(void) { m_masterUseReceiver=true; }

        bool        deliverData(int birdaddr);

// Private data members
private:
        CalStruct   caltable;
        char        m_reportRate;
        BIRD_HEMI   m_hemisphere;
        BIRD_FILT   m_filter;
        bool        m_sudden_output_change_lock;
        
        static const int   MAXCHARSTRINGSIZE;
        char m_port[256];
        char m_calibrationFileName[256];
        
        int m_portId;
        int m_baud;
        int m_syncStyle;
        int m_blocking;
        int m_numBirds;
        int m_xmitterUnitNumber;
        bool m_usingCorrectionTable;
   
        unsigned char* m_groupbuffer;
        int m_grouplength;

        bool        m_samplePrepared;

        // do the master ("transmitter") deliver data ?
        // a ERC as master do not deliver data
        // a "normal Bird unit" as master do deliver data
        bool        m_masterUseReceiver;
        
        // x,y,z       
        float m_position[MAX_SENSORS][3];
#ifdef USE_AFLOCK_QUATERNION
        // x,y,z,w
        float m_orientation[MAX_SENSORS][4];
#else
        // r,y,p
        float m_orientation[MAX_SENSORS][3];
#endif
        
        bool m_active;
        
// bird functions.
private:        
        float rawToFloat(float factor, unsigned char& r1, unsigned char& r2);
        float rawToQuat(unsigned char& r1, unsigned char& r2);
        void  pickBird(const int& sensor, const int& port_id);
        int   open_port(const char* const serialPort, const int& baud, int& port_id );
        void  set_blocking(const int& port, const int& blocking);
        void  set_sync(const int& port, const int& sync);
        void  set_hemisphere(const int& port, const BIRD_HEMI& hem, const int& transmitter);
        void  set_rep_and_stream(const int& port, const char& repRate);
        void  set_pos_angles(const int& port, const int& transmitter);
        void  set_pos_quat(const int& port, const int& transmitter);
        void  set_filter(const int& port, const BIRD_FILT& filter);
        void  set_transmitter(const int& port, const int& transmitter);
        void  set_autoconfig(const int& port);
        void  set_group(const int& port);
        void  set_sudden_output_change_lock(const int& port,bool lock);
        static const int mSleepFactor;
};

#ifdef HAVE_AFLOCK_DEBUG
#define AFLOCK_PRINT(message) fprintf(stderr,message)
#else
#define AFLOCK_PRINT(message)   
#endif

#endif

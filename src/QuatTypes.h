/*
   from http://www1.acm.org/pubs/tog/GraphicsGems/index.html

   This is the official on-line repository for the code from the 
   Graphics Gems series of books (from Academic Press). This series
   focusses on short to medium length pieces of code which perform a 
   wide variety of computer graphics related tasks. All code
   here can be used without restrictions. The code distributions here 
   contain all known bug fixes and enhancements. We also
   provide errata listings for the text of each book. Please report any 
   new errata or bugs to Eric Haines (erich@acm.org). 

   http://www1.acm.org/pubs/tog/GraphicsGems/gemsiv/euler_angle/
*/
/**** QuatTypes.h - Basic type declarations ****/
#ifndef _H_QuatTypes
#define _H_QuatTypes
/*** Definitions ***/
#include "Quaternion.h"
typedef Quaternion Quat;
enum QuatPart {X, Y, Z, W};
typedef float HMatrix[4][4]; /* Right-handed, for column vectors */
typedef Quat EulerAngles;    /* (x,y,z)=ang 1,2,3, w=order code  */
#endif

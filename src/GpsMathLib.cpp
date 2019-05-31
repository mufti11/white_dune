/********************************************************************
** @source JEEPS functions
**
** @author Copyright (C) 1999 Alan Bleasby
** @version 1.0 
** @modified Feb 04 2000 Alan Bleasby. First version
** @@
** 
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
** 
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
** 
** You should have received a copy of the GNU Library General Public
** License along with this library; if not, write to the
** Free Software Foundation, Inc., 59 Temple Place - Suite 330,
** Boston, MA  02111-1307, USA.
********************************************************************/

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "gpsdatum.h"
#include "gpsmath.h"

//BOBMAX INSERT--------------------------------------------------------------------




/* @func GPS_Math_Deg_To_DegMin *****************************************
**
** Convert degrees to degrees and minutes
**
** @param [r] v [double] degrees
** @param [w] d [int32 *]  whole degrees
** @param [w] m [double *] minutes
**
** @return [void]
************************************************************************/

void GPS_Math_Deg_To_DegMin(double v, int32 *d, double *m)
{
    int32 sign;
    
    if(v<(double)0.)
    {
        v *= (double)-1.;
        sign = 1;
    }
    else
        sign = 0;
    
    *d = (int32)v;
    *m = (v-(double)*d) * (double)60.0;
    if(*m>(double)59.999)
    {
        ++*d;
        *m = (double)0.0;
    }

    if(sign)
        *d = -*d;

    return;
}



/* @func GPS_Math_DegMin_To_Deg *****************************************
**
** Convert degrees and minutes to degrees
**
** @param [r] d [int32] whole degrees
** @param [r] m [double] minutes
** @param [w] deg [double *] degrees
**
** @return [void]
************************************************************************/

void GPS_Math_DegMin_To_Deg(int32 d, double m, double *deg)
{

    *deg = ((double)abs(d)) + m / (double)60.0;
    if(d<0)
        *deg = -*deg;

    return;
}



/* @func GPS_Math_Deg_To_DegMinSec *************************************
**
** Convert degrees to degrees, minutes and seconds
**
** @param [r] v [double] degrees
** @param [w] d [int32 *]  whole degrees
** @param [w] m [int32 *]  whole minutes
** @param [w] s [double *] seconds
**
** @return [void]
************************************************************************/

void GPS_Math_Deg_To_DegMinSec(double v, int32 *d, int32 *m, double *s)
{
    int32 sign;
    double t;
    
    if(v<(double)0.)
    {
        v *= (double)-1.;
        sign = 1;
    }
    else
        sign = 0;
    
    *d = (int32)v;
    t  = (v -(double)*d) * (double)60.0;
    *s = (t-(double)*m) * (double)60.0;

    if(*s>(double)59.999)
    {
        ++t;
        *s = (double)0.0;
    }

    
    if(t>(double)59.999)
    {
        ++*d;
        t = 0;
    }

    *m = (int32)t;

    if(sign)
        *d = -*d;

    return;
}



/* @func GPS_Math_DegMinSec_To_Deg *****************************************
**
** Convert degrees, minutes and seconds to degrees
**
** @param [r] d [int32] whole degrees
** @param [r] m [int32] whole minutes
** @param [r] s [double] seconds
** @param [w] deg [double *] degrees
**
** @return [void]
************************************************************************/

void GPS_Math_DegMinSec_To_Deg(int32 d, int32 m, double s, double *deg)
{

    *deg = ((double)abs(d)) + ((double)m + s / (double)60.0) / (double)60.0;
    if(d<0)
        *deg = -*deg;

    return;
}



/* @func GPS_Math_Metres_To_Feet *******************************************
**
** Convert metres to feet 
**
** @param [r] v [double] metres
**
** @return [double] feet
************************************************************************/

double GPS_Math_Metres_To_Feet(double v)
{
    return v*(double)2.7432;
}



/* @func GPS_Math_Feet_To_Metres *******************************************
**
** Convert feet to metres
**
** @param [r] v [double] feet
**
** @return [double] metres
************************************************************************/

double GPS_Math_Feet_To_Metres(double v)
{
    return v/(double)2.7432;
}



/* @func GPS_Math_Deg_To_Semi *******************************************
**
** Convert degrees to semi-circles
**
** @param [r] v [double] degrees
**
** @return [int32] semicircles
************************************************************************/

int32 GPS_Math_Deg_To_Semi(double v)
{
    return (int32) (((double)2.147483e9/(double)180)*(double)v);
}



/* @func GPS_Math_Semi_To_Deg *******************************************
**
** Convert semi-circles to degrees
**
** @param [r] v [int32] semi-circles
**
** @return [double] degrees
************************************************************************/

double GPS_Math_Semi_To_Deg(int32 v)
{
    return (double) (((double)v/(double)2.147483e9) * (double)180);
}



/* @func GPS_Math_Utime_To_Gtime *******************************************
**
** Convert Unix time (1970) to GPS time (1990)
**
** @param [r] v [time_t] Unix time
**
** @return [time_t] GPS time
************************************************************************/

time_t GPS_Math_Utime_To_Gtime(time_t v)
{
    return v-631065600;
}



/* @func GPS_Math_Gtime_To_Utime *******************************************
**
** Convert GPS time (1990) to Unix time (1970)
**
** @param [r] v [time_t] GPS time
**
** @return [time_t] Unix time
************************************************************************/

time_t GPS_Math_Gtime_To_Utime(time_t v)
{
    return v+631065600;
}








/* @func GPS_Math_XYZ_To_LatLonH ***************************************
**
** Convert XYZ coordinates to latitude and longitude and ellipsoidal height
**
** @param [w] phi [double] latitude (deg)
** @param [w] lambda [double] longitude (deg)
** @param [w] H [double] ellipsoidal height (metres)
** @param [r] x [double *] X
** @param [r] y [double *] Y
** @param [r] z [double *] Z
** @param [r] a [double] semi-major axis (metres)
** @param [r] b [double] semi-minor axis (metres)
**
** @return [void]
************************************************************************/
void GPS_Math_XYZ_To_LatLonH(double *phi, double *lambda, double *H,
                             double x, double y, double z,
                             double a, double b)
{
    double esq;
    double nu=0.0;
    double phix;
    double nphi;
    double rho;
    int32    t1=0;
    int32    t2=0;


    if(x<(double)0 && y>=(double)0) t1=1;
    if(x<(double)0 && y<(double)0)  t2=1;

    rho  = pow(((x*x)+(y*y)),(double)0.5);
    esq  = ((a*a)-(b*b)) / (a*a);
    phix = atan(z/(((double)1.0 - esq) * rho));
    nphi = (double)1e20;
    
    while(fabs(phix-nphi)>(double)0.00000000001)
    {
        nphi  = phix;
        nu    = a / pow(((double)1.0-esq*sin(nphi)*sin(nphi)),(double)0.5);
        phix  = atan((z+esq*nu*sin(nphi))/rho);
    }

    
    *phi    = GPS_Math_Rad_To_Deg(phix);
    *H      = (rho / cos(phix)) - nu;
    *lambda = GPS_Math_Rad_To_Deg(atan(y/x));

    if(t1) *lambda += (double)180.0;
    if(t2) *lambda -= (double)180.0;

    return;
}



/* @func GPS_Math_Airy1830LatLonH_To_XYZ **********************************
**
** Convert Airy 1830 latitude and longitude and ellipsoidal height to
** X, Y & Z coordinates
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [r] H [double] ellipsoidal height (metres)
** @param [w] x [double *] X
** @param [w] y [double *] Y
** @param [w] z [double *] Z
**
** @return [void]
************************************************************************/
void GPS_Math_Airy1830LatLonH_To_XYZ(double phi, double lambda, double H,
                                     double *x, double *y, double *z)
{
    double a = 6377563.396;
    double b = 6356256.910;

    GPS_Math_LatLonH_To_XYZ(phi,lambda,H,x,y,z,a,b);

    return;
}


/* @func GPS_Math_XYZ_To_Airy1830LatLonH **********************************
**
** Convert XYZ to Airy 1830 latitude and longitude and ellipsoidal height
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [r] H [double] ellipsoidal height (metres)
** @param [w] x [double *] X
** @param [w] y [double *] Y
** @param [w] z [double *] Z
**
** @return [void]
************************************************************************/
void GPS_Math_XYZ_To_Airy1830LatLonH(double *phi, double *lambda, double *H,
                                     double x, double y, double z)
{
    double a = 6377563.396;
    double b = 6356256.910;

    GPS_Math_XYZ_To_LatLonH(phi,lambda,H,x,y,z,a,b);

    return;
}



/* @func GPS_Math_XYZ_To_WGS84LatLonH **********************************
**
** Convert XYZ to WGS84 latitude and longitude and ellipsoidal height
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [r] H [double] ellipsoidal height (metres)
** @param [w] x [double *] X
** @param [w] y [double *] Y
** @param [w] z [double *] Z
**
** @return [void]
************************************************************************/
void GPS_Math_XYZ_To_WGS84LatLonH(double *phi, double *lambda, double *H,
                                  double x, double y, double z)
{
    double a = 6378137.000;
    double b = 6356752.314;


    GPS_Math_XYZ_To_LatLonH(phi,lambda,H,x,y,z,a,b);

    return;
}

    



/* @func GPS_Math_Airy1830MLatLonToINGEN ************************************
**
** Convert Modified Airy 1830  datum latitude and longitude to Irish
** National Grid Eastings and Northings
**
** @param [r] phi [double] modified Airy latitude     (deg)
** @param [r] lambda [double] modified Airy longitude (deg)
** @param [w] E [double *] NG easting (metres)
** @param [w] N [double *] NG northing (metres)
**
** @return [void]
************************************************************************/
void GPS_Math_Airy1830M_LatLonToINGEN(double phi, double lambda, double *E,
                                      double *N)
{
    double N0      =  250000;
    double E0      =  200000;
    double F0      = 1.000035;
    double phi0    = 53.5;
    double lambda0 = -8.;
    double a       = 6377340.189;
    double b       = 6356034.447;

    GPS_Math_LatLon_To_EN(E,N,phi,lambda,N0,E0,phi0,lambda0,F0,a,b);

    return;
}




/* @func GPS_Math_Airy1830LatLonToNGEN **************************************
**
** Convert Airy 1830 datum latitude and longitude to UK Ordnance Survey
** National Grid Eastings and Northings
**
** @param [r] phi [double] WGS84 latitude     (deg)
** @param [r] lambda [double] WGS84 longitude (deg)
** @param [w] E [double *] NG easting (metres)
** @param [w] N [double *] NG northing (metres)
**
** @return [void]
************************************************************************/
void GPS_Math_Airy1830LatLonToNGEN(double phi, double lambda, double *E,
                                   double *N)
{
    double N0      = -100000;
    double E0      =  400000;
    double F0      = 0.9996012717;
    double phi0    = 49.;
    double lambda0 = -2.;
    double a       = 6377563.396;
    double b       = 6356256.910;

    GPS_Math_LatLon_To_EN(E,N,phi,lambda,N0,E0,phi0,lambda0,F0,a,b);

    return;
}








/* @func GPS_Math_NGENToAiry1830LatLon **************************************
**
** Convert  to UK Ordnance Survey National Grid Eastings and Northings to
** Airy 1830 datum latitude and longitude
**
** @param [r] E [double] NG easting (metres)
** @param [r] N [double] NG northing (metres)
** @param [w] phi [double *] Airy latitude     (deg)
** @param [w] lambda [double *] Airy longitude (deg)
**
** @return [void]
************************************************************************/
void GPS_Math_NGENToAiry1830LatLon(double E, double N, double *phi,
                                   double *lambda)
{
    double N0      = -100000;
    double E0      =  400000;
    double F0      = 0.9996012717;
    double phi0    = 49.;
    double lambda0 = -2.;
    double a       = 6377563.396;
    double b       = 6356256.910;

    GPS_Math_EN_To_LatLon(E,N,phi,lambda,N0,E0,phi0,lambda0,F0,a,b);
    
    return;
}



/* @func GPS_Math_INGENToAiry1830MLatLon **************************************
**
** Convert Irish National Grid Eastings and Northings to modified
** Airy 1830 datum latitude and longitude
**
** @param [r] E [double] ING easting (metres)
** @param [r] N [double] ING northing (metres)
** @param [w] phi [double *] modified Airy latitude     (deg)
** @param [w] lambda [double *] modified Airy longitude (deg)
**
** @return [void]
************************************************************************/
void GPS_Math_INGENToAiry1830MLatLon(double E, double N, double *phi,
                                     double *lambda)
{
    double N0      =  250000;
    double E0      =  200000;
    double F0      = 1.000035;
    double phi0    = 53.5;
    double lambda0 = -8.;
    double a       = 6377340.189;
    double b       = 6356034.447;

    GPS_Math_EN_To_LatLon(E,N,phi,lambda,N0,E0,phi0,lambda0,F0,a,b);
    
    return;
}



/* @func GPS_Math_EN_To_UKOSNG_Map *************************************
**
** Convert Airy 1830 eastings and northings to Ordnance Survey map
** two letter code plus modified eastings and northings
**
** @param [r] E [double] NG easting (metres)
** @param [r] N [double] NG northing (metres)
** @param [w] mE [double *] modified easting (metres)
** @param [w] mN [double *] modified northing (metres)
** @param [w] map [char *] map code
**
** @return [int32] success
************************************************************************/
int32 GPS_Math_EN_To_UKOSNG_Map(double E, double N, double *mE,
                                double *mN, char *map)
{
    int32  t;
    int32  idx;
    
    if(E>=(double)700000. || E<(double)0.0 || N<(double)0.0 ||
       N>=(double)1300000.0)
        return 0;

    idx = ((int32)N/100000)*7 + (int32)E/100000;
    (void) strcpy(map,UKNG[idx]);
    
    t = ((int32)E / 100000) * 100000;
    *mE = E - (double)t;

    t = ((int32)N / 100000) * 100000;
    *mN = N - (double)t;
    
    return 1;
}



/* @func GPS_Math_UKOSNG_Map_To_EN *************************************
**
** Convert Ordnance Survey map eastings and northings plus
** two letter code to Airy 1830 eastings and northings
**
** @param [w] map [char *] map code
** @param [r] mapE [double] easting (metres)
** @param [r] mapN [double] northing (metres)
** @param [w] E [double *] full Airy easting (metres)
** @param [w] N [double *] full Airy northing (metres)

**
** @return [int32] success
************************************************************************/
int32 GPS_Math_UKOSNG_Map_To_EN(char *map, double mapE, double mapN, double *E,
                                double *N)
{
    int32  t;
    int32  idx;
    
    if(mapE>=(double)100000.0 || mapE<(double)0.0 || mapN<(double)0.0 ||
       mapN>(double)100000.0)
        return 0;

    idx=0;
    while(*UKNG[idx])
    {
        if(!strcmp(UKNG[idx],map)) break;
        ++idx;
    }
    if(!*UKNG[idx])
        return 0;
    

    t = (idx / 7) * 100000;
    *N = mapN + (double)t;

    t = (idx % 7) * 100000;
    *E = mapE + (double)t;

    return 1;
}










/* @func GPS_Math_Known_Datum_To_WGS84_C **********************************
**
** Transform datum to WGS84 using Cartesian coordinates
**
** @param [r] Sphi [double] source latitude (deg)
** @param [r] Slam [double] source longitude (deg)
** @param [r] SH   [double] source height  (metres)
** @param [w] Dphi [double *] dest latitude (deg)
** @param [w] Dlam [double *] dest longitude (deg)
** @param [w] DH   [double *] dest height  (metres)
** @param [r] n    [int32] datum number from GPS_Datum structure
**
** @return [void]
************************************************************************/
void GPS_Math_Known_Datum_To_WGS84_C(double Sphi, double Slam, double SH,
                                     double *Dphi, double *Dlam, double *DH,
                                     int32 n)
{
    double Sa;
    double Sif;
    double Sb;
    double Da;
    double Dif;
    double Db;
    double x;
    double y;
    double z;
    int32    idx;
    double sx;
    double sy;
    double sz;
    
    Da  = (double) 6378137.0;
    Dif = (double) 298.257223563;
    Db  = Da - (Da / Dif);
    
    idx  = GPS_Datum[n].ellipse;
    Sa   = GPS_Ellipse[idx].a;
    Sif  = GPS_Ellipse[idx].invf;
    Sb   = Sa - (Sa / Sif);
    
    x    = GPS_Datum[n].dx;
    y    = GPS_Datum[n].dy;
    z    = GPS_Datum[n].dz;

    GPS_Math_LatLonH_To_XYZ(Sphi,Slam,SH,&sx,&sy,&sz,Sa,Sb);
    sx += x;
    sy += y;
    sz += z;
    
    GPS_Math_XYZ_To_LatLonH(Dphi,Dlam,DH,sx,sy,sz,Da,Db);

    return;
}



/* @func GPS_Math_WGS84_To_Known_Datum_C ********************************
**
** Transform WGS84 to other datum using Cartesian coordinates
**
** @param [r] Sphi [double] source latitude (deg)
** @param [r] Slam [double] source longitude (deg)
** @param [r] SH   [double] source height  (metres)
** @param [w] Dphi [double *] dest latitude (deg)
** @param [w] Dlam [double *] dest longitude (deg)
** @param [w] DH   [double *] dest height  (metres)
** @param [r] n    [int32] datum number from GPS_Datum structure
**
** @return [void]
************************************************************************/
void GPS_Math_WGS84_To_Known_Datum_C(double Sphi, double Slam, double SH,
                                     double *Dphi, double *Dlam, double *DH,
                                     int32 n)
{
    double Sa;
    double Sif;
    double Da;
    double Dif;
    double x;
    double y;
    double z;
    int32    idx;
    double Sb;
    double Db;
    double dx;
    double dy;
    double dz;
    
    Sa  = (double) 6378137.0;
    Sif = (double) 298.257223563;
    Sb   = Sa - (Sa / Sif);
    
    idx  = GPS_Datum[n].ellipse;
    Da   = GPS_Ellipse[idx].a;
    Dif  = GPS_Ellipse[idx].invf;
    Db  = Da - (Da / Dif);

    x    = -GPS_Datum[n].dx;
    y    = -GPS_Datum[n].dy;
    z    = -GPS_Datum[n].dz;

    GPS_Math_LatLonH_To_XYZ(Sphi,Slam,SH,&dx,&dy,&dz,Sa,Sb);
    dx += x;
    dy += y;
    dz += z;

    GPS_Math_XYZ_To_LatLonH(Dphi,Dlam,DH,dx,dy,dz,Da,Db);

    return;
}



/* @func GPS_Math_Known_Datum_To_Known_Datum_M *************************
**
** Transform WGS84 to other datum using Molodensky
**
** @param [r] Sphi [double] source latitude (deg)
** @param [r] Slam [double] source longitude (deg)
** @param [r] SH   [double] source height  (metres)
** @param [w] Dphi [double *] dest latitude (deg)
** @param [w] Dlam [double *] dest longitude (deg)
** @param [w] DH   [double *] dest height  (metres)
** @param [r] n1   [int32] source datum number from GPS_Datum structure
** @param [r] n2   [int32] dest   datum number from GPS_Datum structure
**
** @return [void]
************************************************************************/
void GPS_Math_Known_Datum_To_Known_Datum_M(double Sphi, double Slam, double SH,
                                           double *Dphi, double *Dlam,
                                           double *DH, int32 n1, int32 n2)
{
    double Sa;
    double Sif;
    double Da;
    double Dif;
    double x1;
    double y1;
    double z1;
    double x2;
    double y2;
    double z2;
    double x;
    double y;
    double z;
    
    int32    idx1;
    int32    idx2;
    
    
    idx1 = GPS_Datum[n1].ellipse;
    Sa   = GPS_Ellipse[idx1].a;
    Sif  = GPS_Ellipse[idx1].invf;
    x1   = GPS_Datum[n1].dx;
    y1   = GPS_Datum[n1].dy;
    z1   = GPS_Datum[n1].dz;

    idx2 = GPS_Datum[n2].ellipse;
    Da   = GPS_Ellipse[idx2].a;
    Dif  = GPS_Ellipse[idx2].invf;
    x2   = GPS_Datum[n2].dx;
    y2   = GPS_Datum[n2].dy;
    z2   = GPS_Datum[n2].dz;

    x = -(x2-x1);
    y = -(y2-y1);
    z = -(z2-z1);

    GPS_Math_Molodensky(Sphi,Slam,SH,Sa,Sif,Dphi,Dlam,DH,Da,Dif,x,y,z);

    return;
}



/* @func GPS_Math_Known_Datum_To_Known_Datum_C *************************
**
** Transform known datum to other datum using Cartesian coordinates
**
** @param [r] Sphi [double] source latitude (deg)
** @param [r] Slam [double] source longitude (deg)
** @param [r] SH   [double] source height  (metres)
** @param [w] Dphi [double *] dest latitude (deg)
** @param [w] Dlam [double *] dest longitude (deg)
** @param [w] DH   [double *] dest height  (metres)
** @param [r] n1   [int32] source datum number from GPS_Datum structure
** @param [r] n2   [int32] dest   datum number from GPS_Datum structure
**
** @return [void]
************************************************************************/
void GPS_Math_Known_Datum_To_Known_Datum_C(double Sphi, double Slam, double SH,
                                           double *Dphi, double *Dlam,
                                           double *DH, int32 n1, int32 n2)
{
    double Sa;
    double Sif;
    double Da;
    double Dif;
    double x1;
    double y1;
    double z1;
    double x2;
    double y2;
    double z2;
    
    int32    idx1;
    int32    idx2;
    
    double Sb;
    double Db;
    double dx;
    double dy;
    double dz;
    
    idx1  = GPS_Datum[n1].ellipse;
    Sa    = GPS_Ellipse[idx1].a;
    Sif   = GPS_Ellipse[idx1].invf;
    Sb    = Sa - (Sa / Sif);

    x1    = GPS_Datum[n1].dx;
    y1    = GPS_Datum[n1].dy;
    z1    = GPS_Datum[n1].dz;

    idx2  = GPS_Datum[n2].ellipse;
    Da    = GPS_Ellipse[idx2].a;
    Dif   = GPS_Ellipse[idx2].invf;
    Db    = Da - (Da / Dif);

    x2    = GPS_Datum[n2].dx;
    y2    = GPS_Datum[n2].dy;
    z2    = GPS_Datum[n2].dz;

    GPS_Math_LatLonH_To_XYZ(Sphi,Slam,SH,&dx,&dy,&dz,Sa,Sb);
    dx += -(x2-x1);
    dy += -(y2-y1);
    dz += -(z2-z1);

    GPS_Math_XYZ_To_LatLonH(Dphi,Dlam,DH,dx,dy,dz,Da,Db);

    return;
}



/* @func GPS_Math_WGS84_To_UKOSMap_M ***********************************
**
** Convert WGS84 lat/lon to Ordnance survey map code and easting and
** northing. Uses Molodensky
**
** @param [r] lat  [double] WGS84 latitude (deg)
** @param [r] lon  [double] WGS84 longitude (deg)
** @param [w] mE   [double *] map easting (metres)
** @param [w] mN   [double *] map northing (metres)
** @param [w] map  [char *] map two letter code
**
** @return [int32] success
************************************************************************/
int32 GPS_Math_WGS84_To_UKOSMap_M(double lat, double lon, double *mE,
                                  double *mN, char *map)
{
    double alat;
    double alon;
    double aht;
    double aE;
    double aN;


    GPS_Math_WGS84_To_Known_Datum_M(lat,lon,30,&alat,&alon,&aht,86);

    GPS_Math_Airy1830LatLonToNGEN(alat,alon,&aE,&aN);

    if(!GPS_Math_EN_To_UKOSNG_Map(aE,aN,mE,mN,map))
        return 0;

    return 1;
}



/* @func GPS_Math_UKOSMap_To_WGS84_M ***********************************
**
** Transform UK Ordnance survey map position to WGS84 lat/lon
** Uses Molodensky transformation
**
** @param [r] map  [char *] map two letter code
** @param [r] mE   [double] map easting (metres)
** @param [r] mN   [double] map northing (metres)
** @param [w] lat  [double *] WGS84 latitude (deg)
** @param [w] lon  [double *] WGS84 longitude (deg)
**
** @return [int32] success
************************************************************************/
int32 GPS_Math_UKOSMap_To_WGS84_M(char *map, double mE, double mN,
                                  double *lat, double *lon)
{
    double E;
    double N;
    double alat;
    double alon;
    double ht;
    
    if(!GPS_Math_UKOSNG_Map_To_EN(map,mE,mN,&E,&N))
        return 0;

    GPS_Math_NGENToAiry1830LatLon(E,N,&alat,&alon);

    GPS_Math_Known_Datum_To_WGS84_M(alat,alon,0,lat,lon,&ht,78);

    return 1;
}



/* @func GPS_Math_WGS84_To_UKOSMap_C ***********************************
**
** Convert WGS84 lat/lon to Ordnance survey map code and easting and
** northing. Uses cartesian transformation
**
** @param [r] lat  [double] WGS84 latitude (deg)
** @param [r] lon  [double] WGS84 longitude (deg)
** @param [w] mE   [double *] map easting (metres)
** @param [w] mN   [double *] map northing (metres)
** @param [w] map  [char *] map two letter code
**
** @return [int32] success
************************************************************************/
int32 GPS_Math_WGS84_To_UKOSMap_C(double lat, double lon, double *mE,
                                  double *mN, char *map)
{
    double alat;
    double alon;
    double aht;
    double aE;
    double aN;


    GPS_Math_WGS84_To_Known_Datum_C(lat,lon,30,&alat,&alon,&aht,86);

    GPS_Math_Airy1830LatLonToNGEN(alat,alon,&aE,&aN);

    if(!GPS_Math_EN_To_UKOSNG_Map(aE,aN,mE,mN,map))
        return 0;

    return 1;
}



/* @func GPS_Math_UKOSMap_To_WGS84_C ***********************************
**
** Transform UK Ordnance survey map position to WGS84 lat/lon
** Uses cartesian transformation
**
** @param [r] map  [char *] map two letter code
** @param [r] mE   [double] map easting (metres)
** @param [r] mN   [double] map northing (metres)
** @param [w] lat  [double *] WGS84 latitude (deg)
** @param [w] lon  [double *] WGS84 longitude (deg)
**
** @return [int32] success
************************************************************************/
int32 GPS_Math_UKOSMap_To_WGS84_C(char *map, double mE, double mN,
                                  double *lat, double *lon)
{
    double E;
    double N;
    double alat;
    double alon;
    double ht;
    
    if(!GPS_Math_UKOSNG_Map_To_EN(map,mE,mN,&E,&N))
        return 0;

    GPS_Math_NGENToAiry1830LatLon(E,N,&alat,&alon);

    GPS_Math_Known_Datum_To_WGS84_C(alat,alon,0,lat,lon,&ht,78);

    return 1;
}

// BOBMAX INSERT END -------------------------------------------------------






/* @func GPS_Math_Albers_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Albers projection easting and
** northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi1 [double] standard latitude (parallel) 1 (deg)
** @param [r] phi2 [double] standard latitude (parallel) 2 (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Albers_LatLon_To_EN(double phi, double lambda, double *E,
                                  double *N, double phi1, double phi2,
                                  double phi0, double M0, double E0,
                                  double N0, double a, double b)

{
    double dlambda;
    double phis;
    double phic;
    double e;
    double esq;
    double esqs;
    double omesqs2;
    
    double a2;
    double b2;
    double q;
    double q0;
    double q1;
    double q2;
    double m1;
    double m2;
    double n;
    double phi0s;
    double phi1s;
    double phi1c;
    double phi2s;
    double phi2c;
    double ess;
    double om0;
    double m1sq;
    double C;
    double nq;
    double nq0;
    double rho;
    double rho0;
    double theta;
    
    phi     = GPS_Math_Deg_To_Rad(phi);
    phi0    = GPS_Math_Deg_To_Rad(phi0);
    phi1    = GPS_Math_Deg_To_Rad(phi1);
    phi2    = GPS_Math_Deg_To_Rad(phi2);
    lambda  = GPS_Math_Deg_To_Rad(lambda);
    M0      = GPS_Math_Deg_To_Rad(M0);
    
    dlambda = lambda - M0;
    if(dlambda > GPS_PI)
        dlambda -= ((double)2.0 * GPS_PI);
    if(dlambda < -GPS_PI)
        dlambda += ((double)2.0 * GPS_PI);
    
    phis = sin(phi);
    phic = cos(phi);
    
    a2 = a*a;
    b2 = b*b;
    esq = (a2-b2)/a2;
    e   = pow(esq,(double)0.5);


    phi0s = sin(phi0);
    ess = e * phi0s;
    om0 = ((double)1.0 - ess*ess);
    q0  = ((double)1.0 - esq) * (phi0s / om0-((double)1.0/(e+e)) *
                                 log(((double)1.0-ess)/((double)1.0+ess)));
    phi1s = sin(phi1);
    phi1c = cos(phi1);
    ess = e * phi1s;
    om0 = ((double)1.0 - ess*ess);    
    m1 = phi1c/pow(om0,(double)0.5);
    q1  = ((double)1.0 - esq) * (phi1s / om0-((double)1.0/(e+e)) *
                                 log(((double)1.0-ess)/((double)1.0+ess)));

    m1sq = m1*m1;
    if(fabs(phi1-phi2)>1.0e-10)
    {
        phi2s = sin(phi2);
        phi2c = cos(phi2);
        ess   = e * phi2s;
        om0   = ((double)1.0 - ess*ess);
        m2 = phi2c/pow(om0,(double)0.5);
        q2 = ((double)1.0 - esq) * (phi2s / om0-((double)1.0/(e+e)) *
                                    log(((double)1.0-ess)/((double)1.0+ess)));
        n  = (m1sq - m2*m2) / (q2-q1);
    }
    else
        n  = phi1s;
    
    C = m1sq + n*q1;
    nq0 = n * q0;
    if(C < nq0)
        rho0 = (double)0.;
    else
        rho0 = (a/n) * pow(C-nq0,(double)0.5);
    
    
    esqs = e * phis;
    omesqs2 = ((double)1.0 - esqs*esqs);
    q  = ((double)1.0 - esq) * (phis / omesqs2-((double)1.0/(e+e)) *
                                log(((double)1.0-esqs)/((double)1.0+esqs)));
    nq = n*q;
    if(C<nq)
        rho = (double)0.;
    else
        rho = (a/n) * pow(C-nq,(double)0.5);

    theta = n*dlambda;
    *E = rho * sin(theta) + E0;
    *N = rho0 - rho * cos(theta) + N0;

    return;
}




/* @func GPS_Math_Albers_EN_To_LatLon **********************************
**
** Convert Albers easting and northing to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi1 [double] standard latitude (parallel) 1 (deg)
** @param [r] phi2 [double] standard latitude (parallel) 2 (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Albers_EN_To_LatLon(double E, double N, double *phi,
                                  double *lambda, double phi1, double phi2,
                                  double phi0, double M0, double E0,
                                  double N0, double a, double b)
{
    double po2;
    double rho;
    double rho0;
    double C;
    double a2;
    double b2;
    double esq;
    double e;
    double phi0s;
    double q0;
    double q1;
    double q2;
    double phi1s;
    double phi1c;
    double phi2s;
    double phi2c;
    double m1;
    double m1sq;
    double m2;
    double n;
    double nq0;
    
    double dx;
    double dy;
    double rhom;
    double q;
    double qc;
    double qd2;
    double rhon;
    double lat;
    double dphi;
    double phis;
    double ess;
    double om0;
    double theta;
    double tol;


    phi0    = GPS_Math_Deg_To_Rad(phi0);
    phi1    = GPS_Math_Deg_To_Rad(phi1);
    phi2    = GPS_Math_Deg_To_Rad(phi2);
    M0      = GPS_Math_Deg_To_Rad(M0);
    
    a2 = a*a;
    b2 = b*b;
    esq = (a2-b2)/a2;
    e   = pow(esq,(double)0.5);


    phi0s = sin(phi0);
    ess = e * phi0s;
    om0 = ((double)1.0 - ess*ess);
    q0  = ((double)1.0 - esq) * (phi0s / om0-((double)1.0/(e+e)) *
                                 log(((double)1.0-ess)/((double)1.0+ess)));
    phi1s = sin(phi1);
    phi1c = cos(phi1);
    ess = e * phi1s;
    om0 = ((double)1.0 - ess*ess);    
    m1 = phi1c/pow(om0,(double)0.5);
    q1  = ((double)1.0 - esq) * (phi1s / om0-((double)1.0/(e+e)) *
                                 log(((double)1.0-ess)/((double)1.0+ess)));

    m1sq = m1*m1;
    if(fabs(phi1-phi2)>1.0e-10)
    {
        phi2s = sin(phi2);
        phi2c = cos(phi2);
        ess   = e * phi2s;
        om0   = ((double)1.0 - ess*ess);
        m2 = phi2c/pow(om0,(double)0.5);
        q2 = ((double)1.0 - esq) * (phi2s / om0-((double)1.0/(e+e)) *
                                    log(((double)1.0-ess)/((double)1.0+ess)));
        n  = (m1sq - m2*m2) / (q2-q1);
    }
    else
        n  = phi1s;
    
    C = m1sq + n*q1;
    nq0 = n * q0;
    if(C < nq0)
        rho0 = (double)0.;
    else
        rho0 = (a/n) * pow(C-nq0,(double)0.5);


    dphi  = (double) 1.0;
    theta = (double) 0.0;
    tol   = (double) 4.85e-10;
    po2   = (double)GPS_PI / (double)2.0;
    
    dy   = N-N0;
    dx   = E-E0;
    rhom = rho0-dy;
    rho  = pow(dx*dx+rhom*rhom,(double)0.5);

    if(n<0.0)
    {
        rho  *= (double)-1.0;
        dx   *= (double)-1.0;
        dy   *= (double)-1.0;
        rhom *= (double)-1.0;
    }
    
    if(rho)
        theta = atan2(dx,rhom);
    rhon = rho*n;
    q    = (C - (rhon*rhon) / a2) / n;
    qc   = (double)1.0 - ((double)1.0 / (e+e)) *
            log(((double)1.0-e)/((double)1.0+e));
    if(fabs(fabs(qc)-fabs(q))>1.9e-6)
    {
        qd2 = q/(double)2.0;
        if(qd2>1.0)
            *phi = po2;
        else if(qd2<-1.0)
            *phi = -po2;
        else
        {
            lat = asin(qd2);
            if(e<1.0e-10)
                *phi = lat;
            else
            {
                while(fabs(dphi)>tol)
                {
                    phis = sin(lat);
                    ess  = e*phis;
                    om0  = ((double)1.0 - ess*ess);
                    dphi = (om0*om0) / ((double)2.0*cos(lat))*
                           (q/((double)1.0-esq) - phis / om0 +
                            (log(((double)1.0-ess)/((double)1.0+ess)) /
                             (e+e)));
                    lat += dphi;
                }
                *phi = lat;
            }

            if(*phi > po2)
                *phi = po2;
            else if(*phi<-po2)
                *phi = -po2;
        }
    }
    else
    {
        if(q>=0.0)
            *phi = po2;
        else
            *phi = -po2;
    }

    *lambda = M0 + theta / n;
    if(*lambda > GPS_PI)
        *lambda -= GPS_PI * (double)2.0;
    if(*lambda < -GPS_PI)
        *lambda += GPS_PI * (double)2.0;
    if(*lambda>GPS_PI)
        *lambda = GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda = -GPS_PI;

    *phi    = GPS_Math_Rad_To_Deg(*phi);
    *lambda = GPS_Math_Rad_To_Deg(*lambda);

    return;
}



/* @func GPS_Math_LambertCC_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Lambert Conformal Conic projection
** easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi1 [double] standard latitude (parallel) 1 (deg)
** @param [r] phi2 [double] standard latitude (parallel) 2 (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_LambertCC_LatLon_To_EN(double phi, double lambda, double *E,
                                     double *N, double phi1, double phi2,
                                     double phi0, double M0, double E0,
                                     double N0, double a, double b)

{
    double po2;
    double po4;
    double a2;
    double b2;
    double phi0s;
    double e;
    double esq;
    double ed2;
    double ess;
    double t0;
    double t1;
    double t2;
    double m1;
    double m2;
    double phi1s;
    double phi1c;
    double phi2s;
    double phi2c;
    double n;
    double F;
    double Fa;
    double rho;
    double rho0;
    double phis;
    double t;
    double theta;
    double dphi;
    
    phi     = GPS_Math_Deg_To_Rad(phi);
    phi0    = GPS_Math_Deg_To_Rad(phi0);
    phi1    = GPS_Math_Deg_To_Rad(phi1);
    phi2    = GPS_Math_Deg_To_Rad(phi2);
    lambda  = GPS_Math_Deg_To_Rad(lambda);
    M0      = GPS_Math_Deg_To_Rad(M0);


    po2 = (double)GPS_PI / (double)2.0;
    po4 = (double)GPS_PI / (double)4.0;
    a2  = a*a;
    b2  = b*b;
    esq = (a2-b2)/a2;
    e   = pow(esq,(double)0.5);
    ed2 = e / (double)2.0;

    phi0s = sin(phi0);
    ess   = e * phi0s;
    t0    = tan(po4-phi0/(double)2.0) / pow(((double)1.0-ess) /
                                          ((double)1.0+ess),ed2);
    

    phi1s = sin(phi1);
    phi1c = cos(phi1);
    ess   = e * phi1s;
    m1    = phi1c / pow(((double)1.0-ess*ess),(double)0.5);
    t1    = tan(po4-phi1/(double)2.0) / pow(((double)1.0-ess) /
                                          ((double)1.0+ess),ed2);
    
    if(fabs(phi1-phi2)>1.0e-10)
    {
        phi2s = sin(phi2);
        phi2c = cos(phi2);
        ess   = e * phi2s;
        m2    = phi2c / pow(((double)1.0-ess*ess),(double)0.5);
        t2    = tan(po4-phi2/(double)2.0) / pow(((double)1.0-ess) /
                                          ((double)1.0+ess),ed2);
        n     = log(m1/m2) / log(t1/t2);
    }
    else
        n = phi1s;
    
    F  = m1 / (n*pow(t1,n));
    Fa = F*a;

    rho0 = pow(t0,n) * Fa;

    if(fabs(fabs(phi)-po2)>1.0e-10)
    {
        phis = sin(phi);
        ess  = e * phis;
        t    = tan(po4-phi/(double)2.0) / pow(((double)1.0-ess) /
                                          ((double)1.0+ess),ed2);
        rho  = pow(t,n) * Fa;
    }
    else
    {
        if((phi*n)<=(double)0.0)
            return;
        rho = (double)0.0;
    }

    dphi = lambda - M0;
    if(dphi>GPS_PI)
        dphi -= (double)GPS_PI * (double)2.0;
    if(dphi<-GPS_PI)
        dphi += (double)GPS_PI * (double)2.0;
    theta = dphi*n;

    *E = rho * sin(theta) + E0;
    *N = rho0 - rho * cos(theta) + N0;

    return;
}




/* @func GPS_Math_LambertCC_EN_To_LatLon **********************************
**
** Convert Lambert Conformal Conic  easting and northing to latitude and
** longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi1 [double] standard latitude (parallel) 1 (deg)
** @param [r] phi2 [double] standard latitude (parallel) 2 (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_LambertCC_EN_To_LatLon(double E, double N, double *phi,
                                     double *lambda, double phi1, double phi2,
                                     double phi0, double M0, double E0,
                                     double N0, double a, double b)
{
    double po2;
    double po4;
    double a2;
    double b2;
    double phi0s;
    double e;
    double esq;
    double ed2;
    double ess;
    double t0;
    double t1;
    double t2;
    double m1;
    double m2;
    double phi1s;
    double phi1c;
    double phi2s;
    double phi2c;
    double n;
    double F;
    double Fa;
    double rho;
    double rho0;
    double phis;
    double t;
    double theta;

    double dx;
    double dy;
    double rhom;
    double lat;
    double tlat;
    double tol;
    

    
    phi0    = GPS_Math_Deg_To_Rad(phi0);
    phi1    = GPS_Math_Deg_To_Rad(phi1);
    phi2    = GPS_Math_Deg_To_Rad(phi2);
    M0      = GPS_Math_Deg_To_Rad(M0);


    po2 = (double)GPS_PI / (double)2.0;
    po4 = (double)GPS_PI / (double)4.0;
    a2  = a*a;
    b2  = b*b;
    esq = (a2-b2)/a2;
    e   = pow(esq,(double)0.5);
    ed2 = e / (double)2.0;

    phi0s = sin(phi0);
    ess   = e * phi0s;
    t0    = tan(po4-phi0/(double)2.0) / pow(((double)1.0-ess) /
                                          ((double)1.0+ess),ed2);
    

    phi1s = sin(phi1);
    phi1c = cos(phi1);
    ess   = e * phi1s;
    m1    = phi1c / pow(((double)1.0-ess*ess),(double)0.5);
    t1    = tan(po4-phi1/(double)2.0) / pow(((double)1.0-ess) /
                                          ((double)1.0+ess),ed2);
    
    if(fabs(phi1-phi2)>1.0e-10)
    {
        phi2s = sin(phi2);
        phi2c = cos(phi2);
        ess   = e * phi2s;
        m2    = phi2c / pow(((double)1.0-ess*ess),(double)0.5);
        t2    = tan(po4-phi2/(double)2.0) / pow(((double)1.0-ess) /
                                          ((double)1.0+ess),ed2);
        n     = log(m1/m2) / log(t1/t2);
    }
    else
        n = phi1s;
    
    F  = m1 / (n*pow(t1,n));
    Fa = F*a;

    rho0 = pow(t0,n) * Fa;

    tlat = theta = (double)0.0;
    tol  = (double)4.85e-10;

    dx = E - E0;
    dy = N - N0;
    rhom = rho0 - dy;
    rho  = pow(dx*dx + rhom*rhom,(double)0.5);

    if(n<0.0)
    {
        rhom *= (double)-1.0;
        dy   *= (double)-1.0;
        dx   *= (double)-1.0;
        rho  *= (double)-1.0;
    }

    if(rho)
    {
        theta = atan2(dx,rhom);
        t = pow(rho/Fa,(double)1.0/n);
        lat = po2 - (double)2.0*atan(t);
        while(fabs(lat-tlat)>tol)
        {
            tlat = lat;
            phis = sin(lat);
            ess  = e * phis;
            lat  = po2 - (double)2.0 * atan(t*pow(((double)1.0-ess) /
                                                  ((double)1.0+ess),
                                                  e / (double)2.0));
        }
        *phi = lat;
        *lambda = theta/n + M0;

        if(*phi>po2)
            *phi=po2;
        else if(*phi<-po2)
            *phi=-po2;
        if(*lambda>GPS_PI)
            *lambda -= (double)GPS_PI * (double)2.0;
        if(*lambda<-GPS_PI)
            *lambda += (double)GPS_PI * (double)2.0;

        if(*lambda>GPS_PI)
            *lambda = GPS_PI;
        else if(*lambda<-GPS_PI)
            *lambda = -GPS_PI;
    }
    else
    {
        if(n>0.0)
            *phi = po2;
        else
            *phi = -po2;
        *lambda = M0;
    }

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_Miller_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Miller Cylindrical projection easting and
** northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Miller_LatLon_To_EN(double phi, double lambda, double *E,
                                  double *N, double M0, double E0,
                                  double N0, double a, double b)
{
    double a2;
    double b2;
    double R;
    double e2;
    double e4;
    double e6;
    double p2;
    double po2;
    double phis;
    double dlam;
    

    phi     = GPS_Math_Deg_To_Rad(phi);
    lambda  = GPS_Math_Deg_To_Rad(lambda);
    M0      = GPS_Math_Deg_To_Rad(M0);

    po2 = (double)GPS_PI / (double)2.0;
    p2  = (double)GPS_PI * (double)2.0;
    a2  = a*a;
    b2  = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e4*e2;

    R = a*((double)1.0-e2/(double)6.0-(double)17.0*e4/(double)360.0-
           (double)67.0*e6/(double)3024.0);

    if(M0>GPS_PI)
        M0 -= p2;
    
    phis = sin((double)0.8 * phi);

    dlam = lambda - M0;
    if(dlam>GPS_PI)
        dlam-=p2;
    if(dlam<-GPS_PI)
        dlam+=p2;

    *E = R*dlam+E0;
    *N = (R/(double)1.6) * log(((double)1.0+phis) / ((double)1.0-phis)) + N0;

    return;
}




/* @func GPS_Math_Miller_EN_To_LatLon **********************************
**
** Convert latitude and longitude to Miller Cylindrical projection easting and
** northing
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Miller_EN_To_LatLon(double E, double N, double *phi,
                                  double *lambda, double M0, double E0,
                                  double N0, double a, double b)
{
    double a2;
    double b2;
    double R;
    double e;
    double e2;
    double e4;
    double e6;
    double p2;
    double po2;
    double dx;
    double dy;

    dx = E - E0;
    dy = N - N0;

    M0      = GPS_Math_Deg_To_Rad(M0);

    po2 = (double)GPS_PI / (double)2.0;
    p2  = (double)GPS_PI * (double)2.0;
    a2  = a*a;
    b2  = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e4*e2;
    e   = pow(e2,(double)0.5);

    R = a*((double)1.0-e2/(double)6.0-(double)17.0*e4/(double)360.0-
           (double)67.0*e6/(double)3024.0);
    if(M0>GPS_PI)
        M0 -= p2;

    *phi    = atan(sinh((double)0.8*dy/R)) / (double)0.8;
    *lambda = M0+dx/R;

    if(*phi>po2)
        *phi=po2;
    else if (*phi<-po2)
        *phi=-po2;

    if(*lambda>GPS_PI)
        *lambda-=p2;
    if(*lambda<-GPS_PI)
        *lambda+=p2;

    if(*lambda>GPS_PI)
        *lambda=GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_Bonne_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Bonne pseudoconic equal area  projection
**  easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Bonne_LatLon_To_EN(double phi, double lambda, double *E,
                                  double *N, double phi0, double M0, double E0,
                                  double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double M1;
    double m1;
    double c0;
    double c1;
    double c2;
    double c3;
    double j;
    double te4;
    double E1;
    double E2;
    double E3;
    double E4;
    double x;
    double phi0s;
    double lat;
    double phi0c;
    double phi0s2;
    double phi0s4;
    double phi0s6;
    double as;

    double phis;
    double phic;
    double phis2;
    double phis4;
    double phis6;
    double dlam;
    double mm;
    double MM;
    double rho;
    double EE;
    double tol;
    
    
    lambda = GPS_Math_Deg_To_Rad(lambda);
    phi    = GPS_Math_Deg_To_Rad(phi);
    phi0   = GPS_Math_Deg_To_Rad(phi0);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    phi0s = sin(phi0);
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(M0>GPS_PI)
        M0 -= p2;
    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    
    j = (double)45.0*e6/(double)1024.0;
    te4 = (double)3.0 * e4;
    c0 = (double)1.0-e2/(double)4.0-te4/(double)64.0-(double)5.0*e6/
        (double)256.0;
    c1 = (double)3.0*e2/(double)8.0+te4/(double)32.0+j;
    c2 = (double)15.0*e4/(double)256.0+j;
    c3 = (double)35.0*e6/(double)3072.0;
    
    phi0c = cos(phi0);
    m1 = phi0c/ pow(((double)1.0-e2*phi0s*phi0s),(double)0.5);
    lat = c0 * phi0;

    phi0s2 = c1 * sin((double)2.0*phi0);
    phi0s4 = c2 * sin((double)4.0*phi0);
    phi0s6 = c3 * sin((double)6.0*phi0);
    M1 = a*(lat-phi0s2+phi0s4-phi0s6);

    x = pow((double)1.0-e2,(double)0.5);
    E1 = ((double)1.0-x) / ((double)1.0+x);
    E2 = E1*E1;
    E3 = E2*E1;
    E4 = E3*E1;

    if(!phi0s)
        as = (double)0.0;
    else
        as = a*m1/phi0s;
    

    dlam = lambda - M0;
    if(dlam>GPS_PI)
        dlam -= p2;
    if(dlam<-GPS_PI)
        dlam += p2;

    phis = sin(phi);
    phic = cos(phi);

    tol = (double)0.0001;
    if(!(phi-phi0) && (((po2-tol)<fabs(phi)) && (fabs(phi)<po2+tol)))
        *E = *N = (double)0.0;
    else
    {
        mm = phic / pow(((double)1.0-e2*phis*phis),(double)0.5);
        lat   = c0 * phi;
        phis2 = c1 * sin((double)2.0*phi);
        phis4 = c2 * sin((double)4.0*phi);
        phis6 = c3 * sin((double)6.0*phi);
        MM = a * (lat-phis2+phis4-phis6);

        rho = as + M1 - MM;
        if(!rho)
            EE = (double)0.0;
        else
            EE = a * mm * dlam / rho;

        *E = rho * sin(EE) + E0;
        *N = as - rho * cos(EE) + N0;
    }

    return;
}




/* @func GPS_Math_Bonne_EN_To_LatLon **********************************
**
** Convert Bonne pseudoconic equal area easting and northing projection
** to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Bonne_EN_To_LatLon(double E, double N, double *phi,
                                 double *lambda, double phi0, double M0,
                                 double E0, double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double M1;
    double m1;
    double c0;
    double c1;
    double c2;
    double c3;
    double A0;
    double A1;
    double A2;
    double A3;
    double j;
    double te4;
    double E1;
    double E2;
    double E3;
    double E4;
    double x;
    double phi0s;
    double lat;
    double phi0c;
    double phi0s2;
    double phi0s4;
    double phi0s6;
    double as;

    double phis;
    double phic;
    double dx;
    double dy;
    double mu;
    double mm;
    double MM;
    double asdy;
    double rho;
    double smu2;
    double smu4;
    double smu6;
    double smu8;
    double tol;
    
    
    phi0   = GPS_Math_Deg_To_Rad(phi0);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    phi0s = sin(phi0);
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(M0>GPS_PI)
        M0 -= p2;
    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    
    j = (double)45.0*e6/(double)1024.0;
    te4 = (double)3.0 * e4;
    c0 = (double)1.0-e2/(double)4.0-te4/(double)64.0-(double)5.0*e6/
        (double)256.0;
    c1 = (double)3.0*e2/(double)8.0+te4/(double)32.0+j;
    c2 = (double)15.0*e4/(double)256.0+j;
    c3 = (double)35.0*e6/(double)3072.0;
    
    phi0c = cos(phi0);
    m1 = phi0c/ pow(((double)1.0-e2*phi0s*phi0s),(double)0.5);
    lat = c0 * phi0;

    phi0s2 = c1 * sin((double)2.0*phi0);
    phi0s4 = c2 * sin((double)4.0*phi0);
    phi0s6 = c3 * sin((double)6.0*phi0);
    M1 = a*(lat-phi0s2+phi0s4-phi0s6);

    x = pow((double)1.0-e2,(double)0.5);
    E1 = ((double)1.0-x) / ((double)1.0+x);
    E2 = E1*E1;
    E3 = E2*E1;
    E4 = E3*E1;
    A0 = (double)3.0*E1/(double)2.0-(double)27.0*E3/(double)32.0;
    A1 = (double)21.0*E2/(double)16.0-(double)55.0*E4/(double)32.0;
    A2 = (double)151.0*E3/(double)96.0;
    A3 = (double)1097.0*E4/(double)512.0;
    if(!phi0s)
        as = (double)0.0;
    else
        as = a*m1/phi0s;
    

    dx = E - E0;
    dy = N - N0;
    asdy = as - dy;
    rho = pow(dx*dx+asdy*asdy,(double)0.5);
    if(phi0<(double)0.0)
        rho=-rho;
    MM = as+M1-rho;

    mu = MM / (a*c0);
    smu2 = A0 * sin((double)2.0*mu);
    smu4 = A1 * sin((double)4.0*mu);
    smu6 = A2 * sin((double)6.0*mu);
    smu8 = A3 * sin((double)8.0*mu);
    *phi = mu+smu2+smu4+smu6+smu8;
    
    tol = (double)0.00001;
    if(((po2-tol)<fabs(*phi)) && (fabs(*phi)<po2+tol))
        *lambda = M0;
    else
    {
        phic = cos(*phi);
        phis = sin(*phi);
        mm   = phic / pow(((double)1.0-e2*phis*phis),(double)0.5);
        if(phi0<(double)0.0)
        {
            dx = -dx;
            asdy = -asdy;
        }
        *lambda = M0 + rho * (atan2(dx,asdy)) / (a * mm);
    }

    if(*phi>po2)
        *phi = po2;
    else if(*phi<-po2)
        *phi = -po2;

    if(*lambda>GPS_PI)
        *lambda -= p2;
    if(*lambda<-GPS_PI)
        *lambda += p2;

    if(*lambda>GPS_PI)
        *lambda = GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_Cassini_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Cassini transverse cylindrical projection
**  easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Cassini_LatLon_To_EN(double phi, double lambda, double *E,
                                   double *N, double phi0, double M0,
                                   double E0, double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double AM0;
    double c0;
    double c1;
    double c2;
    double c3;
    double om0;
    double A0;
    double A1;
    double A2;
    double A3;
    double j;
    double te4;
    double phi0s2;
    double phi0s4;
    double phi0s6;
    double lat;
    double x;
    double E1;
    double E2;
    double E3;
    double E4;

    double phis;
    double phic;
    double phit;
    double phis2;
    double phis4;
    double phis6;
    double RD;
    double dlam;
    double NN;
    double TT;
    double WW;
    double WW2;
    double WW3;
    double WW4;
    double WW5;
    double CC;
    double MM;
    

    lambda = GPS_Math_Deg_To_Rad(lambda);
    phi0   = GPS_Math_Deg_To_Rad(phi0);
    phi    = GPS_Math_Deg_To_Rad(phi);
    M0     = GPS_Math_Deg_To_Rad(M0);

    
    p2 = (double)GPS_PI * (double)2.;
    po2 = (double)GPS_PI / (double)2.;
    
    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    
    te4 = (double)3.0 * e4;
    j   = (double)45. * e6 / (double)1024.;
    c0 = (double)1.0-e2/(double)4.-te4/(double)64.-(double)5.*e6/(double)256.;
    c1 = (double)3.*e2/(double)8.+te4/(double)32.+j;
    c2 = (double)15.*e4/(double)256.+j;
    c3 = (double)35.*e6/(double)3072.;

    lat = c0*phi0;
    phi0s2 = c1 * sin((double)2.*phi0);
    phi0s4 = c2 * sin((double)4.*phi0);
    phi0s6 = c3 * sin((double)6.*phi0);
    AM0 = a * (lat-phi0s2+phi0s4-phi0s6);

    om0 = (double)1.0 - e2;
    x = pow(om0,(double)0.5);
    E1 = ((double)1.0 - x) / ((double)1.0 + x);
    E2 = E1*E1;
    E3 = E1*E2;
    E4 = E1*E3;
    A0 = (double)3.*E1/(double)2.-(double)27.*E3/(double)32.;
    A1 = (double)21.*E2/(double)16.-(double)55.*E4/(double)32.;
    A2 = (double)151.*E3/(double)96.;
    A3 = (double)1097.*E4/(double)512.;

    
    dlam = lambda - M0;
    if(dlam>GPS_PI)
        dlam -= p2;
    if(dlam<-GPS_PI)
        dlam += p2;

    phis = sin(phi);
    phic = cos(phi);
    phit = tan(phi);
    RD = pow((double)1.-e2*phis*phis,(double).5);
    NN = a/RD;
    TT = phit*phit;
    WW = dlam*phic;
    WW2 = WW*WW;
    WW3 = WW*WW2;
    WW4 = WW*WW3;
    WW5 = WW*WW4;
    CC = e2*phic*phic/om0;
    lat = c0*phi;
    phis2 = c1 * sin((double)2.*phi);
    phis4 = c2 * sin((double)4.*phi);
    phis6 = c3 * sin((double)6.*phi);
    MM = a * (lat-phis2+phis4-phis6);

    *E = NN*(WW-(TT*WW3/(double)6.)-((double)8.-TT+(double)8.*CC)*
             (TT*WW5/(double)120.)) + E0;
    *N = MM-AM0+NN*phit*((WW2/(double)2.)+((double)5.-TT+(double)6.*CC) *
                         WW4/(double)24.) + N0;
    return;
}




/* @func GPS_Math_Cassini_EN_To_LatLon **********************************
**
** Convert Cassini transverse cylindrical easting and northing projection
** to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Cassini_EN_To_LatLon(double E, double N, double *phi,
                                   double *lambda, double phi0, double M0,
                                   double E0, double N0, double a, double b)

{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double AM0;
    double c0;
    double c1;
    double c2;
    double c3;
    double om0;
    double A0;
    double A1;
    double A2;
    double A3;
    double j;
    double te4;
    double phi0s2;
    double phi0s4;
    double phi0s6;
    double lat;
    double x;
    double E1;
    double E2;
    double E3;
    double E4;

    double dx;
    double dy;
    double mu;
    double mus2;
    double mus4;
    double mus6;
    double mus8;
    double M1;
    double phi1;
    double phi1s;
    double phi1c;
    double phi1t;
    double T;
    double T1;
    double N1;
    double R1;
    double RD;
    double DD;
    double D2;
    double D3;
    double D4;
    double D5;
    double tol;
    
    M0 = GPS_Math_Deg_To_Rad(M0);
    phi0 = GPS_Math_Deg_To_Rad(phi0);

    p2 = (double)GPS_PI * (double)2.;
    po2 = (double)GPS_PI / (double)2.;
    
    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    
    te4 = (double)3.0 * e4;
    j   = (double)45. * e6 / (double)1024.;
    c0 = (double)1.0-e2/(double)4.-te4/(double)64.-(double)5.*e6/(double)256.;
    c1 = (double)3.*e2/(double)8.+te4/(double)32.+j;
    c2 = (double)15.*e4/(double)256.+j;
    c3 = (double)35.*e6/(double)3072.;

    lat = c0*phi0;
    phi0s2 = c1 * sin((double)2.*phi0);
    phi0s4 = c2 * sin((double)4.*phi0);
    phi0s6 = c3 * sin((double)6.*phi0);
    AM0 = a * (lat-phi0s2+phi0s4-phi0s6);

    om0 = (double)1.0 - e2;
    x = pow(om0,(double)0.5);
    E1 = ((double)1.0 - x) / ((double)1.0 + x);
    E2 = E1*E1;
    E3 = E1*E2;
    E4 = E1*E3;
    A0 = (double)3.*E1/(double)2.-(double)27.*E3/(double)32.;
    A1 = (double)21.*E2/(double)16.-(double)55.*E4/(double)32.;
    A2 = (double)151.*E3/(double)96.;
    A3 = (double)1097.*E4/(double)512.;



    tol = (double)1.e-5;

    dx = E - E0;
    dy = N - N0;
    M1 = AM0 + dy;
    mu = M1 / (a*c0);
    mus2 = A0 * sin((double)2.*mu);
    mus4 = A1 * sin((double)4.*mu);
    mus6 = A2 * sin((double)6.*mu);
    mus8 = A3 * sin((double)8.*mu);
    phi1 = mu + mus2 + mus4 + mus6 + mus8;
    
    if((((po2-tol)<phi1)&&(phi1<(po2+tol))))
    {
        *phi = po2;
        *lambda = M0;
    }
    else if((((-po2-tol)<phi1)&&(phi1<(-po2+tol))))
    {
        *phi = -po2;
        *lambda = M0;
    }
    else
    {
        phi1s = sin(phi1);
        phi1c = cos(phi1);
        phi1t = tan(phi1);
        T1 = phi1t*phi1t;
        RD = pow((double)1.-e2*phi1s*phi1s,(double).5);
        N1 = a/RD;
        R1 = N1 * om0 / (RD*RD);
        DD = dx/N1;
        D2 = DD*DD;
        D3 = DD*D2;
        D4 = DD*D3;
        D5 = DD*D4;
        T = (double)1. + (double)3.*T1;
        *phi = phi1-(N1*phi1t/R1)*(D2/(double)2.-T*D4/(double)24.);
        *lambda = M0+(DD-T1*D3/(double)3.+T*T1*D5/(double)15.)/phi1c;

        if(*phi>po2)
            *phi=po2;
        else if(*phi<-po2)
            *phi=-po2;

        if(*lambda>GPS_PI)
            *lambda -= p2;
        if(*lambda<-GPS_PI)
            *lambda += p2;

        if(*lambda>GPS_PI)
            *lambda=GPS_PI;
        else if(*lambda<-GPS_PI)
            *lambda=-GPS_PI;
    }

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_Cylea_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Cylindrical equal area  projection
**  easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Cylea_LatLon_To_EN(double phi, double lambda, double *E,
                                 double *N, double phi0, double M0,
                                 double E0, double N0, double a, double b)
{
    double a2;
    double b2;
    double e;
    double e2;
    double e4;
    double e6;
    double k0;
    double ak0;
    double k2;
    double c0;
    double c1;
    double c2;
    double p2;
    double po2;
    double phi0s;
    double phi0c;

    double dlam;
    double qq;
    double x;
    double phis;
    
    lambda = GPS_Math_Deg_To_Rad(lambda);
    phi0   = GPS_Math_Deg_To_Rad(phi0);
    phi    = GPS_Math_Deg_To_Rad(phi);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.;
    po2 = (double)GPS_PI / (double)2.;

    if(M0>GPS_PI)
        M0-=p2;
    
    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    e  = pow(e2,(double).5);
    c0 = e2/(double)3.+(double)31.*e4/(double)180.+(double)517.*
        e6/(double)5040.;
    c1 = (double)23.*e4/(double)360.+(double)251.*e6/(double)3780.;
    c2 = (double)761.*e6/(double)45360.;
    
    phi0s = sin(phi0);
    phi0c = cos(phi0);
    k0    = phi0c / pow((double)1.-e2*phi0s*phi0s,(double).5);
    ak0   = a*k0;
    k2    = k0 * (double)2.;

    dlam = lambda - M0;
    if(dlam>GPS_PI)
        dlam-=p2;
    if(dlam<-GPS_PI)
        dlam+=p2;

    phis = sin(phi);
    x = e * phis;
    qq = ((double)1.-e2)*(phis/((double)1.-x*x)-((double)1./((double)2.*e))*
                          log(((double)1.-x)/((double)1.+x)));
    *E = ak0 * dlam + E0;
    *N = a * qq / k2 + N0;

    return;
}




/* @func GPS_Math_Cylea_EN_To_LatLon **********************************
**
** Convert Cylindrical equal area  easting and northing projection
** to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Cylea_EN_To_LatLon(double E, double N, double *phi,
                                 double *lambda, double phi0, double M0,
                                 double E0, double N0, double a, double b)

{
    double a2;
    double b2;
    double e;
    double e2;
    double e4;
    double e6;
    double k0;
    double ak0;
    double k2;
    double c0;
    double c1;
    double c2;
    double p2;
    double po2;
    double phi0s;
    double phi0c;

    double dx;
    double dy;
    double qp;
    double bt;
    double phis;
    double i;
    double x;
    double bs2;
    double bs4;
    double bs6;


    phi0   = GPS_Math_Deg_To_Rad(phi0);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.;
    po2 = (double)GPS_PI / (double)2.;

    if(M0>GPS_PI)
        M0-=p2;
    
    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    e  = pow(e2,(double).5);
    c0 = e2/(double)3.+(double)31.*e4/(double)180.+(double)517.*
        e6/(double)5040.;
    c1 = (double)23.*e4/(double)360.+(double)251.*e6/(double)3780.;
    c2 = (double)761.*e6/(double)45360.;
    
    phi0s = sin(phi0);
    phi0c = cos(phi0);
    k0    = phi0c / pow((double)1.-e2*phi0s*phi0s,(double).5);
    ak0   = a*k0;
    k2    = k0 * (double)2.;

    dx = E - E0;
    dy = N - N0;
    phis = sin(po2);
    x = e*phis;
    qp = ((double)1.-e2)*(phis/((double)1.-x*x)-((double)1./((double)2.*e))*
                          log(((double)1.-x)/((double)1.+x)));
    i = k2*dy/(a*qp);
    if(i>(double)1.)
        i=(double)1.;
    else if(i<(double)-1.)
        i=(double)-1.;
    bt = asin(i);
    bs2 = c0 * sin((double)2.*bt);
    bs4 = c1 * sin((double)4.*bt);
    bs6 = c2 * sin((double)6.*bt);

    *phi = bt+bs2+bs4+bs6;
    *lambda = M0 + dx/ak0;

    if(*phi>po2)
        *phi=po2;
    else if(*phi<-po2)
        *phi=-po2;

    if(*lambda>GPS_PI)
        *lambda -= p2;
    if(*lambda<-GPS_PI)
        *lambda += p2;

    if(*lambda>GPS_PI)
        *lambda=GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_EckertIV_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Eckert IV equal area elliptical
** pseudocylindrical projection easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_EckertIV_LatLon_To_EN(double phi, double lambda, double *E,
                                    double *N, double M0, double E0, double N0,
                                    double a, double b)
{
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double Ra0;
    double Ra1;
    double po2;
    double p2;
    
    double Ra;

    double phis;
    double theta;
    double dtheta;
    double thetas;
    double thetac;
    double n;
    double dlam;
    double tol;
    

    lambda = GPS_Math_Deg_To_Rad(lambda);
    phi    = GPS_Math_Deg_To_Rad(phi);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.;
    po2 = (double)GPS_PI / (double)2.;

    if(M0>GPS_PI)
        M0-=p2;


    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2) / a2;
    e4 = e2*e2;
    e6 = e2*e4;
    Ra = a*((double)1.-e2/(double)6.-(double)17.*e4/(double)360.-
            (double)67.*e6/(double)3024.);
    Ra0 = Ra * (double)0.4222382;
    Ra1 = Ra * (double)1.3265004;

    theta = phi / (double)2.;
    dtheta = (double)1.;
    tol = (double)4.85e-10;
    phis = sin(phi);

    dlam = lambda - M0;
    if(dlam>GPS_PI)
        dlam -= p2;
    if(dlam<-GPS_PI)
        dlam += p2;

    while(fabs(dtheta)>tol)
    {
        thetas = sin(theta);
        thetac = cos(theta);
        n = theta+thetas*thetac+(double)2.*thetas;
        dtheta = -(n-((double)2.+po2)*phis) /
            ((double)2.*thetac*((double)1.+thetac));
        theta += dtheta;
    }

    *E = Ra0*dlam*((double)1.+cos(theta))+E0;
    *N = Ra1*sin(theta)+N0;

    return;
}




/* @func GPS_Math_EckertIV_EN_To_LatLon **********************************
**
** Convert Eckert IV equal area elliptical pseudocylindrical projection
** easting and northing to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_EckertIV_EN_To_LatLon(double E, double N, double *phi,
                                    double *lambda, double M0, double E0,
                                    double N0, double a, double b)
{
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double Ra0;
    double Ra1;
    double po2;
    double p2;
    
    double Ra;
    double theta;
    double thetas;
    double thetac;
    double n;
    double dx;
    double dy;
    double i;
    

    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.;
    po2 = (double)GPS_PI / (double)2.;

    if(M0>GPS_PI)
        M0-=p2;


    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2) / a2;
    e4 = e2*e2;
    e6 = e2*e4;
    Ra = a*((double)1.-e2/(double)6.-(double)17.*e4/(double)360.-
            (double)67.*e6/(double)3024.);
    Ra0 = Ra * (double)0.4222382;
    Ra1 = Ra * (double)1.3265004;
    
    dx = E - E0;
    dy = N - N0;
    i = dy/Ra1;
    if(i>(double)1.)
        i=(double)1.;
    else if(i<(double)-1.)
        i=(double)-1.;

    theta = asin(i);
    thetas = sin(theta);
    thetac = cos(theta);
    n = theta+thetas*thetac+(double)2.*thetas;

    *phi = asin(n/((double)2. + po2));
    *lambda = M0 + dx / (Ra0*((double)1.+thetac));

    if(*phi>po2)
        *phi=po2;
    else if(*phi<-po2)
        *phi=-po2;

    if(*lambda>GPS_PI)
        *lambda -= p2;
    if(*lambda<-GPS_PI)
        *lambda += p2;

    if(*lambda>GPS_PI)
        *lambda=GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}
    




/* @func GPS_Math_EckertVI_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Eckert VI equal area
** pseudocylindrical projection easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_EckertVI_LatLon_To_EN(double phi, double lambda, double *E,
                                    double *N, double M0, double E0, double N0,
                                    double a, double b)
{
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double Ra;
    double Rsq;
    double IRa;
    double po2;
    double p2;
    
    double phis;
    double theta;
    double dtheta;
    double dlam;
    double tol;
    

    lambda = GPS_Math_Deg_To_Rad(lambda);
    phi    = GPS_Math_Deg_To_Rad(phi);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.;
    po2 = (double)GPS_PI / (double)2.;

    if(M0>GPS_PI)
        M0-=p2;


    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2) / a2;
    e4 = e2*e2;
    e6 = e2*e4;
    Ra = a*((double)1.-e2/(double)6.-(double)17.*e4/(double)360.-
            (double)67.*e6/(double)3024.);
    Rsq = Ra/pow((double)2.+GPS_PI,(double).5);
    IRa = (double)1./Rsq;

    phis = sin(phi);
    theta = phi;
    dtheta = (double)1.;
    tol = (double)4.85e-10;

    dlam = lambda - M0;
    if(dlam>GPS_PI)
        dlam -= p2;
    if(dlam<-GPS_PI)
        dlam += p2;

    while(fabs(dtheta)>tol)
    {
        dtheta = -(theta+sin(theta)-((double)1.+po2)*phis) /
            ((double)1.+cos(theta));
        theta += dtheta;
    }

    *E = Rsq*dlam*((double)1.+cos(theta))+E0;
    *N = (double)2.*Rsq*theta+N0;

    return;
}




/* @func GPS_Math_EckertVI_EN_To_LatLon **********************************
**
** Convert Eckert VI equal area pseudocylindrical projection
** easting and northing to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_EckertVI_EN_To_LatLon(double E, double N, double *phi,
                                    double *lambda, double M0, double E0,
                                    double N0, double a, double b)
{
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double Rsq;
    double IRa;
    double po2;
    double p2;
    
    double Ra;
    double theta;
    double dx;
    double dy;
    double i;
    

    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.;
    po2 = (double)GPS_PI / (double)2.;

    if(M0>GPS_PI)
        M0-=p2;


    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2) / a2;
    e4 = e2*e2;
    e6 = e2*e4;
    Ra = a*((double)1.-e2/(double)6.-(double)17.*e4/(double)360.-
            (double)67.*e6/(double)3024.);
    Rsq = Ra/pow((double)2.+GPS_PI,(double).5);
    IRa = (double)1./Rsq;

    
    dx = E - E0;
    dy = N - N0;
    theta = IRa * dy / (double)2.;
    i = (theta+sin(theta)) / ((double)1.+po2);
    if(i>(double)1.)
        *phi = po2;
    else if(i<(double)-1.)
        *phi = -po2;
    else
        *phi= asin(i);
    *lambda = M0 + IRa * dx / ((double)1.+cos(theta));

    if(*phi>po2)
        *phi=po2;
    else if(*phi<-po2)
        *phi=-po2;

    if(*lambda>GPS_PI)
        *lambda -= p2;
    if(*lambda<-GPS_PI)
        *lambda += p2;

    if(*lambda>GPS_PI)
        *lambda=GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}
    




/* @func GPS_Math_Cyled_LatLon_To_EN **********************************
**
** Convert latitude and longitude to cylindrical equidistant projection
**  easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Cyled_LatLon_To_EN(double phi, double lambda, double *E,
                                 double *N, double phi0, double M0, double E0,
                                 double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double Ra;
    double Rac;
    double phi0c;

    double dlam;
    
    lambda = GPS_Math_Deg_To_Rad(lambda);
    phi    = GPS_Math_Deg_To_Rad(phi);
    phi0   = GPS_Math_Deg_To_Rad(phi0);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(M0>GPS_PI)
        M0 -= p2;

    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    
    Ra = a*((double)1.-e2/(double)6.-(double)17.*e4/(double)360.-
            (double)67.*e6/(double)3024.);
    phi0c = cos(phi0);
    Rac   = Ra * phi0c;

    dlam = lambda - M0;
    if(dlam>GPS_PI)
        dlam -= p2;
    if(dlam<-GPS_PI)
        dlam += p2;

    *E = Rac * dlam + E0;
    *N = Ra * phi + N0;
    
    return;
}




/* @func GPS_Math_Cyled_EN_To_LatLon **********************************
**
** Convert cylindrical equidistant easting and northing projection
** to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Cyled_EN_To_LatLon(double E, double N, double *phi,
                                 double *lambda, double phi0, double M0,
                                 double E0, double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double Ra;
    double Rac;
    double phi0c;

    double dx;
    double dy;
    
    
    phi0   = GPS_Math_Deg_To_Rad(phi0);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(M0>GPS_PI)
        M0 -= p2;

    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    
    Ra = a*((double)1.-e2/(double)6.-(double)17.*e4/(double)360.-
            (double)67.*e6/(double)3024.);
    phi0c = cos(phi0);
    Rac   = Ra * phi0c;

    dx = E - E0;
    dy = N - N0;

    if(!Rac)
        *lambda = (double)0.;
    else
        *lambda = M0 + dx / Rac;

    *phi = dy/Ra;

    if(*phi>po2)
        *phi = po2;
    else if(*phi<-po2)
        *phi = -po2;

    if(*lambda>GPS_PI)
        *lambda -= p2;
    if(*lambda<-GPS_PI)
        *lambda += p2;

    if(*lambda>GPS_PI)
        *lambda = GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_VderGrinten_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Van der Grinten polyconic projection
**  easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_VderGrinten_LatLon_To_EN(double phi, double lambda, double *E,
                                       double *N, double M0, double E0,
                                       double N0, double a, double b)
{
    double po2;
    double p2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double Ra;
    double pRa;

    double gg;
    double pp;
    double pp2;
    double gm0;
    double ppa;
    double thetai;
    double theta;
    double thetas;
    double thetac;
    double qq;
    double tol;
    double aa;
    double aa2;
    double dlam;
    
    lambda = GPS_Math_Deg_To_Rad(lambda);
    phi    = GPS_Math_Deg_To_Rad(phi);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(M0>GPS_PI)
        M0 -= p2;


    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    
    Ra = a*((double)1.-e2/(double)6.-(double)17.*e4/(double)360.-(double)67.*
            e6/(double)3024.);
    pRa = (double)GPS_PI * Ra;

    dlam = lambda - M0;
    if(dlam>GPS_PI)
        dlam -= p2;
    if(dlam<-GPS_PI)
        dlam += p2;

    tol = (double)1.0e-5;

    if(!phi)
    {
        *N = (double)0.0;
        *E = Ra*dlam+E0;
    }
    else if(!dlam || (((po2-tol)<tol)&&(phi<(po2+tol))) ||
            (((-po2-tol)<tol)&&(phi<(-po2+tol))))
    {
        thetai = fabs(((double)2./(double)GPS_PI) * phi);
        if(thetai>(double)1.)
            thetai=(double)1.;
        else if(thetai<(double)-1.)
            thetai=(double)-1.;

        theta = asin(thetai);
        *E = 0;
        *N = pRa * tan(theta/(double)2.) * N0;
        if(phi<(double)0.0)
            *N *= (double)-1.;
    }
    else
    {
        aa = (double).5*fabs((double)GPS_PI/dlam - dlam/(double)GPS_PI);
        thetai = fabs(((double)2./(double)GPS_PI) * phi);
        if(thetai>(double)1.)
            thetai=(double)1.;
        else if(thetai<(double)-1.)
            thetai=(double)-1.;

        theta = asin(thetai);
        thetas = sin(theta);
        thetac = cos(theta);
        gg = thetac/(thetas+thetac-(double)1.);
        pp = gg*((double)2./thetas-(double)1.);
        aa2 = aa*aa;
        pp2 = pp*pp;
        gm0 = gg-pp2;
        ppa = pp2+aa2;
        qq = aa2+gg;
        *E = pRa*(aa*gm0+pow(aa2*gm0*gm0-ppa*(gg*gg-pp2),(double).5))/ppa+E0;
        if(dlam<(double)0.0)
            *E *= (double)-1.;
        *N = pRa*(pp*qq-aa*pow((aa2+(double)1.)*ppa-qq*qq,(double).5))/ppa+N0;
        if(phi<(double)0.0)
            *N *= (double)-1.;
    }

    return;
}




/* @func GPS_Math_VderGrinten_EN_To_LatLon **********************************
**
** Convert Van der Grinten polyconic easting and northing projection
** to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_VderGrinten_EN_To_LatLon(double E, double N, double *phi,
                                       double *lambda, double M0, double E0,
                                       double N0, double a, double b)
{
    double po2;
    double p2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double Ra;
    double pRa;

    double dx;
    double dy;
    double xx;
    double xx2;
    double yy;
    double yy2;
    double tyy2;
    double xpy;
    double c1;
    double c2;
    double c3;
    double c3c3;
    double co;
    double dd;
    double a1;
    double m1;
    double i;
    double theta;

    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(M0>GPS_PI)
        M0 -= p2;


    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    
    Ra = a*((double)1.-e2/(double)6.-(double)17.*e4/(double)360.-(double)67.*
            e6/(double)3024.);
    pRa = (double)GPS_PI * Ra;


    dx = E - E0;
    dy = N - N0;
    xx = dx/pRa;
    yy = dy/pRa;
    xx2 = xx*xx;
    yy2 = yy*yy;
    xpy = xx2+yy2;
    tyy2 = yy2*(double)2.;

    if(!N)
        *phi=(double)0.0;
    else
    {
        c1 = -fabs(yy)*((double)1.+xpy);
        c2 = c1-tyy2+xx2;
        c3 = (double)-2.*c1+(double)1.+tyy2+xpy*xpy;
        co = c2/((double)3.*c3);
        c3c3 = c3*c3;
        dd = yy2/c3+(((double)2.*c2*c2*c2)/(c3c3*c3)-((double)9.*c1*c2)/
                     c3c3)/(double)27.;
        a1 = (c1-c2*co)/c3;
        m1 = (double)2.* pow(-((double)1./(double)3.)*a1,(double).5);
        i = (double)3.*dd/(a1*m1);
        if((i>(double)1.)||(i<(double)-1.))
            *phi=po2;
        else
        {
            theta = ((double)1./(double)3.)*acos((double)3.*dd/(a1*m1));
            *phi = (double)GPS_PI*(-m1*cos(theta+(double)GPS_PI/(double)3.)-
                                   co);
        }
    }

    if(N<(double)0.0)
        *phi *= (double)-1.0;

    if(!xx)
        *lambda = M0;
    else
        *lambda = (double)GPS_PI * (xpy-(double)1.+
                  pow((double)1.+((double)2.*xx2-tyy2)+xpy*xpy,(double).5)) /
                      ((double)2.*xx) + M0;
    if(*phi>po2)
        *phi = po2;
    else if(*phi<-po2)
        *phi = -po2;

    if(*lambda>GPS_PI)
        *lambda -= p2;
    if(*lambda<-GPS_PI)
        *lambda += p2;

    if(*lambda>GPS_PI)
        *lambda = GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_Bonne_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Bonne pseudoconic equal area  projection
** easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi1 [double] latitude of true scale (deg)
** @param [r] lambda1 [double] longitude from pole (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_PolarSt_LatLon_To_EN(double phi, double lambda, double *E,
                                   double *N, double phi1, double lambda1,
                                   double E0, double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4=(double)0.;
    double e;
    double eo2;
    double sh;
    double mc;
    double tc=(double)0.;
    double amc=(double)0.;
    double ta;
    double phi1s;
    double phi1c;
    double es;
    double op;
    double om;
    double pe;
    double polat;
    double polon;

    double dlam;
    double phis;
    double t;
    double rho;

    
    lambda1 = GPS_Math_Deg_To_Rad(lambda1);
    phi1    = GPS_Math_Deg_To_Rad(phi1);
    lambda  = GPS_Math_Deg_To_Rad(lambda);
    phi     = GPS_Math_Deg_To_Rad(phi);

    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;

    ta = a * (double)2.0;
    if(lambda1>GPS_PI)
        lambda1 -= p2;
    if(phi1<(double)0.0)
    {
        sh=(double)1.0;
        polat = -phi1;
        polon = -lambda1;
    }
    else
    {
        sh=(double)0.0;
        polat = phi1;
        polon = lambda1;
    }

    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e  = pow(e2,(double).5);
    eo2 = e/(double)2.;

    if(fabs(fabs(polat)-po2)>(double)1.0e-10)
    {
        phi1s = sin(polat);
        phi1c = cos(polat);
        es    = e*phi1s;
        pe    = pow(((double)1.-es)/((double)1.+es),eo2);
        mc    = phi1c / pow((double)1.-es*es,(double).5);
        amc   = mc * a;
        tc    = tan(((double)GPS_PI/(double)4.)-polat/(double)2.) / pe;
    }
    else
    {
        op = (double)1. + e;
        om = (double)1. - e;
        e4 = pow(pow(op,op)*pow(om,om),(double).5);
    }
    
    

    if(fabs(fabs(phi)-po2)<(double)1.0e-10)
        *E = *N = (double)0.0;
    else
    {
        if(sh)
        {
            phi *= (double)-1.0;
            lambda *= (double)-1.0;
        }
        
        dlam = lambda - polon;
        if(dlam>GPS_PI)
            dlam -= p2;
        if(dlam<-GPS_PI)
            dlam += p2;
        phis = sin(phi);
        es   = e * phis;
        pe   = pow(((double)1.-es)/((double)1.+es),eo2);        
        t    = tan(((double)GPS_PI/(double)4.)-phi/(double)2.) / pe;

        if(fabs(fabs(polat)-po2)>(double)1.0e-10)
            rho = amc * t / tc;
        else
            rho = ta * t / e4;
        *E = rho * sin(dlam) + E0;

        if(sh)
        {
            *E *= (double)-1.;
            *N = rho * cos(dlam) + N0;
        }
        else
            *N = -rho * cos(dlam) + N0;
    }
        
    return;
}




/* @func GPS_Math_PolarSt_EN_To_LatLon **********************************
**
** Convert Polar Stereographic easting and northing projection
** to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi1 [double] latitude of true scale (deg)
** @param [r] lambda1 [double] longitude from pole (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_PolarSt_EN_To_LatLon(double E, double N, double *phi,
                                   double *lambda, double phi1, double lambda1,
                                   double E0, double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4=(double)0.;
    double e;
    double eo2;
    double sh;
    double mc;
    double tc=(double)0.;
    double amc=(double)0.;
    double ta;
    double phi1s;
    double phi1c;
    double es;
    double op;
    double om;
    double pe;
    double polat;
    double polon;

    double dx;
    double dy;
    double t;
    double rho;
    double PHI;
    double PHIS;
    double TPHI;
    
    
    lambda1 = GPS_Math_Deg_To_Rad(lambda1);
    phi1    = GPS_Math_Deg_To_Rad(phi1);

    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;

    ta = a * (double)2.0;
    if(lambda1>GPS_PI)
        lambda1 -= p2;
    if(phi1<(double)0.0)
    {
        sh=(double)1.0;
        polat = -phi1;
        polon = -lambda1;
    }
    else
    {
        sh=(double)0.0;
        polat = phi1;
        polon = lambda1;
    }

    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e  = pow(e2,(double).5);
    eo2 = e/(double)2.;

    if(fabs(fabs(polat)-po2)>(double)1.0e-10)
    {
        phi1s = sin(polat);
        phi1c = cos(polat);
        es    = e*phi1s;
        pe    = pow(((double)1.-es)/((double)1.+es),eo2);
        mc    = phi1c / pow((double)1.-es*es,(double).5);
        amc   = mc * a;
        tc    = tan(((double)GPS_PI/(double)4.)-polat/(double)2.) / pe;
    }
    else
    {
        op = (double)1. + e;
        om = (double)1. - e;
        e4 = pow(pow(op,op)*pow(om,om),(double).5);
    }
    

    dx = E - E0;
    dy = N - N0;
    if(!dx && !dy)
    {
        *phi = po2;
        *lambda = polon;
    }
    else
    {
        if(sh)
        {
            dx *= (double)-1.;
            dy *= (double)-1.;
        }
        rho = pow(dx*dx+dy*dy,(double).5);
        if(fabs(fabs(polat)-po2)>(double)1.0e-10)
            t = rho * tc / amc;
        else
            t = rho * e4 / ta;
        TPHI = (double)0.0;
        PHI  = po2 - (double)2.*atan(t);
        while(fabs(PHI-TPHI)>(double)1.0e-10)
        {
            TPHI=PHI;
            PHIS = sin(PHI);
            es = e * PHIS;
            pe    = pow(((double)1.-es)/((double)1.+es),eo2);
            PHI = po2 - (double)2. * atan(t*pe);
        }
        *phi = PHI;
        *lambda = polon + atan2(dx,-dy);

        if(*phi>po2)
            *phi = po2;
        else if(*phi<-po2)
            *phi = -po2;

        if(*lambda>GPS_PI)
            *lambda -= p2;
        if(*lambda<-GPS_PI)
            *lambda += p2;

        if(*lambda>GPS_PI)
            *lambda = GPS_PI;
        else if(*lambda<-GPS_PI)
            *lambda=-GPS_PI;
    }
    if(sh)
    {
        *phi *= (double)-1.;
        *lambda *= (double)1.;
    }

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_Mollweide_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Mollweide projection easting and
** northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Mollweide_LatLon_To_EN(double phi, double lambda, double *E,
                                     double *N, double M0, double E0,
                                     double N0, double a, double b)
{
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double p2;
    double po2;
    double Ra;
    double sRa2;
    double sRa8;

    double ps;
    double dlam;
    double theta;
    double thetap;
    double d;
    double tol;
    
    phi     = GPS_Math_Deg_To_Rad(phi);
    lambda  = GPS_Math_Deg_To_Rad(lambda);
    M0      = GPS_Math_Deg_To_Rad(M0);

    po2 = (double)GPS_PI / (double)2.0;
    p2  = (double)GPS_PI * (double)2.0;
    a2  = a*a;
    b2  = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e4*e2;

    Ra = a*((double)1.0-e2/(double)6.0-(double)17.0*e4/(double)360.0-
           (double)67.0*e6/(double)3024.0);
    sRa2 = pow((double)2.,(double).5) * Ra;
    sRa8 = pow((double)8.,(double).5) * Ra;

    if(M0>GPS_PI)
        M0 -= p2;
    
    ps  = sin(phi) * (double)GPS_PI;
    d   = (double)0.1745329;
    tol = (double)4.85e-10;
    thetap = phi;

    dlam = lambda - M0;
    if(dlam>GPS_PI)
        dlam-=p2;
    if(dlam<-GPS_PI)
        dlam+=p2;

    while(fabs(d)>tol)
    {
        d = -(thetap+sin(thetap)-ps)/((double)1.+cos(thetap));
        thetap += d;
    }
    theta = thetap / (double)2.;
    *E = (sRa8/(double)GPS_PI) * dlam * cos(theta) + E0;
    *N = sRa2 * sin(theta) + N0;

    return;
}




/* @func GPS_Math_Mollweide_EN_To_LatLon **********************************
**
** Convert latitude and longitude to Mollweide projection easting and
** northing
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Mollweide_EN_To_LatLon(double E, double N, double *phi,
                                     double *lambda, double M0, double E0,
                                     double N0, double a, double b)
{
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double p2;
    double po2;
    double Ra;
    double sRa2;
    double sRa8;

    double dx;
    double dy;
    double theta=(double)0.;
    double tt;
    double i;
    
    M0      = GPS_Math_Deg_To_Rad(M0);

    po2 = (double)GPS_PI / (double)2.0;
    p2  = (double)GPS_PI * (double)2.0;
    a2  = a*a;
    b2  = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e4*e2;

    Ra = a*((double)1.0-e2/(double)6.0-(double)17.0*e4/(double)360.0-
           (double)67.0*e6/(double)3024.0);
    sRa2 = pow((double)2.,(double).5) * Ra;
    sRa8 = pow((double)8.,(double).5) * Ra;

    if(M0>GPS_PI)
        M0 -= p2;

    dx = E - E0;
    dy = N - N0;
    i  = dy/sRa2;
    if(fabs(i)>(double)1.)
    {
        *phi = po2;
        if(N<(double)0.0)
            *phi *= (double)-1.;
    }
    else
    {
        theta = asin(i);
        tt = theta * (double)2.;
        *phi = asin((tt+sin(tt))/(double)GPS_PI);
        if(*phi>po2)
            *phi=po2;
        else if (*phi<-po2)
            *phi=-po2;
    }

    if(fabs(fabs(*phi)-po2)<(double)1.0e-10)
        *lambda = M0;
    else
        *lambda = M0 + (double)GPS_PI * dx / (sRa8 * cos(theta));


    if(*lambda>GPS_PI)
        *lambda-=p2;
    if(*lambda<-GPS_PI)
        *lambda+=p2;

    if(*lambda>GPS_PI)
        *lambda=GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_Orthog_LatLon_To_EN **********************************
**
** Convert latitude and longitude to orthographic projection
**  easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] lambda0 [double] longitude of origin (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Orthog_LatLon_To_EN(double phi, double lambda, double *E,
                                  double *N, double phi0, double lambda0,
                                  double E0, double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double Ra;
    double phi0s;
    double phi0c;

    double phis;
    double phic;
    double dlam;
    double clc;
    double cc;
    
    
    lambda  = GPS_Math_Deg_To_Rad(lambda);
    phi     = GPS_Math_Deg_To_Rad(phi);
    phi0    = GPS_Math_Deg_To_Rad(phi0);
    lambda0 = GPS_Math_Deg_To_Rad(lambda0);
    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(lambda0>GPS_PI)
        lambda0 -= p2;
    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    Ra = a * ((double)1.-e2/(double)6.-(double)17.*e4/(double)360.-(double)67.*
              e6/(double)3024.);
    phi0s = sin(phi0);
    phi0c = cos(phi0);

    dlam = lambda - lambda0;
    if(dlam>GPS_PI)
        dlam -= p2;
    if(dlam<-GPS_PI)
        dlam += p2;


    phis = sin(phi);
    phic = cos(phi);
    clc = phic * cos(dlam);
    cc  = phi0s * phis + phi0c * clc;

    *E = Ra * phic * sin(dlam) + E0;
    *N = Ra * (phi0c * phis - phi0s * clc) + N0;
    
    return;
}




/* @func GPS_Math_Orthog_EN_To_LatLon **********************************
**
** Convert Orthogonal easting and northing projection
** to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] lambda0 [double] longitude of origin (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Orthog_EN_To_LatLon(double E, double N, double *phi,
                                  double *lambda, double phi0, double lambda0,
                                  double E0, double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double Ra;
    double phi0s;
    double phi0c;

    double dx;
    double dy;
    double rho;
    double adod;
    double ror;
    double cc;
    double ccs;
    double ccc;
    
    


    phi0    = GPS_Math_Deg_To_Rad(phi0);
    lambda0 = GPS_Math_Deg_To_Rad(lambda0);
    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(lambda0>GPS_PI)
        lambda0 -= p2;
    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    Ra = a * ((double)1.-e2/(double)6.-(double)17.*e4/(double)360.-(double)67.*
              e6/(double)3024.);
    phi0s = sin(phi0);
    phi0c = cos(phi0);


    dx = E - E0;
    dy = N - N0;
    adod = atan(dx/dy);
    rho = pow(dx*dx+dy*dy,(double).5);
    if(!rho)
    {
        *phi = phi0;
        *lambda = lambda0;
    }
    else
    {
        ror = rho/Ra;
        if(ror>(double)1.)
            ror=(double)1.;
        else if(ror<(double)-1.)
            ror=(double)-1.;
        cc = asin(ror);
        ccs = sin(cc);
        ccc = cos(cc);
        *phi = asin(ccc*phi0s+(dy*ccs*phi0c/rho));
        if(phi0==po2)
            *lambda = lambda0 - adod;
        else if(phi0==-po2)
            *lambda = lambda0 + adod;
        else
            *lambda = lambda0+atan(dx*ccs/(rho*phi0c*ccc-dy*phi0s*ccs));
    }

    if(*phi>po2)
        *phi = po2;
    else if(*phi<-po2)
        *phi = -po2;

    if(*lambda>GPS_PI)
        *lambda -= p2;
    if(*lambda<-GPS_PI)
        *lambda += p2;

    if(*lambda>GPS_PI)
        *lambda = GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_Polycon_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Polyconic  projection
**  easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Polycon_LatLon_To_EN(double phi, double lambda, double *E,
                                   double *N, double phi0, double M0,
                                   double E0, double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double AM0;
    double c0;
    double c1;
    double c2;
    double c3;
    double j;
    double te4;
    double phi0s2;
    double phi0s4;
    double phi0s6;

    double phis;
    double phis2;
    double phis4;
    double phis6;
    double dlam;
    double NN;
    double NNot;
    double MM;
    double EE;
    double lat;
    
    
    lambda = GPS_Math_Deg_To_Rad(lambda);
    phi    = GPS_Math_Deg_To_Rad(phi);
    phi0   = GPS_Math_Deg_To_Rad(phi0);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(M0>GPS_PI)
        M0 -= p2;
    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    
    j = (double)45.0*e6/(double)1024.0;
    te4 = (double)3.0 * e4;
    c0 = (double)1.0-e2/(double)4.0-te4/(double)64.0-(double)5.0*e6/
        (double)256.0;
    c1 = (double)3.0*e2/(double)8.0+te4/(double)32.0+j;
    c2 = (double)15.0*e4/(double)256.0+j;
    c3 = (double)35.0*e6/(double)3072.0;
    
    lat = c0 * phi0;

    phi0s2 = c1 * sin((double)2.0*phi0);
    phi0s4 = c2 * sin((double)4.0*phi0);
    phi0s6 = c3 * sin((double)6.0*phi0);
    AM0    = a*(lat-phi0s2+phi0s4-phi0s6);

    

    dlam = lambda - M0;
    if(dlam>GPS_PI)
        dlam -= p2;
    if(dlam<-GPS_PI)
        dlam += p2;

    phis = sin(phi);

    if(!phi)
    {
        *E = a * dlam + E0;
        *N = -AM0 + N0;
    }
    else
    {
        NN = a / pow((double)1.-e2*phis*phis,(double).5);
        NNot = NN / tan(phi);
        lat = c0 * phi;
        phis2 = c1 * sin((double)2.0*phi);
        phis4 = c2 * sin((double)4.0*phi);
        phis6 = c3 * sin((double)6.0*phi);
        MM    = a*(lat-phis2+phis4-phis6);        
        EE    = dlam *phis;
        *E = NNot * sin(EE) + E0;
        *N = MM - AM0 + NNot * ((double)1.-cos(EE)) + N0;
    }

    return;
}




/* @func GPS_Math_Polycon_EN_To_LatLon **********************************
**
** Convert Polyconic easting and northing projection
** to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Polycon_EN_To_LatLon(double E, double N, double *phi,
                                   double *lambda, double phi0, double M0,
                                   double E0, double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double AM0;
    double c0;
    double c1;
    double c2;
    double c3;
    double j;
    double te4;
    double phi0s2;
    double phi0s4;
    double phi0s6;

    double dx;
    double dy;
    double dxoa;
    double AA;
    double BB;
    double CC=(double)0.;
    double PHIn;
    double PHId;
    double PHIs;
    double PHI;
    double PHIs2;
    double PHIs4;
    double PHIs6;
    double Mn;
    double Mnp;
    double Ma;
    double AAMa;
    double mpb;
    double AAmin;
    double tol;
    double lat;
    

    phi0   = GPS_Math_Deg_To_Rad(phi0);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(M0>GPS_PI)
        M0 -= p2;
    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    
    j = (double)45.0*e6/(double)1024.0;
    te4 = (double)3.0 * e4;
    c0 = (double)1.0-e2/(double)4.0-te4/(double)64.0-(double)5.0*e6/
        (double)256.0;
    c1 = (double)3.0*e2/(double)8.0+te4/(double)32.0+j;
    c2 = (double)15.0*e4/(double)256.0+j;
    c3 = (double)35.0*e6/(double)3072.0;
    
    lat = c0 * phi0;

    phi0s2 = c1 * sin((double)2.0*phi0);
    phi0s4 = c2 * sin((double)4.0*phi0);
    phi0s6 = c3 * sin((double)6.0*phi0);
    AM0    = a*(lat-phi0s2+phi0s4-phi0s6);

    tol = (double)1.0e-12;

    dx = E - E0;
    dy = N - N0;
    dxoa = dx/a;
    if((((-AM0-(double)1.)<dy)&&(dy<(-AM0+(double)1.))))
    {
        *phi = (double)0.;
        *lambda = dxoa + M0;
    }
    else
    {
        AA = (AM0+dy) / a;
        BB = dxoa * dxoa + (AA*AA);
        PHIn = AA;
        PHId = (double)1.;
        
        while(fabs(PHId)>tol)
        {
            PHIs = sin(PHIn);
            CC = pow((double)1.-e2*PHIs*PHIs,(double).5) * tan(PHIn);
            PHI = PHIn * c0;
            PHIs2 = c1 * sin((double)2.0*PHIn);
            PHIs4 = c2 * sin((double)4.0*PHIn);
            PHIs6 = c3 * sin((double)6.0*PHIn);
            Mn    = a*(PHI-PHIs2+PHIs4-PHIs6);
            Mnp = c0 - (double)2.*c1*cos((double)2.*PHIn)+(double)4.*c2*
                cos((double)4.*PHIn)-(double)6.*c3*cos((double)6.*PHIn);
            Ma = Mn / a;
            AAMa = AA * Ma;
            mpb = Ma*Ma+BB;
            AAmin = AA - Ma;
            PHId = (AAMa*CC+AAmin-(double).5*mpb*CC)/
                (e2*PHIs2*(mpb-(double)2.*AAMa) /
                 (double)4.*CC+AAmin*(CC*Mnp-(double)2./PHIs2)-Mnp);
            PHIn -= PHId;
        }
        *phi = PHIn;
        
        if(*phi>po2)
            *phi = po2;
        else if(*phi<-po2)
            *phi = -po2;

        if((((po2-(double).00001)<fabs(*phi))&&
            (fabs(*phi)<(po2+(double).00001))))
            *lambda = M0;
        else
            *lambda = (asin(dxoa*CC)) / sin(*phi) + M0;
    }
    
    if(*lambda>GPS_PI)
        *lambda -= p2;
    if(*lambda<-GPS_PI)
        *lambda += p2;

    if(*lambda>GPS_PI)
        *lambda = GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_Sinusoid_LatLon_To_EN **********************************
**
** Convert latitude and longitude to Sinusoidal projection easting and
** northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Sinusoid_LatLon_To_EN(double phi, double lambda, double *E,
                                    double *N, double M0, double E0,
                                    double N0, double a, double b)
{
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double p2;
    double po2;
    double c0;
    double c1;
    double c2;
    double c3;
    double A1;
    double A0;
    double A2;
    double A3;
    double E1;
    double E2;
    double E3;
    double E4;
    double j;
    double om0;
    double som0;
    
    double phis;
    double phis2;
    double phis4;
    double phis6;
    double mm;
    double MM;
    double dlam;


    phi     = GPS_Math_Deg_To_Rad(phi);
    lambda  = GPS_Math_Deg_To_Rad(lambda);
    M0      = GPS_Math_Deg_To_Rad(M0);

    po2 = (double)GPS_PI / (double)2.0;
    p2  = (double)GPS_PI * (double)2.0;
    a2  = a*a;
    b2  = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e4*e2;

    j = (double)45.*e6/(double)1024.;
    c0 = (double)1.-e2/(double)4.-(double)3.*e4/(double)64.-(double)5.*
        e6/(double)256.;
    c1 = (double)3.*e2/(double)8.+(double)3.*e4/(double)32.+j;
    c2 = (double)15.*e4/(double)256.+j;
    c3 = (double)35.*e6/(double)3072.;
    om0 = (double)1. - e2;
    som0 = pow(om0,(double).5);
    E1 = ((double)1.-som0)/((double)1.+som0);
    E2 = E1*E1;
    E3 = E1*E2;
    E4 = E1*E3;
    A0 = (double)3.*E1/(double)2.-(double)27.*E3/(double)32.;
    A1 = (double)21.*E2/(double)16.-(double)55.*E4/(double)32.;
    A2 = (double)151.*E3/(double)96.;
    A3 = (double)1097.*E4/(double)512.;

    if(M0>GPS_PI)
        M0 -= p2;
    
    phis = sin(phi);

    dlam = lambda - M0;
    if(dlam>GPS_PI)
        dlam-=p2;
    if(dlam<-GPS_PI)
        dlam+=p2;

    mm = pow((double)1.-e2*phis*phis,(double).5);
    phis2 = c1 * sin((double)2.*phi);
    phis4 = c2 * sin((double)4.*phi);
    phis6 = c3 * sin((double)6.*phi);
    MM = a * (c0*phi-phis2+phis4-phis6);
    


    *E = a*dlam*cos(phi)/mm+E0;
    *N = MM + N0;

    return;
}




/* @func GPS_Math_Sinusoid_EN_To_LatLon **********************************
**
** Convert latitude and longitude to Sinusoidal projection easting and
** northing
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Sinusoid_EN_To_LatLon(double E, double N, double *phi,
                                    double *lambda, double M0, double E0,
                                    double N0, double a, double b)
{
    double a2;
    double b2;
    double e2;
    double e4;
    double e6;
    double p2;
    double po2;
    double c0;
    double c1;
    double c2;
    double c3;
    double A1;
    double A0;
    double A2;
    double A3;
    double E1;
    double E2;
    double E3;
    double E4;
    double j;
    double om0;
    double som0;
    
    double dx;
    double dy;
    double mu;
    double mu2s;
    double mu4s;
    double mu6s;
    double mu8s;
    double phis;


    M0      = GPS_Math_Deg_To_Rad(M0);

    po2 = (double)GPS_PI / (double)2.0;
    p2  = (double)GPS_PI * (double)2.0;
    a2  = a*a;
    b2  = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e4*e2;

    j = (double)45.*e6/(double)1024.;
    c0 = (double)1.-e2/(double)4.-(double)3.*e4/(double)64.-(double)5.*
        e6/(double)256.;
    c1 = (double)3.*e2/(double)8.+(double)3.*e4/(double)32.+j;
    c2 = (double)15.*e4/(double)256.+j;
    c3 = (double)35.*e6/(double)3072.;
    om0 = (double)1. - e2;
    som0 = pow(om0,(double).5);
    E1 = ((double)1.-som0)/((double)1.+som0);
    E2 = E1*E1;
    E3 = E1*E2;
    E4 = E1*E3;
    A0 = (double)3.*E1/(double)2.-(double)27.*E3/(double)32.;
    A1 = (double)21.*E2/(double)16.-(double)55.*E4/(double)32.;
    A2 = (double)151.*E3/(double)96.;
    A3 = (double)1097.*E4/(double)512.;


    dx = E - E0;
    dy = N - N0;

    mu = dy/(c0*a);
    mu2s = A0 * sin((double)2.*mu);
    mu4s = A1 * sin((double)4.*mu);
    mu6s = A2 * sin((double)6.*mu);
    mu8s = A3 * sin((double)8.*mu);
    *phi = mu + mu2s + mu4s + mu6s + mu8s;
    
    if(*phi>po2)
        *phi=po2;
    else if (*phi<-po2)
        *phi=-po2;

    if((((po2-(double)1.0e-8)<fabs(*phi))&&(fabs(*phi)<(po2+(double)1.0e-8))))
        *lambda = M0;
    else
    {
        phis = sin(*phi);
        *lambda = M0 + dx*pow((double)1.0-e2*phis*phis,(double).5) /
            (a*cos(*phi));
    }

    if(*lambda>GPS_PI)
        *lambda-=p2;
    if(*lambda<-GPS_PI)
        *lambda+=p2;

    if(*lambda>GPS_PI)
        *lambda=GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_TCylEA_LatLon_To_EN **********************************
**
** Convert latitude and longitude to transverse cylindrical  equal area
** projection easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_TCylEA_LatLon_To_EN(double phi, double lambda, double *E,
                                  double *N, double phi0, double M0, double E0,
                                  double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e;
    double e2;
    double e4;
    double e6;
    double AM0;
    double qp;
    double om;
    double oo;
    double c0;
    double c1;
    double c2;
    double c3;
    double b0;
    double b1;
    double B2;
    double b3;
    double A0;
    double A1;
    double A2;
    double sf;
    double x;
    double som;
    double phis;
    double j;
    double te4;
    double lat;
    double phi0s2;
    double phi0s4;
    double phi0s6;
    double E1;
    double E2;
    double E3;
    double E4;
    
    double dlam;
    double qq;
    double qqo;
    double bt;
    double btc;
    double PHI;
    double PHIs2;
    double PHIs4;
    double PHIs6;
    double bts2;
    double bts4;
    double bts6;
    double PHIc;
    double PHIcs;
    double Mc;
    

    
    sf = (double)1.0; /* scale factor */
    
    lambda = GPS_Math_Deg_To_Rad(lambda);
    phi    = GPS_Math_Deg_To_Rad(phi);
    phi0   = GPS_Math_Deg_To_Rad(phi0);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(M0>GPS_PI)
        M0 -= p2;
    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    e  = pow(e2,(double).5);
    om = (double)1.-e2;
    som  = pow(om,(double).5);
    oo = (double)1./((double)2.*e);

    phis = sin(po2);
    x  = e * phis;
    qp = om*(phis/((double)1.-e2*phis*phis)-oo*
              log(((double)1.-x)/((double)1.+x)));
    
    A0 = e2 / (double)3.+(double)31.*e4/(double)180.+(double)517.*
        e6/(double)5040.;
    A1 = (double)23.*e4/(double)360.+(double)251.*e6/(double)3780.;
    A2 = (double)761.*e6/(double)45360.;
    
    E1 = ((double)1.0-som) / ((double)1.0+som);
    E2 = E1*E1;
    E3 = E2*E1;
    E4 = E3*E1;

    b0 = (double)3.*E1/(double)2.-(double)27.*E3/(double)32.;
    b1 = (double)21.*E2/(double)16.-(double)55.*E4/(double)32.;
    B2 = (double)151.*E3/(double)96.;
    b3 = (double)1097.*E4/(double)512.;
    
    
    j = (double)45.0*e6/(double)1024.0;
    te4 = (double)3.0 * e4;
    c0 = (double)1.0-e2/(double)4.0-te4/(double)64.0-(double)5.0*e6/
        (double)256.0;
    c1 = (double)3.0*e2/(double)8.0+te4/(double)32.0+j;
    c2 = (double)15.0*e4/(double)256.0+j;
    c3 = (double)35.0*e6/(double)3072.0;
    
    lat = c0 * phi0;

    phi0s2 = c1 * sin((double)2.0*phi0);
    phi0s4 = c2 * sin((double)4.0*phi0);
    phi0s6 = c3 * sin((double)6.0*phi0);
    AM0 = a*(lat-phi0s2+phi0s4-phi0s6);


    dlam = lambda - M0;
    if(dlam>GPS_PI)
        dlam -= p2;
    if(dlam<-GPS_PI)
        dlam += p2;

    phis = sin(phi);

    if(phi==po2)
    {
        qq = qp;
        qqo = (double)1.;
    }
    else
    {
        x = e * phis;
        qq = om*(phis/((double)1.-e2*phis*phis)-oo*
              log(((double)1.-x)/((double)1.+x)));
        qqo = qq/qp;
    }

    if(qqo>(double)1.)
        qqo = (double)1.;
    else if(qqo<(double)-1.)
        qqo = (double)-1.;

    bt = asin(qqo);
    btc = atan(tan(bt)/cos(dlam));

    if((fabs(btc)-po2)>(double)1.0e-8)
        PHIc = btc;
    else
    {
        bts2 = A0 * sin((double)2.0*btc);
        bts4 = A1 * sin((double)4.0*btc);
        bts6 = A2 * sin((double)6.0*btc);
        PHIc = btc + bts2 + bts4 + bts6;
    }

    PHIcs = sin(PHIc);
    *E = a*cos(bt)*cos(PHIc)*sin(dlam)/(sf*cos(btc)*
                                        pow((double)1.-e2*PHIcs*PHIcs,
                                            (double).5)) + E0;
    PHI = c0 * PHIc;
    PHIs2 = c1 * sin((double)2.0*PHIc);
    PHIs4 = c2 * sin((double)4.0*PHIc);
    PHIs6 = c3 * sin((double)6.0*PHIc);
    Mc = a*(PHI-PHIs2+PHIs4-PHIs6);
    
    *N = sf * (Mc-AM0) + N0;
    
    return;
}




/* @func GPS_Math_TCylEA_EN_To_LatLon **********************************
**
** Convert transverse cylindrical equal area easting and northing projection
** to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] M0 [double] central meridian (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_TCylEA_EN_To_LatLon(double E, double N, double *phi,
                                  double *lambda, double phi0, double M0,
                                  double E0, double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e;
    double e2;
    double e4;
    double e6;
    double AM0;
    double qp;
    double om;
    double oo;
    double c0;
    double c1;
    double c2;
    double c3;
    double b0;
    double b1;
    double B2;
    double b3;
    double A0;
    double A1;
    double A2;
    double sf;
    double x;
    double som;
    double phis;
    double j;
    double te4;
    double lat;
    double phi0s2;
    double phi0s4;
    double phi0s6;
    double E1;
    double E2;
    double E3;
    double E4;
    
    double dx;
    double dy;
    double bt;
    double btc;
    double btp;
    double btcc;
    double Mc;
    double Muc;
    double mus2;
    double mus4;
    double mus6;
    double mus8;
    double bts2;
    double bts4;
    double bts6;
    double PHIc;
    double Qc;
    double Qco;
    double t;
    
    
    sf = (double)1.0; /* scale factor */
    
    phi0   = GPS_Math_Deg_To_Rad(phi0);
    M0     = GPS_Math_Deg_To_Rad(M0);
    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(M0>GPS_PI)
        M0 -= p2;
    a2 = a*a;
    b2 = b*b;
    e2 = (a2-b2)/a2;
    e4 = e2*e2;
    e6 = e2*e4;
    e  = pow(e2,(double).5);
    om = (double)1.-e2;
    som  = pow(om,(double).5);
    oo = (double)1./((double)2.*e);

    phis = sin(po2);
    x  = e * phis;
    qp = om*(phis/((double)1.-e2*phis*phis)-oo*
              log(((double)1.-x)/((double)1.+x)));
    
    A0 = e2 / (double)3.+(double)31.*e4/(double)180.+(double)517.*
        e6/(double)5040.;
    A1 = (double)23.*e4/(double)360.+(double)251.*e6/(double)3780.;
    A2 = (double)761.*e6/(double)45360.;
    
    E1 = ((double)1.0-som) / ((double)1.0+som);
    E2 = E1*E1;
    E3 = E2*E1;
    E4 = E3*E1;

    b0 = (double)3.*E1/(double)2.-(double)27.*E3/(double)32.;
    b1 = (double)21.*E2/(double)16.-(double)55.*E4/(double)32.;
    B2 = (double)151.*E3/(double)96.;
    b3 = (double)1097.*E4/(double)512.;
    
    
    j = (double)45.0*e6/(double)1024.0;
    te4 = (double)3.0 * e4;
    c0 = (double)1.0-e2/(double)4.0-te4/(double)64.0-(double)5.0*e6/
        (double)256.0;
    c1 = (double)3.0*e2/(double)8.0+te4/(double)32.0+j;
    c2 = (double)15.0*e4/(double)256.0+j;
    c3 = (double)35.0*e6/(double)3072.0;
    
    lat = c0 * phi0;

    phi0s2 = c1 * sin((double)2.0*phi0);
    phi0s4 = c2 * sin((double)4.0*phi0);
    phi0s6 = c3 * sin((double)6.0*phi0);
    AM0 = a*(lat-phi0s2+phi0s4-phi0s6);



    dx = E - E0;
    dy = N - N0;
    Mc = AM0 + dy/sf;
    Muc = Mc / (c0*a);
    
    mus2 = b0 * sin((double)2.0*Muc);
    mus4 = b1 * sin((double)4.0*Muc);
    mus6 = B2 * sin((double)6.0*Muc);
    mus8 = b3 * sin((double)6.0*Muc);
    PHIc = Muc + mus2 + mus4 + mus6 + mus8;

    phis = sin(PHIc);
    x = e * phis;
    Qc = om*(phis/((double)1.-e2*phis*phis)-oo*
              log(((double)1.-x)/((double)1.+x)));
    Qco = Qc/qp;

    if(Qco>(double)1.)
        Qco = (double)1.;
    else if(Qco<(double)-1.)
        Qco = (double)-1.;

    btc = asin(Qco);
    btcc = cos(btc);
    t = sf*dx*btcc*pow((double)1.-e2*phis*phis,(double).5)/(a*cos(PHIc));
    if(t>(double)1.)
        t=(double)1.;
    else if(t<(double)-1.)
        t=(double)-1.;
    btp = -asin(t);
    bt = asin(cos(btp)*sin(btc));
    
    bts2 = A0 * sin((double)2.0*bt);
    bts4 = A1 * sin((double)4.0*bt);
    bts6 = A2 * sin((double)6.0*bt);
    *phi = bt + bts2 + bts4 + bts6;
    *lambda = M0 - atan(tan(btp)/btcc);

    if(*phi>po2)
        *phi = po2;
    else if(*phi<-po2)
        *phi = -po2;

    if(*lambda>GPS_PI)
        *lambda -= p2;
    if(*lambda<-GPS_PI)
        *lambda += p2;

    if(*lambda>GPS_PI)
        *lambda = GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_Mercator_LatLon_To_EN **********************************
**
** Convert latitude and longitude to standard Mercator projection
**  easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] lambda0 [double] longitude of origin (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Mercator_LatLon_To_EN(double phi, double lambda, double *E,
                                    double *N, double phi0, double lambda0,
                                    double E0, double N0, double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e3;
    double e;
    double es;
    double ab;
    double bb;
    double cb;
    double db;
    double ml;
    double phi0s;
    double sf;
    
    double dlam;
    double ct;
    double ex;
    double tt;
    double pt;
    
    
    lambda  = GPS_Math_Deg_To_Rad(lambda);
    phi     = GPS_Math_Deg_To_Rad(phi);
    phi0    = GPS_Math_Deg_To_Rad(phi0);
    lambda0 = GPS_Math_Deg_To_Rad(lambda0);

    ml = ((double)GPS_PI*(double)89.5)/(double)180.;
    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(lambda0>GPS_PI)
        lambda0 -= p2;
    a2 = a*a;
    b2 = b*b;
    es = (a2-b2)/a2;
    e2 = es*es;
    e3 = e2*es;
    e4 = e3*es;

    e  = pow(es,(double).5);
    phi0s = sin(phi0);
    sf = (double)1. / (pow((double)1.-es*phi0s*phi0s,(double).5)/cos(phi0));

    ab = es/(double)2.+(double)5.*e2/(double)24.+e3/(double)12.+(double)13.*
        e4/(double)360.;
    bb = (double)7.*e2/(double)48.+(double)29.*e3/(double)240.+
        (double)811.*e4/(double)11520.;
    cb = (double)7.*e3/(double)120.+(double)81.*e4/(double)1120.;
    db = (double)4279.*e4/(double)161280.;



    if(lambda>(double)GPS_PI)
        lambda -= p2;

    dlam = lambda - lambda0;
    if(dlam>GPS_PI)
        dlam -= p2;
    if(dlam<-GPS_PI)
        dlam += p2;


    ex = e * sin(phi);
    tt = tan((double)GPS_PI/(double)4.+phi/(double)2.);
    pt = pow((((double)1.-ex)/((double)1.+ex)),(e/(double)2.));
    
    ct = tt * pt;
    *N = sf * a * log(ct) + N0;
    *E = sf * a * dlam + E0;

    return;
}




/* @func GPS_Math_Mercator_EN_To_LatLon **********************************
**
** Convert standard Mercator easting and northing projection
** to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] lambda0 [double] longitude of origin (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_Mercator_EN_To_LatLon(double E, double N, double *phi,
                                    double *lambda, double phi0,
                                    double lambda0, double E0, double N0,
                                    double a, double b)
{
    double p2;
    double po2;
    double a2;
    double b2;
    double e2;
    double e4;
    double e3;
    double e;
    double es;
    double ab;
    double bb;
    double cb;
    double db;
    double ml;
    double phi0s;
    double sf;
    
    double dx;
    double dy;
    double x;
    
    phi0    = GPS_Math_Deg_To_Rad(phi0);
    lambda0 = GPS_Math_Deg_To_Rad(lambda0);

    ml = ((double)GPS_PI*(double)89.5)/(double)180.;
    
    p2 = (double)GPS_PI * (double)2.0;
    po2 = (double)GPS_PI / (double)2.0;
    if(lambda0>GPS_PI)
        lambda0 -= p2;
    a2 = a*a;
    b2 = b*b;
    es = (a2-b2)/a2;
    e2 = es*es;
    e3 = e2*es;
    e4 = e3*es;

    e  = pow(es,(double).5);
    phi0s = sin(phi0);
    sf = (double)1. / (pow((double)1.-es*phi0s*phi0s,(double).5)/cos(phi0));

    ab = es/(double)2.+(double)5.*e2/(double)24.+e3/(double)12.+(double)13.*
        e4/(double)360.;
    bb = (double)7.*e2/(double)48.+(double)29.*e3/(double)240.+
        (double)811.*e4/(double)11520.;
    cb = (double)7.*e3/(double)120.+(double)81.*e4/(double)1120.;
    db = (double)4279.*e4/(double)161280.;

    dx = E - E0;
    dy = N - N0;
    *lambda = lambda0 + dx / (sf*a);
    x = (double)GPS_PI / (double)2. -
        (double)2.*atan((double)1./exp(dy/(sf*a)));
    *phi = x+ab*sin((double)2.*x)+bb*sin((double)4.*x)+cb*sin((double)6.*x)
        + db*sin((double)8.*x);

    if(*phi>po2)
        *phi = po2;
    else if(*phi<-po2)
        *phi = -po2;

    if(*lambda>GPS_PI)
        *lambda -= p2;
    if(*lambda<-GPS_PI)
        *lambda += p2;

    if(*lambda>GPS_PI)
        *lambda = GPS_PI;
    else if(*lambda<-GPS_PI)
        *lambda=-GPS_PI;

    *lambda = GPS_Math_Rad_To_Deg(*lambda);
    *phi    = GPS_Math_Rad_To_Deg(*phi);

    return;
}




/* @func GPS_Math_TMerc_LatLon_To_EN **********************************
**
** Convert latitude and longitude to transverse Mercator projection
**  easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] lambda0 [double] longitude of origin (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] F0 [double] scale factor
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_TMerc_LatLon_To_EN(double phi, double lambda, double *E,
                                 double *N, double phi0, double lambda0,
                                 double E0, double N0, double F0,
                                 double a, double b)
{
    GPS_Math_LatLon_To_EN(E,N,phi,lambda,N0,E0,phi0,lambda0,F0,a,b);
    
    return;
}




/* @func GPS_Math_TMerc_EN_To_LatLon **********************************
**
** Convert transverse Mercator easting and northing projection
** to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi0 [double] latitude of origin (deg)
** @param [r] lambda0 [double] longitude of origin (deg)
** @param [r] E0 [double] false easting
** @param [r] N0 [double] false northing
** @param [r] F0 [double] scale factor
** @param [r] a [double] semi-major axis
** @param [r] b [double] semi-minor axis
**
** @return [void]
************************************************************************/
void GPS_Math_TMerc_EN_To_LatLon(double E, double N, double *phi,
                                 double *lambda, double phi0, double lambda0,
                                 double E0, double N0, double F0,
                                 double a, double b)
{
    GPS_Math_EN_To_LatLon(E,N,phi,lambda,N0,E0,phi0,lambda0,F0,a,b);
    
    return;
}




/* @func GPS_Math_Swiss_LatLon_To_EN ***********************************
**
** Convert latitude and longitude to Swiss grid easting and northing
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [w] E [double *] easting (metre)
** @param [w] N [double *] northing (metre)
** @param [r] phi0 [double] latitude origin (deg)     [normally 46.95240556]
** @param [r] lambda0 [double] longitude origin (deg) [normally  7.43958333]
** @param [r] E0 [double] false easting (metre)       [normally 600000.0]
** @param [r] N0 [double] false northing (metre)      [normally 200000.0]
** @param [r] a [double] semi-major axis              [normally 6377397.000]
** @param [r] b [double] semi-minor axis              [normally 6356078.823]
**
** @return [void]
***************************************************************************/
void GPS_Math_Swiss_LatLon_To_EN(double phi, double lambda, double *E,
                                 double *N,double phi0,double lambda0,
                                 double E0, double N0, double a, double b)

{
    double a2;
    double b2;
    double esq;
    double e;
    double c;
    double ephi0p;
    double phip;
    double sphip;
    double phid;
    double slambda2;
    double lambda1;
    double lambda2;
    double K;
    double po4;
    double w;
    double R;
    
    lambda0 = GPS_Math_Deg_To_Rad(lambda0);
    phi0    = GPS_Math_Deg_To_Rad(phi0);
    lambda  = GPS_Math_Deg_To_Rad(lambda);
    phi     = GPS_Math_Deg_To_Rad(phi);

    po4=GPS_PI/(double)4.0;
    
    a2 = a*a;
    b2 = b*b;
    esq = (a2-b2)/a2;
    e   = pow(esq,(double)0.5);

    c = sqrt(1+((esq*pow(cos(phi0),(double)4.))/((double)1.-esq)));

    ephi0p = asin(sin(phi0)/c);

    K = log(tan(po4+ephi0p/(double)2.)) - c*(log(tan(po4+phi0/(double)2.)) -
         e/(double)2. * log(((double)1.+e*sin(phi0)) /
         ((double)1.-e*sin(phi0))));
    lambda1 = c*(lambda-lambda0);
    w = c*(log(tan(po4+phi/(double)2.)) - e/(double)2. *
           log(((double)1.+e*sin(phi)) / ((double)1.-e*sin(phi)))) + K;
    

    phip = (double)2. * (atan(exp(w)) - po4);
        
    sphip = cos(ephi0p) * sin(phip) - sin(ephi0p) * cos(phip) * cos(lambda1);
    phid  = asin(sphip);
        
    slambda2 = cos(phip)*sin(lambda1) / cos(phid);
    lambda2  = asin(slambda2);

    R = a*sqrt((double)1.-esq) / ((double)1.-esq*sin(phi0) * sin(phi0));

    *N = R*log(tan(po4 + phid/(double)2.)) + N0;
    *E = R*lambda2 + E0;
    return;
}




/* @func GPS_Math_Swiss_EN_To_LatLon ************************************
**
** Convert Swiss Grid easting and northing to latitude and longitude
**
** @param [r] E [double] easting (metre)
** @param [r] N [double] northing (metre)
** @param [w] phi [double *] latitude (deg)
** @param [w] lambda [double *] longitude (deg)
** @param [r] phi0 [double] latitude origin (deg)     [normally 46.95240556]
** @param [r] lambda0 [double] longitude origin (deg) [normally  7.43958333]
** @param [r] E0 [double] false easting (metre)       [normally 600000.0]
** @param [r] N0 [double] false northing (metre)      [normally 200000.0]
** @param [r] a [double] semi-major axis              [normally 6377397.000]
** @param [r] b [double] semi-minor axis              [normally 6356078.823]
**
** @return [void]
*************************************************************************/

void GPS_Math_Swiss_EN_To_LatLon(double E, double N, double *phi,
                                 double *lambda, double phi0, double lambda0,
                                 double E0, double N0, double a, double b)
{
    double a2;
    double b2;
    double esq;
    double e;
    double R;
    double c;
    double po4;
    double phid;
    double phi1;
    double lambdad;
    double lambda1;
    double slambda1;
    double ephi0p;
    double sphip;
    double tol;
    double cr;
    double C;
    double K;
    
    lambda0 = GPS_Math_Deg_To_Rad(lambda0);
    phi0    = GPS_Math_Deg_To_Rad(phi0);

    po4=GPS_PI/(double)4.0;
    tol=(double)0.00001;
    
    a2 = a*a;
    b2 = b*b;
    esq = (a2-b2)/a2;
    e   = pow(esq,(double)0.5);

    R = a*sqrt((double)1.-esq) / ((double)1.-esq*sin(phi0) * sin(phi0));

    phid = (double)2.*(atan(exp((N - N0)/R)) - po4);
    lambdad = (E - E0)/R;

    c = sqrt((double)1.+((esq * pow(cos(phi0), (double)4.)) /
                         ((double)1.-esq))); 
    ephi0p = asin(sin(phi0) / c);

    sphip = cos(ephi0p)*sin(phid) + sin(ephi0p)*cos(phid)*cos(lambdad);
    phi1 = asin(sphip);

    slambda1 = cos(phid)*sin(lambdad)/cos(phi1);
    lambda1  = asin(slambda1);

    *lambda = GPS_Math_Rad_To_Deg((lambda1/c + lambda0));

    K = log(tan(po4 + ephi0p/(double)2.)) -c*(log(tan(po4 + phi0/(double)2.)) 
        - e/(double)2. * log(((double)1.+e*sin(phi0)) / 
        ((double)1.-e*sin(phi0))));
    C = (K - log(tan(po4 + phi1/(double)2.)))/c;

    do
    {
        cr = (C + log(tan(po4 + phi1/(double)2.)) - e/(double)2. *
              log(((double)1.+e*sin(phi1)) / ((double)1.-e*sin(phi1)))) *
                  ((((double)1.-esq*sin(phi1)*sin(phi1)) * cos(phi1)) /
                   ((double)1.-esq));
        phi1 -= cr;
    }
    while (fabs(cr) > tol);

    *phi = GPS_Math_Rad_To_Deg(phi1);

    return;
}


/* @func GPS_Math_LatLonH_To_XYZ **********************************
**
** Convert latitude and longitude and ellipsoidal height to
** X, Y & Z coordinates
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [r] H [double] ellipsoidal height (metres)
** @param [w] x [double *] X
** @param [w] y [double *] Y
** @param [w] z [double *] Z
** @param [r] a [double] semi-major axis (metres)
** @param [r] b [double] semi-minor axis (metres)
**
** @return [void]
************************************************************************/
void GPS_Math_LatLonH_To_XYZ(double phi, double lambda, double H,
                             double *x, double *y, double *z,
                             double a, double b)
{
    double esq;
    double nu;

    phi    = GPS_Math_Deg_To_Rad(phi);
    lambda = GPS_Math_Deg_To_Rad(lambda);
    

    esq   = ((a*a)-(b*b)) / (a*a);
    
    nu    = a / pow(((double)1.0-esq*sin(phi)*sin(phi)),(double)0.5);
    *x    = (nu+H) * cos(phi) * cos(lambda);
    *y    = (nu+H) * cos(phi) * sin(lambda);
    *z    = (((double)1.0-esq)*nu+H) * sin(phi);

    return;
}



/* @func GPS_Math_WGS84LatLonH_To_XYZ **********************************
**
** Convert WGS84 latitude and longitude and ellipsoidal height to
** X, Y & Z coordinates
**
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [r] H [double] ellipsoidal height (metres)
** @param [w] x [double *] X
** @param [w] y [double *] Y
** @param [w] z [double *] Z
**
** @return [void]
************************************************************************/
void GPS_Math_WGS84LatLonH_To_XYZ(double phi, double lambda, double H,
                                  double *x, double *y, double *z)
{
    double a = 6378137.000;
    double b = 6356752.3141;

    GPS_Math_LatLonH_To_XYZ(phi,lambda,H,x,y,z,a,b);

    return;
}


/* @func GPS_Math_WGS84_To_Known_Datum_M ********************************
**
** Transform WGS84 to other datum using Molodensky
**
** @param [r] Sphi [double] source latitude (deg)
** @param [r] Slam [double] source longitude (deg)
** @param [r] SH   [double] source height  (metres)
** @param [w] Dphi [double *] dest latitude (deg)
** @param [w] Dlam [double *] dest longitude (deg)
** @param [w] DH   [double *] dest height  (metres)
** @param [r] n    [int32] datum number from GPS_Datum structure
**
** @return [void]
************************************************************************/
void GPS_Math_WGS84_To_Known_Datum_M(double Sphi, double Slam, double SH,
                                     double *Dphi, double *Dlam, double *DH,
                                     int32 n)
{
    double Sa;
    double Sif;
    double Da;
    double Dif;
    double x;
    double y;
    double z;
    int32    idx;
    
    Sa  = (double) 6378137.0;
    Sif = (double) 298.257223563;
    
    idx  = GPS_Datum[n].ellipse;
    Da   = GPS_Ellipse[idx].a;
    Dif  = GPS_Ellipse[idx].invf;
    x    = -GPS_Datum[n].dx;
    y    = -GPS_Datum[n].dy;
    z    = -GPS_Datum[n].dz;

    GPS_Math_Molodensky(Sphi,Slam,SH,Sa,Sif,Dphi,Dlam,DH,Da,Dif,x,y,z);

    return;
}


/* @funcstatic GPS_Math_LatLon_To_UTM_Param *****************************
**
** Transform NAD33
**
** @param [r] lat  [double] NAD latitude (deg)
** @param [r] lon  [double] NAD longitude (deg)
** @param [w] zone [int32 *]  zone number
** @param [w] zc   [char *] zone character
** @param [w] Mc   [double *] central meridian
** @param [w] E0   [double *] false easting
** @param [w] N0   [double *] false northing
** @param [w] F0   [double *] scale factor
**
** @return [int32] success
************************************************************************/
static int32 GPS_Math_LatLon_To_UTM_Param(double lat, double lon, int32 *zone,
                                          char *zc, double *Mc, double *E0,
                                          double *N0, double *F0)
{
    int32 ilon;
    int32 ilat;
    int32 psign;
    int32 lsign;
    
    if(lat >= (double)84.0 || lat < (double)-80.0)
        return 0;
    
    psign = lsign = 0;
    if(lon < (double)0.0)
        lsign=1;
    if(lat < (double)0.0)
        psign=1;

    ilon = abs((int32)lon);
    ilat = abs((int32)lat);
    
    if(!lsign)
    {
        *zone = 31 + (ilon / 6);
        *Mc   = (double)((ilon / 6) * 6 + 3);
    }
    else
    {
        *zone = 30 - (ilon / 6);
        *Mc   = -(double)((ilon / 6) * 6 + 3);
    }

    if(!psign)
    {
        *zc = 'N' + ilat / 8;
        if(*zc > 'N') ++*zc;
    }
    else
    {
        *zc = 'M' - (ilat / 8);
        if(*zc <= 'I') --*zc;
    }


    if(lat>=(double)56.0 && lat<(double)64.0 && lon>=(double)3.0 &&
       lon<(double)12.0)
    {
        *zone = 32;
        *zc   = 'V';
        *Mc   = (double)9.0;
    }
    
    if(*zc=='X' && lon>=(double)0.0 && lon<(double)42.0)
    {
        if(lon<(double)9.0)
        {
            *zone = 31;
            *Mc   = (double)3.0;
        }
        else if(lon<(double)21.0)
        {
            *zone = 33;
            *Mc   = (double)15.0;
        }
        else if(lon<(double)33.0)
        {
            *zone = 35;
            *Mc   = (double)27.0;
        }
        else
        {
            *zone = 37;
            *Mc   = (double)39.0;
        }
    }
    
    if(!psign)
        *N0 = (double)0.0;
    else
        *N0 = (double)10000000;

    *E0 = (double)500000;
    *F0 = (double)0.9996;
    
    return 1;
}


/* @func GPS_Math_NAD83_To_UTM_EN **************************************
**
** Transform NAD33 lat/lon to UTM zone, easting and northing
**
** @param [r] lat  [double] NAD latitude (deg)
** @param [r] lon  [double] NAD longitude (deg)
** @param [w] E    [double *] easting (metres)
** @param [w] N    [double *] northing (metres)
** @param [w] zone [int32 *]  zone number
** @param [w] zc   [char *] zone character
**
** @return [int32] success
************************************************************************/
int32 GPS_Math_NAD83_To_UTM_EN(double lat, double lon, double *E,
                               double *N, int32 *zone, char *zc)
{
    double phi0;
    double lambda0;
    double N0;
    double E0;
    double F0;
    double a;
    double b;

    if(!GPS_Math_LatLon_To_UTM_Param(lat,lon,zone,zc,&lambda0,&E0,
                                     &N0,&F0))
        return 0;

    phi0 = (double)0.0;

    a = (double) GPS_Ellipse[21].a;
    b = a - (a/GPS_Ellipse[21].invf);

    GPS_Math_LatLon_To_EN(E,N,lat,lon,N0,E0,phi0,lambda0,F0,a,b);

    return 1;
}


/* @func GPS_Math_WGS84_To_UTM_EN **************************************
**
** Transform WGS84 lat/lon to UTM zone, easting and northing
**
** @param [r] lat  [double] WGS84 latitude (deg)
** @param [r] lon  [double] WGS84 longitude (deg)
** @param [w] E    [double *] easting (metres)
** @param [w] N    [double *] northing (metres)
** @param [w] zone [int32 *]  zone number
** @param [w] zc   [char *] zone character
**
** @return [int32] success
************************************************************************/
int32 GPS_Math_WGS84_To_UTM_EN(double lat, double lon, double *E,
                               double *N, int32 *zone, char *zc)
{
    double phi;
    double lambda;
    double H;
    
    GPS_Math_WGS84_To_Known_Datum_M(lat,lon,0,&phi,&lambda,&H,77);
    if(!GPS_Math_NAD83_To_UTM_EN(phi,lambda,E,N,zone,zc))
        return 0;

    return 1;
}


/* @func GPS_Math_Deg_To_Rad *******************************************
**
** Convert degrees to radians
**
** @param [r] v [double] degrees
**
** @return [double] radians
************************************************************************/

double GPS_Math_Deg_To_Rad(double v)
{
    return v*(double)((double)GPS_PI/(double)180.);
}



/* @func GPS_Math_Rad_To_Deg *******************************************
**
** Convert radians to degrees
**
** @param [r] v [double] radians
**
** @return [double] degrees
************************************************************************/

double GPS_Math_Rad_To_Deg(double v)
{
    return v*(double)((double)180./(double)GPS_PI);
}


/* @func GPS_Math_Molodensky *******************************************
**
** Transform one datum to another
**
** @param [r] Sphi [double] source latitude (deg)
** @param [r] Slam [double] source longitude (deg)
** @param [r] SH   [double] source height  (metres)
** @param [r] Sa   [double] source semi-major axis (metres)
** @param [r] Sif  [double] source inverse flattening
** @param [w] Dphi [double *] dest latitude (deg)
** @param [w] Dlam [double *] dest longitude (deg)
** @param [w] DH   [double *] dest height  (metres)
** @param [r] Da   [double]   dest semi-major axis (metres)
** @param [r] Dif  [double]   dest inverse flattening
** @param [r] dx  [double]   dx
** @param [r] dy  [double]   dy
** @param [r] dz  [double]   dz
**
** @return [void]
************************************************************************/
void GPS_Math_Molodensky(double Sphi, double Slam, double SH, double Sa,
                         double Sif, double *Dphi, double *Dlam,
                         double *DH, double Da, double Dif, double dx,
                         double dy, double dz)
{
    double Sf;
    double Df;
    double esq;
    double bda;
    double da;
    double df;
    double N;
    double M;
    double tmp;
    double tmp2;
    double dphi;
    double dlambda;
    double dheight;
    double phis;
    double phic;
    double lams;
    double lamc;
    
    Sf = (double)1.0 / Sif;
    Df = (double)1.0 / Dif;
    
    esq = (double)2.0*Sf - pow(Sf,(double)2.0);
    bda = (double)1.0 - Sf;
    Sphi = GPS_Math_Deg_To_Rad(Sphi);
    Slam = GPS_Math_Deg_To_Rad(Slam);
    
    da = Da - Sa;
    df = Df - Sf;

    phis = sin(Sphi);
    phic = cos(Sphi);
    lams = sin(Slam);
    lamc = cos(Slam);
    
    N = Sa /  sqrt((double)1.0 - esq*pow(phis,(double)2.0));
    
    tmp = ((double)1.0-esq) /pow(((double)1.0-esq*pow(phis,(double)2.0)),1.5);
    M   = Sa * tmp;

    tmp  = df * ((M/bda)+N*bda) * phis * phic;
    tmp2 = da * N * esq * phis * phic / Sa;
    tmp2 += ((-dx*phis*lamc-dy*phis*lams) + dz*phic);
    dphi = (tmp2 + tmp) / (M + SH);
    
    dlambda = (-dx*lams+dy*lamc) / ((N+SH)*phic);

    dheight = dx*phic*lamc + dy*phic*lams + dz*phis - da*(Sa/N) +
        df*bda*N*phis*phis;
    
    *Dphi = Sphi + dphi;
    *Dlam = Slam + dlambda;
    *DH   = SH   + dheight;
    
    *Dphi = GPS_Math_Rad_To_Deg(*Dphi);
    *Dlam = GPS_Math_Rad_To_Deg(*Dlam);

    return;
}


/* @func GPS_Math_Known_Datum_To_WGS84_M **********************************
**
** Transform datum to WGS84 using Molodensky
**
** @param [r] Sphi [double] source latitude (deg)
** @param [r] Slam [double] source longitude (deg)
** @param [r] SH   [double] source height  (metres)
** @param [w] Dphi [double *] dest latitude (deg)
** @param [w] Dlam [double *] dest longitude (deg)
** @param [w] DH   [double *] dest height  (metres)
** @param [r] n    [int32] datum number from GPS_Datum structure
**
** @return [void]
************************************************************************/
void GPS_Math_Known_Datum_To_WGS84_M(double Sphi, double Slam, double SH,
                                     double *Dphi, double *Dlam, double *DH,
                                     int32 n)
{
    double Sa;
    double Sif;
    double Da;
    double Dif;
    double x;
    double y;
    double z;
    int32    idx;
    
    Da  = (double) 6378137.0;
    Dif = (double) 298.257223563;
    
    idx  = GPS_Datum[n].ellipse;
    Sa   = GPS_Ellipse[idx].a;
    Sif  = GPS_Ellipse[idx].invf;
    x    = GPS_Datum[n].dx;
    y    = GPS_Datum[n].dy;
    z    = GPS_Datum[n].dz;

    GPS_Math_Molodensky(Sphi,Slam,SH,Sa,Sif,Dphi,Dlam,DH,Da,Dif,x,y,z);

    return;
}


/* @func  GPS_Math_LatLon_To_EN **********************************
**
** Convert latitude and longitude to eastings and northings
** Standard Gauss-Kruger Transverse Mercator
**
** @param [w] E [double *] easting (metres)
** @param [w] N [double *] northing (metres)
** @param [r] phi [double] latitude (deg)
** @param [r] lambda [double] longitude (deg)
** @param [r] N0 [double] true northing origin (metres)
** @param [r] E0 [double] true easting  origin (metres)
** @param [r] phi0 [double] true latitude origin (deg)
** @param [r] lambda0 [double] true longitude origin (deg)
** @param [r] F0 [double] scale factor on central meridian
** @param [r] a [double] semi-major axis (metres)
** @param [r] b [double] semi-minor axis (metres)
**
** @return [void]
************************************************************************/
void GPS_Math_LatLon_To_EN(double *E, double *N, double phi,
                           double lambda, double N0, double E0,
                           double phi0, double lambda0,
                           double F0, double a, double b)
{
    double esq;
    double n;
    double etasq;
    double nu;
    double rho;
    double M;
    double I;
    double II;
    double III;
    double IIIA;
    double IV;
    double V;
    double VI;
    
    double tmp;
    double tmp2;
    double fdf;
    double fde;
    
    phi0    = GPS_Math_Deg_To_Rad(phi0);
    lambda0 = GPS_Math_Deg_To_Rad(lambda0);
    phi     = GPS_Math_Deg_To_Rad(phi);
    lambda  = GPS_Math_Deg_To_Rad(lambda);
    
    esq = ((a*a)-(b*b)) / (a*a);
    n   = (a-b) / (a+b);
    
    tmp  = (double)1.0 - (esq * sin(phi) * sin(phi));
    nu   = a * F0 * pow(tmp,(double)-0.5);
    rho  = a * F0 * ((double)1.0 - esq) * pow(tmp,(double)-1.5);
    etasq = (nu / rho) - (double)1.0;

    fdf   = (double)5.0 / (double)4.0;
    tmp   = (double)1.0 + n + (fdf * n * n) + (fdf * n * n * n);
    tmp  *= (phi - phi0);
    tmp2  = (double)3.0*n + (double)3.0*n*n + ((double)21./(double)8.)*n*n*n;
    tmp2 *= (sin(phi-phi0) * cos(phi+phi0));
    tmp  -= tmp2;

    fde   = ((double)15.0 / (double)8.0);
    tmp2  = ((fde*n*n) + (fde*n*n*n)) * sin((double)2.0 * (phi-phi0));
    tmp2 *= cos((double)2.0 * (phi+phi0));
    tmp  += tmp2;
    
    tmp2  = ((double)35.0/(double)24.0) * n * n * n;
    tmp2 *= sin((double)3.0 * (phi-phi0));
    tmp2 *= cos((double)3.0 * (phi+phi0));
    tmp  -= tmp2;

    M     = b * F0 * tmp;
    I     = M + N0;
    II    = (nu / (double)2.0) * sin(phi) * cos(phi);
    III   = (nu / (double)24.0) * sin(phi) * cos(phi) * cos(phi) * cos(phi);
    III  *= ((double)5.0 - (tan(phi) * tan(phi)) + ((double)9.0 * etasq));
    IIIA  = (nu / (double)720.0) * sin(phi) * pow(cos(phi),(double)5.0);
    IIIA *= ((double)61.0 - ((double)58.0*tan(phi)*tan(phi)) +
             pow(tan(phi),(double)4.0));
    IV    = nu * cos(phi);

    tmp   = pow(cos(phi),(double)3.0);
    tmp  *= ((nu/rho) - tan(phi) * tan(phi));
    V     = (nu/(double)6.0) * tmp;

    tmp   = (double)5.0 - ((double)18.0 * tan(phi) * tan(phi));
    tmp  += tan(phi)*tan(phi)*tan(phi)*tan(phi) + ((double)14.0 * etasq);
    tmp  -= ((double)58.0 * tan(phi) * tan(phi) * etasq);
    tmp2  = cos(phi)*cos(phi)*cos(phi)*cos(phi)*cos(phi) * tmp;
    VI    = (nu / (double)120.0) * tmp2;
    
    *N = I + II*(lambda-lambda0)*(lambda-lambda0) +
             III*pow((lambda-lambda0),(double)4.0) +
             IIIA*pow((lambda-lambda0),(double)6.0);

    *E = E0 + IV*(lambda-lambda0) + V*pow((lambda-lambda0),(double)3.0) +
         VI * pow((lambda-lambda0),(double)5.0);

    return;
}


/* @func  GPS_Math_EN_To_LatLon **************************************
**
** Convert Eastings and Northings to latitude and longitude
**
** @param [w] E [double] NG easting (metres)
** @param [w] N [double] NG northing (metres)
** @param [r] phi [double *] Airy latitude     (deg)
** @param [r] lambda [double *] Airy longitude (deg)
** @param [r] N0 [double] true northing origin (metres)
** @param [r] E0 [double] true easting  origin (metres)
** @param [r] phi0 [double] true latitude origin (deg)
** @param [r] lambda0 [double] true longitude origin (deg)
** @param [r] F0 [double] scale factor on central meridian
** @param [r] a [double] semi-major axis (metres)
** @param [r] b [double] semi-minor axis (metres)
**
** @return [void]
************************************************************************/
void GPS_Math_EN_To_LatLon(double E, double N, double *phi,
                           double *lambda, double N0, double E0,
                           double phi0, double lambda0,
                           double F0, double a, double b)
{
    double esq;
    double n;
    double etasq;
    double nu;
    double rho;
    double M;
    double VII;
    double VIII;
    double IX;
    double X;
    double XI;
    double XII;
    double XIIA;
    double phix;
    double nphi=0.0;
    
    double tmp;
    double tmp2;
    double fdf;
    double fde;

    phi0    = GPS_Math_Deg_To_Rad(phi0);
    lambda0 = GPS_Math_Deg_To_Rad(lambda0);

    n     = (a-b) / (a+b);
    fdf   = (double)5.0 / (double)4.0;
    fde   = ((double)15.0 / (double)8.0);

    esq = ((a*a)-(b*b)) / (a*a);


    phix = ((N-N0)/(a*F0)) + phi0;
    
    tmp  = (double)1.0 - (esq * sin(phix) * sin(phix));
    nu   = a * F0 * pow(tmp,(double)-0.5);
    rho  = a * F0 * ((double)1.0 - esq) * pow(tmp,(double)-1.5);
    etasq = (nu / rho) - (double)1.0;

    M = (double)-1e20;

    while(N-N0-M > (double)0.000001)
    {
        nphi = phix;
        
        tmp   = (double)1.0 + n + (fdf * n * n) + (fdf * n * n * n);
        tmp  *= (nphi - phi0);
        tmp2  = (double)3.0*n + (double)3.0*n*n +
                ((double)21./(double)8.)*n*n*n;
        tmp2 *= (sin(nphi-phi0) * cos(nphi+phi0));
        tmp  -= tmp2;


        tmp2  = ((fde*n*n) + (fde*n*n*n)) * sin((double)2.0 * (nphi-phi0));
        tmp2 *= cos((double)2.0 * (nphi+phi0));
        tmp  += tmp2;
    
        tmp2  = ((double)35.0/(double)24.0) * n * n * n;
        tmp2 *= sin((double)3.0 * (nphi-phi0));
        tmp2 *= cos((double)3.0 * (nphi+phi0));
        tmp  -= tmp2;

        M     = b * F0 * tmp;

        if(N-N0-M > (double)0.000001)
            phix = ((N-N0-M)/(a*F0)) + nphi;
    }
    

    VII  = tan(nphi) / ((double)2.0 * rho * nu);

    tmp  = (double)5.0 + (double)3.0 * tan(nphi) * tan(nphi) + etasq;
    tmp -= (double)9.0 * tan(nphi) * tan(nphi) * etasq;
    VIII = (tan(nphi)*tmp) / ((double)24.0 * rho * nu*nu*nu);

    tmp  = (double)61.0 + (double)90.0 * tan(nphi) * tan(nphi);
    tmp += (double)45.0 * pow(tan(nphi),(double)4.0);
    IX   = tan(nphi) / ((double)720.0 * rho * pow(nu,(double)5.0)) * tmp;

    X    = (double)1.0 / (cos(nphi) * nu);

    tmp  = (nu / rho) + (double)2.0 * tan(nphi) * tan(nphi);
    XI   = ((double)1.0 / (cos(nphi) * (double)6.0 * nu*nu*nu)) * tmp;

    tmp  = (double)5.0 + (double)28.0 * tan(nphi)*tan(nphi);
    tmp += (double)24.0 * pow(tan(nphi),(double)4.0);
    XII  = ((double)1.0 / ((double)120.0 * pow(nu,(double)5.0) * cos(nphi)))
           * tmp;

    tmp  = (double)61.0 + (double)662.0 * tan(nphi) * tan(nphi);
    tmp += (double)1320.0 * pow(tan(nphi),(double)4.0);
    tmp += (double)720.0  * pow(tan(nphi),(double)6.0);
    XIIA = ((double)1.0 / (cos(nphi) * (double)5040.0 * pow(nu,(double)7.0)))
           * tmp;

    *phi = nphi - VII*pow((E-E0),(double)2.0) + VIII*pow((E-E0),(double)4.0) -
           IX*pow((E-E0),(double)6.0);
    
    *lambda = lambda0 + X*(E-E0) - XI*pow((E-E0),(double)3.0) +
              XII*pow((E-E0),(double)5.0) - XIIA*pow((E-E0),(double)7.0);

    *phi    = GPS_Math_Rad_To_Deg(*phi);
    *lambda = GPS_Math_Rad_To_Deg(*lambda);

    return;
}




/* @funcstatic GPS_Math_UTM_Param_To_Mc ********************************
**
** Convert UTM zone and zone character to central meridian value.
** Also return false eastings, northings and scale factor
**
** @param [w] zone [int32]  zone number
** @param [w] zc   [char] zone character
** @param [w] Mc   [double *] central meridian
** @param [w] E0   [double *] false easting
** @param [w] N0   [double *] false northing
** @param [w] F0   [double *] scale factor
**
** @return [int32] success
************************************************************************/
static int32 GPS_Math_UTM_Param_To_Mc(int32 zone, char zc, double *Mc,
                                      double *E0, double *N0, double *F0)
{

    if(zone>60 || zone<0 || zc<'C' || zc>'X')
        return 0;

    if(zone > 30)
        *Mc = (double)((zone-31)*6) + (double)3.0;
    else
        *Mc = (double) -(((30-zone)*6)+3);
    
    if(zone==32 && zc=='V')
        *Mc = (double)9.0;

    if(zone==31 && zc=='X')
        *Mc = (double)3.0;
    if(zone==33 && zc=='X')
        *Mc = (double)15.0;
    if(zone==35 && zc=='X')
        *Mc = (double)27.0;
    if(zone==37 && zc=='X')
        *Mc = (double)39.0;
    
    if(zc>'M')
        *N0 = (double)0.0;
    else
        *N0 = (double)10000000;

    *E0 = (double)500000;
    *F0 = (double)0.9996;
    
    return 1;
}


/* @func GPS_Math_UTM_EN_To_NAD83 **************************************
**
** Transform UTM zone, easting and northing to NAD83 lat/lon
**
** @param [r] lat  [double *] NAD latitude (deg)
** @param [r] lon  [double *] NAD longitude (deg)
** @param [w] E    [double] easting (metres)
** @param [w] N    [double] northing (metres)
** @param [w] zone [int32]    zone number
** @param [w] zc   [char]   zone character
**
** @return [int32] success
************************************************************************/
int32 GPS_Math_UTM_EN_To_NAD83(double *lat, double *lon, double E,
                               double N, int32 zone, char zc)
{
    double phi0;
    double lambda0;
    double N0;
    double E0;
    double F0;
    double a;
    double b;

    if(!GPS_Math_UTM_Param_To_Mc(zone,zc,&lambda0,&E0,&N0,&F0))
        return 0;

    phi0 = (double)0.0;

    a = (double) GPS_Ellipse[21].a;
    b = a - (a/GPS_Ellipse[21].invf);

    GPS_Math_EN_To_LatLon(E,N,lat,lon,N0,E0,phi0,lambda0,F0,a,b);

    return 1;
}



/* @func GPS_Math_UTM_EN_To_WGS84 **************************************
**
** Transform UTM zone, easting and northing to WGS84 lat/lon
**
** @param [w] lat  [double *] WGS84 latitude  (deg)
** @param [w] lon  [double *] WGS84 longitude (deg)
** @param [r] E    [double]   easting (metres)
** @param [r] N    [double]   northing (metres)
** @param [w] zone [int32]    zone number
** @param [w] zc   [char]     zone character
**
** @return [int32] success
************************************************************************/
int32 GPS_Math_UTM_EN_To_WGS84(double *lat, double *lon, double E,
                               double N, int32 zone, char zc)
{
    double phi;
    double lambda;
    double H;

    if(!GPS_Math_UTM_EN_To_NAD83(&phi,&lambda,E,N,zone,zc))
        return 0;

    
    GPS_Math_Known_Datum_To_WGS84_M(phi,lambda,0,lat,lon,&H,77);

    return 1;
}

//------------------------------
void GBEN2WGSEN(double *xxx, double *yyy, char *fuso)
{

  double phi, lambda, H;
  double X, Y;
  double phi0, lambda0;
  double E0, N0, F0, a, b;

  phi=0.0;
  lambda=0.0;
  H=0.0;

  X = (double)*xxx;
  Y = (double)*yyy;


// Parametri per Gauss Boaga (Roma40)


  if(!strcmp(fuso,"FUSO32")){

     phi0    = 41.552551;    //  Lat Long di riferimento
     lambda0 = 12.2708400;

     E0 = 1500000.; // Falsa origine
     N0 = 0.;
     F0 = 0.;
  }else
     if(!strcmp(fuso,"FUSO33")){

        phi0    = 41.552551;    //  Lat Long di riferimento
        lambda0 = 12.2708400;

        E0 = 2520000.; // Falsa origine
        N0 = 0.;
        F0 = 0.;
  }else{
     printf("FUSO INESISTENTE !\n");
     return;
  }

  a = 6378388.000;        // Ellissoide
  b = 6356911.946;        //


  GPS_Math_TMerc_EN_To_LatLon( X,  Y, &phi, &lambda, 
                              phi0, lambda0, E0, N0, F0, a, b);


  *xxx = phi;
  *yyy = lambda;

}

//----------------------------------------------------------
// GBENH2WGSLLH: From UTM GaussBoaga To Wgs84  Lat Lon H
//----------------------------------------------------------
void GBENH2WGSLLH(double *xxx, double *yyy, double *zzz, char *fuso)
{

//  int32 zone;
//  char zc;
//  int32 d, m;
//  double s;

  double phi, lambda;

  double phi0, lambda0;
  double E0, N0, F0, a, b;
  double Dphi, Dlam, DH;

  double Est, Nord, H;


  phi=0.0;
  lambda=0.0;
  H=0.0;

  Est  = (double)*xxx;
  Nord = (double)*yyy;
  H    = (double)*zzz;


// Parametri per Gauss Boaga (Roma40)

  if(!strcmp(fuso,"FUSO32")){

     phi0    = 0.0000;    //  Lat Long di riferimento
     lambda0 = 9.0;

     E0 = 1500000.; // Falsa origine
     N0 = 0.;              
     F0 = 0.9996;
  }else
     if(!strcmp(fuso,"FUSO33")){

        phi0    = 0.0000;    //  Lat Long di riferimento
        lambda0 = 15.0;

        E0 = 2520000.; // Falsa origine     
        N0 = 0.;
        F0 = 0.9996;
  }else{
     printf("FUSO INESISTENTE !\n");
     return;
  }

  a = 6378388.000;        // Ellissoide
  b = 6356911.946;        //

  GPS_Math_TMerc_EN_To_LatLon( Est,  Nord, &phi, &lambda, 
                              phi0, lambda0, E0, N0, F0, a, b);


// Trasformo in Roma40

  GPS_Math_Known_Datum_To_WGS84_M(phi, lambda, H, &Dphi, &Dlam, &DH, 95);



  *xxx = Dphi;
  *yyy = Dlam;
  *zzz = DH;


}


//----------------------------------------------------------
// ED50ENH2WGSLLH: From UTM ED50 To Wgs84 Lat Long H
//----------------------------------------------------------
void ED50ENH2WGSLLH(double *xxx, double *yyy, double *zzz, char *fuso)
{

  double phi, lambda;

  double phi0, lambda0;
  double E0, N0, F0, a, b;
  double Dphi, Dlam, DH;

  double Est, Nord, H;


  phi=0.0;
  lambda=0.0;
  H=0.0;

  Est  = (double)*xxx;
  Nord = (double)*yyy;
  H    = (double)*zzz;


// Ed50 Parameters

  if(!strcmp(fuso,"FUSO32")){

     phi0    = 0.0000;    //  Lat Long of reference
     lambda0 = 9.0;

     E0 = 500000.; // False origin
     N0 = 0.;              
     F0 = 0.9996;
  }else
     if(!strcmp(fuso,"FUSO33")){

        phi0    = 0.0000;    //  Lat Long of reference
        lambda0 = 15.0;

        E0 = 500000.; // False origin     
        N0 = 0.;
        F0 = 0.9996;
  }else{
     printf("NON-EXISTENT FUSE !\n");
     return;
  }


  a = 6378388.000;        // Ed50 Ellipsoid 
  b = 6356911.94612795; 
  

  GPS_Math_TMerc_EN_To_LatLon( Est,  Nord, &phi, &lambda, 
                              phi0, lambda0, E0, N0, F0, a, b);


// Wgs84 Transformation 

  GPS_Math_Known_Datum_To_WGS84_M(phi, lambda, H, &Dphi, &Dlam, &DH, 37);


//   *xxx = phi;
//   *yyy = lambda;
//   *zzz = H;
  
  *xxx = Dphi;
  *yyy = Dlam;
  *zzz = DH;  
  

//  printf("Wgs84 Lat Long H <%lf> <%lf> <%lf>\n", Dphi, Dlam, DH);


}


//----------------------------------------------------------
// GBENH2WGSENH: From UTM GaussBoaga To UTM Wgs84 
//----------------------------------------------------------
void GBENH2WGSENH(double *xxx, double *yyy, double *zzz, char *fuso)
{
  double Est, Nord;
  double phi, lambda, H;  
  int32 zone;
  char zc;
  double x, y, z;


  GBENH2WGSLLH( xxx, yyy, zzz, fuso);

  Est=0.0;
  Nord=0.0;
  H=0.0;

  phi    = (double)*xxx;
  lambda = (double)*yyy;
  H      = (double)*zzz;
  

  GPS_Math_WGS84LatLonH_To_XYZ(phi, lambda, H, &x, &y, &z);

  GPS_Math_WGS84_To_UTM_EN(phi, lambda, &Est, &Nord, &zone, &zc);

  *xxx = Est;
  *yyy = Nord;
  *zzz = H;

} 





//----------------------------------------------------------
// Gcc_To_Gdc_Converter
// Wgs84XYZToWgs84LatLonH: From Wgs84 Lat Lon h To Wgs84 XYZ
//----------------------------------------------------------
void Wgs84XYZToWgs84LatLonH(double xxx, double yyy, double zzz,
                     double *LatN, double *LongE, double *H)
{
  

  GPS_Math_XYZ_To_WGS84LatLonH(LatN, LongE, H, xxx, yyy, zzz);


}

//----------------------------------------------------------
// Gdc_To_Gcc_Converter
// Wgs84LatLonHToWgs84XYZ: From Wgs84 Lat Lon h To Wgs84 XYZ
//----------------------------------------------------------
void Wgs84LatLonHToWgs84XYZ(double LatN, double LongE, double H,
                                         double *xxx, double *yyy, double *zzz )
{
  

  GPS_Math_WGS84LatLonH_To_XYZ(LatN, LongE, H, xxx, yyy, zzz);


}

//------------------------------------------------------------
// Gdc_To_Utm_Converter
// Wgs84LatLonHToWgs84ENH: From Wgs84 Lat Lon h To UTM Wgs84 
//------------------------------------------------------------
void Wgs84LatLonHToWgs84ENH(double LatN, double LongE, double H, char *fuso, 
                                         double *xxx, double *yyy, double *zzz )
{
  int32 zone;
  char zc;
  double x, y, z;
  double phi, lambda;
  double Est, Nord;


  if(fuso){
    zc   = fuso[0];
    zone = atoi(fuso+1);
  }else{
    zc   = 'T';
    zone = 32;
  }

  lambda = LongE; 
  phi    = LatN;  


  GPS_Math_WGS84LatLonH_To_XYZ(LatN, LongE, H, &x, &y, &z);

  GPS_Math_WGS84_To_UTM_EN(phi, lambda, &Est, &Nord, &zone, &zc);

  *xxx = Est;
  *yyy = Nord;
  *zzz = H;

 

}

//----------------------------------------------------------
// Utm_To_Gdc_Converter
// Wgs84ENHToWgs84LatLonH: From UTM Wgs84 To Lat Lon h 
//----------------------------------------------------------
void Wgs84ENHToWgs84LatLonH(double xxx, double yyy, double zzz, char *fuso, 
                                        double *LatN, double *LongE, double *H)
{

  int32 zone;
  char zc;
  double Est, Nord;


  if(fuso){
    zc   = fuso[0];
    zone = atoi(fuso+1);
  }else{
    zc   = 'T';
    zone = 32;
  }


  Est  = xxx;
  Nord = yyy;
  *H   = zzz;

  GPS_Math_UTM_EN_To_WGS84(LatN, LongE, Est, Nord, zone, zc);


}

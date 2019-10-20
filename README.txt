History of dune/white_dune
==========================

"white_dune" continue the work of Stephen F. White on the program "dune".

As today (Thu May 31 15:55:58 CEST 2001), the sourcecode of dune 0.13 
(http://dune.sourceforge.net/download.html) has not been updated since 
more than a year, despite some useful patches and bugfixes with possible 
solutions has been publiced. 
In this situation we decided to continue the work as white_dune, giving
Stephen F. White the possibilty to continue his work at sourceforge.

white_dune is:
- a graphical editor to load/create/store X3D/VRML97 files
- a simple NURBS/Superformula/Subdivide based 3D modeller
- a 3D animation tool
- a commandline X3D/VRML97 compiler
- a program with (currently) 3 different GUIs
- a program for beginners/children to create their first 3D animation ("-4kids")
- a tool (and examples) that supports you in learning X3D or VRML97
- a tool to load static/animated X3D/VRML97 files (directly or via "Inline"
  commands) created by programs like maya, 3D Studio MAX, Cinema4D, blender, 
  wings3d, art of illusion etc. and add interaction/scripting 
- a tool to manipulate 3D data with 3D input devices (e.g. joystick/spaceball)
- a tool to view 3D data with quadbuffer stereoview (e.g. shutterglases)
- a potential simple to use frontend for your gameengine/scenegraph library
- a open framework to create or process 3D data and store it as a ISO standard 
- a open source project

For usage information on a Micro$oft Windows system read "README_MSwindows.txt"
For usage information on a MacOSX (Apple UNIX) system read "README_MacOSX.txt"
For usage information on a IRIX (SGI UNIX) system read "README_IRIX_desktop.txt"

For information about how to compile/install read the text file "INSTALL.txt"

======================================================
Take a look at the directory docs with a html-browser, 
if you want to know, how to work with the program
======================================================


From the README of dune version 0.13 by Stephen F. White
========================================================

 * Copyright (C) 2000 Stephen F. White
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

WHAT IS THIS THING?
-------------------

Dune is a simple tool for the creation of VRML (Virtual Reality Modelling
Language).  Dune is a weekend project that went horribly, horribly wrong.
Dune is delicious and nutritious.  Dune is fun for the whole family.

README of white_dune
====================

 * Copyright (C) 2000 Stephen F. White, 2000-2019 J. "MUFTI" Scheurich, others
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

For instuctions how to build white_dune, refer to the INSTALL.txt document.

White_dune is a low level X3D/VRML97 tool for Unix/Linux and M$Windows. It
can read X3D/VRML97 files, display and let the user change the 
scenegraph/fields.
Unlike most highlevel 3D authoring tools, white_dune uses a lightning model 
based on the X3DV/VRML97 standard. 

Despite dune/white_dune is not complete yet (especially X3D 3.3 in the 3D 
window), it let you already use more X3D/VRML97 features then most other 
graphical VRML97/X3D producing tools.

White_dune do not support the VRML1/Inventor fileformats.
White_dune support XML encoded X3D files if it has been
compiled/linked against the expat library.

Since version 0.25beta white_dune has documentation how to work with
VRML amendment 1 or X3D NURBS modelling. It is possible to create complicated 
(including IndexedFaceSet shapes with spikes and edges) shape objects 
with NURBS modelling.

white_dune has features (e.g. "export as pure VRML97"), to use 
VRML amendment 1 NURBS nodes together with a VRML97/X3D browser which do not 
support the VRML amendment 1 standard yet.

Similarly white_dune support the most important nodes of the X3D NURBS
component. 

The mesh modelling features of white_dune can not compare 
to general purpose 3D modellers with VRML97 export like maya, 
3D Studio MAX, Cinema 4D, blender, wings3D, etc. but (unlike white_dune) 
most of this programs support only a small part of the X3D/VRML97 features.
You can use white_dune to advance the output of such mesh modeller (e.g.
add interaction or scripting) or use white_dune as a intergration tool via
the VRML/X3D "Inline" command (to avoid a broken toolchain).

Examples for open source 3D modellers with mesh VRML97 export are 
wings3D or Art of Illusion. 
Blender support the export of XML encoded X3D files.

Since version 0.27beta white_dune has support for 3D-modelling based
on the so called "superformula":

        pow(pow(fabs(cos(m * angle / 4.0) / a), n2) + 
            pow(fabs(cos(m * angle / 4.0) / b), n3), -1.0 / n1)

This formula has been found by a horticultural engeneer in 1997 and 
can be used to form n-symetric shapes similar to shapes in nature like flowers,
leafs or starfishs.
Superformula based shapes are not part of VRML97 by itself and are 
implemented by a PROTO (self defined VRML97 node) with scripting 
("scripted node"). Superformula based shapes can be converted either to a 
Extrusion node or to a IndexedFaceSet node. IndexedFaceSet based scripted 
nodes can be approximated to a NurbsSurface node as a starting point for
further NURBS-modelling.
White_dune also has a PROTO for the superellipsoid, a special case of a 
superformula shape. It can be used to form a sphere, a box, a cylinder, 
a octaeder and (most important) rounded forms of this shapes with only 
2 parameters.

Since 0.29beta, white_dune supports both english, german and italian menus 
and messages in one binary.
Information how to translate white_dune to foreign languages can be found under
docs/developer_docs/dune_developer.html#localisation
Translation of the menus neither require a special tool nor programmer knowhow,
it can be done with a simple text editor by replacing anything between the 
"" signs in src/dune.english.rc

Current foreign languages translation table:

Language  | Menu               | main 4kids dialogs | dialogs            | errormessages
----------+--------------------+--------------------+--------------------+------
English   | done               | done               | done               | done
----------+--------------------+--------------------+--------------------+------
German    | done               | done               | done               | done
----------+--------------------+--------------------+--------------------+------
Italian   | done (need update) | done (need update) | done (need update) | 
----------+--------------------+--------------------+--------------------+------
French    | done               | almost done        |                    | 
----------+--------------------+--------------------+--------------------+------
Portugense| done               |                    |                    | 

Minimal system requirements: 
white_dune-0.30pl8 has been sucessfully compiled and run sucessfully on a 
PC with Intel 80486 DX2/50, 18MB RAM (according to free(1)), 640x480 screen 
resolution and a 8-Bit graphicscard (256 colors) running Linux 1.2.
With this system, you can animate a simple box (8 polygons) a bit faster than 
1 frame per second. It can take a few seconds, till you get reaction to a 
mouse drag in the 3D window. 
Even simple actions can cause white_dune to run into swapspace. 
You need 72MB swapspace and a day to compile white_dune on this system.
Note: Modern mesa3d/OpenGL implementations usually do not work anymore with 
a 8-Bit graphicscard. 

white_dune has support for stereoscopic "toe in" view via "quadbuffer" 
capable stereo visuals ("stereo in a window"), not splitscreen stereo.

Quadbuffer capable stereo visuals are usually used with shutterglases, 
but beamers that allow the use of shutterglases are often very expensive.
Instead of shutterglases, you can also use either a "XPO" box (stereo splitter)
or a graphicscard (e.g. Nvidia Quadro) with "stereo cloneview" features
to drive a onewall based on 2 beamers with polarised light filters and 
polarised light glases.

This has been successfully tested with the following configurations:

Computer      Operation System/X-Server   Graphics Card       Shutter Display

Intel PC      Linux/XIG DX-Platinum       Matrox G400         ELSA 3D REVELATOR
Intel PC      Linux/XIG DX-Platinum       Matrox G450         ELSA 3D REVELATOR
Intel PC      Linux/XIG DX-Platinum       ATI Radeon 7500     ELSA 3D REVELATOR
Intel PC      Linux/XIG DX-Platinum       ATI Radeon 8500     ELSA 3D REVELATOR
Intel PC      Linux(*)/Nvidia/XFree86     PNY Quadro4 580GLX  ELSA 3D REVELATOR
Intel PC      Linux(*)/Nvidia/XFree86     PNY Quadro4 580GLX  Stereographics 
                                                              StereoEnabler + 
                                                              CrystalEyes 3
SGI Octane    IRIX                        Maximum Impact      CrystalEyes 3
SGI Indigo2   IRIX                        Maximum Impact      CrystalEyes 3
SGI Indigo2   IRIX                        High Impact         CrystalEyes 3
SGI Indigo2   IRIX                        Solid Impact        CrystalEyes 3
IBM 43P       AIX                         GXT3000P            CrystalEyes 3
SGI Indigo2   IRIX                        High Impact         "XPO" box
SUN Blade100  Solaris                     Elite3D-Lite        "XPO" box
Intel PC      Linux(*)/Nvidia/XFree86     PNY Quadro4 580GLX  2 Beamer CloneView
Intel PC      Linux/XIG DX-Platinum       ATI Radeon 7500     "XPO" box
Intel PC      Linux(*)/Nvidia/XFree86     PNY Quadro4 580GLX  "XPO" box
Intel PC      Linux(*)/Nvidia/XFree86     PNY QuadroFX 4500   2 Beamer CloneView
Intel PC      Linux(*)/Nvidia/XFree86     PNY QuadroFX 4500   Cube3D Beamer

(*) use a "preemptive" Linux kernel (CONFIG_PREEMPT=y in /usr/src/linux/.config)

If you start dune in the commandline with the -stereo option in the 
commandline, you can also use stereoview under M$Windows (again: only
when quadbuffer is supported)

Computer      Operation System   Graphics Card        Shutter Display

Intel PC      M$ Windows 2000    ASUS 7700 Deluxe(**) ASUS VR-100
Intel PC      M$ Windows 2000    3DLabs Wildcat 7210  CrystalEyes 3
Intel PC      M$ Windows 2000    PNY Quadro4 580GLX   ELSA 3D REVELATOR
Intel PC      M$ Windows 2000    PNY Quadro4 580GLX   "XPO" box
Intel PC      M$ Windows 2000    PNY QuadroFX 3000    CrystalEyes 2
Intel PC      M$ Windows XP      PNY QuadroFX 4500    2 Beamer CloneView
Intel PC      M$ Windows 7       Nvidia Quadro K1000M Nvidia Nvision 2

(**) needs the usage of extra software to support quadbuffer (see below)

Please tell us, if you tested white_dune with stereoscopic view 
successfully on other systems.

To enable "quadbuffer" capable stereo visuals on different systems, you
can use (as root):

 - Linux/XIG DX-Platinum  

   $ Xsetup 

   to enable stereo in the grapics card menu. 

   Then start/restart the Xserver.


 - Linux/Nvidia Quadro/XFree86 (only for Quadro class Nvidia graphic cards)

   install Nvidias kerneldriver and XFree module
   edit the XF86Config file and add (depending on you stereo display) 
   a line like

   Option "Stereo" "1" 
   Option "Stereo" "2" 
   Option "Stereo" "3"
   ... 
   Option "Stereo" "10" 

   to the Section "Device".

   Then start/restart the Xserver.


 - SUN Solaris   
   
   Type at the commandline:

   $ fbconfig -res stereo

   Then start/restart the Xserver.


 - SGI IRIX
   
   Type something like 

   $ /usr/gfx/setmon -n 1024x768_96s

   at the commandline.
   
   On SGI Indigo2 Impact machines you need to start/restart the Xserver.
   On more modern IRIX machines you can change this on the fly without root
   permissions.

 - AIX 

   As Far as we know, nothing especially to do. You can change the 
   resolution and screen frequency with "smit" in 
  
   System Managment -> Devices -> Graphic Devices -> Select the Display Size

 - Intel PC/Windows 2000/Nvidia Geforce

   The driver of some graphiccards with Nvidia Geforce GPU (e.g. ASUS Deluxe, 
   this are the graphiccards with the connector to the ASUS VR-100 
   shutterglasses) can be patched to support Nvidia Quadro features,
   especially a quadbuffer stereovisual.
   After the installation of some software, you have use it to set the 
   Device ID of the Nvidia Geforce graphicscard to the Device ID of a 
   Nvidia Quadro card and reinstall the driver from Nvidia.
   See http://www.stereovision.net/articles/nvidiaopengl/nvstereoogl.htm
   for more information.
   You need to start dune with the commandlineoption "-stereo"

There is also experimental support for anaglyph glasses (red/green and 
red/blue) via the OpenGL accumlation buffer (see -anaglyph option). 

Currently, white_dune is on its way to become a immersive vrml97 editor
with the possibilty to use 3D inputdevices. 
The usage of 3D inputdevices is extremly usefull for animation creation.

This has been successfully tested with the following configurations:

Dimensions     Operation System   Support    Device  

3D             Linux              joystick    Micro$oft Sidewinder Pro
3D             Linux              nxtdials    Mindstorms NXT robot with 3 motors
4D             Linux              joystick    Gameport 2 x 2D analog joysticks
4D             MacOSX             SDLjoystick Logitech Wingman Rumblepad
6D             Linux              joystick    Logitech Coordless Wingman
6D             Linux              joystick    Logitech Wingman Rumblepad
6D             Linux              xinput      Logicad Magellan
6D             Linux              libsball    Labtec Spaceball 2003
6D             Linux              aflock      Flock of Birds Headtracker
6D             IRIX               xinput      Logicad Magellan
6D             IRIX               xinput      Labtec Spaceball 2003
6D             IRIX               libsball    Labtec Spaceball 2003
6D             IRIX               xinput      SGI Dials
6D             AIX                xinput      Labtec Spaceball 4000FLX (serial) 
                                              device beeps continuously 8-(
6D             MacOSX             libsball    Labtec Spaceball 4000FLX (serial)
6D             Solaris            libsball    Labtec Spaceball 4000FLX (serial)
6D             M$Windows XP       win32 code  3Dconnexion SpacePilot (USB)
6D             M$Windows XP       win32 code  3Dconnexion SpaceNavigator (PE) (USB)

The 3Dconnexion SpacePilot only works on M$Windows XP and higher. Cause
the standard white_dune binary is not limited to M$Windows XP and higher,
you need to compile white_dune for 3Dconnexion SpacePilot support by 
yourself and need to add
#define HAVE_WINDOWS_SPACEBALL 1
to config.h before compiling.

For more details how to use 3D inputdevices see the manpage and the
examples in docs/commandline_examples.
Please tell us, if you tested white_dune with a 3D input device successfully 
on other systems and send us the used commandline.

We also tested 2D joysticks under Linux and M$Windows sucessfully.

Read the file INSTALL, if you want to install white_dune on a Unix system

Dune (and therefore white_dune) is free Software under the 
GNU General Public License (GPL)

Start the program with the option --copyrightdetails to print out exact 
copyright information.

Linux 2.4.x, 2.6.x using lesstif or openmotif, Mesa or Nvidia OpenGL and gcc 3
Solaris 2.10 using lesstif and the sun compilers
For some other versions of the Microsoft compilers, you need to change
click to src/dune.vcxproj and select "build".

(For white_dune: Run the batchfile batch/nt.bat before, to generate a 
WIN32 matching config.h file. If you have the jpeg/png/expat etc. libraries 
installed, run batch/batch/jpg_png_zlib.bat).


Contributors (e.g. from copyright statements):

Stephen F. White          (wrote dune-0.13, a full featured portable
                           graphical VRML editor, the base of this program)
J. "MUFTI" Scheurich      (fixed bugs and added some features
                           like VR support (3D-inputdevices/stereoview)),
                           experimental X3DV export/import etc.
                           most changes in this millenium,
                           added english/german documentation)
Ian Lance Taylor          (rclex.l, rcparse.y)
Patrick Powell            (codebase of mysnprintf.c/h)
Brandon Long              (codebase of mysnprintf.c/h)
Thomas Roessler           (codebase of mysnprintf.c/h)
Michael Elkins            (codebase of mysnprintf.c/h)
Chris Morley/OpenVRML     (pngLoad.c/h and codebase of hull creation of 
                           NodeExtrusion::createMesh and supporting functions)
Kirk L. Johnson/jim frost (gif.c/h)
Thiemo Seufer             (initial debian packaging, modifications to port version 0.17 back to M$Windows)
Christian Hanisch         (ColorCircle.cpp)
Kevin Meinert/VRJuggler   (codebase of Aflock.cpp/h)
J. Dean Brederson/I3Stick (codebase of Aflock.cpp/h)
Herbert Stocker           (modifications to port version 0.19 back to M$Windows,
                           hint about "save before crash" on M$Windows)
E. M. "Bart" Jaeger       (black and white icons, keyboard shortcuts,
                           website design)
Aaron Cram/SAND Dune      (DevIL support, Open in almost the same window)
Maksim Diachkov           (slackware Linux package generation, added russian
                           translation of documentation)
Johannes Schoepfer        (patch of slackware Linux package generation)
John Stewart/FreeWRL      (codebase of 
                           NodeIndexedFaceSet::generateTextureCoordinates, 
                           Background sphere, ImageTexture3D loading,
                           MovieTexture loading)
Sam Lantinga/SDL          (codebase of SDLJoystick)
Max Horn/SDL              (darwin/SDL_sysjoystick.c)
Thomas Rothermel          (primitive to nurbssurface conversion, 
                           nurbsline revolve)
Philippe Lavoie/libnurbs++(part of NurbsCurveDegreeElevate.cpp)
Markus Schneider          (mayor part of MacOSX droplet)
Martin Briegel            (MacOSX droplet)
Jens Wilhelm              (bugfixing and M$Windows related improvements)
Wu Qingwei                (handles of Cylinder, Cone, Sphere and Extrusion, 
                           port of Background sphere/Extrusion rendering)
BobMaX(Roberto Angeletti) (configurable point size patch, italian translation,
                           geospatial component)
Satoshi Konno/cybergarage (codebase of cap creation of 
                           NodeExtrusion::createMesh)
Haining Zhi               (translation of german tutorial to english language, 
                           experimental M$Windows inputdevice drivers,
                           inputdevice settings) 
Free Software Foundation  (gcc (lex string parsing rule))
orbisnap                  (patches to NodeExtrusion::createMesh)
Dr. Guido Kramann         (free human animation motion capture VRML files)
Alan Bleasby/JEEPS        (functions for geospatial component)
Philippe Coval            (debian packaging and related bugfixes)
Pawel W. Olszta           (openglutfont.c/h)
The OpenGLUT contributors (openglutfont.c/h)
all freeglut contributors (openglutfont.c/h)
Spencer Kimball/gimp      (images in lib/textures)
Peter Mattis/gimp         (images in lib/textures)
Luigi Auriemma            (reported two security problems)
Gilles Debunne/QGLViewer  (Quaternion::Quaternion(Vec3f from, Vec3f to))
Leonardo Zide/LeoCAD      (Util::convertLDrawColor2LeoCADColor,
                           icon for LdrawDatExport node)
Janosch Graef             (libnxtusb part of nxtDials::nxtDials)
The Open Group            (swSetFontPath() from xset)
Michalis Kamburelis       (added documentation about rendering bugs)
Giacomo Poderi            (some translation of italian menus)
Brian Paul                (Mesa off screen rendering functions)
Doug Sanden               (XML parser/Windows scripts for X3D XML -> X3DV,
                           autoscale of Transform handles)
Francois Gouget           (winres.h)
Stefan Wolf               (libC++RWD Library)
Jonas Tingaud             (translation of french menus)
Open Wonderland           (files written by WonderlandModuleExport.cpp, Open Wonderland logo)
Guillaume Cottenceau      (base of write_png_file() in WonderlandModuleExport.cpp)
Jasmin Heiermann          (debugging of NurbsSurface.controlPoint vanish bug)
Simon Schampijer          (HelloWorld activity: base of olpc desktop starter) 
Bill Spitzak and others   (base of fluid/fltk OpenGL export example)
Arnaldo Bem Fateixa       (translation of portugenese 4kids menu)
Leemon Baird              (base of (Pseudo-)Fractal PROTO)
Brian Paul
Ron Cemer 
Sven Gothel               (base of jogl java export example)
Slavomir Kaslev           (https://github.com/skaslev/catmull-clark/)
ISTI Visual Computing Lab (meshlab polygom reducer)
Harishankar Narayanan     (https://github.com/codetiger/Font23D)
FTGL                      (https://sourceforge.net/projects/ftgl/)
Poly2Tri                  (http://code.google.com/p/poly2tri/)
Chris Rorden              (https://github.com/neurolabusc/vx)
Brian Wellington          (bill.jpg for Tux warbird from xbill)
Matias Duarte             (bill.jpg for Tux warbird from xbill)
Pixar                     (code from OpenSubdiv tutorial)
Dipl. Ing. Andreas Quiring(raspian hosting) 

==========================================================================

Changed in white_dune Version 0.16:

 Initial png support from OpenVRML (OpenVRML is LGPL), don't trust your eyes 8-)

 SGI type ALT-mouse navigation, beware of your windowmanager 8-)

 You see better, what you type when changing fields

 dune do not eat up PROTO Definitions any longer

 dune do not eat up EXTERNPROTO Definitions any longer, 
     but default values of Fieldvalues are not read

 dune tries now to save it's contence to $HOME/.dune_crash_*_*.wrl, when
      older versions of dune would crash in some situations.

 two simple how to use exercises included, see docs/index.html

===========================================

Changed in white_dune Version 0.17:

 Normal Node in IndexedFacesets is displayed more correctly - still no 
  support for NormalsPerVertex 8-(

 Problematic line in resource.c fixed.
 
 Errors in configure.in (not present in version 0.15) hopefully fixed 8-(

 Write to files now checks for errors 

 Signalhandling should be now a bit more robust 

 some coredump on programexit fixed

 Minor changes in look of program

 Very invisible "better than nothing" Shape->Text Rending for Unix included 8-(

 Port back to Windows NT:
                          to compile, you may want to copy config.h.nt
                          to config.h, execute usebison.bat and useflex.bat
                          and use dune.dsw
===========================================

Changed in white_dune Version 0.18:

 Bug in saveing files corrected

 Real silly bug responsible for forgetting more than one PROTO Definiton
 corrected

 Writing of routes/fields in Scriptnode corrected

 Output indentation is controlled by configure option (see INSTALL) 

 "Option -> Preferences / Show Handles For Selected Item" show now 
 the handle of the selected Item, not all handles of the selected tree

 addtional changes to configure to include some options (see INSTALL)

 initial support to read and write X3D files via the nist.gov 
 X3D translators. 
 Use at your own risk !
 To avoid dataloss, the temporary .wrl files are kept in the directories 
 x3dv2/test and /tmp

 Warning: Currently the translators fail to produce valid VRML97 code for some
 rare used nodes like VisibilitySensor or Collision.
 Currently dune need valid code to work.
 see README.x3dtranslators

 writing of MFInt32 data are written in one line till -1 (if any)

 avoid too near numbers in channelview at width 1280 

 optional use saxon (faster) instead of the x3dv2 nist.gov X3D translator

 coredump reason in motif/tree.c deleted
  
 usage of stdarg corrected

 "quadbuffer stereo" support (no "split screen stereo" !): 
 "Stereobrutality" till there is a 3D Pointer 8-(
 Cause it is difficult to click to objects in stereomode, stereomode
 can be temporary switched off by pressing Mousebutton 2 or "s" on the
 keyboard 

 commandline arguments to start dune with matching distance eye - eye and 
 distance eye - screen or nonstereo mode

 manpage included

 crashfile included to recentfiles

 scenegraph and routeview display DEF Names

 width of nodes in routeview increased - "OrientationInterpolator" is a
 very long word 8-(
===========================================

Changed in white_dune Version 0.18pl2:

 configure changed to test for signal_handler(int), to increase portability
 to random UNIX 
===========================================

Changed in white_dune Version 0.18pl3:

 ported back to SUN Solaris 8

 --with-buginlesstif workaround if click to icon do nothing
===========================================

Changed in white_dune Version 0.18pl4:

 "./configure --with-buginlesstif" workaround work now against problems 
 with creating routes on some lesstif/openmotif versions. 

 --with-buginlesstif is now default in the source-rpm 
===============================================

Changed in white_dune Version 0.19:
                                 
 inputdevice support

 included Linux joystick inputdevice support    

 personal desktop icons for IRIX 6.5, global desktop icons for IRIX 6.2
 include a icon for a new simple fm/worldsToVrml based cosmoworlds to 
 VRML97 converter script

 included libspaceball inputdevice support

 "parse error" output include linenumbers 

 interrupt signals will be ignored

 included Xi/xinput inputdevice support

 icon and handles for changing Transform.center
 
 xerrorhandler also catches Xtoolkit (Xt) errors
 
 colorcircle included

 6D inputdevice input on Transform node makes sense for first level of 
 scenegraph

 navigationicon works with mouse input

 navigation works together with Transform node on first level of scenegraph

 worlds with IndexedFaceSet with empty Coordinate do not crash anymore

 support for combined recording and running in ChannelView

 you can use delete in the ChannelView

 initial support for Ascention Flock of birds 3D device

 6D input can be restricted to 2D or 1D input (only maximal values used)

 bug repaired if fraction > max(key) in OrientationInterpolator

 documentation about combined recording and running in ChannelView updated

 crash avoided for repeated pastes 

===============================================

Changed in  white_dune Version 0.19pl4:

 nonstandard TextureImage.mode is now a configuration option

 bugfix about initialising Xinputdevices

 bugfix about netscape -remote command

===============================================

Changed in  white_dune Version 0.20beta:
 
 dune can now read gzip compressed VRML files

 png textureimages with transpanency work for primitives and 
 indexedfacesets with texturecoordinates

 crash when deleting interpolators fixed

 better support for transforms of 2D and 3D joysticks 

 HOVER/ROCKET mode for transforms

 bugfix for crash when loading scriptnodes by Torsten Blank

 better support for transforms of 4D joysticks 

 bugfix when writing fields of scriptnodes

 bugfix for crash when displaying MFStrings fields initialised with a 
 single String

 manpage updated about new option to switch off input device axes

 better support for navigation with 2D, 3D and 6D joysticks

 very faster performance on 2D operations on Truecolor/Directcolor 
 displays

 added inputdevice support for scale and change center operations

 added handles of different shapes for different transform modes

 added configuration option to search for fpu errors, and some related
 bugfixes

 fixed bug: 6D inputdevice input on Transform node made only sense for 
            limited levels of the scenegraph

 dune can be configured to use Barts blacknwhite icons 

 documentation updated

 added support for making rpm package files on Mandrake Linux
 
 added bugfix about scale operations with 3D inputdevice

 problem with tooltips fixed on some (buggy ?) lesstif implementations

 small changes to fix syntax problems on SGI MIPSpro 7.30 compiler

 better recovery after trying to read invalid VRML97 files

 menus updated 
 
 added HTML help menu

 added nullsize option for inputdevices 

 added Aaron Crams/SAND dune support for DevIL library.
 When DevIL is installed, white_dune can load more texture imageformats 
 like tif, rgb, bmp ... (not required by the VRML97 standard)

 added bugfix for Aaron Crams/SAND dune open in almost same window patch

 added bugfix for loading only last of multiple input files in commandline

 added bugfix for infinite loop when writing scriptnodes with 
 directOutput or mustEvaluate

 added bugfix for problem with redraws in route window (SceneGraphView)
 (motif canvas size must be < SHRT_MAX) 

 added bugfix for libsball usage

 bart added some keyboard shortcuts

 added bugfix for gnuis "test -e" in shellscripts

 added bugfix for "wheel" type inputdevices loosing calibration when 
 changing Transformmodes

 selected nodename in route window (SceneGraphView) change color to blue 

 added file -> import (incomplete for loading textures/urls from other
                       directorys). 

 angle of SFRotation shown in purple in channelview

 bugfix for recursive defined protos

 bugfix for recursive DEF/USE structures

 bugfix for translations of inputdevices in deep scenegraph structures

 writing URLs as relative URLs

 added to developer documentation

 added fileselectors for AudioClip, Inline and MovieTexture.

 file -> import completed

 bugfix for headtracking with Flock of Birds device

 added -headnavigation option 

 manpage updated

 minor bugfix: import fileselection window has "import" title

 bugfix for wrong headnavigation default for non tracking devices

 fix problem with missing nodename update in deep scenegraph structures 
 when making routes

 new configuration options about wwwbroswer/vrmlbrowser, 
 new default wwwbroswer/vrmlbrowser for MacOSX

 bugfix for crash (with loosing all information) on mouse drag over a 
 recursive scenegraph structure.

 added ".wrl" filename completion, if no file extension has been given.

 added fullscreen icon

 added create menu

 added create -> proto usage menupoint

 wrong maximal canvassize for M$Windows fixed

 added EXTERNPROTOs to the create -> proto usage menupoint

 bugfix for wrong graying of TextureTransform button

 clean up web/vrml browser configuration 

 bugfix for possible destroying javascript nodes 

 bugfix for possible destroying of URLs in script nodes 

 replace nonstandard "vrmlscript:" with "javascript:" in URL of script nodes.

 added --with-dontreplacevrmlscript

 workaround for using a PROTO as Appearance node. 

 added automated FreeBSD pkg generation. 

 bugfix for crash after undo operations

 added NurbsSurface PROTO for Cosmoplayer (javascript implementation)

 bugfix for workaround for using a PROTO as SFNode in Shape/Sound node. 

 added undo for ColorCircle.

 added additional start of X11 for MacOSX.

 added automated MacOSX "stuffit expander" compatible package generation.

 added bugfix for compiling under gcc 3.2.1

 added bugfix for using HAVE_WANT_CORE under M$Windows
  
 added bugfix for deleting USEd node

 added bugfix for wrongly used XtDestroyApplicationContext

===============================================

Changed in  white_dune Version 0.22 beta:

 added program name to /dev/console output
 
 Script Node write "url" field at last

 bugfix about graying of colorcirle menupoint

 yet another (glut based) "better than nothing" Unix Textnode implementation 8-(

 bugfix to allow empty PROTOs 

 textedit (Unix/Linux only)

 Script Node (javascript) creation/editing (Unix/Linux only)

 portability fixes for Sun WorkShop 6 update 1 (native Solaris compiler) 

 ecmascript editing moved in own class, now also available from MainWindow

 File->Preview also available from ScriptDialog

 bugfix for missing question about "Save changes to ... ?" on 
 "file -> close/exit" after using "file -> Textedit" 

 bugfix for a possible crash when setting SFNode/MFNode fields in the 
 scenegraph branch of a scriptnode

 documentation (about move, copy and DEF/USE in the scenegraph) updated

 switch on/off running TimeSensor->Interpolator animations in play mode 
 by clicking to the loop field of the matching TimeSensor

 better check if scripteditor is already running 

 bugfix about 0 0 0 ? is a invalid SFRotation after normalize()

 simplified "connect anything" scriptinterface building (useable but incomplete)

 bugfix for missing protoname after "file -> insert" 
 (when there was no proto defined at program start)

 simplified "connect anything" scriptinterface building completed

 added use documentation for simplified "connect anything" scriptinterface 
 building 

 added --without-textedit configureoption to disable file -> textedit 
 cause it would not return on FreeBSD 5.0-Release

 added additional information to Scriptinterface building, only SFBool,
 SFColor (incomplete)

 bugfix for false double writing of ROUTEs in some situations

 bugfix for crash when deleting eventIn/eventOut from Script nodes

 bugfix for confusingly mixing 2 dialogs on "File -> Open"

 added errormessage, if a directory is choosen on "File -> Open"

 open always the first level of scenegraph on UPDATE_ALL 
 (this is helpfull for beginners)

 workaround for crash about invalid setting of current selection
 on undo/redo operations 

 workaround for problem when DEF names become unreadable on move operations

 added slackware packager by Maksim Diachkov

 problem when DEF names become unreadable on move operations solved

 missing "open in almost the same window" for "File -> recent files" fixed

 added additional information to Scriptinterface building for more SFTypes

 inserted "(need something)" into "Create" menues and 

 layont error in documentation about "need something" fixed

 added accounting of TextureCoordinates from FreeWRL (File Polyrep.c)

 bugfix for crash in "open in almost the same window"

 DuneApp class splitted into smaller parts

 Dialog for EmcaScriptSettings added

 Additional information to Scriptinterface building completed

 Added bugfix for problem with resource.h/IDNO

 Better checking if javascript functions are already defined in a script URL

 Added URL edit icon to edit script URLs

 Added bugfix for problems in displaying events in scriptdialog

 Added M$Windows (2D ?) joystick support

 Factor options for single inputdevice axes are now independend of
 -allrot and -allxyz options (better support of joysticks with few axes).

 M$Windows preview made more similar to UNIX way

 Bugfix about handling all 16 bits from Flock of Birds driver

 Bugfix for preview of a file from a readonly directory

 Better handling of file -> open command

 Better handling of failed file -> import

 Better errormessage if gzip compression is unsupported

 Bugfix for crash if ElevationGrid has a empty Normals node 

 Bugfix for crash on missing selection update after undo/redo operations

 Better cleanup of temporary files on program exit

 Added some ECMAscript examples to the typical vrml documentation

 Now using POSIX compatible code in connection with pipe(2) and fcntl(2)
 instead of nonportable iostream/ioctl(2) based code (failed on IRIX 6.2).

 Added bugfix for falsly try to read a byte into a constant in URLedit 

 Added bugfix for forgetting to erase temporary files under M$Windows

 Deleting fink (/sw) library/include path from MacOSX build

===============================================

Changed in  white_dune Version 0.24:

 All magic numbers from getField/setField calls has been deleted

 Added selftest program for setField calls

 Fixed missing default "WALK", "ANY" in NavigationInfo

 Bugfix for false showing fields of "scene" root node

 Bugfix for false graying of toolbar icons

 Bugfix for false use of the scenegraph-orgin-icon

===============================================

Changed in  white_dune Version 0.25beta:

 Bugfix for using Node::setName() instead of Scene::def() in MoveCommand

 Bugfix about Scene::hasAlreadyName only inspected first level of scenegraph

 Added Edit -> DEF

 Added joystick support from SDL

 Bugfix for false handling of "-all" option

 Bugfix for false axes counting when -none option was used

 Added MacOSX SDLjoystick example for Logitech Wingman Rumblepad 
 (expext problems with calibration/initialisation with this joystick)

 Added jump to $HOME/Desktop on MacOSX, before opening GUI fileselectors. 

 Added calibration bugfix for MacOSX joysticks

 Added workaround by ignoring a harmless Xt error, which is handled as 
 fatal on some systems

 Added compilation bugfix for FreeBSD

 Added generation of normals on IndexedFaceSets from creaseangle 
 (for VRML browsers without creaseangle implementation like lookat or cover)

 Added bugfix about possible write behind a internal array

 Added rendering of Viewpoint.fieldOfView

 Added rendering of LOD

 Decreased Aflock starting time by reducing security sleeps

 Added -fieldofview option to overwrite fieldofview fields of VRML viewpoints.
 This can be useful for stereoviewing.

 Added bugfix for Linux joystick test programm
 
 Documentation updated.

 Add better handling of errors in resourcefile by adding exit to yyerror 
 of rcparse.y

 Added Cone/Cylinder/Sphere to Nurbs conversion (makes more sense to 
 nurbsmodelling features) 

 Added dragging of handles to navigation mode.

 Added bugfix for "If more levels than ranges are specified, the extra 
 levels are ignored (ISO/IEC 14772)" problem of LOD.

 Added temporary switch off of stereomode with button 3 of mouse.

 Added portability bugfix.

 Added bugfix for false lengths in NurbsSurface::CreateMesh.

 Added NurbsSurface to IndexedFaceSet conversion.

 Added NurbsCurve VRML200x node.

 Added input of first number of a MFField in the fieldview (a beginning...).

 Added graying of menus to Micro$oft version.

 Added bugfix for crash in NodeLOD::preDraw()

 Added Box to Nurbsgroup conversion

 Added insert of MFString fields in the fieldview Window

 Added delete of MFString fields in the fieldview Window

 Added possibilty of insert of first SFString field of a MFString in the
 fieldview Window

 Added accounting of world coordinates of a object to Ecmascript examples

 Add bugfix to particle ECMAscript example

 Added workaround for crash in fieldview window.

 Added Box to one NurbsSurface conversion (Primitive to NurbsSurface 
 conversion completed)

 Added NurbsCurve rotation (resulting in a NurbsSurface)

 Added german menu (configurable)

 Added menu for a simple NURBS modeller for kids (configurable)

 Added bugfix for transform of handles with 3D input devices 

 Added bugfix for NURBS conversion routines.

 Added x symetric modelling (see actions menu)

 Added workaround for crash in NurbsCurve rotation.

 Added bugfix for crash in NurbsCurve rotation.

 Added rotation of controlpoints after primitive to nurbssurface conversion
 (better support for x symetric modelling)

 Added bugfix for crash about false handling of handles

 Added bugfix for crash when using defaultcolour in NurbsCurve rotation.

 Added better support for x symetric modelling when creating a nurbssurface
 by rotation of a nurbsCurve via Y axis: flattening of nurbsCurve 
 before nurbsCurve to nurbssurface conversion and rotating the resulting
 in a x symetric direction.

 Added dialog for creating NurbsCurve in direction of x, y or z axis

 Added flattening of nurbsCurve before nurbsCurve to nurbssurface conversion 
 via X and Z axis.

 Added bugfix for failed initialisation of emissivcolor in NurbsCurve dialog

 Begin of NURBS modelling documentation.

 Added bugfix for missing update of NURBS shapes after change of MF-fields
 in fieldview.

 Added bugfix for using false color when drawing NodeNurbsCurve and 
 NodeIndexedLineSet

 Added bugfix for missing update of Elevationgrid after change of MF-fields
 in fieldview.

 Added building/packaging of dune4kids to some UNIX/Linux versions

 Bugfix for missing building of mesh after copy of shapes.

 Bugfix about permitted operations with NURBS nodes in the scenegraph.

 Bugfix about typing error resulting in nonsymetric x symetric modelling

 Bugfix about permitted operations with interpolator nodes in the scenegraph.

 Portability fix about compiling with gcc 2.7.2

 Added change of cursor when over dragable numbers in fieldview

 Added 3D Cursor in stereomode (e.g. with shutterglases) to make picking 
 of handles simpler.

 When in stereomode, the size of objecthandle has been doubled.

 Added english nurbsmodelling documentation

 Added writing of NurbsCurve and NurbsGroup EXTERNPROTOs

 Code cleanup for writing of VRML200x EXTERNPROTOs

 Fixed problems with some autoconf versions

 Fixed NurbsCurve PROTO for Cosmoplayer (javascript implementation)

 Added workaround for micro$oft only crash with limited size of bitmap 
 drawing context (used in routeview)

 Bugfix for ignoring weights in X symetric modelling
  
 Bugfix for error in switching off X symetric modelling

 Added X symetric modelling to NurbsSurfaces that are directly contained
 and as shape nodes in a NurbsGroup

 Changed Box2Nurbs Dialog to create a NurbsGroup instead one NurbsSurface.

 Documentation updated.

 Bugfix for missing undo/redo of X symetric modelling related to NurbsGroups.

 Fixed needed multiple undo/redo operations on X symetric modelling related to 
 NurbsGroups.

 Reduced needed multiple undo/redo operations on conversion to Nurbs
 operations.

 Fixed wrong IDC_BOX_NU/VAREAPOINTS assigment in box2nurbs dialog. 

 Fixed wrong "exposedField radius" handling of Sphere node (radius is 
 a field).

 Added convenience menupoint "Action->animate".

 Fixed support for selection TimeSensors in menupoint "Action->animate".

 Fixed missing switch off of 3Dcursor when mouse leave 3D window.

 Added workaround for missing MFVec3f animation support

 Fixed missing menupoint "Action->animate" for dune4kids.
 
 Added preselection of rotation and translation of Transform node in 
 animation dialog of dune4kids.

 Added dune4kids to dune manpage.

 Added menupoint for increase/decrease from input devices

 Added Toolbar buttons for animation, X symetric modelling and 
 increase/decrease from input devices.

 Changed Toolbar buttons layout and fixed some related "magic number" problems

 german resourcefile updated

 English documentation about increase/decrease from input devices was
 updated

 Added bugfix about different eventhandling of M$Windows in animation dialog.

 Cleanup of quaternion related code in InputDevice.* and Aflock.*

 Bugfix for missing return statement in Scene::writeExternProto that
 can result in the impossibility to save

 Bugfix for crash in drawing faces if mesh.coordIndex do not end with -1

 Fixed typing errors in M$Windows/DevIL compilepath.

 Maksim Diachkov added a russian translation of the documentation and a new 
 slackware 9.1 build script.

 Cleanup of rotation related code in InputDevice.*

 Added handlesize to Preferencesdialog
 
 Added Options -> Stereo View Settings for Unix/Linux systems

 Added "File -> Export as -> Export as pure VRML97" menupoint

 Added "pure VRML97" preview setting for VRML browsers without NURBS support

 Bugfix for missing effect of 3D cursor mode settings.

 Bugfix for possible wrong handling of rendering of last face of a mesh.

 Workaround for wrong accouting of normals in NurbsSurface.

 Added documentation about building a IndexedFaceSet with NURBS modelling.

 Added bugfix for incorrectly writing EXTERNPROTO when exporting pure VRML97.

 Added -ignoresize to aflock options.
 
 Added bugfix for incorrectly loading parts of a VRML world twice, when
 the load after file->textedit failed cause of syntax errors.

 Added bugfix for wrong accouting of normals in NurbsSurface.

 Added bugfix for wrong preference name for VRML URL under M$Windows.

 Added bugfix for converting cone to nurbssurface when uv/degree == 1.

 Added bugfix for displaying images upside down when using DevIL.

 Added bugfix for unnessesary generating normalIndex values and creaseAngle 
 in NurbsSurface to IndexedFaceSet conversion.

 Added MacOSX droplet.

 Added workaround for crash when deleting a node.

 Added bugfix for converting sphere and cylinder to nurbssurface when 
 uv/degree == 1.

 Documentation updated.

 Fixed wrong u/v degree assignment in sphere to nurbs dialog

 Fixed wrong placement of "pure VRML97" checkbox in preview options of 
 M$Windows version

 Fixed serious error in 0.25beta122 - 0.25beta131 (one PROTO definition 
 of a VRML file can get lost one each write)

 Added workaround for failing to write MFtypes, when they have the
 same size as the default value

 Bugfix for not jumping to errorness linenumber after failed file->textedit

 Improved workaround for failing to write MFtypes, when they have the
 same size as the default value

 M$Windows rebuild can now work independend from cygwin tools.

 INSTALL document updated.

 Added workaround for error in NurbsCurveDegreeElevate

 Added bugfix for crash when stopping _autoscrolltimer of channelview 
 during program close.

 Completed german documentation.

 Added errormessage if X11 is not found on MacOSX.

 Added white_dune4kids droplet creation for MacOSX

 Added workaround for gcc 3 typename warning 

 Added bugfix for problems when switching on/off stereo view via
 menupoint Options -> StereoView settings.

 Dropped "stereobrutality" code, cause a 3D cursor is now available. 

 Added workaround for crashes related to Node._name under Sun Solarias
 compiled with the native compiler.

 Forgotten change about M$Windows rebuild work independend from cygwin tools
 applied.

 Logo rebuild completely with NURBS modelling (and textures)

 Bugfix for missing setting of $DISPLAY on MacOSX

 Added gentoo Linux build script

 Added bugfix for displaying too less USEd nodes (this was made to avoid problems
 with recursive DEF/USE structures).

 Added developer documentation.

===============================================

Changed in  white_dune Version 0.26pl2:

 Fixed missing icons in black and white icons (symetric X modelling and animate)

 Fixed wrong filename in batch/fixpermissions.sh

===============================================

Changed in  white_dune Version 0.26pl3:

 Bugfix for wrong quoting in gentoo packaging script 
 resulting in wrong URL in NURBS EXTERNPROTOs

===============================================

Changed in  white_dune Version 0.26pl4:

 Fixed clock skew in gentoo archive.

 Added rule to src/Makefile.in to deal better with clock skews.

===============================================

Changed in  white_dune Version 0.27beta:

 Chances recommended by Jens Wilhelm:

    Added move view navigation on CTRL+SHIFT-Mouse Button 1 drag
 
    Bugfix/improvement of Script Dialog for M$Windows

    Added option for M$Windows DDE XTYP_EXECUTE instead of XTYP_REQUEST 

 Added better response of cursor shape on change of keyboard modifiers

 Added yet another bugfix of Script Dialog for M$Windows

 Discarded "#ifndef WIN32" on update of DEF information on M$Windows

 Added german tutorial

 Added bugfix for missing ?Degree > 2 support in Cone/Cylinder/Sphere2Nurbs 
 dialogs

 Added better invalid route checking of input file

 Added illegal2vrml program and man page

 Added bugfix for missing icon in scenetree after move command

 Added set_new_handler

 EcmaScript preferences moved into own file

 Added bugfix for displaying false index in FieldView after insert in a 
 MFString field

 Splitted FieldViewItem.cpp and FieldViewItem.h into 21 pieces

 Added file->upload and options->upload settings... menupoints 
 
 Added "make tar.gz" to main makefile

 Added insert and remove of MFFloat values in Fieldview

 Added insert and remove of MFInt32 values in Fieldview

 Renamed NURBS operations to "VRML97 Amendment 1" in menus

 Changed forgotten magic numbers in NodeCone.cpp and NodeCylinder.cpp

 Added workaround to particle source PROTO for bug in cc3dglut 4.3  

 Added better test if resource.h is valid

 Added Wu Qingwei's handles for cone, cylinder and sphere 
 
 Size of arrow handles increased, to avoid conflict with cone, cylinder 
 and sphere. 

 Added IndexedFaceSet to IndexedLineSet conversion (incomplete)

 Added bugfix for rendererror when changing NurbsCurve.tesselation

 Added NurbsCurve to IndexedLineSet conversion 

 Added Rendering of PointSet (incomplete)

 Added IndexedLineSet to PointSet conversion
 
 Added change of camera on clicking to a viewpoint node

 Added copy of values of current viewpoint when creating new viewpoint

 Added teleporter and switching examples to docs/typical_vrml_examples

 Added better rendering of lines.

 dune4kids and illegal2vrml programs moved into one binary

 Added "VRML1 not supported" error message

 Fixed bug in file->upload when password is empty

 Added "action -> move rest of scenegraph branch" menupoints

 Fixed bug when loading URL starting with "file:localpath"

 Added bugfix for crash when deleting a USED node
 (fixed missing update of geometricParentIndex)

 Added "action -> Set IndexedFaceSet center to" menupoints

 Added bugfix for false handling of filenames with doublequotes under
 M$Windows

 Added collectViewpoints script

 Added PseudoCollision4Examine PROTO

 Added bugfix for false default of near clipping plane resulting
 in false preview of Viewpoint. Remove NearClippingPlaneDistance
 registrykey (M$Windows) or from $HOME/.dunerc (Linux/UNIX/MacOSX).
 
 Updated HTTP address of the VRML97 ISO documents

 Added "action -> Set center to" for more geometry nodes

 Changed geometric parent storage mechanism

 Selected less ambiguous route colors

 Added some advise about common animations when nothing is selected in
 animation dialog

 Added workaround for bug of deleting both node and link to node when 
 deleting link to node in same scenegraph branch

 Added "action -> Set center to" for NurbsCurve

 Added "action -> flip x/y/z" for IndexedFace/LineSet, PointSet and 
 NurbsSurface

 Added NurbsCurve to PositionInterpolator conversion.

 Added support for 8 Bit displays on M$Windows.
 
 Added usage of "None" mousecursor when using the 3D cursor

 Added bugfix for crash related to "None" mousecursor

 "dontcarefocus" configuration option moved into commandline parameters

 Bugfix for crash in "Elevate U/V degree up"

 Added bugfix for crash in "remove illegal children nodes"

 Added flip for NurbsCurve

 Added bugfix for missing ccw, solid fields of NodeNurbsSurface on pure 
 VRML97 export

 Added flip for grouping nodes.

 Added "Action -> set path of all URLs to"

 Added bugfix for gcc 2.95/VC6.0 crash in "Action -> set path of all URLs to"

 Added "Options -> preferences -> Keep URLs" to avoid URL rewrite

 Added X3D draft LoadSensor
 
 Added flip for Viewpoint and some Interpolators

 Changed configure options for VRML97 Amendment 1 PROTOs

 Added mouse FLY mode

 Added PROTO for X3D draft LoadSensor

 Added showFields() virtual functions for nodes like Collision, which
 need to overwrite the "Show field names" preference

 Added support for M$Windows quatbuffer stereoview (e.g. Nvidia quadro
 with shutterglasses), but NOT splitscreen ("OpenglVR") stereoview

 Changed NodeClass constants more consistent to the X3D draft

 Added bugfix against endless loop when using invalid u/vknot values
 in NurbsSurface and NurbsCurve

 Added superellipsoid and supershape to typical ecmascript VRML examples

 www.web3d.org VRML specification url of the week adjusted

 Added bugfix for array access error in inputdevice/xinput code 

 Added workaround for SGI MipsPRO optimization ? problem in inputdevice/xinput 
 code

 Optimized superellipsoid example

 Added superellipsoid PROTO

 Disabled automatic setting of DISPLAY :0 and automatic starting of X11 
 on MacOSX for security reasons

 Added "-startX11aqua" commandline option (setting of DISPLAY :0 and 
 starting X11) on MacOSX for use in the MacOSX droplet

 Added adjustments for MacOSX "panther"

 Added rendering of first level of Inline nodes

 Added supershape PROTO

 Added SuperShape and SuperEllipsoid rendering and conversion to IndexedFaceSet

 Added texturing, mainAxis and texCoord to SuperShape and SuperEllipsoid

 Added border to SuperShape

 Deleted "+" from default of M$Windows Texteditor linenumber option

 Added writing of SuperShape and SuperEllipsoid as IndexedFaceSet mesh
 on "pure VRML" browser preview and "pure VRML" export

 Added binary patch program to MacOSX version to repair absolute path 
 problem in xcode output

 Added better errormessages for MacOSX version

 Added workaround for freeze of closed X11 windows on MacOSX

 Added bugfix for crash when using the minimal value of border when rendering
 SuperShape

 Added bugfix for crash when rendering SuperEllispoid/SuperShape and
 setting uTesselation != vTesselation

 Changed main datatype "Array" to get rid of monster memory leaks

 Fixed missing data recording to interpolators.

 Added supershape and superellipsoid PROTO files working with MacOSX cortona 
 under different filenames

 Fixed crash after copying by added missing update() and reInit() to 
 supershape and superellipsoid.

 Fixed bug of setting false default for Extrusion.crossSection,
 Extrusion.orientation and Extrusion.spine

 Added documentation files to MacOSX package

 Added BobMaX's pointset preferences patch.

 Added sanity check for height/width preferences

 Added SuperEllipsoid to NurbsSurface conversion

 Added SuperShape to NurbsSurface conversion

 Floating point parsing changed to double type.

 Added fix for missing backup for setting NurbsSurface controlpoints via
 handles

 Added bugfix for crash in Switch and LOD

 Added value checking for near clipping plane

 Added keeping of comments, but comments can be dislocated

 Added MacOSX help viewer document

 Added workaround for MacOSX cortona bug about "something/.." path

 Added workaround for (MacOSX ?) cortona bug about strange handling of
 generation of MF-events (thanks to Braden McDaniel)

 Added better aproximation for SuperEllipsoid/Shape to NurbsSurface conversion

 Interface of Mesh constructor simplified

 Functions of Mesh based nodes collected together in a new MeshBasedNode class

 Added bugfix for wrong flip accounting in MFVec3f datatype
 
 Added "options -> output settings..." dialog

 Advanced SuperEllipsoid rendering

 Added "Actions -> Array"

 Added easier input of floating point data in dialogs

 Added zoom of Routeview Window

 Bugfix for problem in scrolling of routewindow

 Added bugfix for false writing of non NULL default MF fields

 Added setting of far clipping plane

 Added choice of Kernigan/Richie like indent in options -> output settings...

 Added handling of %s in "preview with" string to M$Windows version to
 support VRML browsers started from commandline 

 Added setting of number of indentation spaces when writing VRML file

 Added a M$Windows version which is linked against static versions of the
 jpeg, libpng and zlib libraries. 

 Updated INSTALL documentation for M$Windows.

 Added bugfix for false setting of HandleSize in PreferencesDialog 
 in stereoview mode

 Added loading of Inline nodes which are inside a Inline node.

 Added MaxInlinesToLoad preference and Dialog to avoid a crash 
 when loading a recursive Inline structure.

 Added bugfix for missing loading of Inline not at root level of a VRML file

 Added bugfix for crash when reading DEF/USE constructs.

 Added bugfix for crash when scanning for Inline nodes.

 Added bugfix for handling MFFloat in Fieldview when size == 1

 Added update of dragged MF numbers

 Added bugfix for crash when senseless trying to add a Node to a MFNode 
 field via the fieldview.

 Added bugfix for crash cause of missing ref() in MFFloatItem::OnMouseMove

 Added bugfixes for handling MFTime, MFInt32 in FieldView when size == 1

 Added simpler setSFValue/insertSFValue functions to MF-types
 
 Added bugfix for handling MFFloat, MFTime, MFInt32 in FieldView when size == 1
 
 Added bugfix for drawing selected MF-fields in Fieldview too dark 

 Added bugfix for missing care of limits when changing MF-Fields in FieldView

 Added bugfixes for handling MFColor, MFVec2f, MFVec3f, MFRotation in 
 FieldView when size == 1

 Added bugfixes for using wrong number in startEditing() in MFColorItem, 
 MFVec2fItem, MFVec3fItem and MFRotation

 Added bugfix for crash in Interpolators

 Added hiding of NurbsSurface handle mesh when moving handle (configurable)

 Added avoid of collapse of MF-Fields in Fieldview on insert at beginn and
 delete operations.

 Added mlock/Virtuallock calls and set memory to zero commands to protect
 Upload password from moving to swapfile/swapdevice.

 Added mark for often used events in routeview for sensors, Interpolators,
 Transform, AudioClip, MovieTexture and Material.

 Added workaround for problem with AudioClip

 Added bugfix for problem with AudioClip

 Fixed filemasks according to the VRML97 standard in fileselectors for
 AudioClip and MovieTexture

 Added Preference to render faster/worse (without sorting polygons before draw)

 Changed viewpoint handling on mouse based navigation to avoid viewpoint
 jumping.

 Added rendering of sphere of Background node from FreeWRL ported by Wu Qingwei

 Added "use fork" preference for preview on Linux/UNIX/MacOSX

 Added iconify/deiconify when starting preview without fork

 Added bugfixes for rendering background texture box

 Added bugfix for missing values when recording from inputdevices

 Added bugfix for missing update of position of boolean checkboxes in 
 fieldview.

 Added bugfix for missing return/restart of magnetic tracker on browserpreview.

 Fixed bug in MFString->copy(), which made Nodes with MFString default
 (e.g. NavigationInfo) unusable.

 Added bugfix for configuration problem under solaris

 Added scrollbars for the fieldview window

 Added bugfix for missing update/redraw in fieldview

 Default of NearClippingPlaneDistance preference set from 0.01 to 0.02 to 
 avoid jumping problems when using mouse handles in 3D space

 Added bugfix for missing display of colorcircle

 Added bugfix for early free/missing strdup in swt/motif/tree.c resulting
 in possible a waste of DEF names - deja vue 8-(

 Converted NodeIndexedFaceSet and NodeElevationGrid to a MeshBasedNode

 Added count of polygons/primitives

 Added bugfix if linenumberoption has only blanks 

 Added Extrusion rendering from cybergarage

 Added experimental "-anaglyph red_green" commandline option

 Fixed missing accounting of gray values in ImageTextures and Materials
 for anaglyph stereo

 Added fix of micro$oft compile error

 Added workaround for micro$oft windows problem to set/give back 
 the number of bits per component in the accumulation buffer

 Added workaround for using a detected quadbuffer visual despite
 WantStereo setting

 Added bugfix for false calling of XQueryColor in truecolor mode

 Added bugfix for false replacing of XQueryColor in truecolor mode

 Added green_red, red_blue and blue_red arguments to the "-anaglyph" 
 commandline option

 Added copy of exposedField values to first values of a interpolator

 Fixed bugs that prevented NurbsSurface morphing 

 Added bugfix for rendering of IndexedFaceSet/IndexedLineSet/PointSet
 morping animation

 Added bugfix for crash when trying Normal animation

 Added reduced drawing of keys in ChannelView for Coordinate/NormalInterpolator

 Added scripted SuperExtrusion node

 Added bugfix for false "pure VRML97" writing of SuperExtrusion node

 Added conversion from SuperExtrusion to Extrusion

 Added flip/getBoundingBox to SuperExtrusion

 Changed main part of Extrusion code from cybergarage to OpenVRML
 
 Added generation of TextureCoordinates for meshBasedNode

 Added "Adding a new (scripted) geometry node to white_dune"-cookbook to 
 developer documentation

 Added animation for nodes with fields driven by a eventIn (e.g. "spine"
 of Extrusion)

 Added bugfix for false Extrusion rendering when spine is a straight line

 Added additional marks for often used events in routeview

 Added bugfix for wrong sided rendering of beginCap of Extrusion
 
 Completed VRML loading code in InlineLoadControlPROTO.wrl

 Added bugfix for missing test for load when rendering InlineLoadControl

 Added bugfix for missing setField call when changing MFValues in FieldValue
 via the keyboard

 Added bugfix for writting PROTO definitions (smuggled in by Inline nodes)
 too often

 Added NurbsCurve to SuperExtrusion conversion

 Added SuperExtrusion to NurbsCurve conversion

 Moved conversions to menu popup

 Removed "default color" from NurbsCurveToNurbsSurface and copy matching
 colors instead

 Fixed problems with changing from/to emissive/diffuseColor in 
 NurbsCurve/SuperExtrusion conversions

 Fixed division by zero bug in ColorCircle

 Fixed arrayconstraint bug in NodeExtrusion.cpp resulting in invalid
 rendering on some architectures.

 Fixed arrayconstraint bugs in Mesh.cpp

 Fixed missing diffuse to emissive color conversion in IndexedFaceSet to
 IndexedLineSet conversion 

 Added option to send preview errors to console

 Updated PROTOs in install scripts

 Added missing VRML97 Amendment1 nodes (not rendered yet):
 Contour2D, CoordinateDeformer, NurbsCurve2D, NurbsPositionInterpolator,
 NurbsTextureSurface, Polyline2D, TrimmedSurface   

 Added drawing of at least one channel in channelview for MF interpolators

 Added bugfix for missing initial draw of InlineLoadControl

 Added bugfix for wrong fields of CoordinateDeformer

 Added compilation support of AIX with gcc/g++

 Updated black and white icons

 Fixed missing drawing of root of scenegraph icon

 Fixed MacOSX/AIX compile bug by running missing autoconf

 Fixed crash when using a node with Node::showFields() true and
 clicking a empty SF/MFNode field.

 Fixed crash for wrong handling of empty mesh in mesh.cpp

 Added bff package generation for AIX 4.3 (RS/6000) using mklpp from the 
 AIX bull freeware collection

 Added improvement for AIX package generation

 Added workaround for crash when deleting a USEd node

 Fixed bug about missing store of remotecommand in preview settings

 Conversion routine from png-imagefiles in documention to jpg-imagefiles
 simplified

 Updated some install scripts

 Solved qsort crash on Solaris, which was caused by false sorting of faces

 Added workaround for crash and missing update of selection 

 Updated german tutorial

 Added Wu Qingwei's Extrusion handlers

 Fixed missing update after insert into a MF-Field in Fieldview

 Fixed compile bug about "old" C++ style for-loop scoping code

 Added missing update/reInit of SuperExtrusion node

 Fixed bug in gentoo ebuild script

 MacOSX start has been made more robust against failures of the xcode icon
 program part

 Fixed missing undos of changing ElevationGrid, Extrusion,  NurbsCurve
 and SuperExtrusion via handles

 Added bugfix for wrong nodeclass of NurbsGroup

 Added handles for field "size" of ProximitySensor

 Added bugfix for accidently replacing | sign with : sign in inlined
 javascript code. 
 Do not use from white_dune-0.27beta41 to white_dune-0.27beta228 8-(

 Added bugfix for false read of hexadezimal numbers

 Added usage documentation about MF-input, Array tool and scenegraph operations.

 Added usage documentation about file->upload

 Fixed missing update after delete from a MF-Field in Fieldview

 Added bugfix for wrong copy of exposedField values to first values of 
 a interpolator if interpolator is not empty

 Added bugfix for wrong setting of Extrusion.convex true when converting
 SuperExtrusion to Extrusion

 Added bugfix for wrong setting of IndexedFaceSet.convex when converting
 Extrusion to IndexedFaceSet

 Usage documentation updated

 Added bugfix for wrongly creating normals when converting a Extrusion
 or ElevationGrid to IndexedFaceSet

 Usage documentation about morping animation updated

 Usage documentation about NURBS degree elevate added.
  
 Usage documentation completed.

 Added bugfix for missing update of statusbar text when counting polygons

 Added workaround for false creating of normals for a ElevationGrid

===============================================

Changed in white_dune Version 0.28pl2:

 Fixed layout bug in usage documentation

 Fixed wrong language in german menus

 Added more commands to dune4kids 

===============================================

Changed in white_dune Version 0.28pl4:

 Added tutorial english tutorial

 Added minor updates to all documentation files

 Added bugfix for never overwriting a existing file in a emerency crash save

===============================================

Changed in white_dune Version 0.28pl5:

 Added zero command for FieldView sliders as bugfix for M$Windows

===============================================

Changed in white_dune Version 0.28pl6:

 Added bugfix for crash in dune4kids when pressing cancel in animation dialog

 Added bugfix for crash when flipping a copied SuperExtrusion

 Added horn, shell, ufo and insect rear to the dune4kids shape menu

 Added rest of german translation to resource file

===============================================

Changed in white_dune Version 0.29beta:

 Improved tests of the autoconf/configure file

 Added bugfix to particle proto

 Added bugfix for crash when drawing a IndexedFaceSet with one face

 Added bugfix for generation false normal information in IndexedFaceSets

 Added bugfix for crash when try to build first face of a IndexedFaceSet

 Added bugfix for crash when try to build second face of a IndexedFaceSet

 Renamed _Index to _Field in names of functions, which deliver the
 integernumber of a fieldname

 Added bugfix for too small margin width in Motif textwidgets

 Integrated predefined SuperShapes from dune4kids to normal menu

 Added forgotten half sphere SuperShape item

 Added Actions -> Remove in rest of scenegraph branch: -> normals/normalIndex

 Fixed bug of unnecessary read of USEd Inline nodes

 Added Actions -> Create in rest of scenegraph branch: -> normals

 Added bugfix for crash cause of missing node->isInScene()

 Added Actions -> Remove in rest of scenegraph branch: -> texCoord/texCoordIndex

 Added setting of a default filename on File -> save as/export

 Added bugfix for crash when creating a new node while textediting a field

 Added "-cover" option and four extentions nodes for the cover/covise immersive
 VRML renderer

 Added bugfix for crash when accidently trying to add a normal node to
 a IndexedFaceSet with a already existing normal node.

 Added Actions -> Create in rest of scenegraph branch: -> texCoord

 Added Actions -> Set in rest of scenegraph branch: -> creaseAngle

 Added some resets of selection after UPDATE_ALL window updates

 Added Actions -> Create & Remove in rest of scenegraph branch: -> material & 
 (Image)texture

 Added missing flip of Extrusion

 Added bugfix for setting wrong normals when exporting SuperShape/SuperEllipsoid
 to pure VRML97

 Added color change to dune4kids menu

 www.web3d.org VRML specification url of the week adjusted

 Added second question on exit without save in dune4kids version

 Added missing shell paramater dialog to german version

 Added text creation to dune4kids menu

 Added material and text change to dune4kids menu

 Added german example how to use dune4kids

 Fixed accidently german menu setting of the debian packager

 Added empty transform creation to dune4kids menu 

 Added consistency test between menu in different languages
 
 Added bugfix for german menu

 Added limit to printing of nofatal X11 errors to avoid performance problems

 Added bugfix for using wrong datatype to store linux joystick information

 Added workaround for drawing into routeview over system dependend windowsize
 limits

 Added dialog to show routed nodes at the beginning of the routeview.

 Added compilation support of CYGWIN/X11

 Added bugfix for extremly delayed start of textedit when a animation is
 running and ChannelView is shown

 Added draw(int pass) intended for multipass rendering

 Added rendering of PixelTexture

 Added SFImage editing support

 Added bugfix for crash when using a too large 
 SFImage[0](width)/SFImage[1](height) data with not enough pixel data
 The resulting gap is filled with a black/white or transparent/white pattern

 Added bugfix for falsely display pixeltexture with a linear texture parameter

 Added bugfix for crash of MacOSX desktop icon program

 Added "set linear U/Vknot" menupoint for NurbsSurface

 Added bugfix for crash when using PixelTexture

 Added bugfix for wrong rendering of Switch

 Added (limited) PureVRML97 export of SuperShape morphing animation 

 Added bugfix for crash and missing border field in PureVRML97 export of 
 SuperShape morphing animation

 Added additional export a matching NormalInterpolator on a PureVRML97
 export of a SuperShape morphing animation in -cover mode or with matching 
 output setting

 Moved SuperShape morphing export into a new class MeshMorphingNode

 Added PureVRML97 export of a SuperEllipsoid morphing animation

 Added "action -> move rest of scenegraph branch -> Inline"

 Added PureVRML97 export of a NurbsSurface morphing animation

 Fixed crash when using DEF/USE on the first node in a PROTO

 Added atexit handler

 Fixed false accounting of scrollbar size in RouteView

 Fixed crash in ScriptEditor when removing a EventOut

 Fixed minor crash in atexit handler when try to exit a forked process

 Added motif only workaround for missing scrolling when dragging nodes in 
 SceneTree

 Added workaround against invalid list of already used DEF names in parser

 Added bugfix for wrongly display nodes inside PROTOs in RouteView

 Added writing of optimized meshes in MeshMorphingNode if possible

 Added improved transparency rendering

 Added bugfix for immediate crash of MacOSX icon program

 Added installation method that should avoid MacOSX icon program crashs
 in future 

 Fixed selection to nearest object in 3DView

 Added inputdevice navigation mode

 Improved transparency rendering of closed SuperExtrusion 

 Fixed missing polygon counting of SuperExtrusion

 Added SuperExtrusion to NurbsSurface conversion

 Added patches to Extrusion code from the orbisnap vrml browser project

 Added selftest for similarity of NodeEnum and _proto map

 Added bugfix for accidently allowing to set a DEF for a comment

 Fixed wrong accounting of mesh normals containing erroneous polygons with
 less than 3 vertices

 Added incomplete PROTO rendering

 Added bugfix for crash when removing a copied node and selecting its copy 

 Added EventIn and EventOut handling on PROTO rendering

 Fixed bug in parsing files with carrage return as line terminator

 Fixed crash about wrong delete of class Node instead of delete of NodeData
 
 Fixed build problem of wrongly placed link options in src/Makefile.in

 Fixed problem of sending eventIns to any node of the same PROTO

 Fixed crash when using Array::remove for all arrayelements

 Fixed crash when clicking to a node inside a PROTO

 Added write of files in bigger portions

 Fixed bug of accidently call preview when write of file to preview failed

 Fixed bug of causing double free when using Array::remove for all arrayelements

 Fixed bug of wrong interpretation of some comments

 Fixed crash when try to use meshes with illegal coordIndices < -1

 Added -filedialogdir option for configuration of file dialogs 

 Updated options in manpage

 Added compilation support on HP-UX/parisc using cc and aCC

 Fixed bug in missing remove of tempory nodes from scene in MeshMorphingNode
 class

 Restructed/shorten menus

 Added "move to top" and "rebuild" routeview operation menupoints

 Added bugfix for error in building "connect anything" routes to Scripts

 Disabled SDLjoystick for default FreeBSD build cause of problems in FreeBSD 5.4

 Fixed wrong documentation path for gentoo Linux

 Added mouse navigation icon/menupoint

 Added roll icon/menupoint

 Added movement navigation for mouse button 4/5 (mousewheel emulation) under
 UNIX

 Added creation of new shapes always in front of camera

 Added recalibration button

 Fixed transparency and view from inside problems in primitives

 Added create/remove of Appearance

 Fixed missing graying of CubeTexture menupoint/icon

 Added mode -> WALK mouse menupoint

 Added configure option and test against problems when using png_handle_unknown

 Cleaned up mouse navigation modes and added matching icons

 Fixed bug when rendering IndexedFaceSets with "colorPerVertex false" setting

 Fixed crash in NodeNurbsGroup::write

 Added advanced efence debugging 

 Fixed delete/delete[] error in NodeExtrusion::createMesh

 Added edit->find and edit->findagain menupoint

 Added bugfix for handling missing nodes in rest of scenegraph operations

 Added "edit->copy branch to root" menupoint

 Added copy of similar nodename on Node::copy

 Added standUp icon

 Added mesh morphing pure VRML97 export for SuperExtrusion

 Added bugfix against crash on exit after mesh morphing pure VRML97 export

 Added bugfix against wrong rendering of first PROTO usage 

 Added bugfix against wrong order of closing file commands resulting
 in a horrible big .dunerc file under cygwin

 Added bugfix against bug former bugfix

 Added compile for MacOSX 10.4.2 "Tiger"
 
 Added bugfix against wrong returnvalue of lseek(2) in MacOSX

 Added bugfix against bug former bugfix, lseek in MacOSX is correct

 Fixed missing rendering of H-Anim PROTO

 Added workaround for problem of selection via SceneTree

 Added XY-plane, XZ-plane, YZ-plane workaround for problem with
 Nurbscurve to OrientationInterpolator conversion

 Added workaround for crash when deleting a node in a H-Anim PROTO

 Added rotation handles for H-Anim Joint like PROTOs

 Fixed position of rotation handles for H-Anim Joint like PROTOs

 Added workaround for problem when selecting rotation handles

 Added bugfix for crash on false memory handling when creating SFImage data

 Completed X3D profile detection for interchange profile nodes

 Completed X3D profile detection for all nodes except IndexedFaceSet

 Completed X3D profile detection for IndexedFaceSet nodes with defined
 rendering results

 Added $DUNEMAKEFLAGS e.g. for faster compile on multiprocessor machines

 Added icon and menupoint for interaction creation

 Changed default to not to show all nodes in routeview (especially usefull for
 large scale 3D models)

 Fixed missing route update when not all nodes are shown in routeview

 Added motif/lesstif callback for combobox changes

 Use motif/lesstif combobox callback in interaction dialog

 Fixed bug of missing set of window/scroller size in CheckBoxWindow

 Fixed compile problem with symbol SW_0 new defined in newer Linux kernels
 
 Fixed problem with wrong update of route when moving node to top
 
 Added write of PROTO/EXTERNPROTO statement based on parsing result

 Fixed missing handling of NavigationInfo.speed

 Fixed problem when parsing | characters in VRML comments

 Added --nounistd workaround for bug in debian flex

 Fixed wrong handling of headlight of multiple NavigationInfo nodes in scene

 Fixed problem with wrong backface culling during moving in scene

 Fixed problem with --nounistd workaround for bug in debian flex

 Fixed wrong fileextensions in some fileselectordialogs

 Solved problem with shift/reduce conflict in parser.y

 Fixed crash when parsing a Script node with a SFNode field

 Solved performance problem when parsing long files

 Fixed missing write of a IS field in a Script inside a PROTO

 Added export of VRML97 level X3DV files

 Added bugfix for confusing handling of fieldnames after export of a X3DV file.

 Added bugfix for missing drawing of handles

 Added bugfix for missing conversion Nurbs to IndexedFaceSets on export of
 VRML97 level X3DV files 

 Reduced harmless warnings under HPUX

 Ported inserticon script to gimp-2.2

 Removed experimental MFFloatToSFFloat and MFInt32ToSFTime scripted PROTO nodes

 Added documentation and desktop icons to debian packages

 Added fix on crash when rendering invalid IndexedFaceSet node with not enough
 color values 

 Added -purevrml97 and -vrml97levelx3dv commandline converter arguments

 Changed wrong rotation of SuperExtrusion

 Fixed missing purevrml97 preview setting

 Added actions -> xray rendering menupoint

 Added docs/hanim_history with free motion capture VRML files as startpoint
 to create H-Anim compatible data

 Added actions -> swap -> xy/yz/zx 

 Added copy botton to script dialog

 Fixed false rendering of IndexedFaceSet/IndexedLineSet if colorIndex field
 is empty

 Added superextrusion donut example to docs/scriptedNodes

 Fixed crash on conversion from IndexedFaceSet to IndexedLineSet when
 Appearance == NULL

 Fixed wrong color of unlit Lines/Points and glut rendered text  

 Added edit -> USE menupoint

 SFInt32 changes in the fieldview made less sensitive

 Avoid request for PROFILE Full on X3DV export by using COMPONENT Statements 

 Added Box to IndexedFaceSet conversion

 Changed insert of new node inside the current selection if possible

 Implemented jump to begin and end of channelview window

 Added check/repair code of texteditor command  

 Added bugfix for missing return of getComponents() in Scene.cpp

 Added ugly workaround for bug in floating point write

 Added workaround for problem when writing NAN/INFINITE floating points

 Fixed two crashes occured on HPUX

 Simplified ScriptEditor code

 Fixed problem in workaround when writing NAN/INFINITE floating points

 Added external object editing of ImageTexture, MovieTexture and AudioClip node

 Added handles for Coordinate node

 Fixed crash when deleting from a MFField

 Fixed problem when creating geometry in front of the camera

 Fixed wrong keyPressed/keyReleased eventOut name in NodeCOVER

 Fixed wrong call of ImageEditor instead of MovieEditor

 Added more programs to test for Image/Sound/Movie editor

 Fixed wrong writing of eventIn/eventOut etc. instead of 
 inputOnly/outputOnly etc. when writing x3dv files

 Added feature to read white_dune own x3dv output

 Added "Actions -> field pipe" menupoint
 
 Added bugfix for random characters in errormessage after invalid field pipe

 Added bugfix for compile problems when using older bison versions 
 
 Fixed wrong parsing of META statement

 Added missing field pipe of SF/MFNode fields

 Added X3D SF/MFDouble type

 Fixed problem in selftest related to SF/MFDouble

 Added additional rendering of ProximitySensor boundaries

 Fixed missing tracking of morphing animation when changing Coordinate
 node from handles

 Added --with-uninstallcomment to configure

 Moved initialize, eventsProcessed and shutdown handling from 
 ecmascript settings to scripteditor dialog

 Fixed crash when using inputdevice commandline options 

 Shortend InputDeviceSettingsDialog.cpp

 Replaced all sprintf calls in program (except replacement testing program)

 Fixed selection problem of handles for Coordinate based nodes

 Added icons to limit move of handles to a line or a plane

 Fixed some compile problems under HP-UX

 Fixed wrong change of handle size when changing NavigationInfo.speed

 Added workaround for crash on conversion of IndexedFaceSet with 
 textureCoord field to IndexedLineSet

 Added deny of unsupported write of USE or USEd node to Inline

 Fixed problem with adding wrong last line to a piped field 

 Changed mesh of NurbsSurface from quads to triangles, improving x symetry

 Added --without-optbigfiles configure option for compiling on machines
 with low memory

 Moved language translation as commandline option into one binary

 Fixed typo in dune.german.rc

 Fixed missing translation of tooltips

 Simplified the task of language translation
 
 Updated documentation about language translation

 Added workaround for wrong cull when drawing Background cube

 Fixed missing translations of status bar messages on mouseover on menu entries

 Updated rpm spec file

 Added inputdevice support for icons to limit move of handles to a line or a 
 plane

 Fixed missing update of Color node

 Added action->flatten->x/y/z for Coordinate, Extrusion, NurbsCurve, 
 NurbsSurface and SuperExtrusion

 Moved configureoption about black and white icons into a commandline option

 Added ARSensor extension node and some nonstandard fields for Covise/COVER

 Fixed missing interaction dialog of Covise/COVER extension nodes

 Added missing NavigationInfo.isBound eventOut

 Added some Covise/COVER extension nodes and nonstandard fields.

 Updated usage documentation.

 Fixed problem with XmNmarginWidth in Unix textedit widget

 Fixed wrong handling of invisible root node in route view

 Added routes -> Show routes of node on top menupoint
 
 Fixed missing animation tracking of viewpoint 

 Added bitmap for X11 windowmanager iconify 

 Added flatten/flatten to zero menupoint

 Added minor update to documentation

 Added cover nodes Sky, Vehicle and SteeringWheel

 Simplified setting of field flags

 Simplified EXTERNPROTO writing

 Fixed some float/double ambiguousities

 Fixed some Micro$oft Windows compiling problems

 Fixed new website http://vrml.cip.uni-stuttgart.de/dune

 Fixed bug in wrong open recent document of File menu

 Added vrml level combobox to browser preview settings 

 Fixed missing support for covers multiple texCoord files

 Fixed crash in configure script when adding third language translation

 Fixed wrong handling of inputdevice data, if no x/y/z only mode is set

 Added incomplete italian menu translation

 Fixed bug in incomplete italian menu translation

 Added workaround for performance/memory problem on big scenegraphs
 when unnecessarily drawing a hiden routeview

 Added next step in italian menu translation

 Added workaround for performance/memory problem on big scenegraphs
 when not drawing all nodes in routeview

 Fixed problem with debian menu install

 Added bugfix for wrong update of routeview when adding a route 

 Fixed Routes -> show node on top

 Added reconversion of former Covise/COVER extension nodes with nonstandard 
 fields written as PROTOs 

 Fixed bug in handling -?rot commandline arguments 

 Added support of X3D only fields in VRML97 nodes

 Added File->New->X3DV menupoint

 Fixed missing rendering of animation 

 Fixed gaps in FieldView

 Solved m4 compatibility problem

 Fixed bug in Sphere to NurbsSurface conversion

 Fixed crash when changing fields via mousedrag in FieldView

 Changed internal website information to 129.69.35.12 cause of problems
 in DNS management out of our control

 Added workaround for crash when reading PROTOs

 Fixed bug in PROTO rendering

 Fixed bug in pure VRML97 export of COVER node

 Fixed bug in reconversion of cover nodes exported to pure VRML97

 Added FillProperties and LineProperties X3D nodes

 Fixed wrong show of x3d only fields in scene tree view

 Fixed missing MovieTexture.isPaused and NavigationInfo.bindTime

 Completed italian menues

 Fixed wrong menu disabling of MultiTexture and some COVER nodes

 Fixed needed changes to developer documentation

 Fixed wrong routes from/to X3D events in Interpolationdialog and 
 Animationdialog when in VRML97 mode

 Fixed wrong write of metadata events on writing of Script node 

 Fixed wrong routeview hide of nodes in IS fields 

 Changed nurbsplane/curve creation to symetric values

 Added backup of old colors on nurbscurve to nurbssurface change

 Changed File->Import to insert into current selection if possible

 Added VrmlScene and VrmlCut scripted Protos for building sequences of 
 animations 

 Added workaround for problem with Viewpoint.description

 Added workaround for crash in pick handle of nodes inside VrmlScene

 Added workaround for crash when writing VrmlCut/VrmlScene

 Fixed bug in configure options of debian packager

 Simplified functions of nodes containing MFNode fields

 Fixed problem with to late setting switch choice in VrmlCut script 

 Added Change -> Animationtime to -4kids menu

 Added conversion of NurbsCurve to X3D(V), route handling and closed
 field still not supported

 Fixed rendering problems for PointSet, IndexedLineSet and NurbsCurve
 on some systems

 Added some textures make with gimp and povray

 Fixed route handling on conversion of NurbsCurve to X3D(V)

 Fixed some problems on pure (VRML97/X3DV) save/export 

 Fixed crash in conversion of Box to NurbsSurface, if the "6 planes" checkbox 
 is not set

 Added conversion of NurbsSurface to X3D(V) NurbsPatchSurface

 Fixed some problems in controlpoint handling of NurbsCurve

 Added workaround for data loosing bug in File->Import

 Improved x-symetry of Box to converted to NurbsSurface, if the "6 planes" 
 checkbox was not set

 Fixed crash in NurbsPatchSurface->U/V degree elevate

 Added Workarounds for bugs when reading X3DV NurbsPatchSurface nodes

 Fixed wrong writing/handling of X3DV COMPONENT statement

 Added italian usage documentation

 Added X3D SFVec3d/MFVec3d types
 
 Added X3D Geospatial stubs

 Added bugfix when a system do not have a powl function

 Integrated foreign language handling into swLoadString

 Added yes/no translation to Linux/UNIX messageboxes

 Fixed missing undo for Nurbs(Patch)Surface and NurbsCurve

 Updated italian usage documentation and italian menus

 Fixed bug of yes/yes/cancel dialogs

 Updated dune4kids usage example

 Fixed bug in double writing controlPoint in NurbsSurface/NurbsCurve after
 X3DV/VRML conversion

 Fixed compiling bug related to m4 CommandlineLanguages.h.m4

 Rearraigned icons to support machines with a 800x600 size display and 
 without input devices

 Fixed missing change of NurbsSurface name to NurbsPatchSurface in 
 SceneTreeView after save to X3DV

 Changed often needed --with-buginlesstif configure option to rare used
 --with-oldmotif configure option

 Added --with-archives configure option for compile time optimization on
 machines with few RAM memory but enough disk space

 Fixed bug in make depend

 Added icons for GeoNodes

 Added X3D/GeoVRML conversion to GeoCoordinate and GeoElevationGrid

 Completed X3D/GeoVRML stubs conversion 

 Added menuitems for GeoNodes

 Added uniform scale

 Added minimal similification of the Extrusion code

 Updated TODO list

 Added AC3Db file export (MeshBasedNode, Transform and Group nodes only)

 Added AC3Db file export for Color nodes

 Fixed compiling bug, if libdevil is present

 Solved crash when exporting AC3Db file

 Fixed unnecessary bugmessage in case of SIGPIPE

 Added -ac3d commandline option

 Added - argument for input pipe

 Removed normal generation on Box to IndexedFaceSet conversion 

 Fixed minor problems in debian build

 Added ac3d export of Box

 Added solid field rendering of X3DV Box

 Fixed wrong number of shininess in ac3d export

 Added ac3d export of Sphere

 Fixed wrong sided ac3d export 

 Added ac3d export of Cone 

 Fixed minor problems in debian build

 Fixed missing ac3d export of IndexedFaceSet.ccw/solid/creaseAngle fields

 Added ac3d export of Cylinder 

 Added menupoint Actions -> Rest of Scenegraph branch -> set -> solid

 Fixed bug in wrong IndexedFaceSet conversion of cone and cylinder.height

 Added workaround for problem with normal generation of IndexedFaceSet
 conversion sphere and cylinder

 Added handles for SuperExtrusion.a

 Added compiler/linker options for fat binary generation on MacOSX

 Added -tesselation commandline option for rendering on slow machines

 Added bugfixes for wrong conversion of Cone and NurbsSurface to IndexedFaceSet

 Fixed problem of smoothing triangles with IndexedFaceSet.creaseAngle 

 Added X3D solid rendering of Cone and Cylinder

 Added combined DEF and proto name display in scene tree

 Fixed fuzzy display of icons
 
 Removed/replaced all tabulator characters in C/C++ sources

 Added search for tabulator characters to selftest

 Fixed bug in MultiTexture.alpha initialization

 Added Ac3d export of IndexedLineSet

 Fixed wrong Ac3d export of Group and Anchor

 Fixed crash in smoothing triangles

 Fixed missing catt 8 geo export of both sides of mesh, if solid() is not set

 Fixed wrong handling of deleted nodes
 
 Added action -> rest of scenegraph -> remove: DEF name

 Added ac3d and catt 8 geo file export for grouping nodes

 Added german html documentation about commandline processing with white_dune

 Fixed compileproblem on MacOSX 10.4.8
 
 Fixed missing support for ac3d export of Transform.scale and 
 Transform.scaleOritentation

 Added data container nodes for catt 8 src.loc/rec.loc export

 Fixed crash in SuperShape/SuperEllipsoid to NurbsSurface conversion

 Fixed crash when the overflow flag of the selection buffer is set

 Fixed missing write support for transformed catt 8 container nodes

 Added support for files with UTF-8 Byte Order Mark (BOM)

 Replaced the --with-vrml97am1url, --with-x3ddrafturl, --with-scriptednodes
 and --with-covernodes configure options with the --with-protobaseurl
 configure option

 Updated INSTALL document

 Added workaround for wrong pure VRML97 export of TrimmedSurface node

 Solved crash in DEF name map

 Deleted "freeglut-dev" Build-Depend from debian packaging: newer freeglut 
 versions lead to a unexpected program exit.

 Fixed unnecessary write of indent when writing a node

 Replaced requirement for GLUT by integrating parts of OpenGLUT

 Fixed missing "-remote OpenURL" command for old style netscape browsers

 Added bottom, bottomBorder, top, solid, ccw fields to SuperEllipsoid 
 
 Fixed wrong CoordinateInterpolator for SuperEllipsoid morphing 

 Added bottom, bottomBorder, top, solid, ccw fields to SuperShape 

 Fixed wrong rendering of uTesselation/vTesselation in SuperEllipsoid and 
 SuperShape 

 Added mesh optimization before ac3d/catt 8 geo export

 Updated SuperEllipsoid and SuperShape PROTOs for new bottom, bottomBorder, 
 top, solid, ccw fields

 Fixed crash in DEF name handling

 Fixed problem with MacOSX icon

 Added scale of handles of ExportCatt and Transform nodes.

 Fixed problem with efencedune creation

 Added support for use of duma instead of efence

 Fixed minor valgrind detected initialization problem in MainWindow::MainWindow

 Solved bug in Help -> node menupoint

 Added removement of double faces to Meshoptimization

 Added (IndexedFace)Set->optimize menupoint

 Fixed grep/#ifdef compiling problem with some flex versions

 Added fix against crash in scripteditor

 Fixed missing -tesselation handling of Cone drawing

 Fixed unneccessary calls to UpdateViews

 Fixed some unneccassary redraws of node buttons

 Cleaned some magic numbers in MainWindow::UpdateTools

 Fixed crash in connection with wrong creation of Coordinate node as subnode
 of NurbsSurface node

 Fixed wrong call to updateColorCircle

 Added fix for missing [] of some deletes

 Added NurbsSet creation

 Added -4catt commandline option for a simplified user interface for
 simple export to catt 8 data

 Cleaned up some "-Wall" warnings

 Fixed error in AflockDevice::getHemi

 Fixed some (s)scanf format errors

 Fixed crash when clicking to a unused part of Proximity handles

 Fixed missing initialisation in application wide settings 
 This can cause always the start of the wrong (catt 8 exporter) GUI

 Added simpler rest of scenegraph branch one side/two side dialog to 
 catt exporter GUI

 Added handle size/scale settings to catt exporter GUI

 Added flip of side in rest of scenegraph branch

 Added bugfix for crash in mesh optimisation

 Added extra polygon count for catt 8 export

 Added 1 sided/2 sided polygon count for catt 8 export

 Fixed syntax error detected by gcc version 2

 Fixed Win32 LoadString problem by using the motif swLoadString implementation

 Added optional call of a revision control check in command on ever save

 Fixed bug in loading preview level to preview settings dialog

 Added most important icons to 4kids GUI

 Fixed number of axes detection of windows joystick

 Fixed wrong double apply of swap and flip commands

 Fixed missing update in some swap commands

 Updated license statements

 Fixed potential wrong free of NULL pointer

 Improved man page formatting

 Fixed crash when displaying mesh with invalid coordIndex

 Added workaround for worsened examine mode

 Fixed crash in moving scale handles

 Added workaround for problem with exporting NurbsGroup to pure VRML97

 Fixed wrong examine mode

 Changed walking mode to timer based moving

 Fixed problem with exporting NurbsGroup to pure VRML97

 Fixed alignment problem when converting from Vec3f arrays to MFVec3f under HPUX

 Fixed crash when exporting pure VRML97 from X3DV

 Fixed crash in commandline usage

 Added support for DEC Alpha Tru64 CXX

 Fixed floating point crash on DEC Alpha

 Added switching of language translation via LANG environment variable

 Added better workaround for problems with 3D cursor

 Fixed wrong direction when moving up/down in walk mode

 Simplified texture usage in cover mode.

 Added workaround for problem with motif fileselectors

 Added workaround for (libglu ?) problem on some systems
 when drawing cone in quadbuffer stereo mode

 Added workaround for X11 crash on MacOSX after program exit

 Fixed overflow of OpenGL matrix stack on draw of 3D cursor

 Added limited output of OpenGL errors

 Replaced Dampers.wrl with a more recent version

 Fixed wrong writing of [] brackets on SFImage output

 Fixed crash in SFImage comparison

 Fixed missing scale/center handling in x/y/z only mode

 Added move actions -> sibling commands

 Fixed crashes in connection with VrmlCut/VrmlScene scripted nodes

 Added change image repeat to 4kids menu

 Added better workaround for problems in VrmlCut/VrmlScene scripted nodes

 Fixed problem when reading gzip compressed files under M$Windows

 Added yet another better workaround for problems with 3D cursor
 
 Fixed crash when using convertion to NurbsSurface on root node

 Avoided automatic use of "toNurbs" operation in 4Kids "new" menu 
 and added "make deformable (NURBS)" icon/menupoint instead

 Fixed wrong OpenGL stack underflow

 Added color change of selected handle

 Updated dune4kids tutorial

 Rearranged a few menupoints in the 4kids menus

 Solved crash when counting polygons in empty inlines

 Fixed start problems of macosx fat binary icon program

 Additional nodes for "animate" and "add interaction" are added inside of
 same scenegraph branch

 Added more per default selected fields for dune4kids animation dialogs:
 Viewpoint.orientation/position, NurbsSurface.controlPoint, Coordinate.point
 and Material.diffuseColor

 Simplified code for browsing commands
 
 Advanced symetry/usability of Box convertered to IndexedFaceSet

 Added workaround against crash when clicking to open icon under MacOSX

 Fixed crash when clicking to open icon under MacOSX

 Fixed wrong creation of X3D profile statement

 Added example template of a meshbased geometry node

 Changed vrml/x3d/x3dv translation from NIST translators to random shellscripts
 (configurable via "input/output settings" dialogs)

 Added triangulation of IndexedFaceSet

 Fixed crash before showing errormessage when detecting unsupported VRML1 files

 Fixed crash before showing errormessage when detecting unsupported nodes

 Fixed missing sceneview update problems in triangulation of IndexedFaceSet

 Fixed missing selftest of X3DV files.

 Fixed wrong errormessage when loading NurbsCurve in a X3D file.

 Fixed missing convertion from primitive shape to NurbsSurface node 
 on some "Create -> X3D Node -> NurbsPatch ->" menupoints

 Fixed crash before showing errormessage when detecting unsupported fields

 Fixed wrong linenumberreporting on errors caused by comments

 Fixed parser rule, that forbid "inputOutput/exposedField" commands in
 Script nodes. This rule is correct for the old VRML97 fileformat, but no 
 longer correct for the new X3D fileformat.

 Added node semantics for ISO/IEC FCD 19775-1r1:200x X3D draft 
 "Rigidbodyphysics" nodes 

 Added "X3DV with rigid body XJ3D extensions" item in preview settings dialog,
 to generate XJ3D special "xj3d_RigidBodyPhysics" componentname.

 Fixed missing update of brightness selection bar of colorcircle

 Fixed problem in creating animation via input devices 

 Fixed missing show of field names for some RigidBodyPhysics nodes 

 Fixed problem in SceneTree caused by wrong handling of "show of field names"
 setting

 Fixed problem with SceneTree drag'n drop on CollisionSensor node

 Fixed some RigidBody node rendering problems

 Added options menupoints to change language and GUI variant

 Added rendering of initial RigidBody position

 Added workaround for problem of display of scenegraph after a 
 node delete operation

 Added handles for the fields RigidBody.linearVelocity and 
 RigidBody.angularVelocity

 Added some "recommended EventIn/EventOut" flags to some rigid body physics 
 nodes.

 Fixed wrong handling of invalid characters in Edit->DEF dialog

 Added building of RidigBodyCollection from NodeCollidableShape nodes in a 
 scenegraph branch

 Added workaround for texture rendering problem of Box and Cylinder nodes

 Fixed wrong nodetype for joint nodes

 Fixed wrong creation of already used DEF names in Edit->DEF dialog

 Added X3D KeySensor node

 Fixed SuperShape node rendering

 Fixed crash in scripteditor 

 Fixed possible problem with rendering errors after insert of a new 
 CollidableShape node into scenegraph

 Added X3D StringSensor node

 Fixed crash when rendering PROTO

 Fixed wrong fix about SuperShape node rendering

 Fixed wrong SuperShape to NurbsSurface conversion

 Added X3D ColorRGBA node

 Solved crash in creation of color animation

 Fixed wrong copy to first Interpolator.keyValue during animation creation

 Added X3D Position2DInterpolator and Coordinate2DInterpolator

 Moved configure check for ODE library to a new --with-ode configure option

 Added workaround for crash when deleting USE'd node

 Fixed missing close of colorcircle in some circumstances

 Added X3D TriangleSet

 Fixed bug in hasInputDevices() in case of failed device initailisation

 Fixed wrong handling of 6D transform mode

 Added convertion to TriangleSet

 Added convertion of rest of scenegraph branch to TriangleSet

 Added convertion of rest of scenegraph branch to IndexedFaceSet

 Added remove of node in scenegraph branch based on nodename

 Added selection of RigidBody node, when clicking to a child of 
 a RigidBodyCollection node in the 3D preview window

 Modified handle draw of ProximitySensor and CattExportSrc to be visible
 both with and without depthtest

 Avoid writing of default material on ac3d export (if possible) 

 Added field pipe of scenegraph branch

 Fixed wrong second "flip side" command in "Coordinate" nodes, if the command
 is used for a scenegraph branch 

 Changed NurbsCurveSurface implementation to chain based rendering

 Completed rendering of all ISO 19775:2004 Geometry2D component nodes

 Fixed wrong initalisation of empty MFFloat, MFVec2f and MFVec3f values

 Fixed a buffer overflow security problem and a format string security problem
 reported by Luigi Auriemma

 Fixed a format string security problem in the motif version of swDebugf

 Fixed error in parsing MFColorRGBA data

 Fixed rendering errors for Color/NormalPerVertex FALSE meshes

 Added workaround for various crashes based on getParent()

 Added handles for anchorPoint field of matching X3DRigidJoint nodes

 Advanced selection/usage of RigidBody node handles

 Added workaround for crash when parsing X3D Rigid Body Physics component nodes

 Fixed wrong handling of the "The value of the colorPerVertex field is ignored 
 and always treated as TRUE" rule of the TriangleSet node

 Solved crash after removing a RigidBody node

 Added creation of CollisionSensor node to  a scenegraph branch based creation 
 of a RigidBodyCollection node

 Solved missing selection of RigidBody node handles after fresh creation
 of a scenegraph branch based creation of a RigidBodyCollection node

 Added workaround for wrong writing of USE

 Added workaround for bug in ac3d export

 Added extra "ac3d for RAVEN" export as a workaround for a problem of the
 RAVEN program itself

 Fixed bug in removeParent function Fixed bug in Node::removeParent function, 
 that caused unintended creation of DEF names during copy/move commands

 Fixed missing disable of some buttons for X3D Rigid Body Physics nodes

 Added axis handles for RigidJoint nodes 

 Fixed bug in undo/redo of IndexedFaceSet to IndexedLineSet conversion

 Fixed some potential format string security problems

 Added DIS component nodes

 Fixed wrong setting of double DEF name

 Fixed crash cause of wrong initialisation of class Node

 Fixed wrong writing of a comment inside a empty MFNode field

 Fixed missing creation of X3D FillProperties and X3D LineProperties

 Added configure option to disable usage of rendering of gif textures

 Improved selection after delete operations

 Added Ldraw.dat export for meshbased nodes

 Added Ldraw.dat export support for "Material" nodes

 Added BooleanFilter, BooleanToggle, BooleanTrigger, TimeTrigger and 
 TextureCoordinateGenerator X3D nodes (not rendered)

 Added IntegerSequenzer X3D node (not rendered)

 Reduced Ldraw export colors to colors supported by the LeoCAD program

 Added HAnim X3D nodes (not rendered)

 Added IntegerTrigger X3D node (not rendered)

 Split NodeTransform class into two subclasses

 Fixed wrong creation of HAnimSite node

 Added X3D LineSet node

 Added X3D StaticGroup node

 Fixed unneccessary redraw in AnimationDialog window

 Added X3D IndexedTriangleFanSet, IndexedTriangleSet and IndexedTriangleStripSet
 nodes (not rendered)

 Fixed missing fields and wrong X3D fields of NurbsPositionInterpolator node

 Added X3D TriangleFanSet and TriangleStripSet

 Fixed wrong nodename Coordinate2DInterpolator to CoordinateInterpolator2D

 Fixed wrong nodename Position2DInterpolator to PositionInterpolator2D

 Fixed wrong nodename IntegerSequenzer to IntegerSequencer

 Fixed wrong fieldname X3DRigidJointNode.mustOutput to 
 X3DRigidJointNode.forceOutput

 Added X3D CoordinateDouble node (not rendered)

 Fixed missing convertion to NurbsSurface to NurbsPatchSurface on save to
 X3DV of a VRML file

 Fixed VRML convertion problem when using File - Textedit menupoint

 Added X3D NurbsSurfaceInterpolator x3d node (not rendered)

 Added X3D NurbsOrientationInterpolator x3d node (not rendered)

 Added X3D NurbsSweptSurface x3d node (not rendered)

 Fixed wrong MFFloat fields (instead of MFDouble fields) of 
 NurbsOrientationInterpolator x3d node and NurbsSurfaceInterpolator x3d node

 Added X3D NurbsSwungSurface x3d node (not rendered)

 Added X3D NurbsTrimmedSurface x3d node (not rendered)

 Added "Change -> Handles -> Change distance between handles which snap 
 together or handled x-mirred" menupoint to 4kids menu

 Fixed problem with flip of scenegraph branchs

 Fixed problem of unnessecary flip of some primitive nodes

 Added "actions -> set default values (keep scenegraph)" menupoint

 Added modified transform based motion capture human animation examples to
 typical VRML examples documentation

 Fixed missing tooltip for "make deformable" icon in dune4kids gui

 Added some usefull exceptions for "actions -> set default values" menupoint

 Added Covise/COVER plugin extension VirtualAcoustics and VirtualSoundSource 
 nodes

 Extracted ProtoMap from Scene.cpp into SceneProtoMap.cpp

 Updated developer documentation

 Enhanded begin of dune4kids tutorial

 Added LabView Covise/COVER plugin extension node

 Fixed parser error for X3DV InputOutput fields 

 Added export to kanim fileformat for the kambi gameengine

 Added support for a dials inputdevice made of Mindstorms NXT motors

 Fixed problem with kanim fileformat of some NurbsSurface animations

 Solved crash in moving handles of Extrusion node

 Fixed wrong graying of some cover menupoints

 Fixed wrong display of kambi fields in some nodes

 Added toolbar for kambi nodes

 Fixed wrong insertion on DEF/USE writing

 Fixed handle move problem on some OpenGL systems, when a Viewpoint node 
 is used

 Fixed missing "Start next time with kambi extensions" feature

 Fixed support of menupoint for Kambi Toolbar

 Fixed wrong graying of KambiAppearance icon

 Added rendering of IndexedTriangleSet, IndexedTriangleFanSet and
 IndexedTriangleStripSet

 Fixed bug in writing texture data in exported AC3D files

 Added conversion of ImageTexture to bmp fileformat (via imagemagick "convert") 
 when exporting to AC3D files

 Added support for SpaceNavigator devices under M$Windows XP (requires 
 recompilation)

 Fixed strange output formatting 

 Fixed missing write of eventIns/eventOuts in creation of cover/kambi 
 extension protos

 Fixed wrong type of Fog.volumetricDirection kambi extension

 Fixed wrong writing of cover/kambi extension proto on save after pure vrml97 
 export

 Changed texture of AC3D export from BMP to GIF fileformat

 Added EXTERNPROTOs to some kambi nodes

 Fixed some VRML/X3DV formating problems 

 Fixed wrong TextureCoordinate export to AC3D, if texCoordIndex is not set

 Fixed wrong texCoordIndex export to AC3D

 Fixed missing rendering of TriangleStripSet.creaseAngle and 
 IndexedTriangleStripSet.creaseAngle

 Added -fn commandline argument to select other fonts on Linux/Unix/MacOSX

 Added support for double size icons

 Fixed some missing updates in *Triangle*Set nodes

 Added description of some rendering bugs by Michalis Kamburelis

 Added SuperRevolver scripted PROTO

 Advanced x symetry of SuperExtrusion node

 Fixed missing errormessages when the use of nxtdials failed

 Added spindle and mushroom shape to dune4kids menu

 Added SuperRevolver to Nurbs(Patch)Surface conversion

 Added degree elevate to SuperRevolver and SuperExtrusion

 Added menupoint Actions -> Rest of Scenegraph branch -> set -> convex

 Added workaround for inexact SuperExtrusion to Nurbs(Patch)Surface conversion

 Added bugfix for wrong always inserting of new Inline nodes at the root level

 Added bugfix for wrong initialisation of TimeSensor, which can result
 in missing animation or animation recording

 Added workaround for inexact SuperShape to Nurbs(Patch)Surface conversion

 Added missing close field to NurbsCurve2D node

 Fixed wrong double "metadata" field in X3D Metadata nodes

 Fixed crash in connection with -fp option 

 Added workaround for problems with menupoint edit->find in cover mode  

 Partitially fixed olpc font rendering problems
 
 Fixed wrong rendering of quotes strings and backslashes in Text node

 Updated usage documentation

 Completed text of usage documentation

 Added italian translation of usage documentation

 Changed object edit command selection to dialog of
 "Options -> Text/Object Editor Settings..." menupoint

 Added a few images about triangulation to usage documentation

 Added menupoint to 4kids menu to create a heart shaped SuperRevolver

 Minor updates to man page

 Added display of selected vertex information on statusbar 

 Added minor bugfix about false _keycodeMap initialisation

 Activated handle movement in mouse navigation mode

 Fixed wrong drop of a invalid child node

 Fixed missing update of nodename on SceneTree drag'n drop USE command

 Fixed wrong errormessage problems in MacOSX leopard desktop icon program 

 Fixed wrong "flip x" command for SuperRevolver node

 Advanced SuperShape to Nurbs conversion

 Added menupoints to increase/decrease EXAMINE mode turnpoint

 Improved internationalisation of status bar messages 

 Fixed wrong scale on Ldraw dat export

 Improved internationalisation of status bar messages and message boxes
 
 Completed internationalisation of message boxes

 Advanced testing of internationalisation in resource file
 
 Fixed rpm creation script for rpmbuild of fedora 10

 Fixed missing open of OpenGL window on commandline usage of Ldraw.dat export
 
 Fixed missing update of "edit -> find again" menupoint after "edit -> find"
 command

 Fixed wrong x/z swap on Ldraw.dat export

 Changed definition of orange brick color for LeoCAD Ldraw.dat export

 Added definition of transparent orange brick color for LeoCAD Ldraw.dat export

 Fixed bug in selection update in case of "show all field names" preference
 setting

 Fixed and optimized various unnecessary update operations

 Fixed crash in error messagebox of a scriptedit operation

 Fixed OpenGL warning cause of wrong argument to glPushAttrib

 Improved speed of window close operations

 Fixed missing write in PROTO definition of a predefined node

 Changed write of a field in a EXTERNPROTO instance: all fields are always 
 written

 Fixed wrong delete of some array data in the FieldView

 Added export to X3D especially for the SUN wonderland X3D importer multiuser
 world

 Fixed crash in x3d conversion of NurbsSurface/NurbsCurve

 Added some rendering improvement by using display lists for meshes

 Added arrow handles for center field of ProximitySensor

 Fixed wrong writing of PROTO fields

 Fixed crash when using a wrong type in a PROTO declaration

 Fixed missing show of fields on PROTO usage

 Avoid double writing of ROUTE statements in PROTO statements

 Added possiblity to show numbers for some nodes in 4kids GUI

 Added new SuperEllipsoid menupoint to 4kids GUI

 Added "new Ring" menupoint to 4kids GUI

 Fixed temporary rendering problem of Text node in 4kids GUI caused by a 
 error in text change dialog

 Advanced Extrusion node rendering

 Added action -> rest of scenegraph branch -> move to transform selection 

 Updated usage documentation
  
 Updated manpage

 Added some configure tests to use fpclassify

 Fixed wrong creation of faces in SuperRevolver node

 Added some italian translations to italian dune4kids menu

 Added better workaround for SuperEllipsoid to NurbsSurface conversion

 Fixed crash in PROTO initialization

 Enabled import of illegal X3DV files without requried PROFILE statement

 Fixed missing X3D/XML import/export via encoding translators

 Added shellscripts for X3DV <-> X3D/XML translators via InstantPlayer/aopt
 
 Fixed missing <IS></IS> tags in X3D/XML PROTO writing

 Fixed missing EXTERNPROTO generation for some COVER extension nodes.

 Added limited export to C++ source

 Added export to XML encoded X3D

 Fixed crash cause of missing check for wrong types in PROTO declarations
 
 Fixed possible crash when using menupoint
 action -> rest of scenegraph branch -> move to transform selection 

 Changed usage of C++ sourcecode exporter

 Added limited export to C source

 Added limited export to java source

 Fixed error in parsing some SFMatrix3f values

 Fixed crash in 2D graphics part of M$Windows port

 Fixed wrong writing of brackets in fieldpipe of MF* types

 Fixed wrong parsing of NurbsTrimmedSurface

 Fixed wrong VRML97 parsing of DEF names which are only illegal in X3DV

 Fixed wrong parsing of X3DV files with COMPONENT statements 

 Fixed wrong normal null creation

 Added special Cover dialog for Edit->DEF adding a listbox for special
 DEF name prefixes used by the COVER browser

 Fixed missing route colors for some types in SceneGraphView 

 Fixed "code too long" (64KB java classfile limit) problem when exporting 
 java source code

 Fixed wrong normal creation

 Added simple java export example for usage with jMonkey Engine

 Fixed missing creation of Normal node if geometry is a TriangleSet node

 Fixed problem of creating too many normals when creating a new Normal node

 Advanced jMonkey Engine java export example

 Fixed some problems in menupoint actions -> (IndexedFace)Set -> Optimize 

 Added display of normals, if Normal node is selected

 Fixed wrong/ugly normal creation

 Fixed bug in menupoint actions -> (IndexedFace)Set -> Optimize

 Fixed bug in writing of a PROTO declaration

 Fixed crash in parsing a senseless VRML file 

 Added convertion of a mesh based node to IndexedTriangleSet

 Fixed wrong rendering of normals of TriangleSet, if normalPerVertex is false

 Fixed wrong creation of Normal node for IndexedTriangleSet

 Fixed normal based bug in jme export example code

 Added batchscript to start white_dune from a USB stick

 Fixed missing write of EXTERNPROTOs commands for X3D nodes

 Added -prefix option for export to C/C++/java source to solve possible 
 namespace conflicts  

 Fixed wrong ccw field in X3D NurbsPatchSurface node

 Fixed missing X3D specific fields of NurbsPatchSurface and 
 NurbsPositionInterpolator nodes

 Solved crash when parsing a unknown node in a PROTO statement

 Added off screen rendering for commandline export converters which need 
 OpenGL commands

 Fixed error when parsing MFBool values

 Added batch script to semiautomatically create templates of Node classes 
 from the component documents of the X3D specification

 Added commandline arguments to export supported geometry to c/c++/java 
 sourcecode as triangulated IndexedTriangleSet meshes

 Solved wrong drag and drop handling of geometry nodes in SceneTree view

 Fixed crash in mesh creation of cylinder

 Fixed wrong conversion when importing X3D files into VRML files

 Added texture rendering with alpha channel to jME export example

 Fixed missing backupcommands for X3D NurbsCurve/NurbsPatchSurface nodes

 Added some extension nodes for the kambi gameengine

 Added TextureTransform support to jME export example

 Added +c/+c++/+java etc. commandline conversion options to allow the
 concatination of outputfiles

 Failed attempt to fix missing X3D names for TextureBackground texture fields

 Advanced compatibilty with #X3D V3.1 / #X3D V3.2 files 

 Fixed a bison compatibility problem

 Fixed wrong X3D texture name fix for Background texture fields

 Fixed X3D names for TextureBackground texture fields (again)

 Fixed crash caused by a missing glPopName() in PointLight node

 Changed scenegraph traversing order in C/C++/java export

 Added (some) support for the following nodes in jME export: Anchor, Color, 
 Collision, Switch

 Fixed bug in conversion of IndexedLineSet to PointSet

 Added workaround for crash when clicking to a PROTO instance

 Fixed problem when using DEF/USE on C/C++/java export

 Added Quadset and IndexedQuadset nodes

 Fixed crash when showing handles of selected tree  

 Fixed missing update of 3d preview after changing image via action -> object edit 

 Fixed crash in TextureTransform creation in 4Kids GUI

 Fixed wrong parsing of MFBool data

 Fixed draw of CADGeometry nodes

 Advanced compatibilty with TextureProperties node 

 Copied ForcePhysicsModel from GravityPhysicsModel cause the nodename is
 not total clear in the publiced X3D specification

 Copied NodeTexCoordDamper from NodeTexCoordDamper2D cause the nodename is
 not total clear in the publiced X3D specification

 Fixed problem with limited number of (abstract) node types
 
 Added commandline export to a SUN wonderland 0.5 module, 
 Textures are not supported yet

 Added missing axisRotation X3D fields to CylinderSensor and PlaneSensor

 Fixed wrong check for directory existence

 Fixed problems with jME/Wonderland export caused by null pointer exceptions

 Fixed portability problem

 Fixed problem with C++/java keywords in C/C++/java export

 Added disabled Text export to Wonderland, cause it would would 
 void the whole scenegraph. Text export to jME is still working.

 Added workaround for crash when using GLU_TESS_ERROR under M$Windows

 Fixed rendering problem in case of missing TextureCoordinates in 
 Wonderland module export

 Added documentation about the Wonderland module export

 Added menupoint for Wonderland module export

 Fixed crash cause of missing initialisation of solid field of SuperExtrusion
 node

 Fixed minor array read access problem in motif code

 Added picking component nodes

 Splitted X3D components toolbar into 2 parts

 Fixed wrong call convention in M$Windows port resulting in a crash in
 triangulation gluTess functions

 Fixed crash of empty CADFace node

 Fixed wrong node/classtype match

 Fixed crash when loading a Script node

 Fixed bug when handling URLs as filenames with unusual characters

 Added support for export of ImageTexture to SUN Wonderland module source
 
 Fixed missing X3D dependency of textureProperties fields

 Fixed problem with wrong definition of maximal lenght of path

 Changed interpretation of "something/filename" in URL.cpp from 
 "http://something/filename" to "./something/filename"

 Added X3D generator support for all X3D ISO/IEC 19775-1:2008 nodes,
 but a lot of nodes are not (completely) rendered

 Fixed wrong array access error when converting a meshbased node to a 
 IndexedFaceSet node

 Fixed problem with insert into MultiTextureCoordinate and MultiTextureTransform
 nodes

 Fixed problems with insert into a ParticleSystem node

 Fixed problem with insert of CADFace node

 Fixed crashes in wrong commandline parsing

 Added support for TextureCoordinate node in triangulation

 Fixed wrong TextureCoordinate in Box node

 Fixed wrong TextureCoordinate in Cone and Cylinder node

 Fixed possible crash in case of wrong texCoordIndex or TextureCoordinate 

 Fixed wrong texture top of Cylinder node

 Fixed wrong rendering of textured shapes without a Material node

 Added selection of multiple handles via a click on the middle mouse button

 Added handles for field SuperExtrusion.scale

 Fixed crash cause of wrong handle selection

 Fixed problem of wrong handle movement of field SuperExtrusion.a

 Fixed texture path problem when exporting to Wonderland java source

 Fixed yet another "code to long" java export problem

 Fixed missing conversion from ISO/IEC 14772:2002 to ISO/IEC 14772:1997
 when using -vrml97 commandline option 

 Added object/url editor usage for url based shader nodes

 Fixed double free problem in Sphere, Cone and IndexedFaceSet nodes.

 Added workaround for X server limits based crash on olpc 

 Fixed missing support for IMPORT/EXPORT commands

 Added "many classes" export option to java/wonderland export to fight
 against "too much constants" java compile problem

 Fixed bug in wonderland export without "many classes" export option

 Fixed problem with transparent textures in wonderland export

 Fixed wrong quoting of strings on field pipe

 Fixed wrong number of floats used with glMaterialfv() call in Background node

 Fixed crash in exporting triangulated java source code

 Fixed wrong handling of walk navigation after clicking to a object

 Fixed crash is wonderland export is used twice

 Fixed/unified missing errormessages on file -> export operations

 Fixed crash when connecting any type as ROUTE to Script node 

 Added limited support for Billboard node to wonderland export

 Added support for LOD node to wonderland export

 Fixed bug in writing wrong LOD fieldname to C/C++/java source export

 Cleaned up formating errors in java source export

 Added solution for "too much constants" java export compile problem

 Updated documentation about java/wonderland export

 Fixed wrong conversion of NURBS nodes from/to ISO 19776-2:2005 to/from 
 ISO 14772-1:2002

 Changed all NURBS creation dialogs to ask to number of controlpoints,
 not length of object

 Changed options dialogs to fit into 600 pixel high screens

 Rearranged X3D items in the "create" menu

 Fixed crash in triangulation code

 Added exerimental rendering for the NurbsSweptSurface node

 Fixed missing conversion to IndexedFaceSet based morphing on animation
 of NURBS/Supershape based shapes

 Fixed missing drawing of lines of CoordinateInterpolator data in channelview

 Added readme file to exported wonderland module source

 Added a way to add a preview image to a exported wonderland module

 Fixed missing load of EXTERNPROTOs (currently only from files)

 Fixed missing x symetry modelling of NurbSet node

 Fixed crash in handling of PROTO nodes

 Fixed missing Wonderland export of PROTO content 

 Fixed crash in selftest program

 Fixed wrong component level for NurbsTrimmedSurface node

 Fixed crash in C/C++/java export of a simple cyclic scenegraph

 Fixed bug in writing XML encoded X3D files from mainwindow

 Fixed parsing problem of some Geo nodes

 Fixed wrong mass generation of functions in java source export

 Fixed wrong behaviour of transform handles

 Fixed missing conversion of VRML TrimmedSurface node to 
 X3D NurbsTrimmedSurface node

 Fixed wrong display in scenegraph tree view after a copy/paste operation

 Fixed crash when using file->textedit after copy/paste

 Changed default creaseAngle of scripted SuperShape node

===============================================

Changed in white_dune Version 0.30pl1:

 Fixed missing conversion of X3D NurbsTrimmedSurface node to 
 VRML TrimmedSurface node 

 Fixed crash in NurbsTrimmedSurface/TrimmedSurface conversion

 Fixed wrong writing of MFNode in Script

 Updated some Kambi X3D gameengine extensions

 Added Kambi ProjectedTextureCoordinate extension node 
 
 Fixed "edit image" menupoint in -4kids gui

 Fixed error in commandline handling of input device axes

 Fixed crash in Scene Tree Window when displaying a simple cyclic scenegraph

 Fixed error in Wonderland export when writing multiple ImageTexture URLs 

 Fixed yacc grammar errors

 Fixed missing X3D conversion on C/C++/java export

===============================================

Changed in white_dune Version 0.30pl3:

 Fixed memory handling error in Box creation

 Added mesh/IndexedFaceSet based C/C++/java export

 Added Stefan Wolfs libC++RWD Library for C++ Rendering of White_dune Data

 Fixed bug in handling defaultShadowMap kambi extension field 

===============================================

Changed in white_dune Version 0.30pl4:

 Fixed crash in Create -> PROTO menupoint

 Fixed problem in X3DV writing of inputOutput elements of Script or Shader nodes

 Fixed problem in X3DV writing of VrmlScene/VrmlCut nodes

 Fixed problems in XML/X3D writing of Script nodes and PROTO

 Fixed problems with VrmlCut/VrmlScene nodes

Changed in white_dune Version 0.30pl5:

 Added menupoints and node buttons for ClipPlane

 Fixed viewpoint rotation in libC++RWD library

 Added C port of libC++RWD library (libCRWD)

 Fixed crash after export to C mesh
 
 Updated deprecated KambiNavigationInfo.headBobbing* fields

 Fixed error in C/C++/java export of SFImage data

 Added PixelTexture rendering to libC/C++RWD library

Changed in white_dune Version 0.30pl7:

 Fixed problem with transform mode radiobuttons under Micro$soft Windows

 Changed direction of z/zrot axis values of Micro$soft Windows joysticks

Changed in white_dune Version 0.30pl8:

 Fixed failed cygwin support

Changed in white_dune Version 0.30pl10:

 Solved failed conversion to X3DV when saving multiple NurbsSurface nodes

 Fixed wrong default of SuperRevolver.creaseAngle field

 Fixed rendering bug of SuperRevolver node with pieceOfCake flag set

 Fixed wrong flip and swap operations of SuperRevolver node

 Added german workshop documentation

Changed in white_dune Version 0.31beta:

 Fixed error with double addChildren/removeChildren eventIns in Group node

 Made x3d/xml parser to default

 Deleted x3d/xml to x3dv converter
 
 Added "follow mouse" icon

 Added inputdevice selection/settings dialogs

 Added animation support of mesh C source export

 Fixed problem in mesh creation of SuperEllipsoid 

 Removed deprecated fields from Kambi extensions
 
 Added new Kambi extensions LocalFog.volumetric* 

 Added animation support of mesh C++ source export

 Added workaround for failed floating point write

 Added bugfix for failed java source export

 Simplified inputdevice settings dialog

 Added Transform based animation support to triangulated java source export

 Changed NurbsSurface conversion to a triangulated IndexedFaceSet node 

 Fixed error when saving multiple input device settings

 Fixed crash when displaying Rectangle2D node

 Added missing Rectangle2D node to menu/icons

 Fixed error in ElevationGrid rendering

 Fixed crash when parsing a PROTO with a invalid type name

 Fixed crash when converting a superformula based node to NurbsSurface node

 Added translation of menus to french language

 Fixed problem about PROTO fields with different types under
 VRML97 Amendment 1 and X3D

 Added ColorInterpolator animation support to java source export example

 Added ScalarInterpolator animation support to java source export example
 
 Added multiple NurbsSurface handle selection with right mousebotton 

 Added support for "SF" type animations to Open Wonderland export

 Fixed problem with multiple Coordinate handle selection

 Changed SuperEllipsoid conversion to a triangulated IndexedFaceSet node

 Added "Go to TimeSensor" to 4kids menu

 Fixed handling problems of NurbsGroup 

 Fixed crash related to the clipboard

 Fixed some VRML/X3DV file save confusion

 Fixed some memory related problems

 Changed "flatten" menupoints to move to/move by factor menupoints

 Reorganised "4kids" menus (to fit into 600 pixel height screens)

 Solved crash in mesh conversion

 Fixed missing early exit on a failed fieldpipe in a branch command

 Added Script node support of c/c++/java source export

 Fixed missing field TextureBackground.transparency

 Fixed wrong node class of Viewport node

 Added man page documentation about ROUTE and Script support of 
 c/c++/java source export

 Added support for running ant deploy after exporting to Open Wonderland 
 java source export

 Changed X3D/VRML97 Amendment 1 handling of controlpoint data

 Fixed wrong classtype for GeoCoordinate node

 Fixed problems with Catt8 export under M$Windows

 Changed default for handle epsilon

 Added full screen icon to 4kids gui

 Fixed missing redraw of fieldview window after using colorcircle

 Added node create by name menupoint to 4kids gui
 
 Fixed problem with missing update when saving a running animation
 
 Fixed missing update after flip commands

 Fixed missing NurbsSurface animation

 Fixed missing rendering of Fog node

 Fixed crash in setting controlPoint field of NurbsSurface 

 Added example for some "MF" type animations to java export

 Fixed missing direction handling to 4kids Ring creation

 Fixed memory leaks in some primitive mesh creation routines

 Fixed bug caused by missing ccw of NurbsPatchSurface in the X3D ISO standard

 Fixed missing errormessage when trying to save to a readonly file

 Fixed bug in Transform handling of jME/wonderland (java) source export

 Fixed missing transparency handling in jME/wonderland (java) source export

 Simplified additional jME/wonderland code

 Fixed wrong "wheel" problem in device settings

 Fixed crash when removing temporary ROUTE commands

 Added automatic selection of "set_size" in the SuperShape animation dialog
 in 4kids GUI

 Added "paste symetric" menupoints to 4kids menu

 Added IndexedFaceSet based animation to java export (for IndexedFaceSets
 with less than 5 vertices for each polygon)

 Added update of Text and Switch node to java export

 Fixed wrong display of \" in Text.string under Micro$oft Windows

 Changed default "wheel" initialisation of device settings

 Fixed wrong storage of "wheel" information in .dunerc

 Avoided unnecessary writing mesh based node classes on mesh based source 
 exports

 Fixed unnecessary triangulation of IndexedFaceSet on usual mesh export

 Fixed rendering crashs of the C/C++ export OpenGL examples

 Fixed problem with covered lines after ROUTE view rebuild command

 Fixed memory errors

 Fixed error in Wonderland export of PointSet node

 Added DEF name beginning with "Animated" to a exported MeshMorphing node
 cause of a strange Cover/Covise rule

 Fixed missing update after changing Transform handles

 Fixed problem with "move center" icon 

 Added simple TouchSensor starting/stopping TimeSensor interaction to 
 Open Wonderland export

 Fixed bug in displaying inputdevice icons

 Solved problem with multiple Viewpoint on C/C++ source export

 Added better TouchSensor touch/release interaction to Open Wonderland export

 Fixed bug in disabling created animation

 Fixed NurbsSurface.controlPoint vanish bug

 Fixed another NurbsSurface.controlPoint vanish bug

 Added "Delete all animation data" menupoint

 Added "Goto former/next animation value" icons

 Fixed crash in "File -> Import" menupoint 

 Added Background and Fog support to VrmlCut node

 Fixed missing swap of Transform.scale

 Fixed default ci command to accept filenames with blanks

 Added simple KeySensor pressed key interaction to Open Wonderland export

 Splitted Preference settings dialog into modelling and render settings dialog

 Fixed problem in event distribution of exported java source

 Added missing KeySensor.shiftKey eventOut

 Added KeySensor and StringSensor interaction to Open Wonderland export

 Fixed java export error of ROUTEs to mesh based nodes

 Added workaround against crash in NurbsCurve/NurbsSurface EXTERNPROTO parsing

 Fixed wrong types and fields in NurbsTextureSurface

 Added small blue marks for nodes and events supported by the 
 Open Wonderland export

 Updated documentation about Open Wonderland export

 Added support for PositionInterpolator2D and CoordinteInterpolator2D to 
 Open Wonderland export

 Added fix for wrong handle movement on multihandle selection

 Added missing NurbsGroup/NurbsSet menu points 

 Added WonderlandImportJava extension node

 Fixed bug in NurbsSet/NurbsGroup conversion

 Fixed problem with missing checks of wonderland export paths

 Fixed problem of generation wrong variable names on C/C++/java export
 of IndexedFaceSet nodes

 Added 4kids animation menu to white_dune main GUI

 Added "search Interpolator" menupoint

 Added workaround for a problem related to EXTERNPROTO parsing

 Fixed crash in Wonderland export path check

 Added a button in the Wonderland export dialog to make it possible to 
 reduce the number of exported classes

 Fixed missing DEF name generation on Pure VRML97 export of NURBS
 based morphing 

 Fixed another java export problem of a IndexedFaceSet morphing animation

 Added menupoint for IndexedFaceSet optimization of scenegraph branch 

 Fixed wrong walk navigation problem in mouse navigation mode

 Fixed format string bugs

 Deleted wrong cyclic scenegraph warning

 Fixed unnecessary event distribution of PlaneSensor node

 OLPC font problem solved by waiting: unnecessay xft motif code for OLPC deleted

 Removed wrong creaseAngle field from *TriangleStripSet nodes

 Fixed wrong export of Color/ColorRGB nodes to Open Wonderland export

 Fixed crash after Background node selection

 Fixed harmless format error in foreign resource files
 
 Fixed wrong Wonderland exported PlaneSensor drag events for moved sliders

 Fixed missing delete of old nodes in scene tree view on conversion in
 USE nodes

 Fixed C/C++/java source export compile problem after a copy/paste operation

 Added incomplete support for Wonderland export of ProximitySensor  

 Fixed wrong writing to ROUTE statements to X3D/XML files

 Added better alignment of nodes in the route view window after animation
 creation

 Completed Wonderland export of ProximitySensor

 Choosen more general last fallback font

 Choosen bigger default font on OLPC

 Added Wonderland export of CylinderSensor

 Added "better than nothing" Wonderland export of SphereSensor

 Fixed crash in executing java source export of Inline node

 Fixed problem with source export of TimeSensor driven animations

 Fixed problem with Viewpoint rendering on OpenGL rendering of 
 C/C++ source export 
 
 Fixed crash when changing NURBS controlpoints via FieldView 

 Added fluid/fltk OpenGL rendering example for C++ source export

 Fixed wrong IndexedFaceSet.texCoord generation if texCoordIndex is empty

 Added support for X3D 3.2 XML strings

 Changed header version to X3D 3.2 when writing X3D/XML files

 Added "new X3D/XML" menupoint 

 Fixed non windows problem about accepting only image/movie/sound editors 
 with their path
 
 Fixed wrong X3D to VRML97 conversion of TrimmedNurbsSurface node

 Fixed wrong X3D to VRML97 conversion of VrmlScene/VrmlCut PROTO

 Fixed error in repeated symetric paste commands 

 Added symetric paste to all menus

 Fixed problems in missing conversion to pure VRML97 

 Fixed crash when using text editor settings command under m$windows

 Fixed problem about quoted text editor settings

 Fixed crash in save command (caused by a invalid PROTO)

 Added workaround against crash related to IndexedFaceSet optimize commands

 Fixed crash caused by closing one of multiple main windows
 
 Added -x3dom commandline argument for exporting to a X3DOM html file

 Fixed various output formatting errors

 Fixed endless loop when branch optimizing IndexedFaceSets of blender 
 VRML97 output

 Fixed crash in selection of a node in a VrmlCut scripted PROTO

 Added some missing kambi extension nodes to menu 

 Simplified creation of MultiTexture* nodes
 
 Fixed error in writing ROUTEs in x3d/xml and x3dom/html output

 Fixed error in wrong EXTERNPROTO generation when writing different fileformats

 Fixed upside down curve animation

 Fixed missing /index.html when creating file based help URL

 Fixed backward curve animation

 Fixed problem in creation of MultiTextureCoordinate node

 Fixed problems in Texture creation (caused by wrong handling of newer Kambi 
 extensions)

 Added workaround against a bug in FreeWRL about SuperEllipsoid/SuperShape 
 scripted rendering

 Fixed wrong writing of NULL fields in X3D/XML PROTO declarations

 Fixed wrong writing of USE fields in X3D/XML PROTO declarations

 Fixed wrong writing of double quotes in X3D/XML ROUTE statements

 Revoked wrong patch about writing of USE fields in X3D/XML PROTO declarations

 Fixed missing X3D/XML writing of inputOutput fields in script nodes

 Fixed wrong ElevationGrid.colorPerVertex FALSE rendering

 Updated list of special covise DEF-Names in DEF-Dialog

 Unfortunatly, it is not possible to implement NurbsCurveAnimation as a
 VRML97/X3D PROTO, therefore "create curve animation" functionality has been 
 moved form NurbsCurveAnimation node to the new CurveAnimation node

 Added backward compatibilty compile flag to convert the deprecated 
 NurbsCurveAnimation node to the new CurveAnimation node

 Fixed wrong jump of objects in 3D preview window when some keyboard
 keys are released

 Completed javascript implementation of CurveAnimation PROTO 
 (still to be done: new "hover" field)
 
 Fixed crash caused by wrong handling of IndexedTriangle*Set.colorPerVertex

 Fixed display of missing menu items in preview dialog

 Fixed bugs in javascript implementation of CurveAnimation PROTO 

 CurveAnimation API still in progress

 Added Arnaldo Bem Fateixas translation of portugenese 4kids menu

 Completed CurveAnimation

 Added colorcircle for Color nodes 

 Fixed crash in using colorcircle

 Added colorcircle support for ElevationGrid.color

 Added colorcircle support for *Triangle*Set.color

 Added colorcircle support for IndexedLineSet.color and PointSet.color 

 Fixed crash in displaying Geo* nodes
 
 Splitted InteractionDialog into 3 parts

 Removed 2 unnecessy friend statements
 
 Fixed crash in NurbsPositionInterpolator

 Fixed unnecessay warning about unloaded EXTERNPROTO

 Fixed wrong setting of number of floating point digits

 Fixed rounding error in floating point output

 Fixed error in XML writing of VrmlCut scripted node

 Fixed crash in XML parsing of SFNode NULL

 Fixed various errors in several VRML97 PROTOs/examples

 Fixed debian build to new naming convention

 Fixed rpm build to new naming convention

 Fixed "move to new PROTO" function

 Turned heart (into red 8-)

 Added better requirements for rpm package on fedora19

 Replaced bad default: bad gimp commandline with pinta commandline

===============================================

Changed in white_dune Version 0.99pl64:
 
 Added MetadataBoolean and volumerendering node for X3D 3.3, but none
 of the new nodes is rendered in the preview window.

===============================================

Changed in white_dune Version 0.99rc:

Fixed error in menu test script

Fixed wrong EXTERNPROTO writing of some X3D nodes

Fixed -ac3d commandline option

Added dialog for Script event name

Fixed C/C++ export of CurveAnimation node

Optimized C/C++ export

Added X3DOM to preview options menu

Added ImageTexture support to C/C++ export

Fixed rotation error in C/C++ export opengl examples

Fixed NormalInterpolator export on X3DOM export, X3DOM do not support them yet

Fixed missing OrientationInterpolator output if exporting to Pure VRML97 or 
X3DOM

Updated website URL to http://wdune.ourproject.org/

Fixed performance related error in relation to clearFlags()

Add support for MF/SF strings without double quotes in XML files

Fixed missing undo for Sphere, Cone and Cylinder

Fixed box to nurbs conversion

Added multiple undo/redo

Fixed crash when redoing animation

Added autoscaling of Transform handles

Fixed missing XML parse of containerfield

Added warning for incorrect number of arguments in PROTO/EXTERNPROTO statements

Fixed crash in PROTO event handling

Simplified PROTO handling

Added workaround for overwriding PROTO data

Added fix for false Inline/ImageTexture loading

Fixed wrong export of Super* PROTOs on X3DOM export

Fixed bug when a PROTO header uses "Field"

Added more fields to "show numbers" 4kids menu

Fixed jump of object on release of two mousebuttons 

Fixed missing mesh triangulation

Fixed toolbar updates

Avoided ShowFieldView preference writing in 4kids version

Fixed texture loading of a local file

Added jogl export example (without ImageTexture rendering)

Fixed wrong File->Import menu command

Fixed disabled z-only icon in windows version

Added jogl java export example

Added workaround for freeze of firefox on X3DOM preview

Fixed crash in changing SuperRevolver

Updated kambi extensions

Fixed XML parsing error

Fixed wrong XML writing

Fixed NurbsGroup X3D conversion

Fixed TrimmedSurface X3D conversion

Fixed rounding error in floating point writing

Fixed crash in free(3)

Added Switch node to OpenGL/jogl export rendering

Moved "Curve Animation" menupoint

Added Arch Linux packager script

Fixed channelview borders

Fixed wrong default about normal on mesh creation

Added missing lights to opengl/jogl export examples

Cleaned up deprecated xj3d Ode* nodes
 
Reduced toolbar/menu updates on field changes for no 4kids option

Added PROTO editing, still no way to change PROTO interface

Fixed MF* PROTO editing

Fixed missing conversion after preview to X3DV

Fixed missing parsing error for wrong IS eventIn/out

Added PROTO editing and documentation

Enabled remove of ExposedField in PROTO Dialog

Fixed crash in node converson inside a PROTO

Fixed missing inlined EXTERNPROTO loading

Added workaround for wrong rotation of Background

Fixed wrong transparency for RGBA and LUMINANCE_ALPHA mode textures

Fixed k&r formatting for PROTOs

Switched to default k&r formatting

Fixed some export to C/C++/java tests

Added rendering of TwoSidedMaterial node

Fixed rendering of ImageTexture when storing into a different directory

Added MultiTexture rendering for Linux

Added MultiTextureCoordinate rendering for Linux

Added PixelTexture to MultiTexture rendering for Linux

Fixed error in ImageTexture loading after File->Textedit

Added MultiTextureTransform rendering for Linux

Added ClipPlane rendering

Added ClipPlane handles

Added flip/swap to ClipPlane

Added workaround for overfloated FieldView

Fixed wrong default for Viewpoint.centerOfRotation

Added support of animation with TimeSensor in PROTO

Fixed crash of node move inside PROTO

Added workaround for missing redraws in FieldView

Added face selections for IndexedFaceSets

Fixed crash in IndexedFaceSet creation

Fixed missing write of X3D/XML comments

Fixed wrong draw of USEd Humanoid.skeleton nodes

Fixed bug in edit -> paste*

Fixed compatibility error of X3D V3.0 ContourPolyline2D.controlPoint

Fixed X3D header writings in case of version > 3.0

Fixed wrong field name RigidBodyCollection.collider 

Fixed containerField error in XML parser

Fixed modulo 0 crash in Map.h

Added (IndexedFace)Set->subdivide

Fixed Box conversion to IndexedFaceSet

Fixed (IndexedFace)Set->operations for Coordinate selection

Fixed edit->undo problem on scene->replaceNode()

Added subdivide to 4kids menu and documentation

Added (IndexedFace)Set->split selected Faces 

Added (IndexedFace)Set->extrude face 

Fixed selection of faces

Fixed movement of multiple selected faces

Added (IndexedFace)Set->extrude face

Fixed valgrind warning on X11 startup

Fixed deprecated X11 function warnings

Fixed FieldView scrolling error

Fixed some crashes in ScriptEditor

Added line handles for IndexedFaceSet

Added export/preview to Cobweb html

CurveAnimation changes with the ChannelView with the same TimeSensor

Simplified File->Import

Fixed missing combined X3D/Kambi fields in FieldView

Fixed wrong rendering of IndexedFaceSet without normals

Added "split quad face into pieces" to menu

Fixed "action->scale" by and "action->move to" menupoint for faces and lines

Fixed parsing of UNIT statement

Added trianguate/quad menu to toIndexedFaceSet

Fixed face selection for IndexedFaceSet.ccw false

Solved crash in SuperRevolver.nurbsTesselation 1

Fixed rpmlint warnings

Fixed handling of UNIT statement

Fixed ArcClose2D to be drawn as lines not faces

Fixed C/C++/java export examples

Fixed test run

Fixed cylinder rendering

Fixed \" to " conversion when storing to a XML/X3D file

Added right button face selection

Fixed ArcClose2D to be drawn as faces not lines

Fixed bug in MainWindow::OnFileSaveAs

Added convex hull using CGAL library

Fixed mesh smooth

Fixed colors of highlighted selection in fieldview window

Added documentation of convex hull

Fixed stack overflow in PointSet rendering

Added polygon reducer using Meshlab/VCGlib

Advanced cobweb html output

Fixed missing XML parsing of X3D only fieldnames

Fixed missing LOD.forceTransitions

Fixed PROTO parsing, when EXTERNPROTO interface differs from PROTO interface

Added workaround for XML writing of ContainerField

Added HAnimHumanoid skin and bones rendering

Fixed bug when writing XML/X3D file

Fixed bug when writing containerField on XML export

Fixed clicking to a HAnimSegment Shape

Fixed texture(Coordinate) rendering

Fixed nodeclass of GeoElevationGrid

Fixed unneccary EXTERNPROTO parsing

Fixed headlight for C/C++/java export rendering

Fixed PROTO writing for C/C++/java export

Fixed simple PROTO rendering

Fixed memory leak in HanimHumanoid::draw

Fixed error in PositionInterpolator handling in C export library

Added HanimHumanoid handling to C/C++ export library

Fixed missing degree elevate for CurveAnimation node

Added workaround for scroll error when clicking into a full scene tree

Fixed scene tree selection error when selecting show all field names

Fixed crash when rendering HAnimHumanoid

Fixed HAnimHumanoid normals rendering.

Fixed WIN32 FieldView colors

Fixed crash when scaling NO_HANDLE

Changed animation defaults from 10 seconds to 5 seconds

Solved crash in XML parsing

Fixed make symetric

Fixed another bug in make symetric

Fixed bugs in XML/X3D writing

Added boolean modelling from CGAL

Added downloads of URLs to ~/Downloads (default)

Fixed error in output settings dialog about x_ite path

Added File -> Open URL

Fixed Chaser and Damper nodes

Fixed X3DOM pure X3D export

Added X3DOM extensions

Fixed NurbsSet icon

Fixed Contour2D children creation

Fixed crash in NodeNurbsCurve2D

Fixed NurbsSweptSurface animation for X3D 3.0

Added NurbsSwungSurface rendering

Solved problem with missing swDeiconify in gnome by sacrificing redo after preview 8-(

Added NurbsPatchSurface weights == 1 setter

Added NurbsTrimmedSurface rendering

Fixed missung upate in NurbsCurve2D

Fixed bug in the XML parser

Added ContourPolyline2D handles

Added fix against a problem that causes firefox to freeze

Added update of selection after textedit and preview

Fixed update of selection after insert of Contour2D node

Added "keep closed" feature to NurbsCurve2D to avoid a crash in libGLU

Added x-symetric behavour to NurbsCurve2D handles

Added deleteSelected twice for NurbsCurve2D deletion

Changed desktop files to start with a bigger font and in x3dv mode

Changed default window size from 800x600 to 1024x768

Fixed Inline/EXTERN PROTO paths

Fixed endless loop in + operator of MyString

Fixed XML/X3D writing of EXTERN PROTO

Fixed double read of scenegraph after preview

Fixed crash in restoreTempPath() 

Fixed posix path for M$Windows X_ITE

Added rubberband for mouse dragging with right mouse button pressed

Fixed -demomode option

Fixed crash in misplaced gluScaleImage call

Fixed walk navigation

Fixed parsing error when using \ to continue strings

Added "x3domscript:" tag for X3DOM scripting

Added half automatic "x3domscript:" creation if in x3dom mode

Fixed X3D/XML parsing error

Fixed X3D/XML writing error

Added rebuild of Nurbs(Patch)Surface

Added range selection

Fixed error in MSWindows POSIX path loading

Added workaround for layout error on bigger fonts

Added possibility to hide vertex handles 

Improved CSG operations to accept some NURBS meshes

Fixed Color node editing

Added ColorRGBA node editing

Fixed crash in M$Windows color circle

Added ColorRGBA and Group to dune4kids menu

Added glider model tutorial

Added set turnpoint for selection (replacing stand up icon)

Added static export to RenderMan Interface Bytestream (RIB) file format

Added workaround for export to ac3d

Added limited export to Renderman Image Bytestream

Fixed crash when writing to Renderman Image Bytestream

Fixed error in texture URL handling of RIB export

Added more efficent handling of RIB export Texturer

Fixed errors in texture handling of RIB export

Fixed *Index problem of RIB export

Fixed ambientlight instead of distantlight of RIB export

Fixed colorPerVertex problem of RIB export

Fixed HAnimHumanid color problem of RIB export

Fixed error in VrmlCut

Added Font23D string rendering on UNIX/Linux

Fixed performance problem in MyMesh::smoothNormls()

Fixed problems in Inline and NodeImageTexture nodes

Fixed problems when converting a Font23D text to IndexedFaceSet

Fixed problem, when using -files and only one frame

Added RIB export for X3D lights

Fixed M$Windows File->Save

Added FontStyle.size support to Font23D text rendering

Fixed error in File->Textedit

Added workaround for problem when dragging z rotation value

Added FontStyle.justify/spacing support to Font23D text rendering

Fixed endless loop in Scene3DView.cpp

Fixed drawing of all lines

Fixed update after image editor 

Added inset face

Fixed one file RiB export

Added workaround on error on line handle chance

Fixed depth problem for face selection

Solved crash in ~MF*

Added workaround for CoordinteDouble

Fixed crash if m_statusBar == NULL

Updated some icons

Updated test script

Added workaround for OrthoViewpoint

Fixed error in Text c/c++/java export

Fixed errors in Color c/c++/java export

Fixed error in selection

Fixed error in rendering a incomplete HAnimHummanoid

Added red/cyan anaglyph rendering

Fixed error in SceneTreeView::UpdateNode

Added multiple right mouse buttom selection

Added human.x3d (made with makehuman) to documentation

Optimized HAnimHumanoid no weight handle display

Fixed HAnimHumanoid rendering if skin.Shape.IndexedFaceSet.coord is selected

Fixed X3D/XML component statement

Added improvements in the HAnimJoint.skinCoord* handling

Fixed SceneTreeView selection of HAnimHumanoid joint

Fixed SceneTreeView display of HAnimHumanoid

Fixed error in DEF/USE handling of XML parser

Fixed crash in recursive Node::receiveEvent

Added ViewPort node rendering

Fixed missing MovieTexture.description

Fixed strange loop in Scene::readExternProto resulting in recursive crash

Added LayerSet node rendering

Fixed wrong ExposedField IndexedFaceSet.ccw

Fixed error in Interpolator

Added Boolean/IntegerSequencer rendering

Fixed CSG operations

Fixed error in Interpolators

Fixed minor error in Interpolators

Added desktop start file to FreeBSD package

Added initial ImageTexture3D rendering

Added initial PixelTexture3D rendering

Fixed missing disable GL_LIGHTING in *Texture3D rendering

Stripped unneeded Imlib2 requirement

Fixed Devil source path

Fixed missing X3DOM extensions in VolumeData node

Cleaned some -Wall warnings

Fixed memory error in Interpolator.cpp

Fixed CurveAnimationPROTO to run with freewrl and instantplayer

Fixed Text line spacing

Added workaround for SceneTreeView scrolling error

Enabled Animation icon for NurbsCurve and NurbsSurface in X3D mode

Fixed showField for NurbsCurve

Added workaround for CurveAnimation ring problem

Fixed getNodeClass() of LayoutLayer

Added workaround for Viewport rendering

Fixed -files option for RIB file creation

Added questionable Layout, LayoutLayer and LayOutGroup rendring

Added questionable ScreenFontStyle and ScreenGroup rendring

Removed outdated node GravityPhysicsModel

Added ParticleSystem, ConeEmitter, PointEmitter and ExplosionEmitter rendering

Added ForcePhysicsModel and WindPhysicsModel rendering

Added ParticleSystem to C++ export

Fixed bug in update of toolbars

Removed wrong NURBS rendering from C export main program

Added ParticleSystem to C and java export

Simplified jogl_example export Makefile

Fixed bug in NurbsSurface/NurbsCurve to X3D conversion

Fixed selection error when creating a HAnimHumanoid node 

Fixed ColorRGA problem in C/C++ export libraries

Fixed Normal node data creation of HAnimHumanoid C/C++ export

Fixed minor formatting error when writing PROTO

Changed desktop icons from ,png to .bmp in packagers

Added human animation to C++ export example

Modified java export

Fixed CurveAnimation java export

Fixed crash when clicking to a PROTO

Fixed crash is scenegraph branch commands

Fixed error in HAnimJoint creation

Fixed error in HAnimHumanoid java rendering

Added workaround for crash when creating a GeoViewpoint

Updated test directory

Added Geospatial rendering

Added double types to Script dialog

Fixed GeoCoordinate problem for *Triangle*Set nodes

Added FogCoord rendering

Fixed FontStyle.leftToRight rendering

Fixed DEVIL related errors

Added MovieTexture rendering

Fixed FontStyle.horizontal

Added Followers component rendering

Added PolylineEmitter rendering

Fixed missing save of NurbsCurve.coordinate in X3D mode

Fixed crash in MyMesh

Added automatic knot and weight generation on NurbsSurface/NurbsCurve

Added VolumeEmitter rendering

Fixed NurbsCurve to CurveAnimation conversion

Added workaround for wrong selection in "show all field names" mode 

Added SurfaceEmitter rendering

Fixed crash in C export rendering code

Fixed error in java export Particlesystem rendering

Updated Emitter code of C/C++/java export 

Fixed HAnimHumanoid rendering

Fixed error in constraint handle settings

Fixed deadlock situation after Selection->Range

Fixed automatic knot generation

Added make X symetric for NurbsSurface

Added make Y symetric for NurbsSurface

Added NURBS extrusion

Fixed missing HAnimHumanoid.skinCoord in SceneTreeView 

Updated usage_docs directory

Updated dune4kids menus

Added NURBS neighbour selection

Added NurbsTextureCoordinate rendering

Fixed scroll error in SceneTree

Fixed NurbsTextureSurface/Coodinate nodes

Fixed missing undo operation for NURBS extrusion

Fixed double move when moving handles

Added "split HAnimJoint into 2 joints" to HAnimJointDialog

Added animated 4 legged animal tutorial

Added texture rendering to HAnimHumanoid node

Fixed missing HAnim 1 rendering

Fixed splitted morphing rendering

Fixed multiple parent update problem

Fixed -Wall warnings if possible

Fixed missing nodeClass of *CubeMapTexture

Fixed crash in getParent()->getParent() of Image/PixelTexture

Added interactive TouchSensor.touchTime support on C/C++/java export

Fixed Viewpoint handling in java/jogl export

Added interactive PlaneSensor.translation_changed support on C++/C/java export

Added interactive CylinderSensor.rotation_changed support on C++/C/java export

Added interactive SphereSensor.rotation_changed support on C++/C/java export

Added interactive ProximitySensor.position_changed support on C++/C/java export

Added interactive ProximitySensor.rotation_changed support on C++/C/java export

Fixed layout error in script dialog

Added interactive TouchSensor.isActive support on C++/C/java export

Fixed switch.choice -1 crash in java export

Fixed export bug, when 2 or more ScriptNodes are used

Added interactive TouchSensor.isOver support on C++/C/java export

Fixed endless loop in C write of ROUTE loop

Added Tux warbird WRL game and added C++ scripting

Added interactive TouchSensor.hitPoint_changed support on C/C++/java export

Added interactive *Sensor.trackPoint_changed support on C/C++/java export

Added interactive *Sensor.isOver support on C/C++/java export

Fixed missing translateField() problem of NavigationInfo node

Added interactive Collision support on C/C++/java export

Added limited NavigationInfo support on C/C++/java export

Fixed "close icon" problem on java export

Fixed crash when moving handles

Fixed "Actions -> set HAnimJoint weight" to add handles to existing node

Fixed bugs in test directory

Fixed crash in C/C++ export

Added QT C++ export example

Fixed bug in dragging multiple "x == 0" handles in Xsymetric mode

Optimized handle selection

Added timer based WALK navigation on C/C++/java export

Added (Indexed)LineSet rendering and PointSet rendering on C/C++/java export

Fixed bug in LineSet rendering

Fixed Normal creation on IndexedFaceSet export to C/C++/java

Fixed HAnimHumanoid animation rendering on export to C/C++/java

Added delete of whole MF-field

Added edit->copy/paste in FieldView and ChannelView

Fixed missing recording keys when changing NurbsPatchSurface handles

Fixed rendering error of NurbsTrimmedSurface

Fixed error when animating NurbsTrimmedSurface

Added edit->symetric paste in FieldView and ChannelView

Fixed by one error of paste in ChannelView

Addeded OpenSubdiv call for better subdivision

Added slider to "inset" dialog

Fixed missing normal/texCoord delete on inset  

Added better selection change after undo/redo and field pipe

Fixed crash if field pipe remove node

Added exit of whitedune starter on MacOSX

Fixed bugs in man page

Disabled camera orientation of new node on convex hull and CSG operations.

Added collection of vertices to convex hull creation

Added x-symetric collection of vertices to convex hull creation

Added branch csg union 

Fixed crash in remove invalid child nodes

Added workaround for crash in Path.cpp

Added fix for missing NurbsSurface animation in pure VRML97 export

Added missing NurbsCurve animation in pure VRML97 export

Fixed missing update of colorcircle icon after color selection

Changed default tesselation

Added vertex modifier

Added normal/direction choice to vertex modifier 

Added documentation for vertex modifier

Fixed unnecessary getLang() calls

Fixed undo/redo for VertexModifier changes

Added workaround for crash in undo/redo and following VertexModifier change

Added workaround for normal/texCoord creation on IndexedTriangleSet conversion

Fixed wrong writing of VRML97 amendment 1 NurbsSurface node

Changed int to size_t/long in Array.h

Added Text3D (as IndexedFaceSet) to 4kids menu

Fixed ColorCircle when ColorRGBA is selected

Fixed crash caused by sign bug in Array.h

Fixed misssing update of make symetric -> y/z menupoint 

Fixed bug in Node::doWithBranch()

Fixed bug in branch CSG operation

Changed configure to require libpng, libjpeg and zib

Solved a lot of warnings if compiled with g++ -Wall

Fixed bug in german "Datei -> URL oeffnen" menupoint

Solved some of warnings if compiled with g++ -Wall -D_FORTIFY_SOURCE=2

g++ compiles with -Wall -Werror if -Wno- flags are used (caused by e.g. vcglib)

Fixed C/C++/java export bug caused by solved warnings

Fixed bug in MyArray::extractData

Fixed unsigned/signed bug in MovieTexture load

Removed X_ITE from preview settings cause of webbrowser sandboxing problems

Fixed by one error in preview settings combo box

Reenabled "if (this == NULL)" code parts 

Fixed crash cause of false Proto::deleteElements call

Fixed rendering bug of SuperRevolver node

Added packager snap for snap package creation

Added access to OpenSuvdiv in /usr/local to snap package

Added conversion from ImageTexture to PixelTexture for X3DOM preview

Fixed bug in conversion from ImageTexture to PixelTexture

Fixed missing containerField bug

Fixed error of unnecessary warnings in EXTERN PROTO warnings

Added better errormessage for failed downloads

Added selection of non remote browser if X3DOM is selected and PROTOs are used

(X3DOM do not support PROTOs) 

Added configure option for raspbian

Advanced M$Windows preview settings dialog

Fixed portuguese (-4kids) language settings

Added PROTO expansion for X3DOM preview

Added Inline expansion for X3DOM preview

Added reading/writing of a XML well formed X3DOM html file

Fixed bug of loosing X3DOM HTML data during preview

Added mouse wheel support for M$Windows version

Changed shape of 4kids "spindle" object

Fixed missing update of exposedField in routing

Fixed crash in C/C++/java export

Fixed crash in NodeImageTexture::bind

Fixed another crash in NodeImageTexture::bind

Fixed bugs in C/C++/java export

Advanced formatting of C/C++/java export

Avoided unneccesary "save ?" dialog

Fixed animation completely inside a PROTO

Fixed animation of a RROTO noode eventOut

Fixed C/C++/java export of animation completely inside a PROTO

Fixed lost editing of PROTOs

Fixed PROTO rendering

Avoided problem of PROTO editing for animation completely inside a PROTO

Added object edit for nodes with MFString field (e.g. NodeText) 

Fixed text editor selection in configure (new default "gedit")

Updated german "Schiff" documentation

Fixed ugly object flipping bug on mouse Transform move 

Added branch set of bounding box for Transform, Group and StaticGroup

Added workaround for X3DOMs missing support of StaticGroup

Fixed lost pure VRML97 export

Fixed EventIn/EventOut PROTO problem

Fixed File->Save problem of X3D files with downloads.

Contact: MUFTI <mufti11@web.de>

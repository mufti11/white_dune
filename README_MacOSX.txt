This file is free software; the Free Software Foundation    
gives unlimited permission to copy, distribute and modify it.

To use white_dune under MacOSX, you need to install "X11 for Mac OS X" 

Since version 10.4 of MacOSX "X11 for Mac OSX X" is installed by default.
On version 10.3 ("Panther") of MacOSX a "X11 for Mac OSX X" 
package is delivered somewhere on the installation CDs. 
For earlier versions it was available from
    http://www.apple.com/macosx/x11/
If anything else fails, you can use the opensource Xfree86/Xorg port for 
MacOSX, but unlike the native X11 versions, it do not 3D acceleration.
For versions since 10.8 you need to install xquartz.org from
https://www.xquartz.org/.

On MacOSX, you can run dune from the commandline like on a traditional UNIX
system, but direct usage requires to start a X11 server and set the DISPLAY
environment variable. The commandline argument "-startX11aqua" tries to
start a X11 server and set the DISPLAY environment variable.
Of course "-startX11aqua" may not be used, if dune is started remotely from a 
traditional LINUX/UNIX X11 workstation in the usual way.

The MacOSX binary archive comes with a icon of white_dune for the AQUA desktop.
The icon program internally use the "-startX11aqua" commandline argument.


To build white_dune from scratch, you need to install the MacOSX developer CD
including the X11 SDK. Additional you need to install either motif or lesstif 
or openmotif. It is recommended to install the libraries libpng, jpeg, zlib,
glut, CGAL, vcglib and libcurl too. When compiling libraries, you should 
remember, that some versions of MacOSX are one of the now rare operation 
systems, that need the "ranlib" command after the "ar" command to create 
libraries.

Use the script packager/macosx/mksit.sh to build a archive of the 
complete program including the icon program.

The icon program can be found in the archive desktop/macosx/white_dune.app.tar

If you want to pass commandline arguments to this icon, edit the file
 
desktop/macosx/white_dune.app/Contents/MacOS/dunestarter

and set the enviroment variable DUNEOPTIONS

A example what commandline arguments can be passed to white_dune on MacOSX
can be found under docs/commandline_examples/wingmanrumblepad_macosx.sh

If you want to recompile the icon program with xcode, you need to unpack
the sources desktop/macosx/xcode.tar
With the default xcode settings, you must execute the script 
batch/repair_apple_bugs.sh and copy the files MyDocument.ob and main.ob from 
the xcode "location for intermediate build files" to 
desktop/macosx/white_dune.app/Contents/MacOS
Otherwise the icon program would contain a absolute path to this object files 
and the resulting archive would not be distributable.

Use 

strings desktop/macosx/white_dune.app/Contents/MacOS/white_dune

to check for the path to the object files

There is also a rather difficult to find way in xcode, to avoid the creation 
of absolute paths in the binaries.

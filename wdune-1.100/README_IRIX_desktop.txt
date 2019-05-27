This file is free software; the Free Software Foundation    
gives unlimited permission to copy, distribute and modify it.

On SGI IRIX, you can install the icons of white_dune on the desktop 
and define a VRML filetype, that cooperates with SGI VRML tools like
cosmoworlds/cosmoplayer, Open Source tools like FreeWRL, lookat or
XJ3D and tools like covise/cover or your favorite texteditor ($WINEDITOR).

If you use the packager/irix/mkpg.sh script that creates a irix installation 
package, such icons will be created and are available during the normal
installation via inst or swmgr.

If you want to pass arguments to this icons, you have to cd into the
right directory in the directory desktop/irix

- for global installation of icons on IRIX 6.2 (require root rights):

   $ cd desktop/irix/irix6.2_global

- for personal installation of icons on IRIX 6.5:

   $ cd desktop/irix/irix6.5_personal

- for global installation of icons on IRIX 6.5 (require root rights)

   $ cd desktop/irix/irix6.5_global

Then edit the file "duneoptions.m4". You can change the commandlinearguments 
(DUNEOPTIONS) and the path of the binary (DUNEPATH).

Example (to use /usr/freeware/bin/dune with the options using a spaceball):


# insert your options to start dune here
define(DUNEOPTIONS,`-xinput spaceball -allxyz=20,200 -allrot=40,400 $*')

# insert the installpath for dune here
define(DUNEPATH,/usr/freeware/bin)


after editing, type 

   $ make

The new icons will be available in a new started filemanager. Usually,
icons on the desktop background are controlled by a filemanager started
with the option "-b". 

If you do not want to logout and login new to update the desktop, 
restart the filemanager controlling the desktop, search for process 
"fm" with option "-b" in the process list and kill/restart this process.


According to directions from R. Underwood (SGI), the following tags 
(see "man tag" under IRIX) are assigned to the white_dune project:

dune            0x00ff1a00
nostereodune    0x00ff1a01
fm_cosmo2vrml97 0x00ff1a02

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

# FTR File, only for IRIX 6.2 

TYPE X3d
   LEGEND X3D XML File
   SETVAR "DefaultExtension"      xml
   MATCH glob("*.xml");

TYPE VRML97
   LEGEND VRML97 Data file for white_dune, cosmoplayer and other VRML tools
   SETVAR "DefaultExtension"      wrl
# match #VRML V2.0 [Uu][Tt][Ff]8[\n\r] 
   MATCH (string(0,11)=="#VRML V2.0 ") && ((uchar(11)==85)||(uchar(11)==117)) && ((uchar(12)==84)||(uchar(12)==116)) && ((uchar(13)==70)||(uchar(13)==102)) && uchar(14)==56 && ((uchar(15)==10)||(uchar(15)==13));    
   SUPERTYPE Vrml2.0 Vrml2.0File Vrml2.0DataFile X3d
   CMD OPEN DUNEPATH/dune $LEADER DUNEOPTIONS $REST
   CMD ALTOPEN $WINEDITOR $LEADER $REST
   MENUCMD "Netscape/Cosmoplayer" netscape $LEADER
   MENUCMD :458:"Edit" $WINEDITOR $LEADER $REST
   MENUCMD "cosmoworlds" cosmoworlds $LEADER 
   MENUCMD "FreeWRL" freewrl $LEADER 
   MENUCMD "lookat" lookat $LEADER
   MENUCMD "xj3d" xj3d $LEADER
   MENUCMD "cover" cover $LEADER
   MENUCMD "nostereodune" DUNEPATH/nostereodune $LEADER DUNEOPTIONS $REST
   ICON  {
             if (opened) {
                 include("iconlib/Dunebackground.fti");
                 include("iconlib/Dune.open.fti");
             } else {
                 include("iconlib/Dunebackground.fti");
                 include("iconlib/Dune.fti");
             }
         }

TYPE white_dune_vrml_editor
   LEGEND white_dune executable (stereoscopic view enabled on a stereo visual)
   MATCH tag==0x00ff1a00;
   SUPERTYPE Executable
   DROPIF VRML97 Vrml2.0 Vrml2.0File Vrml2.0DataFile X3d
   CMD OPEN $LEADER DUNEOPTIONS $REST
   CMD DROP $TARGET DUNEOPTIONS  $SELECTED
   ICON  {
             if (opened) {
                 include("iconlib/Dunebackground.fti");
                 include("iconlib/Dune.open.fti");
             } else {
                 include("iconlib/Dunebackground.fti");
                 include("iconlib/Dune.fti");
             }
         }

TYPE white_dune_4_kids_vrml_editor
   LEGEND white_dune for kids executable (stereoscopic view enabled on a stereo visual)
   MATCH tag==0x00ff1a03;
   SUPERTYPE Executable
   DROPIF VRML97 Vrml2.0 Vrml2.0File Vrml2.0DataFile X3d
   CMD OPEN $LEADER DUNEOPTIONS $REST
   CMD DROP $TARGET DUNEOPTIONS  $SELECTED
   ICON  {
             if (opened) {
                 include("iconlib/Dunebackground.fti");
                 include("iconlib/Dune4kids.open.fti");
             } else {
                 include("iconlib/Dunebackground.fti");
                 include("iconlib/Dune4kids.fti");
             }
         }

TYPE white_dune_vrml_editor_no_stereo
   LEGEND script to start white_dune with stereoscopic view disabled
   MATCH tag==0x00ff1a01;
   SUPERTYPE Script
   DROPIF VRML97 Vrml2.0 Vrml2.0File Vrml2.0DataFile
   CMD OPEN $LEADER DUNEOPTIONS $REST
   CMD DROP $TARGET DUNEOPTIONS $SELECTED
   ICON  {
             if (opened) {
                 include("iconlib/Dunebackground.fti");
                 include("iconlib/nostereoDune.open.fti");
             } else {
                 include("iconlib/Dunebackground.fti");
                 include("iconlib/nostereoDune.fti");
             }
         }

TYPE fm_cosmo2vrml97
   LEGEND script to convert cosmoworldsfiles to $HOME/.cosmo2vrml97 and open a filemanager
   MATCH tag==0x00ff1a02;
   SUPERTYPE Script
   DROPIF CosmoWorlds1.0Data
   CMD OPEN $LEADER $REST
   CMD DROP $TARGET $SELECTED
   ICON  {
             if (opened) {
                 include("iconlib/Dune.open.fti");
                 include("/usr/lib/filetype/iconlib/generic.script.fti");
                 include("/usr/lib/filetype/iconlib/generic.exec.open.fti");
                 include("/usr/lib/filetype/install/iconlib/cosmoworlds.open.fti");
             } else {
                  include("/usr/lib/filetype/install/iconlib/cosmoworlds.closed.fti");
                  include("/usr/lib/filetype/iconlib/generic.script.fti");
                 include("iconlib/Dune.fti");
             }
         }

# senseless, conversion only works for printing 8-(
#
#CONVERT CosmoWorlds1.0Data white_dune_vrml_editor
## Cost per fileconvert(1): 300 (Noticable data loss)
#        COST 300
#	FILTER /usr/freeware/bin/cosmo2vrml97filter

#CONVERT CosmoWorlds1.0Data white_dune_vrml_editor_no_stereo
## Cost per fileconvert(1): 300 (Noticable data loss)
#        COST 300
#	FILTER /usr/freeware/bin/cosmo2vrml97filter



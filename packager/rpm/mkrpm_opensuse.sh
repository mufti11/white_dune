#!/bin/sh
#set -x

export DUNEMAKEFLAGS=-j4
export CPPFLAGS=-std=gnu++11

RPM_SRC_LIST="/usr/src/rpm /usr/src/RPM /usr/src/redhat /usr/src/packages"
RPM_SRC_LIST="$RPM_SRC_LIST $HOME/rpmbuild"

unset MKRPM_SRC
for i in $RPM_SRC_LIST ; do 
   if test -d $i/SOURCES ; then
      MKRPM_SRC=$i/SOURCES
   fi
done
if test "$MKRPM_SRC" = "" ; then
   echo -n non of typical rpm source directories 1>&2
   for i in $RPM_SRC_LIST ; do 
       echo -n " " $i/SOURCES 1>&2 
   done
   echo " " found 1>&2 
   echo extend variable RPM_SRC_LIST in $0 for your system 1>&2
   exit 1
fi

VERSION=wdune-`sh ../../batch/getversion.sh`
VERSION2=`echo $VERSION | awk '{gsub("wdune-","",$0);print $0}'`
VERSION3=white_dune-$VERSION2

OPTIMIZE="--with-optimization"
RPM_OPT_FLAGS='$RPM_OPT_FLAGS'

RPM_BUILD_ROOT='$RPM_BUILD_ROOT'

cat > /tmp/$VERSION3.spec << EOT 
Summary: A graphical X3D/VRML97 editor, simple 3D modeler and animation tool
Name: white_dune
Version: $VERSION2
Release: 1
#Copyright: GPL
License: GPL-3.0+
Group: Applications/Multimedia
URL: http://wdune.ourproject.org/
Source: ftp://ftp.ourproject.org/pub/wdune/$VERSION3.tar.bz2
BuildRoot: /var/tmp/%{name}buildroot
BuildRequires: gcc-c++, gawk, make, bash, fileutils, motif-devel, libXi-devel, libXmu-devel, libpng-devel, libjpeg-devel, glu-devel, libexpat-devel, flex, bison, libusb-devel, update-desktop-files, fdupes, libcgal-devel, libcurl-devel, ImageMagick, freetype2-devel, bitstream-vera-fonts, libavcodec-devel libavutil-devel libavformat-devel libavresample-devel libswscale-devel libswresample-devel
Requires: rcs, firefox, gedit, gimp, kolourpaint, audacity, lxterminal, ImageMagick, bitstream-vera-fonts, xorg-x11-fonts-core

%description
white_dune is a graphical X3D/VRML97 editor, 3D modeler and animation tool.
It is a simple extrusion/NURBS/Superformula 3D modeler.
With white_dune you can create/change 3D objects and animate them (in a easy 
way if you choose the -4kids GUI).The result can be shown in any webgl enabled 
web browser or can be converted to the RIB format for movie creation. 
X3D and VRML97 are the ISO standard for displaying 3D data over the web.
With Cobweb or X3DOM it can displayed in any webgl enabled web browser. 
This includes animation, real-time interaction and multimedia (image, movie, 
sound). VRML97 can be written by popular programs like maya, catia, 
3D Studio MAX, cinema4D, blender, wings3d and others, but (unlike white_dune) 
most of this programs support only a small part of the VRML97 features.
Dune can read X3DV/VRML97 files, display and let the user change the 
scene-graph and all fields of all nodes. 
Some documentation how to use dune is included.
The most powerful 3D modeling features of white_dune support the 
VRML97 Amendment1 style NURBS nodes and Superformula based PROTOs. 
This is mainly convenient for building some rounded shapes.
For artistic mesh modeling work, the usage of a static 3D modeler with 
VRML97 export features may be useful. The result can be either used unchanged
in a bigger VRML97 file via the Inline command without breaking the 
tool-chain or improved in white_dune.
Examples for free (speech) static 3D modelers with VRML97 export available 
under Linux are blender, wings3d, art of illusion or sharp construct.
Under Linux, white_dune support some 3D input-devices like joysticks, game-pads
or all devices supported via the Xinput protocol.
White_dune support quad-buffer stereo visuals. Under Linux, this can be used
with Elsa Revelator or Crystal eyes shutter-glasses and special drivers for 
expensive graphic-cards like Nvidia Quadro or ATI FireGL 4.
This version of white_dune support both English and German menus and messages.
Information how to translate white_dune to foreign languages can be found under
docs/developer_docs/dune_developer.html#localisation
Partitial translations of menus exist for French, Italian and Portuguese.
%prep
%setup -q



%build
./configure $OPTIMIZE --without-devil --with-vrmlbrowser=firefox --with-helpurl="/usr/share/doc/packages/white_dune/docs/"  --with-protobaseurl="/usr/share/doc/packages/white_dune/docs" --with-checkincommand="ci" --with-x11-editor="gedit" --with-imageeditor="gimp" --with-imageeditor4kids="kolourpaint" --with-soundeditor=audacity
CXXFLAGS="$CXXFLAGS -Wno-ignored-attributes -Wnonnull-compare -Wmaybe-uninitialized"
make -j1 RPM_OPT_FLAGS="$RPM_OPT_FLAGS" 

%if 0%{?suse_version} < 1330
%post
%desktop_database_post
%endif

%if 0%{?suse_version} < 1330
%postun
%desktop_database_postun
%endif

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/share/man/man1
mkdir -p $RPM_BUILD_ROOT/usr/share/misc/white_dune
mkdir -p $RPM_BUILD_ROOT/usr/share/applications
mkdir -p $RPM_BUILD_ROOT/usr/share/pixmaps/
mkdir -p $RPM_BUILD_ROOT/usr/src/debug/$VERSION3
mkdir -p $RPM_BUILD_ROOT/usr/lib/debug/usr/bin/

install -m 755 bin/dune $RPM_BUILD_ROOT/usr/bin/dune
install -m 755 bin/dune4kids $RPM_BUILD_ROOT/usr/bin/dune4kids
install -m 755 bin/illegal2vrml $RPM_BUILD_ROOT/usr/bin/illegal2vrml
install -m 755 tools/run_dune_and_aqsis.sh $RPM_BUILD_ROOT/usr/bin/run_dune_and_aqsis.sh
install -m 644 include/white_dune/libC++RWD_namespace.h $RPM_BUILD_ROOT/usr/include/white_dune/libC++RWD_namespace.h
install -m 644 include/white_dune/libC++RWD.h $RPM_BUILD_ROOT/usr/include/white_dune/libC++RWD.h
install -m 644 include/white_dune/libCRWD.h $RPM_BUILD_ROOT/usr/include/white_dune/libCRWD.h
install -m 644 desktop/kde/dune.desktop $RPM_BUILD_ROOT/usr/share/applications/dune.desktop
install -m 644 desktop/kde/dune.png $RPM_BUILD_ROOT/usr/share/pixmaps/dune.png
install -m 644 desktop/kde/dune4kids.desktop $RPM_BUILD_ROOT/usr/share/applications/dune4kids.desktop
install -m 644 desktop/kde/dune4kids.png $RPM_BUILD_ROOT/usr/share/pixmaps/dune4kids.png
install -m 644 man/dune.1 $RPM_BUILD_ROOT/usr/share/man/man1/dune.1
install -m 644 man/dune4kids.1 $RPM_BUILD_ROOT/usr/share/man/man1/dune4kids.1
install -m 644 man/illegal2vrml.1 $RPM_BUILD_ROOT/usr/share/man/man1/illegal2vrml.1

/usr/lib/rpm/find-debuginfo.sh

%fdupes
%changelog
* Wed Sep 20 2017 <mufti11@web.de> 0.99pl765
-- Initial packaging
%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README.txt COPYING docs

/usr/bin/dune
/usr/bin/dune4kids
/usr/bin/run_dune_and_aqsis.sh
/usr/bin/illegal2vrml
/usr/include/white_dune/libC++RWD.h
/usr/include/white_dune/libCRWD.h
/usr/include/white_dune/libC++RWD_namespace.h
/usr/share/applications/dune.desktop
/usr/share/pixmaps/dune.png
/usr/share/applications/dune4kids.desktop
/usr/share/pixmaps/dune4kids.png
/usr/share/man/man1/dune.1*
/usr/share/man/man1/dune4kids.1*
/usr/share/man/man1/illegal2vrml.1*
/usr/src/debug/$VERSION3
/usr/lib/debug/usr/bin/dune.debug
EOT

(
   cd ../../.. && 
   rm -rf /tmp/$VERSION3 && \
   cp -r $VERSION /tmp/$VERSION3 && \
   cd /tmp/$VERSION3 && 
   sh -x batch/fix_not_translated_rcfiles.sh &&
   make realclean && rm -rf desktop/macosx desktop/irix
) && \
(cd /tmp && tar -cpf - $VERSION3) | \
bzip2 -c > $MKRPM_SRC/$VERSION3.tar.bz2

if rpm -ba /tmp/$VERSION3.spec ; then
   echo > /dev/null
else
   rpmbuild -ba /tmp/$VERSION3.spec
fi
#rm -f /tmp/$VERSION3.spec


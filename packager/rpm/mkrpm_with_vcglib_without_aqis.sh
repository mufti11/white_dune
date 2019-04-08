#!/bin/sh
#set -x

export DUNEMAKEFLAGS=-j8

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

UNAME_A=`uname -a`
UNAME_PS2="Linux $USER 2.2.1 #1 Wed Nov 14 18:28:00 JST 2001 mips unknown"

VERSION1=wdune-`sh ../../batch/getversion.sh`
VERSION2=`echo $VERSION1 | awk '{gsub("wdune-","",$0);print $0}'`
REV=pl
if test "X_"`echo $VERSION1 | grep rc` != "X_" ; then
    REV=rc
fi
export REV
VERSION3=`awk -v x="$VERSION2" -v rev="$REV" 'BEGIN {split(x, a, rev);print a[1]}'`
VERSION4=`awk -v x="$VERSION2" -v rev="$REV" 'BEGIN {split(x, a, rev);print a[2]}'`
. /etc/os-release
VERSION5=wdune-$VERSION3
VERSION6=$VERSION3
VERSION=wdune$VERSION2

(
   cd ../../.. && 
   rm -rf /tmp/$VERSION && \
   cp -r $VERSION1 /tmp/ && \
   cd /tmp/$VERSION1 && 
   sh -x batch/fix_not_translated_rcfiles.sh &&
   make realclean && rm -rf desktop/macosx desktop/irix
) && \
(cd /tmp && tar -cpf - $VERSION1) | \
bzip2 -c > $MKRPM_SRC/$VERSION1.tar.bz2

if test "$UNAME_A"="$UNAME_PS2"; then
   # disable optimization on Playstation 2 cause of compiler bugs
   OPTIMIZE="--with-optimization"
   RPM_OPT_FLAGS=""
else 
   OPTIMIZE="--with-optimization"
   RPM_OPT_FLAGS='$RPM_OPT_FLAGS -O2 -g'
fi

RPM_BUILD_ROOT='$RPM_BUILD_ROOT'
REVISION="$REV""$VERSION4"
cat > /tmp/wdune.spec << EOT 
Summary: A graphical X3D/VRML97 editor, simple 3D modeler and animation tool
Name: wdune
%global extraver $REVISION
%global upstream_version %{version}%{?extraver}
Version: $VERSION6
Release: 2%{?extraver}%{?dist}
#Copyright: GPL
License: GPLv2+ and GPLv3+ and LGPLv3+ and BSD and Public Domain
URL: http://wdune.ourproject.org/
Source: ftp://ftp.ourproject.org/pub/wdune/wdune-%{upstream_version}.tar.bz2
BuildRequires: gcc-c++
BuildRequires: gawk
BuildRequires: make
BuildRequires: bash
BuildRequires: motif-devel
BuildRequires: libXi-devel
BuildRequires: libpng-devel
BuildRequires: libjpeg-devel
BuildRequires: mesa-libGLU-devel
BuildRequires: expat-devel
BuildRequires: flex
BuildRequires: bison
BuildRequires: imlib2-devel
BuildRequires: libusb-devel
BuildRequires: CGAL-devel
BuildRequires: gmp-devel
BuildRequires: libcurl-devel
BuildRequires: eigen3-devel
BuildRequires: vcglib-devel
BuildRequires: ImageMagick
#BuildRequires: aqsis-core
BuildRequires: freetype-devel 
BuildRequires: imlib2-devel
BuildRequires: bitstream-vera-sans-fonts
BuildRequires: desktop-file-utils
BuildRequires: xdg-utils
BuildRequires: grep
BuildRequires: pkg-config
BuildRequires: pkgconfig(eigen3) pkgconfig(vcglib)
BuildRequires: freeglut-devel
Requires: rcs
Requires: ImageMagick
#Requires: aqsis-core
Requires: bitstream-vera-sans-fonts
Requires: xorg-x11-fonts-misc
Recommends: vim
Recommends: lxterminal
Recommends: kolourpaint
Recommends: audacity

%description
The white_dune program is a graphical X3D/VRML97 editor, 
simple extrusion/NURBS/Superformula 3D modeler and animation tool.
With white_dune you can create/change 3D objects and animate them (in a easy 
way if you choose the -4kids GUI). The result can be shown in any WebGL enabled 
web browser or can be converted to the RIB format for movie creation. 
X3D and VRML97 are the ISO standard for displaying 3D data over the web. 
With Cobweb or X3DOM it can displayed in any WebGL enabled web browser. 

Under Linux, white_dune support some 3D input-devices like joysticks, game-pads
or all devices supported via the Xinput protocol.
White_dune support quad-buffer stereo visuals. Under Linux, this can be used
with Elsa Revelator, Crystal Eyes or Nvidia 3D Vision shutter-glasses and 
special drivers for expensive graphic-cards like Nvidia Quadro or ATI FireGL 4.

%package opengl-examples
License: BSD and MIT
Summary: Compiled OpenGL examples for white_dune
Requires: freeglut
Recommends: wdune
%description opengl-examples
Compiled OpenGL examples for white_dune

%package docs
License: GPLv2+ and BSD and MIT
BuildArch: noarch
Summary: Documentation for white_dune
%description docs
Documentation for white_dune

%prep
%setup -q -n "wdune-%{upstream_version}"

%build
%configure \
    --with-about="white_dune-%{upstream_version}" \\
    --with-optimization \\
    --without-devil \\
    --with-helpurl="%{_docdir}/wdune-docs/docs/" \\
    --with-protobaseurl="%{_docdir}/wdune-docs/docs" \\
    --with-checkincommand="ci" \\
    --with-imageeditor="kolourpaint" \\
    --with-imageeditor4kids="kolourpaint" \\
    --with-soundeditor=audacity
%make_build 
pushd docs/export_example_c++/opengl_example
make render_with_picture_path
popd

%install
mkdir -p $RPM_BUILD_ROOT/%{_bindir}
mkdir -p $RPM_BUILD_ROOT/%{_mandir}/man1
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/white_dune/opengl_example
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/applications
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/pixmaps/

install -m 755 -p bin/dune $RPM_BUILD_ROOT/%{_bindir}/dune
install -m 755 -p bin/dune4kids $RPM_BUILD_ROOT/%{_bindir}/dune4kids
install -m 755 -p bin/illegal2vrml $RPM_BUILD_ROOT/%{_bindir}/illegal2vrml
install -m 644 -p desktop/kde/dune.desktop $RPM_BUILD_ROOT/%{_datadir}/applications/dune.desktop
install -m 644 -p desktop/kde/dune.bmp $RPM_BUILD_ROOT/%{_datadir}/pixmaps/dune.bmp
install -m 644 -p desktop/kde/dune4kids.desktop $RPM_BUILD_ROOT/%{_datadir}/applications/dune4kids.desktop
install -m 644 -p desktop/kde/dune4kids.bmp $RPM_BUILD_ROOT/%{_datadir}/pixmaps/dune4kids.bmp
install -m 644 -p man/dune.1 $RPM_BUILD_ROOT/%{_mandir}/man1/dune.1
install -m 644 -p man/dune4kids.1 $RPM_BUILD_ROOT/%{_mandir}/man1/dune4kids.1
install -m 644 -p man/illegal2vrml.1 $RPM_BUILD_ROOT/%{_mandir}/man1/illegal2vrml.1
install -m 755 -p docs/export_example_c++/opengl_example/render_with_picture_path $RPM_BUILD_ROOT/%{_bindir}/white_dune_opengl_example
# remove binary from noarch package
rm docs/export_example_c++/opengl_example/render_with_picture_path 
# remove big C++ file from opengl-examples package (can be restored using white_dune)
rm docs/export_example_c++/opengl_example/C++Export.cc
cp -r docs/export_example_c++/opengl_example/* $RPM_BUILD_ROOT/%{_datadir}/white_dune/opengl_example

desktop-file-install                                    \\
--add-category="Graphics"                               \\
--delete-original                                       \\
--dir=$RPM_BUILD_ROOT/%{_datadir}/applications          \\
$RPM_BUILD_ROOT/%{_datadir}/applications/dune.desktop
desktop-file-install                                    \\
--add-category="Graphics"                               \\
--delete-original                                       \\
--dir=$RPM_BUILD_ROOT/%{_datadir}/applications          \\
$RPM_BUILD_ROOT/%{_datadir}/applications/dune4kids.desktop

%files
%license COPYING
%{_bindir}/dune
%{_bindir}/dune4kids
%{_bindir}/illegal2vrml
%{_datadir}/applications/dune.desktop
%{_datadir}/pixmaps/dune.bmp
%{_datadir}/applications/dune4kids.desktop
%{_datadir}/pixmaps/dune4kids.bmp
%{_mandir}/man1/dune.1*
%{_mandir}/man1/dune4kids.1*
%{_mandir}/man1/illegal2vrml.1*
%files opengl-examples
%{_bindir}/white_dune_opengl_example
%{_datadir}/white_dune/opengl_example/fin.png
%{_datadir}/white_dune/opengl_example/fire.png
%{_datadir}/white_dune/opengl_example/Makefile
%{_datadir}/white_dune/opengl_example/README.txt
%{_datadir}/white_dune/opengl_example/README_fedora.txt
%{_datadir}/white_dune/opengl_example/main.cpp
%{_datadir}/white_dune/opengl_example/robot.x3dv
%files docs
%license COPYING
%doc README.txt docs

%changelog

EOT

rpmdev-bumpspec --comment="Initial RPM release" --userstring="J. Scheurich <mufti11@web.de>" /tmp/wdune.spec

if rpm -ba /tmp/wdune.spec ; then
   echo > /dev/null
else
   rpmbuild -ba /tmp/wdune.spec
fi




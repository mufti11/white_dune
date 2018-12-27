#!/bin/sh
#set -x

TMP=$HOME/tmp
mkdir -p $TMP

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

RPM_SRC_LIST="rpm RPM redhat packages"

UNAME_A=`uname -a`
UNAME_PS2="Linux $USER 2.2.1 #1 Wed Nov 14 18:28:00 JST 2001 mips unknown"

VERSION=`sh ../../batch/getversion.sh`
(
   cd ../../.. && 
   cp -r white_dune-$VERSION $TMP && \
   cd $TMP/white_dune-$VERSION && 
   sh batch/fix_not_translated_rcfiles.sh &&
   make realclean && rm -rf desktop/macosx desktop/irix
) && \
(cd $TMP && tar -cpf - white_dune-$VERSION) | \
gzip -c > $MKRPM_SRC/white_dune-$VERSION.tar.gz

if test "$UNAME_A"="$UNAME_PS2"; then
   # disable optimization on Playstation 2 cause of compiler bugs
   OPTIMIZE=""
   RPM_OPT_FLAGS=""
else 
   OPTIMIZE=--with-optimization
   RPM_OPT_FLAGS='$RPM_OPT_FLAGS'
fi

RPM_BUILD_ROOT='$RPM_BUILD_ROOT'

rm -f $TMP/white_dune-$VERSION.spec
cat > $TMP/white_dune-$VERSION.spec << EOT 
Summary: A graphical X3DV/VRML97 editor, simple NURBS/Superformula 3D modeller and animation tool
Name: white_dune
Version: $VERSION
Release: 1
#Copyright: GPL
License: GPL
Group: Applications/Multimedia
Source: http://wdune.ourproject.org/white_dune-$VERSION.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot

%description
The white_dune program is a graphical X3DV/VRML97 editor, simple NURBS/Superformula
3D modeller and animation tool.
X3D and VRML97 are the ISO standards for displaying 3D data over the web. 
This includes for animation, realtime interaction and multimedia (image, 
movie, sound). 
VRML97 can be written by popular programs like maya, catia, 3D Studio MAX, 
cinema4D, blender, wings3d and others, but (unlike white_dune) most of this 
programs support only a small part of the X3DV/VRML97 features.
Dune can read X3DV/VRML97 files, display and let the user change the 
scenegraph and all fields of all nodes. 
Some documentation how to use dune is included.
The most powerfull 3D modelling features of white_dune support the 
X3D/VRML97 Amendment1 style NURBS nodes and Superformula based PROTOs. 
This is mainly convenient for building some rounded shapes.
For artistic mesh modelling work, the usage of a static 3D modeller with 
VRML97 export features may be useful. The result can be either used unchanged
in a bigger VRML97 file via the Inline command without breaking the 
toolchain or improved in white_dune.
Examples for free (speech) static 3D modellers with VRML97 export available 
under Linux are blender, wings3d, art of illusion or sharp construct.
Under Linux, white_dune support some 3D inputdevices like joysticks, gamepads
or all devices supported via the Xinput protocol.
White_dune support quadbuffer stereo visuals. Under Linux, this can be used
with Elsa Revelator or Crystal eyes shutterglasses and special drivers for 
expensive graphiccards like Nvidia Quadro or ATI FireGL 4.
This version of white_dune support both english and german menus and messages.
Information how to translate white_dune to foreign languages can be found under
docs/developer_docs/dune_developer.html#localisation
%prep
%setup -q

%build
./configure $OPTIMIZE --with-olpc --without-devil --with-vrmlbrowser=mozilla --with-helpurl="/usr/share/doc/white_dune-$VERSION/docs"  --with-protobaseurl="/usr/share/doc/white_dune-$VERSION/docs"
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS" 

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/share/man/man1
mkdir -p $RPM_BUILD_ROOT/usr/share/misc/white_dune
mkdir -p $RPM_BUILD_ROOT/usr/share/applications
mkdir -p $RPM_BUILD_ROOT/usr/share/icons/Bluecurve/48x48/apps
mkdir -p $RPM_BUILD_ROOT/usr/share/pixmaps/

install -s -m 755 bin/dune $RPM_BUILD_ROOT/usr/bin/dune
install -m 755 bin/dune4kids $RPM_BUILD_ROOT/usr/bin/dune4kids
install -m 755 bin/illegal2vrml $RPM_BUILD_ROOT/usr/bin/illegal2vrml
install -m 644 desktop/kde/redhat/dune.desktop $RPM_BUILD_ROOT/usr/share/applications/dune.desktop
install -m 644 desktop/kde/dune.png $RPM_BUILD_ROOT/usr/share/icons/Bluecurve/48x48/apps/dune.png
install -m 644 desktop/kde/dune.png $RPM_BUILD_ROOT/usr/share/pixmaps/dune.png
install -m 644 desktop/kde/redhat/dune4kids.desktop $RPM_BUILD_ROOT/usr/share/applications/dune4kids.desktop
install -m 644 desktop/kde/dune4kids.png $RPM_BUILD_ROOT/usr/share/icons/Bluecurve/48x48/apps/dune4kids.png
install -m 644 desktop/kde/dune4kids.png $RPM_BUILD_ROOT/usr/share/pixmaps/dune4kids.png
install -m 644 man/dune.1 $RPM_BUILD_ROOT/usr/share/man/man1/dune.1
install -m 644 man/illegal2vrml.1 $RPM_BUILD_ROOT/usr/share/man/man1/illegal2vrml.1

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README.txt COPYING.txt docs

/usr/bin/dune
/usr/bin/dune4kids
/usr/bin/illegal2vrml
/usr/share/applications/dune.desktop
/usr/share/icons/Bluecurve/48x48/apps/dune.png
/usr/share/pixmaps/dune.png
/usr/share/applications/dune4kids.desktop
/usr/share/icons/Bluecurve/48x48/apps/dune4kids.png
/usr/share/pixmaps/dune4kids.png
/usr/share/man/man1/dune.1*
/usr/share/man/man1/illegal2vrml.1*
EOT

if rpm -ba $TMP/white_dune-$VERSION.spec ; then
   echo > /dev/null
else
   rpmbuild -ba $TMP/white_dune-$VERSION.spec
fi

#!/bin/sh
TEMP=/tmp/mkpkg_$$

PATH=/usr/xpg4/bin/:$PATH
export PATH

VERSION=`sh ../../batch/getversion.sh`

for i in "beta" "pre" "pl" ; do
   if test "$MINOR" = "" ; then
     MAYOR=`awk -v version=$VERSION -v col=$i 'BEGIN {split(version,a,col);print a[1]}'`
   fi
   if test "$MINOR" = "" ; then
     MINOR=`awk -v version=$VERSION -v col=$i 'BEGIN {split(version,a,col);print a[2]}'` 
   fi
done

PKG=whitedune.$MAYOR.$MINOR.SPARC.pkg

PKGPROTOFILE=/tmp/pkgprotofile

echo i pkginfo > $PKGPROTOFILE
echo d none /opt 0`perl -e '($dev,$ino,$mode)=stat("/opt");printf("%o",$mode & 0777)'` root sys >> $PKGPROTOFILE
cat >> $PKGPROTOFILE <<EOT
d none /opt/White_Dune 0755 root sys
d none /opt/White_Dune/bin 0755 root sys
f none /opt/White_Dune/bin/white_dune=/tmp/white_dune/desktop/cde/os/solaris/white_dune 0755 root sys 
d none /opt/White_Dune/man 0755 root sys
d none /opt/White_Dune/man/man1 0755 root sys
d none /opt/White_Dune/data 0755 root sys
f none /opt/White_Dune/bin/dune=/tmp/white_dune/bin/dune 0755 root sys
f none /opt/White_Dune/bin/dune4kids=/tmp/white_dune/bin/dune4kids 0755 root sys
f none /opt/White_Dune/bin/illegal2vrml=/tmp/white_dune/bin/illegal2vrml 0755 root sys
f none /opt/White_Dune/man/man1/dune.1=/tmp/white_dune/man/dune.1 0644 root sys
f none /opt/White_Dune/man/man1/illegal2vrml.1=/tmp/white_dune/man/illegal2vrml.1 0644 root sys
f none /usr/dt/appconfig/icons/C/VRML97.m.bm=/tmp/white_dune/desktop/cde/icons/VRML97.m.bm 0644 root sys 
f none /usr/dt/appconfig/icons/C/VRML97.m.pm=/tmp/white_dune/desktop/cde/icons/VRML97.m.pm 0644 root sys 
f none /usr/dt/appconfig/icons/C/VRML97.m_m.bm=/tmp/white_dune/desktop/cde/icons/VRML97.m_m.bm 0644 root sys 
f none /usr/dt/appconfig/icons/C/VRML97.t.bm=/tmp/white_dune/desktop/cde/icons/VRML97.t.bm 0644 root sys 
f none /usr/dt/appconfig/icons/C/VRML97.t.pm=/tmp/white_dune/desktop/cde/icons/VRML97.t.pm 0644 root sys 
f none /usr/dt/appconfig/icons/C/VRML97.t_m.bm=/tmp/white_dune/desktop/cde/icons/VRML97.t_m.bm 0644 root sys 
f none /usr/dt/appconfig/icons/C/white_dune.l.bm=/tmp/white_dune/desktop/cde/icons/white_dune.l.bm 0644 root sys 
f none /usr/dt/appconfig/icons/C/white_dune.l.pm=/tmp/white_dune/desktop/cde/icons/white_dune.l.pm 0644 root sys 
f none /usr/dt/appconfig/icons/C/white_dune.m.bm=/tmp/white_dune/desktop/cde/icons/white_dune.m.bm 0644 root sys 
f none /usr/dt/appconfig/icons/C/white_dune.m.pm=/tmp/white_dune/desktop/cde/icons/white_dune.m.pm 0644 root sys 
f none /usr/dt/appconfig/icons/C/white_dune.t.bm=/tmp/white_dune/desktop/cde/icons/white_dune.t.bm 0644 root sys 
f none /usr/dt/appconfig/icons/C/white_dune.t.pm=/tmp/white_dune/desktop/cde/icons/white_dune.t.pm 0644 root sys 
f none /usr/dt/appconfig/types/C/white_dune.dt=/tmp/white_dune/desktop/cde/os/solaris/opt_White_Dune_bin_white_dune.dt 0644 root sys 
f none /usr/dt/config/C/wsmenu/Applications/white_dune=/tmp/white_dune/desktop/cde/os/solaris/white_dune 0755 root sys 
EOT
(cd ../..;pkgproto docs ) | awk '{print $1,$2,"/opt/White_Dune/" $3 "=/tmp/white_dune/" $3,$4,"root sys"}' >> $PKGPROTOFILE

PKGINFO=/tmp/pkginfo
LC_TIME=C
export LC_TIME
DATE=`/usr/bin/date '+%dth%b%Y'`
PROCESSOR=`uname -p`

cat > $PKGINFO << EOT
PKG="WHITEdune"
NAME="white_dune-$VERSION - VRML97 editor, simple NURBS 3D modeller and animation tool (in development)"
VERSION="$MAYOR.$MINOR"
ARCH="$PROCESSOR"
CLASSES="none"
CATEGORY="application"
DESC="VRML97/X3D editor, simple 3D modeller and animation tool http://wdune.ourproject.org"
VENDOR="Stephan F. White and others (GNU Public License)"
PSTAMP="$DATE"
EMAIL="mufti@csv.ica.uni-stuttgart.de"
ISTATES="S s 1 2 3"
RSTATES="S s 1 2 3"
BASEDIR="/"
EOT


(rm -rf /tmp/white_dune && \
 cd ../../.. && \
 cp -r white_dune-$VERSION /tmp/white_dune && \
 cd /tmp/white_dune && \
 sh batch/fix_not_translated_rcfiles.sh && \
 make realclean && \
 ./configure \
  --without-usb \
  --with-helpurl="/opt/White_Dune/docs/" \
  --with-protobaseurl="/opt/White_Dune/docs" \
 && make) && \
 mkdir $TEMP && \
 rm -f $TEMP/$PKG && \
 rm -rf $TEMP/WHITEdune && \
 pkgmk -d $TEMP -f $PKGPROTOFILE -o WHITEdune && \
 cd $TEMP && \
 pkgtrans -s . $PKG WHITEdune && \
 gzip -9 $PKG && \
 cd /tmp && \
 mv $TEMP/$PKG.gz /tmp && \
 rm -rf $TEMP /tmp/white_dune $PKGPROTOFILE $PKGINFO
 echo "use \"gzip -cd /tmp/$PKG.gz | pkgadd -n -d /dev/fd/0 WHITEdune\" to install"

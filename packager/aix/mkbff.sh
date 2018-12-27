#!/bin/ksh
# set -x

CAN_CHOWN=FALSE
touch /tmp/chowntest_$$
if chown root.system /tmp/chowntest_$$ ; then
   CAN_CHOWN=TRUE
fi
rm /tmp/chowntest_$$

if test $CAN_CHOWN = FALSE ; then
    echo warning: root required to chown files 1>&2
    sleep 60
fi

# AIX .bff install script
# calls mklpp from the AIX bull freeware collection
# http://www.bullfreeware.com/download/aix43/bull.mklpp-1.2.9.0.exe

VERSION=`sh ../../batch/getversion.sh`

for i in "beta" "pre" "pl" ; do
   if test "$MINOR" = "" ; then
     MAYOR=`awk -v version=$VERSION -v col=$i 'BEGIN {split(version,a,col);print a[1]}'`
     NUM1=`awk -v mayor=$MAYOR 'BEGIN {split(mayor,a,".");print a[1]}'`
     NUM2=`awk -v mayor=$MAYOR 'BEGIN {split(mayor,a,".");print a[2]}'`
     STATUS=$i
   fi
   if test "$MINOR" = "" ; then
     MINOR=`awk -v version=$VERSION -v col=$i 'BEGIN {split(version,a,col);print a[2]}'` 
   fi
done

DUNE_LPP_DIR=/tmp/mkbff_white_dune-''$VERSION
rm -rf $DUNE_LPP_DIR

NAME=$NUM1$NUM2$STATUS$MINOR

cp -r `dirname $0`/lppdir $DUNE_LPP_DIR || exit 1
mkdir $DUNE_LPP_DIR/usr/lpp/freeware.white_dune$NAME

#DUNE=`dirname $0`/../../bin/dune
DOCS=`dirname $0`/../../docs
MAN=`dirname $0`/../../man
ICONS=`dirname $0`/../../desktop/cde/icons/

#(cd ../.. && sh build.sh)

#LIBRARIES=`ldd $DUNE | gawk '{split($0,a,"(");print a[1]}' | sort | uniq | grep -v $DUNE`

#FILESETS=`for i in $LIBRARIES; do lslpp -w -c -q $i | awk '{split($0,a,":");print a[2]}' ; done | sort | uniq`

cat > $DUNE_LPP_DIR/lpp_name << EOT
4 R I freeware.white_dune$NAME {
freeware.white_dune$NAME.rte 0$MAYOR.0000.0000 01 N U en_US white_dune$NAME X3D/VRML97 editor, animation tool and simple NURBS 3D-modeller  (GNU public license)
[
EOT
for i in $FILESETS; do
    lslpp -q -l $i | awk '{if ($1!="Path:") {print "*prereq " $1 " " $2;exit(0)}}'
done  >> $DUNE_LPP_DIR/lpp_name

echo "%" >> $DUNE_LPP_DIR/lpp_name
#echo /usr/local/bin/dune `du $DUNE | awk '{print $1}'`   >> $DUNE_LPP_DIR/lpp_name
#echo /usr/local/share/white_dune `du -s $DOCS | awk '{print $1}'`   >> $DUNE_LPP_DIR/lpp_name
#echo /usr/local/man/man1 `du $MAN | awk '{print $1}'`   >> $DUNE_LPP_DIR/lpp_name

cat  >> $DUNE_LPP_DIR/lpp_name << EOT
%
%
%
%
]
}
EOT

cat >  $DUNE_LPP_DIR/.info/freeware.white_dune$NAME.rte.copyright << EOT
 white_dune $VERSION
 X3DV/VRML97 editor, animation tool and simple NURBS 3D-modeller 
 * Copyright (C) 2004 Stephen F. White and others
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * see http://wdune.ourproject.org for more information
EOT

mkdir -p $HOME/lppdir/out
mkdir -p $HOME/lppdir/in
mkdir -p $HOME/lppdir/bff

mkdir -p $DUNE_LPP_DIR/usr/local/bin
mkdir -p $DUNE_LPP_DIR/usr/local/share/white_dune
mkdir -p $DUNE_LPP_DIR/usr/local/man/man1
mkdir -p $DUNE_LPP_DIR/usr/dt/appconfig/icons/C/
mkdir -p $DUNE_LPP_DIR/usr/dt/appconfig/types/C/
mkdir -p $DUNE_LPP_DIR/usr/dt/config/C/wsmenu/Applications/

(rm -rf /tmp/white_dune && \
 cd ../.. && make realclean && \
 cd .. && \
 cp -r white_dune-$VERSION /tmp/white_dune && \
 cd /tmp/white_dune && \
 sh batch/fix_not_translated_rcfiles.sh && \
 make realclean && \
 ./configure \
  --with-helpurl="/usr/local/share/white_dune" \
  --with-protobaseurl="/usr/local/share/white_dune" \
 && make) && \
 cp -r /tmp/white_dune/bin/dune $DUNE_LPP_DIR/usr/local/bin && \
 cp -r $DOCS $DUNE_LPP_DIR/usr/local/share/white_dune && \
 cp $MAN/* $DUNE_LPP_DIR/usr/local/man/man1 && \
 cp $ICONS/*.?m $DUNE_LPP_DIR/usr/dt/appconfig/icons/C/ && \
 cp /tmp/white_dune/desktop/cde/os/aix/usr_local_bin_white_dune.dt $DUNE_LPP_DIR/usr/dt/appconfig/types/C/white_dune.dt && \
 cp /tmp/white_dune/desktop/cde/os/aix/white_dune $DUNE_LPP_DIR/usr/dt/config/C/wsmenu/Applications/ && \
 cp /tmp/white_dune/desktop/cde/os/aix/white_dune $DUNE_LPP_DIR/usr/local/bin && \
 find $DUNE_LPP_DIR -type f -print | xargs chmod 444 && \
 chmod 755 $DUNE_LPP_DIR/usr/dt/config/C/wsmenu/Applications/white_dune && \
 chmod 755 $DUNE_LPP_DIR/usr/local/bin/white_dune && \
 chmod 755 $DUNE_LPP_DIR/usr/local/bin/dune && \
 if test $CAN_CHOWN = TRUE ; then chown -R root.system $DUNE_LPP_DIR ; fi  && \
 find $DUNE_LPP_DIR -type d -print | xargs chmod 755 && \
 cd $DUNE_LPP_DIR && \
 /usr/local/bin/mklpp && \
 gzip -9f $HOME/lppdir/bff/white_dune$NAME-$MAYOR.0.0.bff
 echo install with 
 echo "cd $HOME/lppdir/bff && gzip -d $HOME/lppdir/bff/white_dune$NAME-$MAYOR.0.0.bff.gz && installp -a -d white_dune$NAME-$MAYOR.0.0.bff all"



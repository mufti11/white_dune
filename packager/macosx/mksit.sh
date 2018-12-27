#!/bin/sh
#set -x

OS=`sw_vers | awk '/ProductVersion:/ {split($2,a,".");print a[1] "." a[2]}'`

OS_ARCHIVE=
if test "$OS" = "10.7" ; then
   OS="Lion"-fat-binary
fi

if test "$OS" = "10.6" ; then
   OS="Snowleopard"-fat-binary
fi

if test "$OS" = "10.5" ; then
   OS="Leopard"-fat-binary
fi

if test "$OS" = "10.4" ; then
   OS="Tiger"-fat-binary
   OS_ARCHIVE=_tiger
fi

if test "$OS" = "10.3" ; then
   OS="Panther"-`uname -p`
fi

if test "$OS" = "10.2" ; then
   OS="Jaguar"-`uname -p`
fi

VERSION=`sh ../../batch/getversion.sh`
MAYOR=`awk -v version=$VERSION -v col=pl 'BEGIN {split(version,a,col);print a[1]}'`
rm -rf /tmp/white_dune-$VERSION
(cd ../../.. && cp -r wdune-$VERSION /tmp) && \
cd /tmp/wdune-$VERSION && 
sh batch/fix_not_translated_rcfiles.sh &&
make realclean && \
(cd desktop/macosx && tar -xf white_dune$OS_ARCHIVE.app.tar) &&
./configure --with-uninstallcomment="find and remove directory/application white_dune.app" --with-optimization --without-devil --with-helpurl="http://wdune.ourproject.org/docs" && make &&
rm -f desktop/macosx/white_dune.app/Contents/MacOS/dune &&
cp bin/dune desktop/macosx/white_dune.app/Contents/MacOS &&
strip desktop/macosx/white_dune.app/Contents/MacOS/dune &&
chmod +x desktop/macosx/white_dune.app/Contents/MacOS/*dune* &&
awk -v mayor=$MAYOR '{sub("<string>0.27</string>","<string>" mayor "</string>");print $0}' \
     desktop/macosx/white_dune.app/Contents/Info.plist > /tmp/$$_Info.plist &&
cp /tmp/$$_Info.plist desktop/macosx/white_dune.app/Contents/Info.plist &&
rm /tmp/$$_Info.plist &&
cp -r docs desktop/macosx/white_dune.app/Contents/Resources/WhiteDuneHelp &&
cd desktop/macosx && 
tar -cf /tmp/white_dune-MacOSX-$OS-$VERSION.tar white_dune.app && 
gzip -9f /tmp/white_dune-MacOSX-$OS-$VERSION.tar &&
echo stuffit expander compatible archive is /tmp/white_dune-MacOSX-$OS-$VERSION.tar.gz 



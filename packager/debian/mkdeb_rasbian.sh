#!/bin/sh
# set -x

# creates a .deb (dpkg) installable binary package for raspbian

VER=`sh ../../batch/getversion.sh`
VERSION=wdune-$VER
VERSION_DEBIAN=`awk -v ver=$VERSION 'BEGIN {gsub("_","",ver);print ver}'`
VERSION_ORIG=$VERSION_DEBIAN.orig

#../../batch/findcopyrights.sh > copyright

rm -rf /tmp/$VERSION_DEBIAN /tmp/$VERSION_ORIG

(cd ../../.. && cp -r $VERSION /tmp/$VERSION_DEBIAN) &&
(cd /tmp/$VERSION_DEBIAN && make realclean) &&
(cd /tmp/$VERSION_DEBIAN && rm -rf desktop/macosx) &&
(cd ../../.. && cp -r $VERSION /tmp/$VERSION_ORIG) &&
(cd /tmp/$VERSION_ORIG && make realclean) &&
(cd .. && tar -cf - debian) | (cd /tmp/$VERSION_DEBIAN && tar -xf -) &&
cp control_stretch /tmp/$VERSION_DEBIAN/debian/control
cp rules_rasbian /tmp/$VERSION_DEBIAN/debian/rules

chmod a+x /tmp/$VERSION_DEBIAN/debian/mkinstalldirs
chmod a+x /tmp/$VERSION_DEBIAN/debian/postinst
chmod a+x /tmp/$VERSION_DEBIAN/debian/prerm
chmod a+x /tmp/$VERSION_DEBIAN/debian/rules

cat << EOT > /tmp/$VERSION_DEBIAN/debian/changelog
wdune ($VER-1) experimental; urgency=low

  * Version $VER debian package
    For details see the README file of the source package

EOT

DATE=`date -R`
echo ' -- J. "MUFTI" Scheurich <mufti11@web.de>  '$DATE >> /tmp/$VERSION_DEBIAN/debian/changelog
echo >> /tmp/$VERSION_DEBIAN/debian/changelog

cat changelog >> /tmp/$VERSION_DEBIAN/debian/changelog

cat Makefile.debian_extension >> /tmp/$VERSION_DEBIAN/Makefile

head -`wc -l ../../src/Makefile.in | awk '{print $1 -1}'` \
../../src/Makefile.in > /tmp/$VERSION_DEBIAN/src/Makefile.in 
cat Makefile.in.debian_extension >> /tmp/$VERSION_DEBIAN/src/Makefile.in

#dpkg-buildpackage -us -uc -sa -rfakeroot
# us = unsigned uc= unsigned changes
(
   cd /tmp/$VERSION_DEBIAN && 
   sh batch/fix_not_translated_rcfiles.sh &&
   export CXXFLAGS="-g -O2"
   export CFLAGS="-g -O2"
   export LDFLAGS="-g"
   export DEB_BUILD_OPTIONS="nostrip noopt debug"
   dpkg-buildpackage -i -D -us -uc -rfakeroot
) &&
cp /tmp/$VERSION_DEBIAN/debian/changelog . &&
VERSION_DEB=`awk -v ver=$VERSION_DEBIAN 'BEGIN {gsub("-","_",ver);print ver}'` &&
rm -rf /tmp/$VERSION_DEBIAN

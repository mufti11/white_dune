#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

cd `dirname $0`/..

VER=`sh batch/getversion.sh`
VERSION=wdune-$VER

mkdir -p debian &&
cp packager/debian/rules.debian debian/rules &&
chmod +x debian/rules &&
cp packager/debian/control_buster debian/control &&
cp packager/debian/mkinstalldirs debian &&
cp packager/debian/postinst debian &&
cp packager/debian/prerm debian &&
cp packager/debian/changelog debian &&
cp packager/debian/menu debian &&
cp packager/debian/compat debian &&
cp packager/debian/copyright debian &&
cp packager/debian/wdune.install debian &&
cp packager/debian/wdune-docs.install debian &&
mkdir -p debian/wdune/usr &&
mkdir -p debian/source &&
echo 1.0 > debian/source/format &&

cat >debian/changelog  <<EOT
wdune ($VER-1) experimental; urgency=low

  * Version $VER debian package
    For details see the README.txt file of the source archive

EOT

DATE=`date -R`
echo ' -- J. "MUFTI" Scheurich <mufti11@web.de>  '$DATE >> debian/changelog

echo use as root:
echo dpkg-buildpackage -i -D -us -uc -rfakeroot -T wdune -T wdune-docs


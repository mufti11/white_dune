# Copyright 1999-2003 Gentoo Technologies, Inc.
# Distributed under the terms of the GNU General Public License v2
#
# J. "MUFTI" Scheurich 

# set -x

emerge --noreplace dev-libs/expat
emerge --noreplace media-libs/libpng
emerge --noreplace media-libs/jpeg                   
emerge --noreplace sys-libs/zlib
emerge --noreplace virtual/opengl 
emerge --noreplace virtual/glu 
emerge --noreplace virtual/glut
emerge --noreplace virtual/x11
emerge --noreplace x11-libs/openmotif
emerge --noreplace app-text/rcs
VERSION=`sh ../../batch/getversion.sh`
MAYOR=`echo $VERSION | awk '{split($0,a,"[a-z]+"); print a[1]}'`
MINOR=`echo $VERSION | awk '{split($0,a,"[a-z]+"); print a[2]}'` 
if test "X_$MINOR" = "X_" ; then
    echo started in wrong directory 1>&2 
    exit 1
fi
NAME=white_dune-$MAYOR"".$MINOR
FILE=/usr/portage/media-gfx/white_dune/$NAME.ebuild
mkdir -p /usr/portage/media-gfx/white_dune
cp white_dune.ebuild $FILE
mkdir -p /usr/portage/media-gfx/white_dune/files
ebuild /usr/portage/media-gfx/white_dune/$NAME.ebuild digest
ebuild /usr/portage/media-gfx/white_dune/$NAME.ebuild manifest
#(cd /tmp && wget ftp://ftp.ourproject.org/pub/wdune/wdune-$VERSION.tar.bz2)
(
   cd ../../.. && 
   rm -f $NAME &&
   ln -s white_dune-$VERSION $NAME && 
   sh $NAME/batch/fix_not_translated_rcfiles.sh &&
   tar -chf /tmp/$NAME-gentoo.tar $NAME && 
   gzip -9f /tmp/$NAME-gentoo.tar
)
cp /tmp/$NAME-gentoo.tar.gz /usr/portage/distfiles
ebuild /usr/portage/media-gfx/white_dune/$NAME.ebuild digest
ebuild /usr/portage/media-gfx/white_dune/$NAME.ebuild manifest
#ebuild $FILE setup
#ebuild $FILE preinst
ebuild $FILE compile
ebuild $FILE install
ebuild $FILE qmerge
tar -cvPf /tmp/white_dune-""$VERSION""-gentoo.tar /usr/portage/distfiles/$NAME-gentoo.tar.gz /usr/portage/media-gfx/white_dune/files/digest-$NAME /usr/portage/media-gfx/white_dune/Manifest $FILE
gzip -9f /tmp/white_dune-""$VERSION""-gentoo.tar
echo "use \"tar -xzPf /tmp/white_dune-""$VERSION""-gentoo.tar.gz && emerge /usr/portage/media-gfx/white_dune/$NAME.ebuild\" to install"


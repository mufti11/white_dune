#!/bin/sh
# set $PREFIX, if you do not want to install to /usr

export CPPFLAGS=-I/usr/local/include

WTMP=/tmp/mkpkg_$$
mkdir $WTMP || exit 1
export WTMP

if test "X_$PREFIX" = "X_" ; then
PREFIX=/usr
export PREFIX
fi

VERSION=`sh ../../batch/getversion.sh`
export VERSION

(cd ../../.. && cp -r wdune-$VERSION $WTMP &&
 cd $WTMP/wdune-$VERSION && 
 sh batch/fix_not_translated_rcfiles.sh &&
 make realclean && 
 ./configure --with-optimization --without-devil \
 --without-sdljoystick --with-vrmlbrowser=mozilla \
 --with-helpurl="$PREFIX/share/doc/wdune" \
 --with-protobaseurl="$PREFIX/share/doc/wdune" \
 && make -j4) || exit 1

mkdir -p $PREFIX/bin/ &&
cp $WTMP/wdune-$VERSION/bin/dune $PREFIX/X11R6/bin/ &&
cp $WTMP/wdune-$VERSION/bin/dune4kids $PREFIX/X11R6/bin/ &&
cp $WTMP/wdune-$VERSION/bin/illegal2vrml $PREFIX/bin/ &&
chmod 755 $PREFIX/X11R6/bin/dune &&
chmod 755 $PREFIX/X11R6/bin/dune4kids &&
chmod 755 $PREFIX/bin/illegal2vrml &&

mkdir -p $PREFIX/man/man1 &&
cp $WTMP/wdune-$VERSION/man/dune.1 $PREFIX/man/man1 &&
gzip -9f $PREFIX/man/man1/dune.1 &&
cp $WTMP/wdune-$VERSION/man/illegal2vrml.1 $PREFIX/man/man1 &&
gzip -9f $PREFIX/man/man1/illegal2vrml.1

chmod 644 $PREFIX/man/man1/dune.1.gz
chmod 644 $PREFIX/man/man1/illegal2vrml.1.gz

mkdir -p $PREFIX/share/doc/wdune/ &&
cp -r $WTMP/wdune-$VERSION/docs/* $PREFIX/share/doc/wdune/

find $PREFIX/share/doc/wdune/ -type f -print | xargs chmod 644
find $PREFIX/share/doc/wdune/ -type d -print | xargs chmod 755

echo @name wdune-$VERSION > $WTMP/pkg-list

#if sh -c "ldd $PREFIX/X11R6/bin/dune | grep jpeg > /dev/null" ; then
#   echo @depend `pkg_info | egrep ^jpeg- | awk '{print $1}'` >> $WTMP/pkg-list
#fi
#if sh -c "ldd $PREFIX/X11R6/bin/dune | grep png > /dev/null" ; then
#   echo @depend `pkg_info | egrep ^png- | awk '{print $1}'` >> $WTMP/pkg-list
#fi
#echo @depend `pkg_info | egrep ^Mesa- | awk '{print $1}'` >> $WTMP/pkg-list
#echo @depend `pkg_info | egrep ^open-motif- | awk '{print $1}'` >> $WTMP/pkg-list
#echo @depend `pkg_info | egrep ^XFree86-libraries- | awk '{print $1}'` >> $WTMP/pkg-list

cat >> $WTMP/pkg-list << EOT
@cwd $PREFIX
bin/dune
bin/dune4kids
bin/illegal2vrml
man/man1/dune.1.gz
man/man1/illegal2vrml.1.gz
EOT

cp pkg-descr $WTMP
cp pkg-comment $WTMP

(cd $PREFIX && 
 find share/doc/wdune -type f -print >> $WTMP/pkg-list) 
(cd $PREFIX && 
 find share/doc/wdune -type d -print | sort -r | xargs -n 1 echo @dirrm >> $WTMP/pkg-list)

(cd /tmp && 
pkg_create -f $WTMP/pkg-list -p /usr -D MAINTAINER=mufti11@web.de -D COMMENT="Graphical X3DV/VRML97 editor" -d $WTMP/pkg-descr wdune-$VERSION.tgz)

mv /tmp/wdune-$VERSION.tgz /tmp/wdune-OpenBSD-$VERSION.tgz
echo use \"pkg_add /tmp/wdune-OpenBSD-$VERSION.tgz\" to install
rm -r $WTMP

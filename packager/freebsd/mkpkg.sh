#!/bin/sh
# set $PREFIX, if you do not want to install to /usr

WTMP=/tmp/mkpkg_$$
mkdir $WTMP || exit 1
export WTMP

if test "X_$PREFIX" = "X_" ; then
PREFIX=/usr/local
export PREFIX
fi

VERSION=`sh ../../batch/getversion.sh`
export VERSION

export CXX=clang
export CPPFLAGS=-I/usr/local/include/freetype2
export LIBS=-lstdc++

(cd ../../.. && cp -r wdune-$VERSION $WTMP &&
 cd $WTMP/wdune-$VERSION && 
 sh batch/fix_not_translated_rcfiles.sh &&
 make realclean && 
 ./configure --with-optimization --without-devil \
 --with-vrmlbrowser=firefox \
 --with-helpurl="$PREFIX/share/doc/wdune" \
 --with-protobaseurl="$PREFIX/share/doc/wdune" \
 && make -j4) || exit 1

mkdir -p $PREFIX/bin/ &&
cp $WTMP/wdune-$VERSION/bin/dune $PREFIX/bin/ &&
cp $WTMP/wdune-$VERSION/bin/dune4kids $PREFIX/bin/ &&
cp $WTMP/wdune-$VERSION/bin/illegal2vrml $PREFIX/bin/ &&
chmod 755 $PREFIX/bin/dune &&
chmod 755 $PREFIX/bin/dune4kids &&
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

if sh -c "ldd $PREFIX/bin/dune | grep jpeg > /dev/null" ; then
   echo @pkgdep `pkg info | egrep ^jpeg- | awk '{print $1}'` >> $WTMP/pkg-list
fi
if sh -c "ldd $PREFIX/bin/dune | grep png > /dev/null" ; then
   echo @pkgdep `pkg info | egrep ^png- | awk '{print $1}'` >> $WTMP/pkg-list
fi
echo @pkgdep `pkg info | egrep ^libGL- | awk '{print $1}'` >> $WTMP/pkg-list
echo @pkgdep `pkg info | egrep ^libGLU- | awk '{print $1}'` >> $WTMP/pkg-list
echo @pkgdep `pkg info | egrep ^xorg-libraries- | awk '{print $1}'` >> $WTMP/pkg-list

mkdir $WTMP/manifest

cat >> $WTMP/manifest/+MANIFEST << EOT
name: wdune
version: $VERSION
comment: "graphical VRML97/X3D editor"
origin: graphics/wdune
desc: "graphical VRML97/X3D editor, 3D modeller and animation tool"
maintainer: mufti11@web.de
www: https://wdune.ourproject.org/
prefix: /usr/local
EOT

(cd $PREFIX && 
 find share/doc/wdune -type f -print >> $WTMP/pkg-list) 
(cd $PREFIX && 
 find share/doc/wdune -type d -print | sort -r | xargs -n 1 echo @dirrm >> $WTMP/pkg-list)

(cd /tmp && 
pkg create -p $WTMP/pkg-list -m $WTMP/manifest wdune-$VERSION)

mv /tmp/wdune-$VERSION.txz /tmp/wdune-FreeBSD-$VERSION.txz
echo use \"pkg add /tmp/wdune-FreeBSD-$VERSION.txz\" to install
rm -r $WTMP


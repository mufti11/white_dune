#!/bin/sh
# set $PREFIX, if you do not want to install to /usr

WDUNETMP=/tmp/mkpkg_$$
mkdir $WDUNETMP || exit 1
export WDUNETMP

if test "X_$PREFIX" = "X_" ; then
PREFIX=/usr/local
export PREFIX
fi

VERSION=`sh ../../batch/getversion.sh`
export VERSION

export CC=clang
export CFLAGS="-O2 -Wno-unused-command-line-argument -L/usr/local/lib"
export CXX=clang++
export CXXFLAGS="-O2 -Wno-tautological-undefined-compare \
                 -Wno-tautological-pointer-compare \
                 -Wno-deprecated-declarations"
export CPPFLAGS=-I/usr/local/include/freetype2
export LIBS=-lstdc++

(cd ../../.. && cp -r wdune-$VERSION $WDUNETMP &&
 cd $WDUNETMP/wdune-$VERSION && 
 sh batch/fix_not_translated_rcfiles.sh &&
 make realclean && 
 ./configure --with-optimization --without-devil \
 --with-vrmlbrowser=firefox --with-cgalheaders \
 --with-helpurl="$PREFIX/share/doc/white_dune/index.html" \
 --with-protobaseurl="$PREFIX/share/doc/wdune" \
 --prefix=$PREFIX && 
make -j8 && make install) || exit 1

mkdir -p $PREFIX/bin/ &&
cp $WDUNETMP/wdune-$VERSION/bin/dune $PREFIX/bin/ &&
cp $WDUNETMP/wdune-$VERSION/bin/dune4kids $PREFIX/bin/ &&
cp $WDUNETMP/wdune-$VERSION/bin/gitview.sh $PREFIX/bin/ &&
cp $WDUNETMP/wdune-$VERSION/bin/illegal2vrml $PREFIX/bin/ &&
cp $WDUNETMP/wdune-$VERSION/tools/run_dune_and_povray_freebsd.sh $PREFIX/bin/run_dune_and_aqsis.sh &&
chmod 755 $PREFIX/bin/dune &&
chmod 755 $PREFIX/bin/dune4kids &&
chmod 755 $PREFIX/bin/gitview &&
chmod 755 $PREFIX/bin/illegal2vrml &&
chmod 755 $PREFIX/bin/run_dune_and_povray.sh &&

mkdir -p $PREFIX/man/man1 &&
cp $WDUNETMP/wdune-$VERSION/man/dune.1 $PREFIX/man/man1 &&
gzip -9f $PREFIX/man/man1/dune.1 &&
cp $WDUNETMP/wdune-$VERSION/man/illegal2vrml.1 $PREFIX/man/man1 &&
gzip -9f $PREFIX/man/man1/illegal2vrml.1
cp $WDUNETMP/wdune-$VERSION/man/gitview.1 $PREFIX/man/man1 &&
gzip -9f $PREFIX/man/man1/gitview.1

chmod 644 $PREFIX/man/man1/dune.1.gz
chmod 644 $PREFIX/man/man1/illegal2vrml.1.gz
chmod 644 $PREFIX/man/man1/gitview.1.gz

(cd $WDUNETMP/wdune-$VERSION && rm -r vcglib)


mkdir -p $PREFIX/share/doc/wdune/ &&
cp -r $WDUNETMP/wdune-$VERSION/docs/* $PREFIX/share/doc/wdune/ &&
find  $PREFIX/share/doc/wdune/ | grep pkgsave | xargs rm

find $PREFIX/share/doc/wdune/ -type f -print | xargs chmod 644
find $PREFIX/share/doc/wdune/ -type d -print | xargs chmod 755

mkdir -p $PREFIX/share/applications/
cp $WDUNETMP/wdune-$VERSION/desktop/kde/dune.desktop $PREFIX/share/applications
chmod 644 $PREFIX/share/applications/dune.desktop
cp $WDUNETMP/wdune-$VERSION/desktop/kde/dune4kids.desktop $PREFIX/share/applications
chmod 644 $PREFIX/share/applications/dune4kids.desktop

mkdir -p $PREFIX/share/pixmaps
cp $WDUNETMP/wdune-$VERSION/desktop/kde/dune.png $PREFIX/share/pixmaps
chmod 644 $PREFIX/share/pixmaps/dune.png
cp $WDUNETMP/wdune-$VERSION/desktop/kde/dune4kids.png $PREFIX/share/pixmaps
chmod 644 $PREFIX/share/pixmaps/dune4kids.png
 
echo @name wdune-$VERSION > $WDUNETMP/pkg-list

if sh -c "ldd $PREFIX/bin/dune | grep jpeg > /dev/null" ; then
   echo @pkgdep `pkg info | egrep ^jpeg- | awk '{print $1}'` >> $WDUNETMP/pkg-list
fi
if sh -c "ldd $PREFIX/bin/dune | grep png > /dev/null" ; then
   echo @pkgdep `pkg info | egrep ^png- | awk '{print $1}'` >> $WDUNETMP/pkg-list
fi
echo @pkgdep `pkg info | egrep ^git- | awk '{print $1}'` >> $WDUNETMP/pkg-list
echo @pkgdep `pkg info | egrep ^bash- | awk '{print $1}'` >> $WDUNETMP/pkg-list
echo @pkgdep `pkg info | egrep ^awk- | awk '{print $1}'` >> $WDUNETMP/pkg-list
echo @pkgdep `pkg info | egrep ^kdialog- | awk '{print $1}'` >> $WDUNETMP/pkg-list
echo @pkgdep `pkg info | egrep ^cgal- | awk '{print $1}'` >> $WDUNETMP/pkg-list
echo @pkgdep `pkg info | egrep ^mesa-libs- | awk '{print $1}'` >> $WDUNETMP/pkg-list
echo @pkgdep `pkg info | egrep ^libGLU- | awk '{print $1}'` >> $WDUNETMP/pkg-list
echo @pkgdep `pkg info | egrep ^xorgproto- | awk '{print $1}'` >> $WDUNETMP/pkg-list
echo @pkgdep `pkg info | egrep ^povray37- | awk '{print $1}'` >> $WDUNETMP/pkg-list
echo @pkgdep `pkg info | egrep ^mencoder- | awk '{print $1}'` >> $WDUNETMP/pkg-list

mkdir $WDUNETMP/manifest

FVERSION=`awk -v version=$VERSION 'BEGIN { c1=substr(version,1,1); c3=substr(version,3,1); sub(c1 "." c3, c1 "." c3 ".", version); print version }'`
export FVERSION

cat >> $WDUNETMP/manifest/+MANIFEST << EOT
name: wdune
version: $FVERSION
comment: "graphical VRML97/X3D editor"
origin: graphics/wdune
desc: "graphical VRML97/X3D editor, 3D modeller and animation tool"
maintainer: mufti11@web.de
www: https://wdune.ourproject.org/
prefix: /usr/local
EOT

cat pkg-plist >> $WDUNETMP/pkg-list
(cd $PREFIX && 
 find share/doc/wdune -type f -print >> $WDUNETMP/pkg-list) 
(cd $PREFIX && 
 find share/doc/wdune -type d -print | sort -r | xargs -n 1 echo @dir >> $WDUNETMP/pkg-list)

(cd /tmp && 
pkg create -p $WDUNETMP/pkg-list -m $WDUNETMP/manifest wdune-$VERSION)

mv /tmp/wdune-$FVERSION.txz /tmp/wdune-FreeBSD-$VERSION.txz
echo use \"pkg add /tmp/wdune-FreeBSD-$VERSION.txz\" to install
#rm -r $WDUNETMP


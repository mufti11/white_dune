#!/bin/sh

#########################################################################
# Author - Maksim Diachkov						#
# Please send error reports and suggestions to madj0388@uosis.mif.vu.lt	#
# modified by MUFTI for
# Slackware 2.1 revision						#
# todo:	check $TMP for sufficient rights, check for *pkg avaliability	#
#########################################################################

CFLAGS=
export CFLAGS
CXXFLAGS=
export CXXFLAGS

VERSION=`sh ../../batch/getversion.sh`
#echo $VERSION
OS=slackware

TMP=tmp
DUNE_DIR=white_dune-$VERSION
INSTALL_DIR=install
DESCRIPTION_DIR=files/$INSTALL_DIR
PACKAGE_DIR=packager/$OS
TMP_DIR=$TMP/$DUNE_DIR/
DOCS_DIR=docs
DUNE_BINARY="*dune*"
ILLEGAL_BINARY="illegal2vrml"
DUNE_MAN="dune.1"
ILLEGAL_MAN="illegal2vrml.1"
PACKAGE_NAME=white_dune-$VERSION-i386-1.tgz

ADD_FILES="COPYING.txt INSTALL.txt README.txt"

cd ../..

sh batch/fix_not_translated_rcfiles.sh
make realclean
 ./configure --with-oldmotif --without-optbigfiles \
 --with-wwwbrowser=mozilla \
 --with-helpurl="/usr/doc/$DUNE_DIR" \
 --with-protobaseurl="/usr/doc/$DUNE_DIR" \
&& make

rm -fr /$TMP_DIR

install -c -d /$TMP_DIR/$INSTALL_DIR /$TMP_DIR/usr/bin  /$TMP_DIR/usr/doc/$DUNE_DIR  /$TMP_DIR/usr/man/man1
install -c -m 644 ./$PACKAGE_DIR/$DESCRIPTION_DIR/slack-desc /$TMP_DIR/$INSTALL_DIR/slack-desc

install -c -m 755 ./bin/$DUNE_BINARY  /$TMP_DIR/usr/bin
install -c -m 644 ./man/$DUNE_MAN /$TMP_DIR/usr/man/man1

install -c -m 755 ./bin/$ILLEGAL_BINARY  /$TMP_DIR/usr/bin
install -c -m 644 ./man/$ILLEGAL_MAN /$TMP_DIR/usr/man/man1

install -c -m 644 ./$ADD_FILES /$TMP_DIR/usr/doc/$DUNE_DIR

cp -fr  $DOCS_DIR/* /$TMP_DIR/usr/doc/$DUNE_DIR

#$PACKAGE_DIR/$P_FILES/*

cd /$TMP_DIR

cat >/$TMP_DIR/$INSTALL_DIR/doinst.sh <<EOT
#!/bin/sh
echo $PACKAGE_NAME: X3DV/VRML97 editor, animation and simple 3D Modeller
EOT

chmod 755 /$TMP_DIR/$INSTALL_DIR/doinst.sh

echo n | /sbin/makepkg $PACKAGE_NAME

mv /$TMP_DIR/$PACKAGE_NAME /tmp && rm -fr /$TMP_DIR

echo 
echo Now type "'installpkg /$TMP/$PACKAGE_NAME'" to install white_dune on your system
echo 

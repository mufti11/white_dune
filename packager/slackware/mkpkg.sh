#!/bin/sh

#########################################################################
# Author - Maksim Diachkov						#
# Please send error reports and suggestions to madj0388@uosis.mif.vu.lt	#
# Slackware 9.1 revision						#
# todo:	check $TMP for sufficient rights, check for *pkg avaliability	#
#########################################################################


VERSION=`sh ../../batch/getversion.sh` &&
OS=slackware &&

TMP=tmp &&

DUNE_DIR=white_dune-$VERSION  &&
INSTALL_DIR=install &&
DESCRIPTION_DIR=files/$INSTALL_DIR &&
PACKAGE_DIR=packager/$OS &&
TMP_DIR=$TMP/$DUNE_DIR/ &&
DOCS_DIR=docs &&
DUNE_BINARY="*dune*" &&
DUNE_MAN="dune.1" &&
PACKAGE_NAME=white_dune-$VERSION-`uname -m`-1.tgz &&

ADD_FILES="COPYING.txt INSTALL.txt README.txt" &&

cd ../.. &&

sh batch/fix_not_translated_rcfiles.sh &&
make realclean &&
./configure --with-optimization --with-wwwbrowser=mozilla \
 --with-helpurl="/usr/doc/$DUNE_DIR" \
 --with-protobaseurl="/usr/doc/$DUNE_DIR" \
&& make -j4 &&

rm -fr /$TMP_DIR &&

install -c -d /$TMP_DIR/$INSTALL_DIR /$TMP_DIR/usr/bin  /$TMP_DIR/usr/doc/$DUNE_DIR  /$TMP_DIR/usr/man/man1 &&
install -c -m 644 ./$PACKAGE_DIR/$DESCRIPTION_DIR/slack-desc /$TMP_DIR/$INSTALL_DIR/slack-desc &&

install -c -m 755 ./bin/$DUNE_BINARY /$TMP_DIR/usr/bin &&
install -c -m 644 ./man/$DUNE_MAN /$TMP_DIR/usr/man/man1 &&
find /$TMP_DIR/usr/man/man1 -type f -exec gzip -9 {} \; &&

install -c -m 644 ./$ADD_FILES /$TMP_DIR/usr/doc/$DUNE_DIR &&

cp -fr  $DOCS_DIR/* /$TMP_DIR/usr/doc/$DUNE_DIR &&

cd /$TMP_DIR &&

/sbin/makepkg -l y -c n ../$PACKAGE_NAME &&

rm -fr /$TMP_DIR &&

echo  &&
echo "Now type as root 'installpkg /$TMP/$PACKAGE_NAME' to install white_dune on your system" &&
echo 


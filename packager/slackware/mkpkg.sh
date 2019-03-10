#!/bin/sh

# Slackware build script for white_dune

# Redistribution and use of this script, with or without modification, is
# permitted provided that the following conditions are met:
#
# 1. Redistributions of this script must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#########################################################################
# Author - Maksim Diachkov						#
# Please send error reports and suggestions to madj0388@uosis.mif.vu.lt	#
# Slackware 9.1 revision						#
# todo:	check $TMP for sufficient rights, check for *pkg avaliability	#
#########################################################################

# the newest version of white_dune is available at https://wdune.ourproject.org/

set -e

VERSION=`sh ../../batch/getversion.sh`
cp -r ../../../wdune-$VERSION ../../../white_dune-$VERSION
cp -r $HOME/vcglib ../../../white_dune-$VERSION
(cd ../../../white_dune-$VERSION && make realclean)
sh ../../batch/mktargzwhite_dune.sh
cd /tmp
MD5SUM=`md5sum white_dune-$VERSION.tar.gz | awk '{print $1}'` 
cd -

cat > white_dune/white_dune.info << EOT
PRGNAM="white_dune"
VERSION="$VERSION"
HOMEPAGE="https://wdune.ourproject.org/"
DOWNLOAD="ftp://ftp.ourproject.org/pub/white_dune-$VERSION.tar.gz"
MD5SUM="$MD5SUM"
DOWNLOAD_x86_64="ftp://ftp.ourproject.org/pub/white_dune-$VERSION.tar.gz"
MD5SUM_x86_64="$MD5SUM"
REQUIRES=""
MAINTAINER="J. Scheurich"
EMAIL="mufti11@web.de"
EOT

OS=slackware &&

TMP=tmp &&

DUNE_DIR=white_dune-$VERSION  &&
INSTALL_DIR=install &&
DESCRIPTION_DIR=files/$INSTALL_DIR &&
PACKAGE_DIR=packager/$OS &&
PKG=$TMP/$DUNE_DIR/ &&
DOCS_DIR=docs &&
DUNE_BINARY="*dune*" &&
DUNE_MAN="dune.1" &&
PACKAGE_NAME=white_dune-$VERSION-`uname -m`-1.tgz &&

ADD_FILES="COPYING INSTALL.txt README.txt" &&

cd /tmp && gzip -cd /tmp/white_dune-$VERSION.tar.gz | tar -xf - &&

cd /tmp/white_dune-$VERSION &&

sh batch/fix_not_translated_rcfiles.sh &&
make realclean &&
./configure --with-optimization --with-wwwbrowser=firefox \
 --with-helpurl="/usr/doc/$DUNE_DIR" \
 --with-protobaseurl="/usr/doc/$DUNE_DIR" \
&& make -j4 &&

rm -fr /$PKG &&

install -c -d /$PKG/$INSTALL_DIR /$PKG/usr/bin  /$PKG/usr/doc/$DUNE_DIR  /$PKG/usr/man/man1 &&
install -c -d /$PKG/usr/share/applications /$PKG/usr/share/pixmaps &&
install -c -m 644 ./$PACKAGE_DIR/$DESCRIPTION_DIR/slack-desc /$PKG/$INSTALL_DIR/slack-desc &&

install -c -m 755 ./bin/$DUNE_BINARY /$PKG/usr/bin &&
install -c -m 644 ./man/$DUNE_MAN /$PKG/usr/man/man1 &&
find /$PKG/usr/man/man1 -type f -exec gzip -9 {} \; &&

install -c -m 644 ./$ADD_FILES /$PKG/usr/doc/$DUNE_DIR &&

cp -fr  $DOCS_DIR/* /$PKG/usr/doc/$DUNE_DIR &&

install -D -m 0644 ./desktop/kde/dune.desktop \
        /$PKG/usr/share/applications/dune.desktop
install -D -m 0644 ./desktop/kde/dune.png \
        /$PKG/usr/share/pixmaps/dune.png

cd /$PKG &&

/sbin/makepkg -l y -c n ../$PACKAGE_NAME &&

rm -fr /$PKG &&

echo  &&
echo "Now type as root 'installpkg /$TMP/$PACKAGE_NAME' to install white_dune on your system" &&
echo 


#!/bin/sh

set -x

cd `dirname $0`

make clean

pkgname=wdune
pkgver=`sh ../../batch/getversion.sh`

export name=white_dune
name=white_dune

sh ../../batch/mktarbz2.sh > /dev/null
cp /tmp/$pkgname-$pkgver.tar.bz2 .
export MD5SUM=`md5sum $pkgname-$pkgver.tar.bz2 | awk '{ print $1 }'`

cat > PKGBUILD << EOT
# Maintainer: J. Scheurich <mufti11@web.de>
pkgname=$pkgname
pkgver=$pkgver
pkgrel=1
epoch=
pkgdesc="white_dune X3D/VRML97 tool"
arch=()
url="ftp://ftp.ourproject.org/pub/wdune/\$pkgname-\$pkgver.tar.bz2"
license=('GPL')
groups=()
depends=(xorg-fonts-misc
         xorg-fonts-alias
)
makedepends=(gcc
             fakeroot
             glu 
             mesa 
             libx11 
             libxt 
             libxmu 
             libxext
             libxi
             libxp
             openmotif
             libpng 
             libjpeg-turbo
             zlib
             expat
             ffmpeg
             opensubdiv
             gcc
             flex
             bison
             gawk
             m4
             make
             rcs
             xdg-utils
             gedit
             lxterminal
             gimp
             audacity
#             cinelerra-cv #not available under ArchLinuxARM
             kolourpaint
             imagemagick
             netpbm
             freeglut
             jdk8-openjdk
             cgal
             boost
             curl
             freetype2
             ttf-bitstream-vera             
             xorg-fonts-misc
             xorg-fonts-alias
)
checkdepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=("\$pkgname-\$pkgver.tar.bz2")
noextract=()
md5sums=($MD5SUM)
validpgpkeys=()

build() {
	cd "\$pkgname-\$pkgver"
	./configure --prefix=/usr --without-devil --with-uninstallcomment="pacman -R white_dune" --with-helpurl="/usr/share/doc/\$name/docs" --with-protobaseurl="/usr/share/doc/\$name/docs" --with-checkincommand="ci" --with-partial-fortify 
        (cd src && make duneLowMem)
}

package() {
	cd "\$pkgname-\$pkgver"
        install -Dm755 bin/dune \$pkgdir/usr/bin/dune 
        mkdir -p "\$pkgdir/usr/share/doc/\$name/docs"
        cp -r "docs" "\$pkgdir/usr/share/doc/\$name/"
        install -Dm644 desktop/kde/dune.desktop \$pkgdir/usr/share/applications/dune.desktop
	install -Dm644 desktop/kde/dune.png \$pkgdir/usr/share/pixmaps/dune.png
	install -Dm644 desktop/kde/dune4kids.desktop \$pkgdir/usr/share/applications/dune4kids.desktop
	install -Dm644 desktop/kde/dune4kids.png \$pkgdir/usr/share/pixmaps/dune4kids.png
}
EOT

updpkgsums
makepkg -Ac

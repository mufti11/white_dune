# Copyright 1999-2003 Gentoo Technologies, Inc.
# Distributed under the terms of the GNU General Public License v2
#
# J. "MUFTI" Scheurich (tips from Torben Hohn)
#
# you can also use x11-libs/lesstif instead of x11-libs/openmotif
# but it looks like there is no "virtual/motif" in the current gentoo 
# distribution 8-(

S=${WORKDIR}/${P}
DESCRIPTION="graphical X3DV/VRML editor, simple NURBS 3D modeller and animation tool"
SRC_URI=`echo http://wdune.ourproject.org/${PN}-${PV}.tar.gz | awk '{gsub(".tar.gz","-gentoo.tar.gz");print $0}'`
HOMEPAGE="http://wdune.ourproject.org"
KEYWORDS="x86 ppc sparc sparc64"
SLOT="0"
LICENSE="GPL-2"
IUSE=""
DEPEND="media-libs/libpng
        media-libs/jpeg                   
        sys-libs/zlib 
        virtual/opengl 
        virtual/glu 
        virtual/glut
        virtual/x11
        x11-libs/openmotif
        app-text/rcs"

src_compile() {
        cd $S
	local myconf
	myconf="--with-optimization --without-devil --with-vrmlbrowser=mozilla --with-helpurl=/usr/share/doc/${PN} --with-protobaseurl=/usr/share/doc/${PN}"
	econf ${myconf} || die "configure failure" 
	emake || die "make failure"
}

src_install() {
        exeinto /usr/bin 
        doexe bin/dune
        doexe bin/dune4kids
        doexe bin/illegal2vrml

        doman man/dune.1
        doman man/illegal2vrml.1
	dodoc docs COPYING.txt README.txt INSTALL.txt
        cp -R docs ${D}/usr/share/doc/${PN}
}

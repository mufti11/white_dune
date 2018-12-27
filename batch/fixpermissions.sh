#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

# fix unix filepermissions
# this is important, if the white_dune file tree come back from a 
# different filesystem without unix filepermissions (e.g. M$Windows)

if test "`uname`" = "CYGWIN_NT-5.0" ; then
    # unix filepermissions are useless under CYGWIN / M$Windows
    exit 0
fi

DUNE=`dirname $0`/..

find $DUNE -type f -print0 | xargs -0 chmod 644 
find $DUNE -type d -print0 | xargs -0 chmod 755

chmod a+x $DUNE/build*.sh
chmod a+x $DUNE/configure

chmod a+x $DUNE/bin/*

chmod a+x $DUNE/tools/*.sh

chmod a+x $DUNE/batch/*.sh
chmod a+x $DUNE/batch/*.bat
chmod a+x $DUNE/batch/gdb*start
chmod a+x $DUNE/batch/dbxstart

chmod a+x $DUNE/buildscripts/*.sh

if test -d $DUNE/desktop/irix ; then
   chmod a+x $DUNE/desktop/irix/data/cosmo2vrml*
   chmod a+x $DUNE/desktop/irix/irix*/fm_cosmo2vrml97
fi

if test -d $DUNE/desktop/macosx/white_dune.app ; then
   chmod a+x $DUNE/desktop/macosx/white_dune.app/Contents/MacOS/dunestarter
   chmod a+x $DUNE/desktop/macosx/white_dune.app/Contents/MacOS/white_dune
fi

chmod a+x $DUNE/docs/commandline_examples/*.sh

chmod a+x $DUNE/docs/developer_docs/man2html.sh

chmod a+x $DUNE/packager/debian/*.sh
chmod a+x $DUNE/packager/debian/mkinstalldirs
chmod a+x $DUNE/packager/debian/postinst
chmod a+x $DUNE/packager/debian/prerm
chmod a+x $DUNE/packager/debian/rules

chmod a+x $DUNE/packager/aix/mkbff.sh

chmod a+x $DUNE/packager/debian/mkdeb_wheezy.sh

chmod a+x $DUNE/packager/freebsd/mkpkg.sh

chmod a+x $DUNE/packager/irix/mkpkg.sh

chmod a+x $DUNE/packager/macosx/mksit.sh

chmod a+x $DUNE/packager/rpm/mkrpm.sh

chmod a+x $DUNE/packager/slackware/mkpkg.sh

chmod a+x $DUNE/packager/solaris/mkpkg.sh

if test -f $DUNE/src/swt/rc/rc ; then
   chmod a+x $DUNE/src/swt/rc/rc
fi 

if test -d $DUNE/debian ; then
   chmod a+x $DUNE/debian/mkinstalldirs
   chmod a+x $DUNE/debian/postinst
   chmod a+x $DUNE/debian/prerm
   chmod a+x $DUNE/debian/rules
fi

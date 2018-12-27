#!/bin/sh

VERSION=`sh ../../batch/getversion.sh`

for i in "beta" "pre" "pl" ; do
   if test "$MAYOR" = "" ; then
     MAYOR=`awk -v version=$VERSION -v col=$i 'BEGIN {split(version,a,col);print a[1]}'`
   fi
   if test "$MINOR" = "" ; then
     MINOR=`awk -v version=$VERSION -v col=$i 'BEGIN {split(version,a,col);print a[2]}'` 
   fi
done

INTVERSION=`echo $MAYOR | awk '{print substr($0,3);}`



DUNE_VERSION=white_dune-$VERSION

BASEPWD=/tmp/compile_$DUNE_VERSION/$DUNE_VERSION
mkdir -p $BASEPWD

(cd ../../.. && \
 rm -rf $BASEPWD && \
 cp -r $DUNE_VERSION $BASEPWD && \
 cd $BASEPWD && \
 make realclean && \
 ./configure  --without-devil --with-oldmotif --with-optimization \
  --with-helpurl="/usr/freeware/doc/white_dune/docs" \
  --with-protobaseurl="/usr/freeware/share/dune" \
&& /sbin/make -P)

mkdir -p /tmp/dist/fw_$DUNE_VERSION

cat > /tmp/fw_white_dune_$$.idb <<EOT
f 0644 root sys usr/freeware/bin/cosmo2vrml $BASEPWD/desktop/irix/data/cosmo2vrml fw_white_dune.sw.cosmoworldsconverter
f 0755 root sys usr/freeware/bin/dune $BASEPWD/bin/dune fw_white_dune.sw.base
f 0755 root sys usr/freeware/bin/dune4kids $BASEPWD/bin/dune4kids fw_white_dune.sw.base
f 0755 root sys usr/freeware/bin/fm_cosmo2vrml97 $BASEPWD/desktop/irix/irix6.5_global/fm_cosmo2vrml97 fw_white_dune.sw.cosmoworldsconverter
f 0755 root sys usr/freeware/bin/illegal2vrml $BASEPWD/bin/illegal2vrml fw_white_dune.sw.base
f 0755 root sys usr/freeware/bin/nostereodune $BASEPWD/bin/nostereodune fw_white_dune.sw.base
EOT

for i in vrml97Amendment1 x3d scripted_Nodes coverNodes exportContainers; do 
    (
    cd ../../docs/$i && ls *PROTO.wrl | \
    awk -v basepwd=$BASEPWD -v dirpwd=docs/$i \
    '{print "f 0644 root sys usr/freeware/share/dune/" $1 " "  \
           basepwd "/" dirpwd "/" $1 " fw_white_dune.sw.base" }'
    )
done | sort -k 5 >> /tmp/fw_white_dune_$$.idb 


(cd ../.. && find docs -type d -print | grep -v developer_docs | \
awk -v basepwd=$BASEPWD \
'{print "d 0755 root sys usr/freeware/doc/white_dune/" $1 " "  \
         basepwd "/" $1 " fw_white_dune.books.htmlusage" }' \
) >> /tmp/fw_white_dune_$$.idb

(cd ../.. && find docs -type f -print | grep -v developer_docs | \
awk -v basepwd=$BASEPWD \
'{print "f 0644 root sys usr/freeware/doc/white_dune/" $1 " "  \
         basepwd "/" $1 " fw_white_dune.books.htmlusage" }' \
) >> /tmp/fw_white_dune_$$.idb

(cd ../.. && find docs -type d -print | grep developer_docs | \
awk -v basepwd=$BASEPWD \
'{print "d 0755 root sys usr/freeware/doc/white_dune/" $1 " "  \
         basepwd "/" $1 " fw_white_dune.books.htmldeveloper" }' \
) >> /tmp/fw_white_dune_$$.idb

(cd ../.. && find docs -type f -print | grep developer_docs | \
awk -v basepwd=$BASEPWD \
'{print "f 0644 root sys usr/freeware/doc/white_dune/" $1 " "  \
         basepwd "/" $1 " fw_white_dune.books.htmldeveloper" }' \
) >> /tmp/fw_white_dune_$$.idb

cat >> /tmp/fw_white_dune_$$.idb <<EOT
f 0644 root sys usr/freeware/man/man1/dune.1 $BASEPWD/man/dune.1 fw_white_dune.man.manpage
f 0644 root sys usr/freeware/man/man1/illegal2vrml.1 $BASEPWD/man/illegal2vrml.1 fw_white_dune.man.manpage
f 0644 root sys usr/freeware/packager/irix/relnotes/Img/dunelogo.jpg $BASEPWD/packager/irix/relnotes/Img/dunelogo.jpg fw_white_dune.man.relnotes
f 0644 root sys usr/freeware/packager/irix/relnotes/white_dune.html $BASEPWD/packager/irix/relnotes/white_dune.html fw_white_dune.man.relnotes
EOT

cat >> /tmp/fw_white_dune_$$.idb <<EOT
f 0644 root sys usr/freeware/share/dune/dune.wrl $BASEPWD/logo/dune.wrl fw_white_dune.sw.examples
f 0644 root sys usr/freeware/share/dune/flow2.jpg $BASEPWD/logo/flow2.jpg fw_white_dune.sw.examples
f 0644 root sys usr/freeware/share/dune/manta.wrl $BASEPWD/logo/manta.wrl fw_white_dune.sw.examples
f 0644 root sys usr/freeware/share/dune/manta2.wrl $BASEPWD/logo/manta2.wrl fw_white_dune.sw.examples
f 0644 root sys usr/freeware/share/dune/white_dune_text.png $BASEPWD/logo/white_dune_text.png fw_white_dune.sw.examples
f 0644 root sys usr/freeware/share/dune/whitesand.jpg $BASEPWD/logo/whitesand.jpg fw_white_dune.sw.examples
f 0644 root sys usr/freeware/src/dune/fw_$DUNE_VERSION.tar.gz tmp/fw_$DUNE_VERSION.tar.gz fw_white_dune.src.white_dune
f 0644 root sys usr/lib/filetype/install/Dune.ftr $BASEPWD/desktop/irix/irix6.5_global/Dune.ftr fw_white_dune.sw.desktop postop(cd /usr/lib/filetype && make) removeop(cd /usr/lib/filetype && make)
f 0644 root sys usr/lib/filetype/install/iconlib/Dune.fti $BASEPWD/desktop/irix/data/Dune.fti fw_white_dune.sw.desktop postop(cd /usr/lib/filetype && make) removeop(cd /usr/lib/filetype && make)
f 0644 root sys usr/lib/filetype/install/iconlib/Dune.open.fti $BASEPWD/desktop/irix/data/Dune.open.fti fw_white_dune.sw.desktop postop(cd /usr/lib/filetype && make) removeop(cd /usr/lib/filetype && make)
f 0644 root sys usr/lib/filetype/install/iconlib/Dune4kids.fti $BASEPWD/desktop/irix/data/Dune4kids.fti fw_white_dune.sw.desktop postop(cd /usr/lib/filetype && make) removeop(cd /usr/lib/filetype && make)
f 0644 root sys usr/lib/filetype/install/iconlib/Dune4kids.open.fti $BASEPWD/desktop/irix/data/Dune4kids.open.fti fw_white_dune.sw.desktop postop(cd /usr/lib/filetype && make) removeop(cd /usr/lib/filetype && make)
f 0644 root sys usr/lib/filetype/install/iconlib/Dunebackground.fti $BASEPWD/desktop/irix/data/Dunebackground.fti fw_white_dune.sw.desktop postop(cd /usr/lib/filetype && make) removeop(cd /usr/lib/filetype && make)
f 0644 root sys usr/lib/filetype/install/iconlib/nostereoDune.fti $BASEPWD/desktop/irix/data/nostereoDune.fti fw_white_dune.sw.desktop postop(cd /usr/lib/filetype && make) removeop(cd /usr/lib/filetype && make)
f 0644 root sys usr/lib/filetype/install/iconlib/nostereoDune.open.fti $BASEPWD/desktop/irix/data/nostereoDune.open.fti fw_white_dune.sw.desktop postop(cd /usr/lib/filetype && make) removeop(cd /usr/lib/filetype && make)
f 0644 root sys usr/lib/images/Dune.icon $BASEPWD/desktop/irix/data/Dune.icon fw_white_dune.sw.desktop postop(cd /usr/lib/filetype && make) removeop(cd /usr/lib/filetype && make)
EOT

sort -k 5 /tmp/fw_white_dune_$$.idb > /tmp/fw_white_dune.idb 

cat > /tmp/fw_white_dune.spec <<EOT
product fw_white_dune
    id "fw_white_dune $MAYOR"
    image sw
        id "$DUNE_VERSION Software"
        version $INTVERSION
        order 9999
        subsys base default
            id "$DUNE_VERSION execution only env"
            replaces self
            prereq (
                compiler_eoe.sw32.lib 1232792133 1235539210
                motif_eoe.sw32.eoe 1232792120 1232792120
                x_eoe.sw32.eoe 1232792130 1233007720
                eoe.sw.base 1274627340 1274627340
                eoe.sw.gfx 1 maxint
                c++_eoe.sw.lib 1274567300 1277790710
                fw_libjpeg.sw.lib 1235000000 1239999900
                fw_libpng.sw.lib 1235000000 1239999900
                fw_libz.sw.lib 1235000000 1239999900
            )
            prereq (
                motif_eoe.sw.eoe 1253422545 1253422545
                x_eoe.sw32.eoe 1253422531 1253422531
                x_eoe.sw.eoe 1274627333 1274627333
                eoe.sw.base 1274627340 1274627340
                c++_eoe.sw.lib 1274567300 1277790710
                fw_libjpeg.sw.lib 1235000000 1239999900
                fw_libpng.sw.lib 1235000000 1239999900
                fw_libz.sw.lib 1235000000 1239999900
            )
            prereq (
                motif_eoe.sw.eoe 1263370532 1263370532
                x_eoe.sw.eoe 1263561138 1263561138
                eoe.sw.gfx 1263561140 1263561140
                eoe.sw.base 1274627340 1274627340
                c++_eoe.sw.lib 1274567300 1277790710
                fw_libjpeg.sw.lib 1235000000 1239999900
                fw_libpng.sw.lib 1235000000 1239999900
                fw_libz.sw.lib 1235000000 1239999900
            )
            prereq (
                motif_eoe.sw.eoe 1274627333 1274627333
                x_eoe.sw.eoe 1274627333 1274627333
                eoe.sw.gfx 1274627340 1274627340
                eoe.sw.base 1274627340 1274627340
                c++_eoe.sw.lib 1274567300 1277790710
                fw_libjpeg.sw.lib 1235000000 1239999900
                fw_libpng.sw.lib 1235000000 1239999900
                fw_libz.sw.lib 1235000000 1239999900
            )
            exp fw_white_dune.sw.base
        endsubsys
        subsys desktop
            id "$DUNE_VERSION desktop icons"
            replaces self
            prereq (
                desktop_eoe.sw.envm 1 maxint
                fw_white_dune.sw.base $INTVERSION maxint
            )
            exp fw_white_dune.sw.desktop
        endsubsys
        subsys examples
            id "$DUNE_VERSION examples"
            replaces self
            exp fw_white_dune.sw.examples
        endsubsys
        subsys cosmoworldsconverter
            id "$DUNE_VERSION import desktop icon and script to convert cosmoworlds 1.03 output to a VRML97 file"
            replaces self
            prereq (
                fw_white_dune.sw.desktop $INTVERSION maxint
                imgtools.sw.tools 1 maxint
                eoe.sw.base 1 maxint
                desktop_eoe.sw.envm 1 maxint
                cosmoworlds.sw.cosmoworlds 1234684420 1234684420
            )
            exp fw_white_dune.sw.cosmoworldsconverter
        endsubsys
    endimage
    image man
        id "$DUNE_VERSION Man Pages"
        version $INTVERSION
        order 9999
        subsys manpage default
            id "$DUNE_VERSION man page"
            replaces self
            exp fw_white_dune.man.manpage
        endsubsys
        subsys relnotes
            id "$DUNE_VERSION Release Notes"
            replaces self
            prereq (
                fw_common.man.legal 1234723321 1239999900
            )
            exp fw_white_dune.man.relnotes
        endsubsys
    endimage
    image books
        id "$DUNE_VERSION Books"
        version $INTVERSION
        order 9999
        subsys htmlusage default
            id "white_dune-0.20 html usage documentation"
            replaces self
            exp fw_white_dune.books.htmlusage
        endsubsys
        subsys htmldeveloper
            id "$DUNE_VERSION developer documentation"
            replaces self
            prereq (
                fw_white_dune.src.white_dune 22 maxint
            )
            exp fw_white_dune.books.htmldeveloper
        endsubsys
    endimage
    image src
        id "$DUNE_VERSION source"
        version $INTVERSION
        order 9999
        subsys white_dune
            id "$DUNE_VERSION source"
            replaces self
            exp fw_white_dune.src.white_dune
        endsubsys
    endimage
endproduct
EOT

(cd ../../.. && rm -rf /tmp/$DUNE_VERSION /tmp/fw_$DUNE_VERSION && \
 cp -r $DUNE_VERSION /tmp && mv /tmp/$DUNE_VERSION /tmp/fw_$DUNE_VERSION && \
 cd /tmp/fw_$DUNE_VERSION && 
 sh batch/fix_not_translated_rcfiles.sh &
 make realclean > /dev/null && \
 rm -rf desktop/macosx docs packager/[L-f]* packager/[r-s]* logo && \
 cd /tmp && /usr/bin/tar -cf /tmp/fw_$DUNE_VERSION.tar fw_$DUNE_VERSION && \
 rm -f /tmp/fw_$DUNE_VERSION.tar.gz && gzip -9 /tmp/fw_$DUNE_VERSION.tar \
) 
if test -x $BASEPWD/bin/dune ; then
    /usr/sbin/gendist -sbase / -idb /tmp/fw_white_dune.idb \
                  -spec /tmp/fw_white_dune.spec -dist \
                  /tmp/dist/fw_$DUNE_VERSION -verbose -all
else
   echo compilation failed 1>&2
   exit 1
fi
(cd /tmp/dist && /usr/bin/tar -cf /tmp/fw_$DUNE_VERSION.tardist fw_$DUNE_VERSION)
echo use \"swmgr -f /tmp/fw_$DUNE_VERSION.tardist\" or \"inst -f /tmp/fw_$DUNE_VERSION.tardist\" to install

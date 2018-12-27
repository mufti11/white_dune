#!/bin/sh

#set -x

MD5SUM=md5sum

VERSION=$1

if test "$1" = "" ; then
    echo usage: 1>&2
    echo $0 version  1>&2
    exit 1
fi

for i in "beta" "pre" "pl" ; do
   if test "$MINOR" = "" ; then
     MAYOR=`awk -v version=$VERSION -v col=$i 'BEGIN {split(version,a,col);print a[1]}'`
     LEVEL=$i
   fi
   if test "$MINOR" = "" ; then
     MINOR=`awk -v version=$VERSION -v col=$i 'BEGIN {split(version,a,col);print a[2]}'` 
   fi
done

SPARC_PKG=whitedune.$MAYOR.$MINOR.SPARC.pkg

AIX_MAYOR=`awk -v mayor=$MAYOR 'BEGIN {gsub("\\\.","",mayor); print mayor}'`

cat << EOT
<table width="100%" cellpadding="10" cellspacing="1" border="0">
EOT

cat << EOT
   <tr><td></td>
        <td valign="top" bgcolor="#DFEFF7" class="text">
EOT

FILE=white_dune-$VERSION.tar.gz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <b>source and documentation</b> .tar.gz file: <A 
		href="http://wdune.ourproject.org/$FILE">$FILE</A> 
                <br>
		$SIZE kB (MD5 checksum: $MD5)
EOT
   if test -f white_dune-$VERSION.exe ; then
      CURRENT=$PWD
      TARGZ=$CURRENT/white_dune-$VERSION.tar.gz
      cd /tmp
      gzip -cd $TARGZ | tar -xf -
      zip -rq white_dune-$VERSION.zip white_dune-$VERSION
      cd $CURRENT
      cp /tmp/white_dune-$VERSION.zip .
   fi
fi
MD5=
SIZE=
FILE=


FILE=white_dune-$VERSION.zip
if test -f $FILE ; then
   chmod a+r $FILE1;
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
   <br><br>
		white_dune $VERSION <b>source and documentation</b> .zip file: <A 
		href="http://wdune.ourproject.org/$FILE">$FILE</A> 
                <br>
		$SIZE kB (MD5 checksum: $MD5)
EOT
fi
MD5=
SIZE=
FILE=

cat << EOT
        </td>
    </tr> 
    <tr><td class="head" align="right" bgcolor="#dfeff7" valign="top">MacOSX
        </td> 
        <td valign="top" bgcolor="#FFFFFF" class="text">
EOT

FILE=white_dune-MacOSX-10.6-$VERSION.tar.gz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                <a name="macosx">
		white_dune </a>$VERSION <b>MacOSX</b> 10.6
                (Apple UNIX) stuffit expander compatible 
                <a href="http://wdune.ourproject.org/$FILE">
                $FILE</A>
	        fat binary package (*) 
                based on <a href="http://www.apple.com/macosx/x11/" target="_blank">
	        X11 for Mac OS X</a> with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)               
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune-MacOSX-Tiger-fat-binary-$VERSION.tar.gz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                <a name="macosx">
		white_dune </a>$VERSION <b>MacOSX</b> 10.4 "Tiger" 
                (Apple UNIX) stuffit expander compatible 
                <a href="http://wdune.ourproject.org/$FILE">
                $FILE</A>
	        fat binary package (*) 
                based on <a href="http://www.apple.com/macosx/x11/" target="_blank">
	        X11 for Mac OS X</a> with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)               
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune-MacOSX-Leopard-fat-binary-$VERSION.tar.gz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                <a name="macosx">
		white_dune </a>$VERSION <b>MacOSX</b> 10.5 "Leopard" 
                (Apple UNIX) expertimental stuffit expander compatible 
                <a href="http://wdune.ourproject.org/$FILE">
                $FILE</A>
	        fat binary package (*) 
                based on <a href="http://www.apple.com/macosx/x11/" target="_blank">
	        X11 for Mac OS X</a> with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)               
EOT
fi

MD5=
SIZE=
FILE=
FILE=white_dune-MacOSX-Lion-fat-binary-$VERSION.tar.gz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                <a name="macosx">
		white_dune </a>$VERSION <b>MacOSX</b> 10.7 "Lion" 
                (Apple UNIX) expertimental stuffit expander compatible 
                <a href="http://wdune.ourproject.org/$FILE">
                $FILE</A>
	        i386/x86_64 binary package (*) 
                based on <a href="http://www.apple.com/macosx/x11/" target="_blank">
	        X11 for Mac OS X</a> with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)               
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune-MacOSX-Tiger-powerpc-$VERSION.tar.gz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                <a name="macosx">
		white_dune </a>$VERSION <b>MacOSX</b> 10.4 "Tiger" 
                (Apple UNIX) stuffit expander compatible 
                <a href="http://wdune.ourproject.org/$FILE">
                $FILE</A>
	        fat binary package (*) tested on PowerPC 
                based on <a href="http://developer.apple.com/qa/qa2001/qa1232.html" target="_blank">
	        X11 for Mac OS X</a> with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)               
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune-MacOSX-Jaguar-$VERSION.tar.gz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
   <br><br>
		white_dune $VERSION <b>MacOSX</b> 10.2 "Jaguar" (Apple UNIX)
                stuffit expander compatible <a 
                href="http://wdune.ourproject.org/$FILE">
                $FILE</A>
	        package (*) based on <a href="http://www.apple.com/macosx/x11/" target="_blank">
	        X11 for Mac OS X</a> with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)               
EOT
fi
MD5=
SIZE=
FILE=

cat << EOT
        </td>
    </tr> 
    <tr><td class="head" align="right" bgcolor="#dfeff7" valign="top">
        M\$Windows</td>
        <td valign="top" bgcolor="#DFEFF7" class="text">
EOT

FILE=white_dune-$VERSION.exe
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   MD5LIB=`$MD5SUM < expat_jpeg_png_zlib_win32static2_vc6.zip`
   MD5LIB3=`$MD5SUM < expat_jpeg_png_zlib_win32static3.zip`
   cat << EOT
                white_dune $VERSION <b>M\$Windows</b> binary only: <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A> <br>
                $SIZE kB (MD5 checksum: $MD5) <br>
                This version is linked against static libraries. For easy 
                recompilation you can download the archives with 
                sources/libraries/project files for <b>M\$Windows</b> 
                from the following link<br>
                <a href="expat_jpeg_png_zlib_win32static4.zip">expat_jpeg_png_zlib_win32static4.zip</a> (MD5 checksum: $MD5LIB3)
                <br>
                (Version for older compiler: <a href="expat_jpeg_png_zlib_win32static2_vc6.zip">expat_jpeg_png_zlib_win32static2_vc6.zip</a> (MD5 checksum: $MD5LIB))
                <br>
EOT
mkdir -p /tmp/tmp
rm /tmp/tmp/*
echo .\\$FILE -german > /tmp/tmp/dunegerman.bat
echo .\\$FILE -4kids > /tmp/tmp/dune4kids.bat
echo .\\$FILE -cover > /tmp/tmp/cover_dune.bat
echo .\\$FILE -anaglyph red_green > /tmp/tmp/red_green.bat
echo .\\$FILE -anaglyph red_blue > /tmp/tmp/red_blue.bat
echo .\\$FILE -anaglyph green_red > /tmp/tmp/green_red.bat
echo .\\$FILE -anaglyph blue_red > /tmp/tmp/blue_red.bat
echo .\\$FILE -stereo > /tmp/tmp/stereo.bat
echo .\\$FILE -joystick 0 -axes=2 > /tmp/tmp/2Djoystick.bat
echo .\\$FILE -4catt > /tmp/tmp/dune4catt.bat
echo 'set DUNERC=%~d0:\dune.rc' > /tmp/tmp/dune_stick.bat
echo .\\$FILE >> /tmp/tmp/dune_stick.bat
cp dune4catt.ico  dune4kids.ico  stereodune.ico /tmp/tmp
cp $FILE /tmp/tmp
cat > /tmp/tmp/setup.sh << EOT 
#!/bin/sh

# this script requires cygwin, see http://www.cygwin.com

mkdir -p /cygdrive/c/programs/white_dune &&
cp white_dune-$1.exe /cygdrive/c/programs/white_dune &&
mkshortcut -A -P   -d '"white_dune vrml97 editor"' -n "white_dune" "/c:/programs/white_dune/white_dune-$1.exe" &&
cp dune4catt.ico /cygdrive/c/programs/white_dune &&
mkshortcut -A -P  -a "-4catt" -d '"white_dune for catt8/ac3d export"' -n "dune4catt" -i "c:\\\\programs\\\\white_dune\\\\dune4catt.ico" "/c:/programs/white_dune/white_dune-$1.exe" &&
cp dune4kids.ico /cygdrive/c/programs/white_dune &&
mkshortcut -A -P  -a "-4kids" -d '"white_dune for kids"' -n "dune4kids" -i "c:\\\\programs\\\\white_dune\\\\dune4kids.ico" "/c:/programs/white_dune/white_dune-$1.exe" &&
cp stereodune.ico /cygdrive/c/programs/white_dune &&
mkshortcut -A -P  -a "-stereo" -d '"white_dune for quadbuffer stereoview"' -n "stereodune" -i "c:\\\\programs\\\\white_dune\\\\stereodune.ico" "/c:/programs/white_dune/white_dune-$1.exe"

EOT
chmod 755 /tmp/tmp/setup.sh
DIR=$PWD
(cd /tmp/tmp && sh -c 'zip -q $DIR/batchfiles.zip *')
MD5=`$MD5SUM < batchfiles.zip`
echo                 "archive of <a href=batchfiles.zip>experimental batch files</a>" '('MD5 checksum: $MD5')'
echo "<br><br>"
cp /tmp/tmp/dunegerman.bat .
FILE=dunegerman.bat
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                <img src=docs/deutsch.jpg>Seit version 0.29beta319 gibt es
                alle Sprachen in white_dune nur noch in einer Datei.
                Downloaden Sie zus&auml;tzlich zur normalen Version die 
                folgende <b>M\$Windows</b> Batchdatei
                <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A> um white_dune in deutscher Sprache starten zu koennen
                <br>
                $SIZE kB (MD5 checksum: $MD5)
<br><br>
EOT
fi
fi
MD5=
SIZE=
FILE=

FILE=white_dune-$VERSION""XP.exe
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                white_dune $VERSION <b>XP (and higher)</b> binary only
                with support for Space Navigator devices 
                <A href="http://wdune.ourproject.org/$FILE">
                $FILE</A> <br>
                $SIZE kB (MD5 checksum: $MD5) <br>
EOT
fi
MD5=
SIZE=
FILE=
				
FILE=white_dune-$VERSION-devil.exe
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                white_dune $VERSION <b>M\$Windows</b> binary only: <A 
                href="http://wdune.ourproject.org/$FILE">
                $FILE</A>
                (need <a href="http://openil.sourceforge.net/download.php" target="new">DevIL-1.6.5</a>	
                <a href="http://prdownloads.sourceforge.net/openil/DevIL-EndUser-1.6.5.zip" target="new">dll's</a> 
                e.g. in the same directory) 
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
fi
MD5=
SIZE=
FILE=


FILE=white_dune-$VERSION-4kids.exe
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                white_dune $VERSION <b>M\$Windows</b> binary only with a very simple NURBS 
                modeller
                menu for kids: <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A> 
                (need <a href="http://openil.sourceforge.net/download.php" target="new">DevIL-1.6.5</a>	
                <a href="http://prdownloads.sourceforge.net/openil/DevIL-EndUser-1.6.5.zip" target="new">dll's</a> 
                e.g. in the same directory) 
                <br>
                $SIZE kB (MD5 checksum: $MD5)
                
EOT
fi
MD5=
SIZE=
FILE=


FILE=white_dune-$VERSION-4germankids.exe
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                <img src=docs/deutsch.jpg>white_dune $VERSION ausf&uuml;hrbares <b>M\$Windows</b> Programm 
                mit sehr einfachem NURBS Modelling Men&uuml;
                in deutscher Sprache fuer junge Leute:
                <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A> 
                (ben&ouml;tigt <a href="http://openil.sourceforge.net/download.php" target="new">DevIL-1.6.5</a>	
                <a href="http://prdownloads.sourceforge.net/openil/DevIL-EndUser-1.6.5.zip" target="new">dll's</a> 
                zum Beispiel im gleichen Ordner wie das Programm)
                <br>
                $SIZE kB (MD5 checksum: $MD5)
EOT
fi
MD5=
SIZE=
FILE=

FILE=red_green_white_dune-$VERSION.exe
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                white_dune with experimental support for red/green anaglyph glasses $VERSION <b>M\$Windows</b> binary only: <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A> <br>
                $SIZE kB (MD5 checksum: $MD5) <br>
                This version is linked against static libraries. 
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=

FILE=red_blue_white_dune-$VERSION.exe
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                white_dune with experimental support for red/blue anaglyph glasses $VERSION <b>M\$Windows</b> binary only: <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A> <br>
                $SIZE kB (MD5 checksum: $MD5) <br>
                This version is linked against static libraries. 
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=

FILE=black_n_white_dune-$VERSION.exe
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                white_dune $VERSION <b>M\$Windows</b> binary only with Bart's black and white icons: <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A> 
                $SIZE kB (MD5 checksum: $MD5)
EOT
fi
MD5=
SIZE=
FILE=



FILE=white_dune$AIX_MAYOR$LEVEL$MINOR-$MAYOR.0.0.bff
if test -f $FILE.gz ; then
cat << EOT
        </td>
    </tr> 
    <tr><td></td>
        <td valign="top" bgcolor="#FFFFFF" class="text">
EOT   chmod a+r $FILE.gz
   MD5=`$MD5SUM < $FILE.gz`
   SIZE=`ls -l $FILE.gz | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <b>AIX</b> (IBM UNIX) 4.3 (RS/6000)
                <b>gzip -cd <a 
                href="http://wdune.ourproject.org/$FILE.gz">
                $FILE.gz</A> && installp -a -d $FILE all</b>
	        package (*)</a> with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)<br>
                This version is linked against dynamic free software libraries from
                <a href="http://www.bullfreeware.com/download/aix43/" target="_blank">
                http://www.bullfreeware.com/download/aix43/</a> <br>
                <ul>
                <li><a href="http://www.bullfreeware.com/download/aix43/gcc-3.0.1.0.exe" target="_blank">gcc 3.3</a>
                <li><a href="http://www.bullfreeware.com/download/aix43/jpeg-6.0.2.0.exe" target="_blank">jpeg v6b</a>
                <li><a href="http://www.bullfreeware.com/download/aix43/libpng-1.0.9.0.exe" target="_blank">libpng 1.0.9</a>
                <li><a href="http://www.bullfreeware.com/download/aix43/zlib-1.1.4.0.exe" target="_blank">zlib 1.1.4</a>
                </ul>
EOT
fi
MD5=
SIZE=
FILE=

cat << EOT
        </td>
    </tr> 
    <tr><td class="head" align="right" bgcolor="#dfeff7" valign="top">UNIX</td>
        <td valign="top" bgcolor="#ffffff" class="text">
EOT

FILE=$SPARC_PKG.gz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <b>SunOS</b> SPARC 5.10 (Solaris 10) (SUN UNIX)<B>
		gzip -cd <A 
		href="http://wdune.ourproject.org/$SPARC_PKG.gz">
                $SPARC_PKG.gz</A> | pkgadd -n -d /dev/fd/0 WHITEdune </B>installable package (*) with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)
EOT
fi
MD5=
SIZE=
FILE=

cat << EOT
        </td>
    </tr> 
    <tr><td class="head" align="right" bgcolor="#dfeff7" valign="top"></td>
        <td valign="top" bgcolor="#DFEFF7" class="text">
EOT

FILE=fw_white_dune-$VERSION.tardist
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <b>IRIX</b> 6.5 (SGI UNIX) <B>swmgr -f <A 
		href="http://wdune.ourproject.org/fw_white_dune-$VERSION.tardist">
                fw_white_dune-$VERSION.tardist</A></B> 
		installable package (*) with source and documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)
EOT
fi
MD5=
SIZE=
FILE=

cat << EOT
        </td>
    </tr> 
    <tr><td class="head" align="right" bgcolor="#dfeff7" valign="top"></td>
        <td valign="top" bgcolor="#ffffff" class="text">
EOT


FILE=white_dune_HPUX_PA-RISC-$VERSION
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                <a name="hpux"></a>
		white_dune </a>$VERSION <b>HPUX</b> 11 
                (HP UNIX) <b> chmod +x 
                <a href="http://wdune.ourproject.org/$FILE">
                $FILE</A></b> pure binary (*) 
                <br>
                $SIZE kB (MD5 checksum: $MD5)               
EOT
fi
MD5=
SIZE=
FILE=

cat << EOT
        </td>
    </tr> 
    <tr><td class="head" align="right" bgcolor="#dfeff7" valign="top"></td> 
        <td valign="top" bgcolor="#DFEFF7" class="text">
EOT

FILE=white_dune_TRU64_alpha-$VERSION
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
                <a name="tru64"></a>
		white_dune </a>$VERSION <b>TRU64</b> 5.0a
                (DEC UNIX) <b> chmod +x 
                <a href="http://wdune.ourproject.org/$FILE">
                $FILE</A></b> pure binary (*) 
                <br>
                $SIZE kB (MD5 checksum: $MD5)               
EOT
fi
MD5=
SIZE=
FILE=

cat << EOT
        </td>
    </tr> 
    <tr><td class="head" align="right" bgcolor="#dfeff7" valign="top"></td> 
        <td valign="top" bgcolor="#ffffff" class="text">
EOT

FILE=whitedune-FreeBSD-$VERSION.tbz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION i386 <b>FreeBSD</b> 7.2-RELEASE 
                <B> pkg_add <a href=http://wdune.ourproject.org/$FILE>
                $FILE</a></B>
                installable package (*) with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)
EOT
fi
MD5=
SIZE=
FILE=

FILE=whitedune-OpenBSD-$VERSION.tgz
if test -f $FILE ; then
cat << EOT
        </td>
    </tr> 
    <tr><td class="head" align="right" bgcolor="#dfeff7" valign="top"></td>
        <td valign="top" bgcolor="#DFEFF7" class="text">
EOT
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION i386 <b>OpenBSD</b> 4-? 
                <B> pkg_add <a href=http://wdune.ourproject.org/$FILE>
                $FILE</a></B>
                installable package (*) with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)
EOT
fi
MD5=
SIZE=
FILE=

cat << EOT
        </td>
    </tr> 
    <tr><td class="head" align="right" bgcolor="#dfeff7" valign="top">
        cygwin/X11 (not really UNIX)</td>
        <td valign="top" bgcolor="#DFEFF7" class="text">
EOT

FILE=white_dune_cygwin_x11-$VERSION.exe
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION i386 
                <b><a href="http://cygwin.com">cygwin</a>/<a href="http://x.cygwin.com/">X11</a></b> 
                <a href=http://wdune.ourproject.org/$FILE>
                white_dune_cygwin_x11-$VERSION</a>
                pure binary
                <br>
                $SIZE kB (MD5 checksum: $MD5)
EOT
fi
MD5=
SIZE=
FILE=



cat << EOT
        </td>
    </tr> 
    <tr><td class="head" align="right" bgcolor="#dfeff7" valign="top">
        Linux</td>
        <td valign="top" bgcolor="#ffffff" class="text">
EOT

FILE=white_dune-$VERSION-1.src.rpm
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>rpm(build) --rebuild <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		(<b>Linux</b>) Source-rpm with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
else
 FILE=white_dune-$VERSION-0.src.rpm
 if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>rpm --rebuild <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		(<b>Linux</b>) Source-rpm with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
 fi
fi
MD5=
SIZE=
FILE=


FILE=white_dune_fedora16-$VERSION-1.i686.rpm
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>rpm -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		for i686 Fedora 16 <b>Linux</b> rpm (*) with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)<br>                
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=


FILE=white_dune_fedora14-$VERSION-1.i686.rpm
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>rpm -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		for i686 Fedora 14 <b>Linux</b> rpm (*) with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)<br>                
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune-$VERSION-1.i586.rpm
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>rpm -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		for i586 SuSE 11.1 <b>Linux</b> rpm (*) with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)<br>                
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=


FILE=white_dune_lsb-$VERSION-1.i386.rpm
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>rpm -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		experimental 
                <a href="http://www.linux-foundation.org/en/Application_Compatibility">LSB</a>
                <b>Linux</b> rpm based with on NVidia graphics drivers (*) 
                with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)<br>                
       <!-- 
                (For various i386 RedHat <b>Linux</b> rpm versions not based on NVidia graphics drivers see on the
                 <a href="http://atrpms.physik.fu-berlin.de/name/white_dune/" target="new">ATrpms website</a>)
        -->
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune-$VERSION-0.i386.rpm
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>rpm -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		for i386 RedHat 9 <b>Linux</b> rpm based on NVidia graphics drivers (*) with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)<br>
       <!-- 
                (For various i386 RedHat <b>Linux</b> rpm versions not based on NVidia graphics drivers see on the
                 <a href="http://atrpms.physik.fu-berlin.de/name/white_dune/" target="new">ATrpms website</a>)
        -->
   <br><br>                
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune-$VERSION-1.x86_64.rpm
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>rpm -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		for 64 Bit PC Scientific Linux 5.1 <b>Linux</b> 
                rpm based on NVidia graphics drivers (*) with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)<br>                
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune-$VERSION-1.mipsel.rpm
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>rpm -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		Sony Playstation 2 (compiled against mesa3D) <b>Linux</b> rpm (*) with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune_olpc11.3.0-$VERSION-1.i386.rpm
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>rpm -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		experimental rpm (*) with documentation for the 
                <a href="http://laptop.org/">OLPC (Version 1)</a>
                running <b>Linux</b> Version 11.3.0
                <br>
                $SIZE kB (MD5 checksum: $MD5)<br>                
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune_olpc11-$VERSION-1.i386.rpm
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>rpm -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		experimental rpm (*) with documentation for the 
                <a href="http://laptop.org/">OLPC (Version 1)</a>
                running <b>Linux</b> Version 11.3.0
                <br>
                $SIZE kB (MD5 checksum: $MD5)<br>                
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune-$VERSION-i386-1.tgz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>installpkg <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		Slackware 13.37 i386 <b>Linux</b> binary package (*) with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)
                
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune-$VERSION-i686-1.tgz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>installpkg <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		Slackware 13.37 i686 <b>Linux</b> binary package (*) with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)
                
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune-$VERSION-x86_64-1.tgz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>installpkg <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		Slackware 13.37 amd64 <b>Linux</b> binary package (*) with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)
                
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=

FILE=white_dune_slackware_2.1-$VERSION-i386-1.tgz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>installpkg <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		Slackware 2.1 <b>Linux</b> binary package with documentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)
                
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=


FILE=whitedune_$VERSION-1_i386.deb
if test -f $FILE ; then
   DEBFILE=$FILE
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>dpkg -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		experimental debian "squeeze" <b>Linux</b> i386 binary package (*) with documentation 
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
   sh ./repository-update.sh $FILE 1>&2
fi
MD5=
SIZE=
FILE=

FILE=white_dune_squeeze-$VERSION-1_i386.deb
if test -f $FILE ; then
   DEBFILE=$FILE
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>dpkg -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		experimental debian "squeeze" <b>Linux</b> i386 binary package (*) with documentation 
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
   sh ./repository-update.sh $FILE 1>&2
fi
MD5=
SIZE=
FILE=

FILE=whitedune_$VERSION-1_amd64.deb
if test -f $FILE ; then
   DEBFILE=$FILE
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>dpkg -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		experimental debian "lenny" <b>Linux</b> amd64 binary package (*) with documentation 
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
   sh ./repository-update.sh $FILE 1>&2
fi
MD5=
SIZE=
FILE=

FILE=whitedune_lenny_$VERSION-1_i386.deb
if test -f $FILE ; then
   DEBFILE=$FILE
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>dpkg -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		experimental debian "lenny" <b>Linux</b> i386 binary package (*) with documentation 
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
   sh ./repository-update.sh $FILE 1>&2
fi
MD5=
SIZE=
FILE=

FILE=whitedune_lenny_$VERSION-1_amd64.deb
if test -f $FILE ; then
   DEBFILE=$FILE
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>dpkg -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		experimental debian "lenny" <b>Linux</b> amd64 binary package (*) with documentation 
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
   sh ./repository-update.sh $FILE 1>&2
fi
MD5=
SIZE=
FILE=

FILE=whitedune_squeeze_$VERSION-1_i386.deb
if test -f $FILE ; then
   DEBFILE=$FILE
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>dpkg -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		experimental debian "squeeze" <b>Linux</b> i386 binary package (*) with documentation 
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
   sh ./repository-update.sh $FILE 1>&2
fi
MD5=
SIZE=
FILE=

FILE=whitedune_squeeze_$VERSION-1_amd64.deb
if test -f $FILE ; then
   DEBFILE=$FILE
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>dpkg -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		experimental debian "squeeze" <b>Linux</b> amd64 binary package (*) with documentation 
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
   sh ./repository-update.sh $FILE 1>&2
fi
MD5=
SIZE=
FILE=

FILE=whitedune_$VERSION-1_powerpc.deb
if test -f $FILE ; then
   DEBFILE=$FILE
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>dpkg -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		experimental debian "squeeze" <b>Linux</b> powerpc binary package (*) with documentation 
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
   sh ./repository-update.sh $FILE 1>&2
fi
MD5=
SIZE=
FILE=

FILE=white_dune_ubuntu-$VERSION-1_i386.deb
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		white_dune $VERSION <B>dpkg -i <A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A></B> 
		experimental ubuntu 10.4 <b>Linux</b> i386 binary package (*) with documentation <br>
                $SIZE kB (MD5 checksum: $MD5)<br>
   <br><br>                
EOT
   sh ./repository-update.sh $FILE $DEBFILE 1>&2
fi
MD5=
SIZE=
FILE=

FILE=whitedune_$VERSION''german-1_i386.deb
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		<img src="docs/deutsch.jpg"> white_dune $VERSION <b>dpkg -i<A 
		href="http://wdune.ourproject.org/$FILE">
                $FILE</a> </b> ausführbares Programm in deutscher Sprache für
		debian <b>Linux</b> i386 mit Dokumentation
                <br>
                $SIZE kB (MD5 checksum: $MD5)
   <br><br>                
EOT
fi
MD5=
SIZE=
FILE=


FILE=white_dune-$VERSION-gentoo.tar.gz
if test -f $FILE ; then
   chmod a+r $FILE
   MD5=`$MD5SUM < $FILE`
   SIZE=`ls -l $FILE | awk '{print int($5/1000)}'`
   cat << EOT
		<BR>white_dune $VERSION <b> tar -xzPf <A
		href="http://wdune.ourproject.org/$FILE">
                $FILE</A> && emerge /usr/portage/media-gfx/white_dune/white_dune-$MAYOR.$MINOR.ebuild
		</B> gentoo <b>Linux</b> source file
                <br>
                $SIZE kB (MD5 checksum: $MD5)                                
   <br><br>
EOT
fi
MD5=
SIZE=
FILE=


cat << EOT
        </td>
    </tr> 
EOT


cat << EOT
</table>
EOT

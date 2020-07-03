#!/bin/sh

if test X_$# = X_0 ; then 
   echo usage: $0 file.x3dv 2>&1
   echo '# the resulting output files are file.mp4 or file*.tif' 2>&1 
   echo '# attension: removes file*.rib overwrites phong.slx' 2>&1 
   exit 1
fi
if test -f $1 ; then 
   echo > /dev/null 
else
   echo "file $1 not found" 2>&1 
   exit 1
fi

dune -files `grep processor /proc/cpuinfo | wc -l` -rib  $1
FILE=`awk -v fn=$1 'BEGIN { 
     for (i = length(fn); i > 0; i--) { 
         if (substr(fn, i, 1) == ".") {
             fn = substr(fn, 1, i - 1);
             break
         } 
     } 
     for (i = length(fn); i > 0; i--) { 
         if (substr(fn, i, 1) == "/") {
             print substr(fn, i + 1);
             exit
         } 
      } 
      print fn;
}'`
for j in $FILE*.rib ; do (aqsis -shaders /usr/share/white_dune/shaders:/usr/share/aqsis/shaders/light/ $j &); done
while test "X_`ps -ef | grep aqsis | grep -v grep | grep -v run_dune_and_aqsis.sh`" != "X_" ; 
do sleep 1; done
for j in $FILE*.rib; do rm $j; done
if test -x "`which mencoder`" ; then
    # if mencoder is available, create movie and delete the .tif files 
    mencoder -quiet "mf://`ls $FILE*.tif | sort -V | awk '{printf $0 ","}' `"  -mf type=tif:fps=24  -ovc x264 -oac lavc -lavcopts acodec=libfaac:abitrate=56 -srate 48000 -af channels=2 -of lavf -ofps 25 -lavfopts format=mp4 -o $FILE.mp4 2>/dev/null > /dev/null &&
    rm $FILE*.tif
    rm -f divx2pass.log divx2pass.log.mbtree
fi

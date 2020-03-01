#!/bin/sh

if test X_$# = X_0 ; then 
   echo usage: $0 file.x3dv 2>&1
   echo '# the resulting output files are output*.tif' 2>&1 
   echo '# attension: removes output*.rib' 2>&1 
   exit 1
fi
if test -f $1 ; then 
   echo > /dev/null 
else
   echo "file $1 not found" 2>&1 
   exit 1
fi

dune -files `grep processor /proc/cpuinfo | wc -l` -rib  $1
FILE=`awk -v file=$1 'BEGIN { split(file, a, "."); print a[1]; }'`
for j in $FILE*.rib ; do (aqsis $j &); done
while test "X_`ps -ef | grep aqsis | grep -v grep | grep -v run_dune_and_aqsis.sh`" != "X_" ; 
do sleep 1; done
for j in $FILE*.rib; do rm $j; done
if test -x /usr/bin/mencoder ; then
    # if mencoder is available, create movie and delete the .tif files 
    mencoder -o $FILE.mp4 "mf://$FILE*.tif" -nosound -of lavf -lavfopts format=mp4 -ovc x264 -x264encopts pass=1:bitrate=2000:crf=24 -mf type=tif:fps=24 &&
    rm divx2pass.log divx2pass.log.mbtree
    rm $FILE*.tif
fi

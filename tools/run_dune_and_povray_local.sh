#!/bin/sh

if test X_$# = X_0 ; then 
   echo usage: $0 file.x3dv 2>&1
   echo '# the resulting output files are file_0*.pov' 2>&1 
   echo '# attension: removes file_0*.pov and file_0*.png' 2>&1 
   exit 1
fi
if test -f $1 ; then 
   echo > /dev/null 
else
   echo "file $1 not found" 2>&1 
   exit 1
fi


DUNE=`dirname $0`/../bin/dune
NPROC=`grep processor /proc/cpuinfo | wc -l`
FILE=`awk -v file=$1 'BEGIN { split(file, a, "."); print a[1] "_0"; }'`
$DUNE -povray  $1 > $FILE.pov
FRAMES=`awk '($1~/\/\// && $2~/number_of_frames/) { print $3 }' $FILE.pov`
awk -v file=$FILE.pov -v nproc=$NPROC -v frames=$FRAMES 'BEGIN { 
endFrame=-1; numFrames=int(frames/nproc);
for (i = 0; i < nproc-1; i++) {
    beginFrame = endFrame + 1; 
    endFrame = beginFrame + numFrames;
    system("povray -D +KFI" beginFrame " +KFF" endFrame " " file " 2>/dev/null&");
}
beginFrame = endFrame + 1; 
system("povray -D +KFI" beginFrame " " file " 2>/dev/null & ");}
'
while test "X_`ps -ef | grep povray | grep -v grep | grep -v run_dune_and_povray_local.sh`" != "X_" ; 
do sleep 1; done
#for j in $FILE*.pov; do rm $j; done
if test -x /usr/bin/mencoder ; then
    # if mencoder is available, create movie and delete the .png files 
    mencoder "mf://`ls $FILE*.png | sort -V | awk '{printf $0 ","}' `" -o $FILE.mp4 -nosound -mf type=png:fps=24 -of lavf -lavfopts format=mp4 -ovc x264 -x264encopts pass=1:bitrate=2000:crf=24 &&
#    rm divx2pass.log divx2pass.log.mbtree
    rm $FILE*.png
fi

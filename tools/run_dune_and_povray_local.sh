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
for j in $FILE*.pov; do rm $j; done
if test -x "`which mencoder`" ; then
    # if mencoder is available, create movie and delete the .png files 
    mencoder -quiet "mf://`ls $FILE*.tif | sort -V | awk '{printf $0 ","}' `"  -mf type=tif:fps=24  -ovc x264 -oac lavc -lavcopts acodec=libfaac:abitrate=56 -srate 48000 -af channels=2 -of lavf -ofps 25 -lavfopts format=mp4 -o $FILE.mp4 2>/dev/null > /dev/null &&
    rm $FILE*.png
    rm -f divx2pass.log divx2pass.log.mbtree
fi

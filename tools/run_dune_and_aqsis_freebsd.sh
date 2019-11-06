
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

dune -o output.rib -files `sysctl hw.model hw.machine hw.ncpu | \
awk '/hw.ncpu/ { print $2 * <system("sysctl -n hw.ncpu")}'` -rib  $1
for j in output*.rib; do (aqsis $j &); done ; \
while test "X_`ps -ef | grep aqsis | grep -v grep | grep -v run_dune_and_aqsis.sh`" != "X_" ; 
do sleep 1; done
for j in output*.rib; do rm $j; done ;


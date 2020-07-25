#!/bin/sh

if test X_$# = X_0 ; then
    echo starts a command for a selectable git-revision of a file 1>&2
    echo usage: $0 command file 1>&2
    echo needs: awk kdialog dirname basename git 1>&2
    echo example: $0 dune file_stored-by_dune
    exit 1
fi

# part of the white_dune distribution 

DIALOG=kdialog
COMMAND=$1
DIR=`dirname $2`
BASE=`basename  $2`
FILE=`awk -v dir=$DIR -v base=$BASE -v pwd=$PWD \
    'BEGIN { sub(".",pwd,dir); print dir "/" base}'`
PROG=`git log --pretty=oneline $FILE | 
awk -v dialog=$DIALOG -v fn=$FILE 'BEGIN {printf dialog " --radiolist text "}
                 {print($1,$1,(i++ == 0) ? "ON" : "OFF", " \\\\")} 
                 END { print(""); }'`
export PROG
REV=`sh -c "$PROG"`
git restore $FILE -s $REV
$1 $2

#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

#set -x

COLLECT_FILE=/tmp/collect_geo_$$.txt
ALL_COLLECT_FILE=/tmp/all_collect_geo_$$.txt
ALL_FILE=/tmp/all_geo_$$.txt
NEXT_GEO_FILES=Master.geo
touch $COLLECT_FILE
while test "X_$NEXT_GEO_FILES" != "X_" ; do
   ls $NEXT_GEO_FILES >> $COLLECT_FILE
   if test "$?" != "0" ; then
       exit 1
   fi
   NEXT_GEO_FILES=`awk '/^INCLUDE/ { 
      inc_file=$2;
      gsub("\r","",inc_file);
      if (system("ls " inc_file "> /dev/null") == 0) 
         print inc_file
      else {
         print inc_file;
         exit(1)
      }
   }' $NEXT_GEO_FILES`
   if test "$?" != "0" ; then
       exit 1
   fi
done
sort $COLLECT_FILE | uniq > $ALL_COLLECT_FILE
ls *.geo | sort | uniq > $ALL_FILE
awk '{ 
   if (FILENAME==ARGV[1]) 
      list[i++]=$1 
   else 
      if (list[j++] != $1) 
         { 
         print $1;
         j--;
         } 
}' $ALL_COLLECT_FILE $ALL_FILE | xargs echo

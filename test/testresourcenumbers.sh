#!/bin/sh

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

# shellscript to test if the resourcenumbers in ../src/resource.h are valid

# require a nawk compatible awk version. If $AWK is not set, try gawk

unset FOUND
OLDIFS=$IFS
IFS=:
if test "$AWK" = "" ; then
   # if gawk is not available, print a warning and return 0
   for i in $PATH ; do 
      if test -f $i/gawk ; then 
         FOUND=TRUE
      fi  
   done
   if test "$FOUND" != "TRUE" ; then
      echo warning: resourcenumber test do not find gawk, set '$'AWK to a nawk compatible awk 1>&2
      exit 0
   else
      AWK=gawk
   fi
fi

IFS=$OLDIFS

RESOURCE_H=`dirname $0`/../src/resource.h
SWT_H=`dirname $0`/../src/swt/include/swt.h

# test if there are double numbers in resource.h

$AWK -v lines=`wc -l $RESOURCE_H | awk '{print $1}'` '
   {
   if (($2!="ID_FILE_MRU_FIRST") && ($2!="ID_FILE_MRU_LAST") &&
       ($2!="IDS_SAVE_CHANGES") && ($2!="IDS_DE_SAVE_CHANGES") &&
       ($2!="IDS_IT_SAVE_CHANGES")) {
      if (($3+0!=0) && ($3+0!=1) && (NR < lines-6)) 
         {
         if ($3+0 <= lastnumber) 
            {
            print $3 " not greater than " lastnumber
            exit(1);
            }
         lastnumber = $3+0;
         }
      }
   }' $RESOURCE_H 1>&2
if test $? != 0 ; then 
   echo failed: test for numbers in resource.h not always raising 1>&2
   exit 1
fi

# test if the defines in swt.h are identical to them in resource.h

$AWK -v firstfile=$SWT_H '
/^#define/ 
   { 
   if (firstfile==FILENAME) 
      {
      if (substr($2,1,2) == "ID") 
         {
         list[$2]=$3;
         }
      }
   else 
      {
      if (substr($2,1,2) == "ID") 
         if ((list[$2] != "") && (list[$2] != $3))
            {
            print "macro " $2 " in " FILENAME " not identical in " firstfile;
            exit(1)
            }
      }
   } {print "dummy";}' $SWT_H $RESOURCE_H | $AWK '/^macro/' 1>&2
exit $?

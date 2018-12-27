#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

DIRNAME=`dirname $0`
export FILE_TO_PATCH=$DIRNAME/../desktop/macosx/white_dune.app/Contents/MacOS/white_dune
strings $FILE_TO_PATCH | awk -v perldir=$DIRNAME -v patchfile=$FILE_TO_PATCH '
function findname(f)
{
slashs=split(f,a,"/");
ret="";
for (i=2;i<=slashs;i++)
   ret = ret "/" a[i];
return ret;
}
/MyDocument.ob/ { print "perl " perldir "/bin_gsub.pl " patchfile " " findname($1) " ./MyDocument.ob" }
/main.ob/ { print "perl  " perldir "/bin_gsub.pl " patchfile " " findname($1) " ./main.ob" } 
' | sh


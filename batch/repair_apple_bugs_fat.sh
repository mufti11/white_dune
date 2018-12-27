#!/bin/sh
set -x
DIRNAME=`dirname $0`
#export PATCH_DIR=$DIRNAME/../desktop/macosx/white_dune.app/Contents/MacOS/
export PATCH_DIR=.
(
cd $PATCH_DIR && 
mv white_dune white_dune.orig &&
lipo white_dune.orig -remove ppc -output white_dune.i386 &&
lipo white_dune.orig -remove i386 -output white_dune.ppc &&
for i in i386 ppc ; do
  export FILE_TO_PATCH=white_dune.$i
  strings $FILE_TO_PATCH | awk -v perldir=$DIRNAME -v patchfile=$FILE_TO_PATCH -v arch=$i '
  function findname(f)
  {
  slashs=split(f,a,"/");
  ret="";
  for (i=2;i<=slashs;i++)
    ret = ret "/" a[i];
  return ret;
  }
  /MyDocument.ob/ { print "perl " perldir "/binary_patch.pl " patchfile " " findname($1) " ./" arch "/MyDocument.ob" }
/main.ob/ { print "perl  " perldir "/binary_patch.pl " patchfile " " findname($1) " ./" arch "/main.ob" } 
' | sh
done &&
lipo white_dune.i386 white_dune.ppc -create -output white_dune
) 


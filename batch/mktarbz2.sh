#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

DUNE=`dirname $0`/..

DUNENAME=wdune-`sh $DUNE/batch/getversion.sh`
export DUNENAME

MKTARBZ2TMP=/tmp/mktarbz2_$$
export MKTARBZ2TMP

mkdir $MKTARBZ2TMP &&
(cd $DUNE/.. && tar -cf - $DUNENAME) | (cd $MKTARBZ2TMP && tar -xf -) &&
(cd $MKTARBZ2TMP/$DUNENAME && make -s realclean) &&
cp $MKTARBZ2TMP/$DUNENAME/src/config.h.nt $MKTARBZ2TMP/$DUNENAME/src/config.h &&
(cd $MKTARBZ2TMP && tar -cf /tmp/$DUNENAME.tar $DUNENAME) &&
bzip2 -9f /tmp/$DUNENAME.tar &&
rm -rf $MKTARBZ2TMP &&
echo written /tmp/$DUNENAME.tar.bz2


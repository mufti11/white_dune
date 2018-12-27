#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

DUNE=`dirname $0`/..

DUNENAME=wdune-`sh $DUNE/batch/getversion.sh`
export DUNENAME
DUNENAME2=white_dune-`sh $DUNE/batch/getversion.sh`
export DUNENAME2

MKTARBZ2TMP=/tmp/mktarbz2_$$
export MKTARBZ2TMP

mkdir $MKTARBZ2TMP &&
(cd $DUNE/.. && tar -cf - $DUNENAME2) | (cd $MKTARBZ2TMP && tar -xf -) &&
(cd $MKTARBZ2TMP/$DUNENAME2 && make -s realclean) &&
cp $MKTARBZ2TMP/$DUNENAME2/src/config.h.nt $MKTARBZ2TMP/$DUNENAME2/src/config.h &&
(cd $MKTARBZ2TMP && tar -cf /tmp/$DUNENAME2.tar $DUNENAME2) &&
bzip2 -9f /tmp/$DUNENAME2.tar &&
rm -rf $MKTARBZ2TMP &&
echo written /tmp/$DUNENAME2.tar.bz2


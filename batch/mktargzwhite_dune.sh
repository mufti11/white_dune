#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

DUNE=`dirname $0`/..

DUNENAME=wdune-`sh $DUNE/batch/getversion.sh`
export DUNENAME
DUNENAME2=white_dune-`sh $DUNE/batch/getversion.sh`
export DUNENAME2

MKTARGZTMP=/tmp/mktargz_$$
export MKTARGZTMP

mkdir $MKTARGZTMP &&
(cd $DUNE/.. && tar -cf - $DUNENAME2) | (cd $MKTARGZTMP && tar -xf -) &&
(cd $MKTARGZTMP/$DUNENAME2 && make -s realclean) &&
cp $MKTARGZTMP/$DUNENAME2/src/config.h.nt $MKTARGZTMP/$DUNENAME2/src/config.h &&
(cd $MKTARGZTMP && tar -cf /tmp/$DUNENAME2.tar $DUNENAME2) &&
gzip -9f /tmp/$DUNENAME2.tar &&
rm -rf $MKTARGZTMP &&
echo written /tmp/$DUNENAME2.tar.gz


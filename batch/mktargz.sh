#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

DUNE=`dirname $0`/..

DUNENAME=wdune-`sh $DUNE/batch/getversion.sh`
export DUNENAME

MKTARGZTMP=/tmp/mktargz_$$
export MKTARGZTMP

mkdir $MKTARGZTMP &&
(cd $DUNE/.. && tar -cf - $DUNENAME) | (cd $MKTARGZTMP && tar -xf -) &&
(cd $MKTARGZTMP/wdune-`sh $DUNE/batch/getversion.sh` && make -s realclean) &&
cp $MKTARGZTMP/$DUNENAME/src/config.h.nt $MKTARGZTMP/$DUNENAME/src/config.h &&
(cd $MKTARGZTMP && tar -cf /tmp/$DUNENAME.tar $DUNENAME) &&
gzip -9f /tmp/$DUNENAME.tar &&
rm -rf $MKTARGZTMP &&
echo written /tmp/$DUNENAME.tar.gz


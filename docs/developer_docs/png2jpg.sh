#!/bin/sh 

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

# replace png files used in html files with jpg files
# example: $0 developer_docs dune dune_developer.html 
set -x

cp /tmp/white_dune_docs/docs/$1/$2 /tmp/white_dune_docs/tmp_1_$$ &&
for i in ../$1/*.png ; do
   FILENAME=`basename $i .png` 
   convert ../$1/$i /tmp/white_dune_docs/docs/$1/$FILENAME.jpg && 
   awk -v image=$FILENAME '{gsub(image ".png",image ".jpg");print $0}' /tmp/white_dune_docs/tmp_1_$$ > /tmp/white_dune_docs/tmp_2_$$ &&
   mv /tmp/white_dune_docs/tmp_2_$$ /tmp/white_dune_docs/tmp_1_$$ 
done &&
mv /tmp/white_dune_docs/tmp_1_$$ /tmp/white_dune_docs/docs/$1/$2




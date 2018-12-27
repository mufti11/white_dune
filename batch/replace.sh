TMP=/tmp/$$
gawk -v from=$1 -v to=$2 '{gsub(from, to);print $0;}' $3 > $TMP && cp $TMP $3
rm -f $TMP

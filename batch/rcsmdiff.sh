#!/bin/sh

TMP=/tmp/rcsmdiff_$$
co -p $1 > $TMP
mgdiff $1 $TMP
rm -f $TMP


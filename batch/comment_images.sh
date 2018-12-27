#!/bin/sh

# this program is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

find . -type f -name '*.png' -print | awk '{
print "cp " $1 " /tmp/test.png && convert -comment \"This Makefile is free software; the Free Software Foundation gives unlimited permission to copy, distribute and modify it.\" /tmp/test.png " $1}' | sh
rm /tmp/test.png

find . -type f -name '*.jpg' -print | awk '{
print "cp " $1 " /tmp/test.jpg && convert -comment \"This Makefile is free software; the Free Software Foundation gives unlimited permission to copy, distribute and modify it.\" /tmp/test.jpg " $1}' | sh
rm /tmp/test.jpg

DIR=`dirname $0`/../lib/textures
find $DIR -type f -name '*.png' -print | awk '{
print "cp " $1 " /tmp/test.png && convert -comment \"\" /tmp/test.png " $1}' | sh
rm /tmp/test.png

find $DIR -type f -name '*.jpg' -print | awk '{
print "cp " $1 " /tmp/test.jpg && convert -comment \"\" /tmp/test.jpg " $1}' | sh
rm /tmp/test.jpg


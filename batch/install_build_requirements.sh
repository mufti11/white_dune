#!/bin/sh

if test $# = 0 ; then
  echo usage: sh $0 \'install command\'
  exit 1  
fi

$1 make bison flex gawk m4 x11proto-core-dev libexpat1-dev libxmu-dev \
   libxi-dev libxi-dev libgl-dev libglu-dev libmotif-dev libpng-dev \
   libjpeg62-dev g++ gcc libusb-dev libcurl4-openssl-dev \
   aqsis libfreetype6-dev ttf-bitstream-vera imagemagick


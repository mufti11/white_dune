#!/bin/bash

# install script for required installation packages under several  distributions

if test $# = 0 ; then
  echo usage: bash $0 \'install command\' package-suffix 1>&2
  echo example: bash install_build_requirements.sh \'pacman -S --noconfirm\' 1>&2
  echo example: bash install_build_requirements.sh \'apt install\' -dev 1>&2
  echo example: baash install_build_requirements.sh \'yum install -y\' -devel 1>&2
  echo example: baash install_build_requirements.sh \'pacman -S\' 1>&2
  echo example: baash install_build_requirements.sh \'pkg install\' 1>&2
  exit 1  
fi

pack="make bison flex gawk m4 x11proto-core$2 libexpat1$2 expat$2 libxmu$2 \
   libXmu$2 libxi$2 libXi$2 libgl$2 libglu$2 glu$2 openmotif$2 libmotif$2 \
   libpng$2 libjpeg$2 \
   g++ gcc gcc-g++ libusb$2 libcurl4-openssl$2 libcurl$2 ffmpeg \
   aqsis libfreetype6$2 libcgal$2 freetype$2 libfreetype$2 cgal$2 libcgal$2 \
   ttf-bitstream-vera imagemagick xbase-clients \
   libavcodec$2 libavutil$2 libavformat$2 libavresample$2 libswscale$2 \
   libswresample$2 \
   avcodec$2 avutil$2 avformat$2 avresample$2 swscale$2 swresample$2" 

for i in $pack ; do
$1 $i
done 

if $1 opensubdiv$2 ; then
   echo done
else
    echo do not forget to install OpenSubdiv from sources
fi

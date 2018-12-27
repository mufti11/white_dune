#!/bin/sh

dune -o RibExport.rib -rib file.x3dv && rm -f RibExport*.tif && aqsis RibExport.rib && mencoder "mf://RibExport*.tif" -o movie.avi -ovc lavc -lavcopts vcodec=mjpeg && totem movie.avi

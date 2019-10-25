Under debian Linux, you can create a .deb package in two ways:

- create a debian subdirectory in /tmp to build packages

  # as root
  (cd wdune-*/packager/debian && sh mkdeb_buster.sh)

  the .deb files are in wdune-*/..

- create a debian subdirectory in wdune-*

  cd wdune-*
  make debian
  # as root
  dpkg-buildpackage -i -D -us -uc -rfakeroot -T wdune -T wdune-docs

  the .deb files are in wdune-*/..

To remove the debian directory, use

  cd wdune-*
  make debianclean

debian buster currently do not have a OpenSubdiv library. 
You need to install it e.g. to /usr/local or link it statically.
white_dune also compiles without the OpenSubdiv library and let you
use subdivision, but the results with the OpenSubdiv library are
very much better.


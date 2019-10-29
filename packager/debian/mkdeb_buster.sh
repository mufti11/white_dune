#!/bin/sh
# set -x

export PATH=$PATH:/bin:/usr/sbin:/sbin
(cd ../../ && make debian)
(cd ../../ && dpkg-buildpackage -i -D -us -uc -rfakeroot -Twdune -Twdune-docs)
(cd ../../ && make debianclean)



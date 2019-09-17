#!/bin/sh
# set -x

(cd ../../ && make debian)
(cd ../../ && dpkg-buildpackage -i -D -us -uc -rfakeroot -T wdune -T wdune-docs)
(cd ../../ && make debianclean)



#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

if test $# != 1 ;then
   echo usage: $0 1>&2
   exit 1
fi

cat << EOT
};

#endif
EOT

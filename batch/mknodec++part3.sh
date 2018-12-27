#!/bin/sh 

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

if test $# != 1 ;then
   echo usage: $0 nodename 1>&2
   exit 1
fi
NODENAME=$1

cat << EOT
}

Node *
Proto$NODENAME::create(Scene *scene)
{ 
    return new Node$NODENAME(scene, this); 
}

Node$NODENAME::Node$NODENAME(Scene *scene, Proto *def)
  : Node(scene, def)
{
}
EOT

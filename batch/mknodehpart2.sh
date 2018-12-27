#!/bin/sh 

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

if test $# != 1 ;then
   echo usage: $0 nodename 1>&2
   exit 1
fi
NODENAME=$1

cat << EOT
};

class Node$NODENAME : public Node {
public:
                    Node$NODENAME(Scene *scene, Proto *proto);

//    virtual int     getProfile(void) const { return PROFILE_; }
//    virtual const char* getComponentName(void) const { return ""; }
//    virtual int         getComponentLevel(void) const { return -1; }
    virtual Node   *copy() const { return new Node$NODENAME(*this); }

EOT

#!/bin/sh
ARG1=$1
shift
grep -n $ARG1 $* /dev/null | 
awk '{
   split($1,args,":");
   line=args[2];
   if (line <= 9) 
      line = 1;
   else
      line -= 9;
   print "less +" line " " args[1];
}' | sh

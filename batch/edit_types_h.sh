#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

if test "_$EDITOR" = "_" ; then
   EDITOR=vi
fi
   
$EDITOR $* `ls FieldValue.h SF*.h MF*.h | grep -v Item | grep -v Command | grep -v SFMF | xargs echo` 

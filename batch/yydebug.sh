#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

awk 'BEGIN {print "#define YYDEBUG 1"} {gsub("int yydebug","int yydebug=1",$0);print $0}' parser.cpp > /tmp/parser_$$
cp /tmp/parser_$$ parser.cpp
rm /tmp/parser_$$

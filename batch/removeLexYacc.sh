#!/bin/sh
EXECDIR=`dirname $0`

cp $EXECDIR/../src/Makefile /tmp/Makefile_$$ && 
awk '
/^lexer\.cpp\: lexer\.l/ { lex = 1; }
/^parser.cpp: parser.y/ { yacc = 1; }
{
  if ($0 == "") {
      lex = 0;
      yacc = 0;
  }
  if ((lex == 0) && (yacc == 0))
      print $0;
}' < /tmp/Makefile_$$ > $EXECDIR/../src/Makefile
rm /tmp/Makefile_$$


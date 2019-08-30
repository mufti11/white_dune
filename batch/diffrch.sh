diff -rq $1 $2 -x 'dune.rc' -x 'Makefile' -x 'Makefile.bak' -x '*.o' -x '*.a' -x '*.bmp' -x 'resource.c' -x 'parser.cpp' -x 'lexer.cpp' -x 'y.tab.h' -x 'rc' |
awk '{ if ($1 == "Files") diff " " $2 " " $4 }' | sh

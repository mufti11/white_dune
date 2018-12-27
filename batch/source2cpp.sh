#!/bin/sh

# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

awk -v wantLinenumberCount=$LINE_NUMBER_COUNT '{ 
      line = $0;
      gsub("\\\"", "\\\"", line);
      print "    RET_ONERROR( mywritestr(f, \"" line "\\n\") )";
      if (wantLinenumberCount != "")
          print "    TheApp->incSelectionLinenumber();"
     }' $1

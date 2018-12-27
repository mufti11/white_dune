#!/bin/sh
# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

# shellscript to test if the resourcenumbers in ../src/resource.h are valid

# require a nawk compatible awk version. If $AWK is not set, try gawk

unset FOUND
OLDIFS=$IFS
IFS=:
if test "$AWK" = "" ; then
   # if gawk is not available, print a warning and return 0
   for i in $PATH ; do 
      if test -f $i/gawk ; then 
         FOUND=TRUE
      fi  
   done
   if test "$FOUND" != "TRUE" ; then
      echo warning: resourcenumber test do not find gawk, set '$'AWK to a nawk compatible awk 1>&2
      exit 0
   else
      AWK=gawk
   fi
fi
IFS=$OLDIFS

RC_DIR=`dirname $0`/../src
RC_FILE_EN=dune.english.rc
RC_FILES=`(cd $RC_DIR && ls dune.*.rc)`

# test if there are differences in the menus of different languages

for i in $RC_FILES ; do 
   if test $i != $RC_FILE_EN ; then
      $AWK -v rc_dir=$RC_DIR -v enfile=$RC_FILE_EN -v currentfile=$i '
         function upper_case(str, i) {
            for (i=97; i<123; i++)
                gsub(sprintf("%c",i),sprintf("%c",i-32),str);
            return str;
         }
         BEGIN { 
            num_en = 0; 
            num = 0; 
            num_id_en = 0;
            num_id = 0;
         }
         $1 ~ /MENUITEM/ { 
            if (FILENAME == rc_dir "/" enfile)
               items_en[num_en++] = $NF;
            else 
               {
               if ($NF != items_en[num])
                  {
                  print FILENAME ":" FNR " " $NF " not identical to " items_en[num]; 
                  error = 1;
                  exit(1);
                  }
               num++;
               }  
         }
         $1 ~ /^ID/ && $1 !~ /^ID[BP]_.*/ {
            if (FILENAME == rc_dir "/" enfile)
               items_id_en[num_id_en++] = $1;
            else 
               {
               if (currentfile == "dune.german.rc")
                   lang_token = "DE";
               else if (currentfile == "dune.portuguese.rc")
                   lang_token = "PT";
               else
                   lang_token = upper_case(substr(currentfile, 6, 2));
               similar = 0;
               if (!($1 ~ /^IDS/))
                   if ($1 == items_id_en[num_id])
                       similar = 1;

               rest1 = $1;
               gsub(/^IDR_/,"",rest1);
               gsub(/^IDD_/,"",rest1);
               gsub(/^ID_/,"",rest1);
               gsub(/^IDS_/,"",rest1);

               rest2 = items_id_en[num_id];
               gsub(/^IDR_/,"",rest2);
               gsub(/^IDD_/,"",rest2);
               gsub(/^ID_/,"",rest2);
               gsub(/^IDS_/,"",rest2);
               if (rest1 == (lang_token "_" rest2))
                   similar = 1;
               if (similar == 0)
                  {
                  print FILENAME ":" FNR " " $1 " not similar to " items_id_en[num_id]; 
                  error = 1;
                  exit(1);
                  }
               else
                  num_id++;
               }  
         }
         END {
            if (error == 1)
               exit(1);
            if (num != num_en)
               {
               print "number of MENUITEMs in " ARGV[2] " differ from " ARGV[1];
               exit(1); 
               }
         }  
      ' $RC_DIR/$RC_FILE_EN $RC_DIR/$i 1>&2
      if test $? != 0 ; then 
         exit 1
      fi
   fi
done
exit 0

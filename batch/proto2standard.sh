awk '
($4 == "{") {
  ignore = 1;
  print $0;
}
($1 == "}") {
  ignore = 1;
  print $0;
}
{ 
  if (ignore == 0) 
    {
    printf($2 " " $1 " ");
    for (i = 3; i <= NF; i++) 
      printf(" " $i);
    printf("\n");
    }
  ignore = 0;
}
' $*

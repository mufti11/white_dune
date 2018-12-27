# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

  {
  sub("dune-","",version);
  sub("_swt","",version);
  sub(replace,version);
  print $0
  }


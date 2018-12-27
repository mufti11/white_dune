#!/bin/sh

# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

awk 'BEGIN {
    num_nodes = 0;
    num_protos = 0;
}

/}/ {
   startNodes=0;
}

/^\/\// {
   comment=1;
}

{
   if (startNodes && !comment)
      {
      split($1,a,",");
      nodes[num_nodes++]=a[1];
      }
}

/_protos\[\".*\"\] = new Proto.*\(this\)\;/ {
   if (!comment)
      {
      split($1,a,"]");
      split(a[1],b,"[");
      if (b[1]=="_protos")
         {
         split(b[2],c,"\"");
         protos[num_protos++]=c[2];
         }
      }
}

{
   comment=0;
}

/^enum NodeEnum \{/ {
   startNodes=1;
}

END {
   for (i = 0; i < num_protos - 1; i++)
        print "#include \"Node" protos[i] ".h\"";
   print "#include \"Scene.h\"";
   print "#include \"xerrorhandler.h\"";
   print "#include <stdio.h>";
   print "int main(int argc, char** argv)";
   print "   {";
   print "   TheApp = new DuneApp();"
   print "   Scene *scene = new Scene();"
   numNodes = 0;
   for (i = 0; i < num_protos - 1; i++)
      {
      if (nodes[numNodes] == "VRML_SCRIPT")
          continue;
      if (nodes[numNodes] == "X3D_NURBS_SURFACE_INTERPOLATOR")
          continue;
      print "   Node *node" protos[i] " = new Node" protos[i] "(scene, new Proto"  protos[i] "(scene));";
      print "   if (node" protos[i] "->getType() != " nodes[numNodes] ")";
      print "      {"
      print "      fprintf(stderr,\"NodeEnum in Node.h not similar to _proto map in Scene.cpp\\n\");";
      print "      fprintf(stderr,\"wrong nodetype " nodes[numNodes] " for " protos[i] "\\n\");";
      print "      normalExit(1);";
      print "      }";
      numNodes++;
      }
   print "   normalExit(0);";
   print "   }";
}

' `dirname $0`/../src/Node.h `dirname $0`/../src/Scene.cpp

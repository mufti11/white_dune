# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

# parse a X3D specification component text file and
# generate templates of sourcefiles for not already existing nodes in white_dune
#
# example usage:
#
# lynx -dump http://www.web3d.org/x3d/specifications/ISO-IEC-19775-1.2-X3D-AbstractSpecification/Part01/components/CADGeometry.html \
# | awk -f mknodesource.awk 

function mka2z(i)
{
    A = 65;
    a2z = "";
    for (i = 97; i <= 122; i++) 
       a2z = a2z sprintf("%c", i)
}

BEGIN {
   mka2z();
}

function to_upper(name, i)
{
    ret = "";
    for (i = 1; i <= length(name); i++)
       {
       letter = substr(name, i, 1);
       if (index(a2z,letter) != 0)
          ret = ret sprintf("%c", index(a2z,letter) + A - 1);
       else 
          ret = ret letter;
       }
    return ret;
}

function Upper2_(name, i)
{
    ret = "";
    for (i = 1; i <= length(name); i++)
       {
       letter = substr(name, i, 1);
       if (index(a2z,letter) != 0)
          ret = ret sprintf("%c", index(a2z,letter) + A - 1);
       else 
          {
          ret = ret "_";
          ret = ret letter;
          }
       }
    return ret;
}

function toNodenameH(nodename)
{
    return "_NODE" Upper2_(nodename) "_H"; 
}

function toNodetype(nodename)
{
    return "X3D" Upper2_(nodename); 
}

function validNodename(nodename)
{
    if (substr(nodename, 1, 3) == "X3D")
      return 0;
    if (nodename == "NurbsPatchSurface")
      return 0;
    # header file already exist ?
    if (replace == 0)
      if (system("ls " path "../src/Node" nodename ".h > /dev/null 2> /dev/null") == 0)
        return 0;
    return 1;
}

function vrmlEvent(event) 
{
    if (event == "[in,out]")
        return "ExposedField";
    if (event == "[in,put]")
        return "ExposedField";
    if (event == "[]")
        return "Field";
    if (event == "[in]")
        return "EventIn";
    if (event == "[out]")
        return "EventOut";
    if (event == "inputOutput")
        return "ExposedField";
    if (event == "initializeOnly")
        return "Field";
    if (event == "inputOnly")
        return "EventIn";
    if (event == "outputOnly")
        return "EventOut";
    print "error: unknown event " event " !\n"
    exit 1;     
    return "";
}

/}/ {
  if (fieldflags && validNodename(nodename) && (substr($1,1,2) != "}}")) {
    hfile = path "../src/Node" nodename ".h";
    cppfile = path "../src/Node" nodename ".cpp";
    system("sh " path "mknodehpart1.sh " nodename " " toNodenameH(nodename) " " toNodetype(nodename) " > " hfile);
    for (i = 0; i < numfields; i++) {
      if (name[i] != "metadata")
        if ((event[i] != "[in]") && (event[i] != "[out]"))
          print "    FieldIndex " name[i] ";" >> hfile;
    }
    system("sh " path "mknodehpart2.sh " nodename " >> " hfile);
    for (i = 0; i < numfields; i++) {
      if (name[i] != "metadata")
        if ((event[i] != "[in]") && (event[i] != "[out]"))
          print "    fieldMacros(" type[i] ", " name[i] ", Proto" nodename");" >> hfile ;
    }
    system("sh " path "mknodehpart3.sh " nodename " >> " hfile);

    system("sh " path "mknodec++part1.sh " nodename " > " cppfile);

    for (i = 0; i < numfields; i++) 
      if (name[i] != "metadata") {
        print "#include \"" type[i] ".h\"" >> cppfile;
      }

    system("sh " path "mknodec++part2.sh " nodename " >> " cppfile);

    for (i = 0; i < numfields; i++) 
      if (name[i] != "metadata") {
        if ((event[i] != "[in]") && (event[i] != "[out]")) {
          print "    " name[i] ".set(" >> cppfile;
          printf("    ") >> cppfile;
        }
        printf("    add" vrmlEvent(event[i]) "(") >> cppfile;
        printf(to_upper(type[i]) ", \"" name[i] "\"") >> cppfile; 
        if ((event[i] != "[in]") && (event[i] != "[out]")) {
          printf(", new " type[i] "(" default[i] ")") >> cppfile;
          printf(")") >> cppfile;
        }
        print ");" >> cppfile;
      }

    system("sh " path "mknodec++part3.sh " nodename " >> " cppfile);

    print "#include \"Node" nodename ".h\"" >>  path "../src/FilesMissingInWindowsProjects.h"; 
    print "#include \"Node" nodename ".cpp\"" >> path "../src/FilesMissingInWindowsProjects.cpp";

    print "#include \"Node" nodename ".h\"" >>  path "../src/SceneProtoMap.cpp"; 
    print "    protos->add(\"" nodename "\", new Proto" nodename "(scene));" >>  path "../src/SceneProtoMap.cpp"; 

    print "//           MENUITEM \"" nodename "\",                ID_NEW_" Upper2_(nodename) >> "../src/dune.english.rc"; 
    print "//           MENUITEM \"" nodename "\",                ID_NEW_" Upper2_(nodename) >> "../src/dune.german.rc"; 
    print "//           MENUITEM \"" nodename "\",                ID_NEW_" Upper2_(nodename) >> "../src/dune.italian.rc"; 

    print "//    ID_NEW_" Upper2_(nodename) "    \"Create a new " nodename " node.\\n" nodename "\"" >> "../src/dune.english.rc"; 
    print "//    ID_DE_NEW_" Upper2_(nodename) "    \"Erzeuge einen neuen " nodename " Knoten.\\n" nodename "\"" >> "../src/dune.german.rc"; 
    print "//    ID_IT_NEW_" Upper2_(nodename) "    \"Crea un nuovo " nodename " node.\\n" nodename "\"" >> "../src/dune.italian.rc"; 

    print "        Node" nodename ".o \\";
  }
  fieldflags = 0;
  skipfields = 0;
  nodename = "";
}

{ 
  if ($1 == "#") { // shader node
    skipfields = 1;
  } else if (fieldflags && (!skipfields) && (NF > 3)) {
    type[numfields] = $1;
    event[numfields] = $2;
    name[numfields] = $3;
    if ($1 == "SFVec2f")
      default[numfields] = $4 ", " $5;
    else if ($1 == "SFVec2d")
      default[numfields] = $4 ", " $5;
    else if ($1 == "SFVec3f")
      default[numfields] = $4 ", " $5 ", " $6;
    else if ($1 == "SFVec3d")
      default[numfields] = $4 ", " $5 ", " $6;
    else if ($1 == "SFColor")
      default[numfields] = $4 ", " $5 ", " $6;
    else if ($1 == "SFVec4f")
      default[numfields] = $4 ", " $5 ", " $6 ", " $7;
    else if ($1 == "SFRotation")
      default[numfields] = $4 ", " $5 ", " $6 ", " $7;
    else if ($1 == "SFColorRGBA")
      default[numfields] = $4 ", " $5 ", " $6 ", " $7;
    else if ($1 == "SFMatrix3f")
      default[numfields] = $4 ", " $5 ", " $6 ", " $7 ", " $8 ", " $9 ", " $10 ", " $11 ", " $12;
    else if ($1 == "SFMatrix4f")
      default[numfields] = $4 ", " $5 ", " $6 ", " $7 ", " $8 ", " $9 ", " $10 ", " $11 ", " $12 ", " $13 ", " $14 ", " $15 ", " $16 ", " $17 ", " $18 ", " $19;
    else if (($1 == "SFBool") && ($4 == "TRUE"))
      default[numfields] = "true";
    else if (($1 == "SFBool") && ($4 == "FALSE"))
      default[numfields] = "false";
    else if (($1 == "SFNode") && ($4 == "[]"))
      default[numfields] = "";
    else if (($1 == "SFNode") && ($4 == "NULL"))
      default[numfields] = "";
    else if (substr($1,1,2) == "MF") {
      default[numfields] = "";
      if ($4 != "[]")
        default[numfields] = "correct manually: MFtype with default: " $4;
    } else
      default[numfields] = $4;
    numfields++;
  }
}

/{/ {
  if ((substr($1,1,3) != "X3D") && ($2 == ":")) {
    fieldflags = 1;
    nodename = $1;
    numfields = 0;
  }
}



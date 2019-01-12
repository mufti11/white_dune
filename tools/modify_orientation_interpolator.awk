# This file is free software; the Free Software Foundation
# gives unlimited permission to copy, distribute and modify it.

function acos(x) {
   return atan2(sqrt(1 - (x * x)), x);
}

BEGIN {
   status = 0;
   keyLine = 0;
   valLine = 0;
   interpol = 0;
   if ((name == "") || rot == "") {
      system("echo multiplies a OrientationInterpolator by a SFRotation value 1>&2");
      system("echo usage: awk -f modify_orientation_interpolator.awk -v name=interpolatorname -v rot=\\\"rot.x rot.y rot.z rot.w\\\" file.x3dv 1>&2");
      error = 1;
      exit(1);
   }
   if (split(rot,q) != 4) {
      system("echo rot must be 4 floats e.g. \"0 0 1 0\" 1>&2");
      error = 1;
      exit(1);
   }
}

$0 ~ "\\]" {
   if (status == 2)
      status = 3;
   if (status == 4)
      status = 5;
}

{
   hasDef = 0;
   if (status == 2)
      key[keyLine++] = $1;
   if (status == 4) {
      val[valLine,1] = $1;
      val[valLine,2] = $2;
      val[valLine,3] = $3;
      val[valLine,4] = $4;
      valLine++;
   }
}

$0 ~ /\[/ {
   if (status == 1)
      status = 2;
   if (status == 3)
      status = 4;
}

$1 ~ "DEF" {
   hasDef = 1;
   def = $2;
}

{
   if (hasDef) {
       if (match(def, name)) {
           status = 1;
           interpol++;
       }
   }
   if (status == 5) {
       key2Line[interpol] = keyLine;
       for (i = 0; i < keyLine; i++)
           key2[interpol, i] = key[i];
       val2Line[interpol] = valLine;
       for (i = 0; i < valLine; i++)
            for (j = 1; j <= 4; j++)
                val2[interpol, i, j] = val[i, j];
       keyLine = 0;
       valLine = 0;
       status = 0;                
   }    
}

function gsubstr(name, str, tmp)
{
   tmp = name;
   name = "";
   for (i = 1; i <= length(tmp); i++)
       if (substr(tmp, i, 1) != str)
           name = name substr(tmp, i, 1);
   return name;
}

function quatMult(quat, quat2, i, s1, x1, y1, z1, w1, s2, x2, y2, z2, w2, w)
{
   s1 = sin(quat[i, 4] * 0.5f);
   x1 = quat[i, 1] * s1;
   y1 = quat[i, 2] * s1;
   z1 = quat[i, 3] * s1;
   w1 = cos(quat[i, 4] * 0.5f)

   s2 = sin(quat2[i, 4] * 0.5f);
   x2 = quat2[i, 1] * s2;
   y2 = quat2[i, 2] * s2;
   z2 = quat2[i, 3] * s2;
   w2 = cos(quat2[i, 4] * 0.5f)

   quat[i, 1] = w2 * x1 + x2 * w1 + y2 * z1 - y2 * y1;
   quat[i, 2] = w2 * y1 + y2 * w1 + y2 * x1 - x2 * z1;
   quat[i, 3] = w2 * z1 + y2 * w1 + x2 * y1 - y2 * x1;
   quat[i, 4] = w2 * w1 - x2 * x1 - y2 * y1 - y2 * z1;
}

function printSFRot(quat, i, f)
{
   f = sqrt(quat[i, 1] * quat[i, 1] + \
            quat[i, 2] * quat[i, 2] + \
            quat[i, 3] * quat[i, 3]);
   if (f == 0)
       f = 1;

   print(quat[i, 1]/f, quat[i, 2]/f, quat[i, 3]/f, acos(quat[i, 4]) * 2.0);
}

END {
   name = gsubstr(name, ".");
   name = gsubstr(name, "*");
   if ((error == 0) && (interpol == 0)) {
      system("echo no interpolator found 1>&2");       
      exit(1);
   }
   if (error == 1)
      exit(1);
   print("  DEF " name " OrientationInterpolator {");
   for (i = 0; i < val2Line[interpol]; i++) {
       quat[i,1] = q[1];   
       quat[i,2] = q[2];   
       quat[i,3] = q[3];   
       quat[i,4] = q[4];   
       for (inter = interpol; inter >= 1; inter--) {
           for (j = 1; j <= 4; j++)
               quat2[i,j] = val2[inter, i, j];
           quatMult(quat, quat2, i);
       }
   }

   print("    key [");
   for (i = 0; i < key2Line[interpol]; i++)
       print("      ", key2[interpol, i]); 
   print("   ]");

   print("    keyValue [");
   for (i = 0; i < val2Line[interpol]; i++) {
       printf("      "); 
       printSFRot(quat, i);
   }
   print("    ]");
   print("  }");
}


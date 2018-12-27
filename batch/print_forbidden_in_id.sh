awk 'BEGIN {
  a[l++]=0x22;
  a[l++]=0x23;
  a[l++]=0x27;
  a[l++]=0x2b;
  a[l++]=0x2c;
  a[l++]=0x2d;
  a[l++]=0x2e;
  a[l++]=0x5b;
  a[l++]=0x5c;
  a[l++]=0x5d;
  a[l++]=0x7b;
  a[l++]=0x7d;
  a[l++]=0x7f;
  for (i = 0x30; i <= 0x39; i++)
    a[l++]=i;
  for (i=0; i < l; i++)
    printf("IdFirstChar forbids %d %c\n",a[i], a[i]);
  printf("X3D IdFirstChar forbids %d %c\n",0x3a, 0x3a);
}'

awk 'BEGIN {
  a[l++]=0x22;
  a[l++]=0x23;
  a[l++]=0x27;
  a[l++]=0x2c;
  a[l++]=0x2e;
  a[l++]=0x5b;
  a[l++]=0x5c;
  a[l++]=0x5d;
  a[l++]=0x7b;
  a[l++]=0x7d;
  a[l++]=0x7f;
  for (i=0; i < l; i++)
    printf("IdRestChar forbids %d %c\n",a[i], a[i]);
  printf("X3D IdFirstChar forbids %d %c\n",0x3a, 0x3a);
}'


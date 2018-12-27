# This file is free software; the Free Software Foundation    
# gives unlimited permission to copy, distribute and modify it.

# repair executables created by xcode
# this executables include a static path e.g.

$filename=$ARGV[0]; 
$from=$ARGV[1];
$to=$ARGV[2];
undef $/;
if (length($to)>length($from)) {
    print STDERR "not enough space in binary, binary patch already applied ?\n";
    exit(0);
}
open(EXE,"+<$filename") or die "can not open $filename: $!";
binmode(EXE);
$content=<EXE>;
$content=~s/$from/$to/g;
seek(EXE, 0, 0);
print EXE $content or die "can not write content";
close(EXE);

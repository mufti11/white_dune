REM This Makefile is free software; the Free Software Foundation
REM gives unlimited permission to copy, distribute and modify it.

REM wingman rumblepad for M$Windows
REM only two axes deliver usefull information - under Linux, this is a 6DOF device 8-(
white_dune.exe -joystick 0 -axes=3 -none=2 -sendalways -allxyz=20,,,0.000001 -allrot=0.2,,,0.0000001 

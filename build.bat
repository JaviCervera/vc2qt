@echo off

echo building...
g++ -O2 -m32 -o _build/vc2qt.exe src/vc2qt.cc

echo done.
pause

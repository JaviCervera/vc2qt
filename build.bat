@echo off

echo building...
g++ -std=c++11 -O2 -m32 -o _build/vc2qt.exe src/vc2qt.cc src/projectinfo.cc

echo done.
pause

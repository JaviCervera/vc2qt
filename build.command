#!/bin/sh
cd `dirname $0`

echo "building..."
g++ -O2 -m32 -o _build/vc2qt src/vc2qt.cc

echo "done."

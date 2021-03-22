#! /usr/bin/env bash
$EXTRACTRC `find . -name \*.ui -o -name \*.rc -o -name \*.kcfg` >> rc.cpp
$XGETTEXT `find . -name \*.cpp -o -name \*.cc -o -name \*.h ` rc.cpp -o $podir/komport.pot
$XGETTEXT -L Python `find . -name \*.py` -j -o $podir/komport.pot
rm -f rc.cpp

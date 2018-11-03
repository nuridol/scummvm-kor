#!/bin/sh

LOCAL_PATH=`dirname $0`
LOCAL_PATH=`cd $LOCAL_PATH && pwd`

if [ \! -d ../../../androidsdl ] ; then
   cd ../../..
   git clone git://github.com/pelya/commandergenius androidsdl
   cd androidsdl
   git checkout 0fd27781c65243a880bed7cc78e64aead91e16e1
   git submodule update --init project/jni/iconv/src
   (
   cd project/jni/iconv/src
   git checkout 1f31202bf552e0a272f05e49615eccf38dd7123b
   cp ../../../../../scummvm-kor/dists/androidsdl/icu4c-59_1-src.tgz .
   )
   cd ../scummvm-kor/dists/androidsdl
fi

if [ \! -d scummvm/scummvm ] ; then
   ln -s ../../../../scummvm-kor scummvm/scummvm
fi

if [ \! -d ../../../androidsdl/project/jni/application/scummvm ] ; then
   ln -s ../../../../scummvm-kor/dists/androidsdl/scummvm ../../../androidsdl/project/jni/application
fi

cd ../../../androidsdl
./build.sh scummvm

mv project/bin/MainActivity-debug.apk ../scummvm-kor/dists/androidsdl/scummvm-kor-debug.apk
cd ../scummvm-kor/dists/androidsdl

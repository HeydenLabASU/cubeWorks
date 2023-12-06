#!/bin/bash

#compile either all (default) or one (command line arg)
exe=(
resolvate
cube2dat
cubeMax
cubeSmooth
cubeAdd
cubeSum
cubeSub
cubeMult
cubeDiv
cubeScale
cubeScaleInv
cubeMove
)
if [ ! -z $1 ]; then
exe=( $1 )
fi

cd src
cd functions
files=$(ls *.c)
for f in ${files[@]}
do
echo "compiling functions/${f}"
gcc -c ${f}
done
cd ..
files=$(ls *.c)
for f in ${files[@]}
do
echo "compiling ${f}"
gcc -c ${f}
done
cd ..
if [ ! -d lib ]; then
mkdir lib
fi
if [ ! -d lib/functions ]; then
mkdir lib/functions
fi
mv src/functions/*.o lib/functions/
mv src/*.o lib/
if [ ! -d bin ]; then
mkdir bin
fi
for x in ${exe[@]}
do
echo "linking ${x}.exe"
gcc lib/functions/*.o lib/${x}.o -lfftw3f -o bin/${x}.exe
done
cd ..

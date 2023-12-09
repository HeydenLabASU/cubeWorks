#!/bin/bash

#compile either all (default) or one (command line arg)
exe=(
cube2dat
cubeAdd
cubeDiv
cubeFilter
cubeMax
cubeMove
cubeMult
cubeRot
cubeScale
cubeScaleInv
cubeSub
cubeSum
cubeTitle
resolvate
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
gcc -c ${f} -lm -O3
done
cd ..
files=$(ls *.c)
for f in ${files[@]}
do
echo "compiling ${f}"
gcc -c ${f}
done
cd ..
if [ ! -d obj ]; then
mkdir obj
fi
if [ ! -d obj/functions ]; then
mkdir obj/functions
fi
mv src/functions/*.o obj/functions/
mv src/*.o obj/
if [ ! -d bin ]; then
mkdir bin
fi
for x in ${exe[@]}
do
echo "linking ${x}.exe"
gcc obj/functions/*.o obj/${x}.o -lfftw3f -lm -o bin/${x}.exe
done
cd ..

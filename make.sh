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
noFT=0
if [ ! -z $1 ]; then
if [ "$1" = "noFT" ]; then
noFT=1
else
exe=( $1 )
fi
fi

cd src
cd functions
files=$(ls *.c)
for f in ${files[@]}
do
if [ "$f" != "filter.c" ] || [ $noFT -eq 0 ]; then
echo "compiling functions/${f}"
gcc -c ${f} -lm -O3
fi
done
cd ..
files=$(ls *.c)
for f in ${files[@]}
do
if [ "$f" != "cubeFilter.c" ] || [ $noFT -eq 0 ]; then
echo "compiling ${f}"
gcc -c ${f}
fi
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
if [ "$x" != "cubeFilter" ]; then
echo "linking ${x}.exe"
gcc obj/functions/*.o obj/${x}.o -lm -o bin/${x}.exe
elif [ $noFT -eq 0 ]; then
echo "linking ${x}.exe"
gcc obj/functions/*.o obj/${x}.o -lfftw3f -lm -o bin/${x}.exe
fi
done
cd ..
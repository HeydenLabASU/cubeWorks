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
if [ "$1" = "noFT" ] || [ "$1" != "cubeFilter" ]; then
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
if [ "$f" != "filter.c" ]; then
echo "compiling functions/${f}"
gcc -c ${f} -lm -O3
fi
done
cd ..
for f in ${exe[@]}
do
if [ "$f" != "cubeFilter" ]; then
echo "compiling ${f}.c"
gcc -c ${f}.c -lm -O3
fi
done
cd ..
mkdir -p obj
mkdir -p obj/functions
mv src/functions/*.o obj/functions/
if [ -f obj/functions/filter.o ]; then
rm obj/functions/filter.o
fi
mv src/*.o obj/
mkdir -p bin
for x in ${exe[@]}
do
if [ "$x" != "cubeFilter" ]; then
echo "linking ${x}.exe"
gcc obj/functions/*.o obj/${x}.o -lm -o bin/${x}.exe
fi
done

if [ $noFT -eq 0 ]; then
cd src/functions
f=filter.c
echo "compiling functions/${f}"
gcc -c ${f} -lm -O3
cd ..
f=cubeFilter.c
echo "compiling ${f}"
gcc -c ${f} -lm -O3
cd ..
mv src/functions/*.o obj/functions/
mv src/*.o obj/
x=cubeFilter
echo "linking ${x}.exe"
gcc obj/functions/*.o obj/${x}.o -lm -lfftw3f -o bin/${x}
fi
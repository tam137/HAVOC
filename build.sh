#!/bin/bash

set -euo pipefail

cd $(dirname $0)
WORKDIR=$PWD

RELEASENAME="Havoc.out"
DEBUGNAME="main"
LIBS="Logger.o UciApi.o Hash.o MoveHelper.o FieldMapping.o EvalImpl.o Helper.o Gamehandler.o Tree.o Board.o Move.o MoveGen.o"
BUILDOPTS="-std=c++11 -g -c"
RELEASEFOLDER="../githubhavoc"

useasserts () {
    cppFiles=$(ls *cpp)
    for file in $cppFiles; do
        sed -e s'/USEASSERTS\ \ .*/USEASSERTS\ \ 1/'g -i $file
    done
    sed -e s'/USEASSERTS\ \ .*/USEASSERTS\ \ 1/'g -i Unittest.h
    sed -e s'/USETHREADS\ \ .*/USETHREADS\ \ 0/'g -i main.cpp Gamehandler.cpp
    sed -e s'/USEVARIANCE\ \ .*/USEVARIANCE\ \ 0/'g -i main.cpp
}

production () {
    cppFiles=$(ls *cpp)
    for file in $cppFiles; do
        sed -e s'/USEASSERTS\ \ .*/USEASSERTS\ \ 0/'g -i $file
    done
    sed -e s'/USEASSERTS\ \ .*/USEASSERTS\ \ 0/'g -i Unittest.h
    sed -e s'/USETHREADS\ \ .*/USETHREADS\ \ 1/'g -i main.cpp Gamehandler.cpp
    sed -e s'/USEVARIANCE\ \ .*/USEVARIANCE\ \ 0/'g -i main.cpp
}

valgrindProfiler () {
    valgrind --tool=callgrind $RELEASENAME
}


compile () {
    rm -f *o
    if [ x"$1" == xdebug ]; then
        BUILDOPTS="$BUILDOPTS -g"
    fi
    g++ MoveGen.cpp $BUILDOPTS
    g++ Move.cpp $BUILDOPTS
    g++ Board.cpp $BUILDOPTS
    g++ Tree.cpp $BUILDOPTS
    g++ Gamehandler.cpp $BUILDOPTS
    g++ Helper.cpp $BUILDOPTS
    g++ EvalImpl.cpp $BUILDOPTS
    g++ FieldMapping.cpp $BUILDOPTS
    g++ MoveHelper.cpp $BUILDOPTS
    g++ Hash.cpp $BUILDOPTS
    g++ UciApi.cpp $BUILDOPTS
    g++ Logger.cpp $BUILDOPTS
    if [ x"$1" == xdebug ]; then
        g++ $LIBS main.cpp -pthread -g -o $RELEASENAME
    else
        g++ $LIBS main.cpp -pthread -o $RELEASENAME
    fi
    echo -e "compile finished $((cat *h && cat *cpp) | wc -l) lines\n"
}


run () {
    ./$RELEASENAME
}


memcheck () {
    valgrind --tool=memcheck --leak-check=full ./$RELEASENAME
}


mkrelease () {
    rm $RELEASEFOLDER/*

    cppFiles=$(ls *cpp)
    hFiles=$(ls *h)
    shFiles=$(ls *sh)

    for cpp in $cppFiles; do
        cp $cpp $RELEASEFOLDER
    done

    for h in $hFiles; do
        cp $h $RELEASEFOLDER
    done

    for sh in $shFiles; do
        cp $sh $RELEASEFOLDER
    done

    cp README.md $RELEASEFOLDER

    cd $RELEASEFOLDER
    echo "copy done to $RELEASEFOLDER .. compile"
    ./build.sh -c
    rm *o

    cd $WORKDIR
    SHORTHASH=$(git log -1 | awk 'FNR==1 {print $2}' | cut -c -8)
    COMMITDATE=$(git log -1 | awk 'FNR==3 {print}' | cut -c 9-)

    sed -e s/SHORTHASH/"$SHORTHASH"/g -i $RELEASEFOLDER/README.md
    sed -e s/COMMITDATE/"$COMMITDATE"/g -i $RELEASEFOLDER/README.md
    tail -2 $RELEASEFOLDER/README.md
}


usage() {
    echo -e "Usage:
    compile and run with asserts        -acr
    unsets USETREADS\n
    compile and run production          -pcr
    sets thread and variance\n
    compile in debugmode                -d
    sets also USETREADS\n
    run valgrand profiler               -pcv
    memory leak check                   -pcm
    make release                        -pb
    help                                -h  zeigt die Hilfeseite\n"
    
1>&2; exit 1; }


while getopts "apcdhrvmb" o; do
    case "${o}" in
        a)
            useasserts
            ;;
        p)
            production
            ;;
        c)
            compile "normal"
            ;;
        d)
            compile "debug"
            ;;
        r)
            run
            ;;
        v)
            valgrindProfiler
            ;;
        m)
            memcheck
            ;;
        h)
            usage
            ;;
        b)
            mkrelease
            ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))



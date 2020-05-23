#!/bin/bash

cd $(dirname $0)
WORKDIR=$PWD

./compile.sh clean

valgrind --tool=memcheck --leak-check=full ./main.out

#!/bin/bash

cd $(dirname $0)
rm -f callgrind*
valgrind --tool=callgrind ./main.out

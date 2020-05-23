#!/bin/bash

#cppFile=test.cpp
cppFile=main.cpp

clear
echo ">COMPILE: $((cat *h && cat *cpp) | wc -l) lines"
if [ x"$1" == "xclean" ]; then
    g++ main.cpp -std=c++11 -o main.out
else
    g++ main.cpp -std=c++11 -lpqxx -lpq -I/usr/local/include -o main.out
fi
if [ $? -ne "0" ]; then
  exit 1;
fi
./main.out | tee havoc.log
echo -e ""

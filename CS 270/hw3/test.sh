#!/bin/sh
# When this command is executed by the start script and 
# runs in the TestWorld directory

echo
echo "* ================================================ *"
echo "|      RERUN of Tests for CS270 Assignment 3       |"
echo "|       Comparison with Expected Output            |"
echo "* ================================================ *"
echo 
echo "You must create the expected files."
ls funcs.cpp
ls funcs.h
ls main.cpp
ls makefile
ls table.cpp
ls table.h

echo
echo "touch everything"

touch funcs.cpp
touch funcs.h
touch main.cpp
touch table.cpp
touch table.h

echo
echo "make"
make

echo
echo "make should not build anything"
sleep 1
make

echo
echo "touch main.cpp, make"
sleep 1    # make sure the date of touch is after build
touch main.cpp
make

echo
echo "touch table.cpp, make"
sleep 1
touch table.cpp
make

echo
echo "touch funcs.cpp, make"
sleep 1
touch funcs.cpp
make

echo
echo "touch funcs.h, make"
sleep 1
touch funcs.h
make

echo
echo "make all"
sleep 1
make all

echo
echo "make clean"
ls *.o
make clean
ls *.o

echo
echo "make should build everthing"
sleep 1
make

echo 
echo "* End of testing"

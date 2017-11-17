#!/bin/sh
# When this command is executed by the start script and 
# runs in the TestWorld directory

PATH=`pwd`:$PATH
# echo $PATH

echo
echo "* ================================================ *"
echo "|      Tests for CS270 Assignment 2                |"
echo "|       Comparison with Expected Output            |"
echo "* ================================================ *"

echo
echo DO: 'find Animals -type f'
find Animals -type f
echo
echo DO: 'cd Animals/Fish/'
cd Animals/Fish/
echo
echo DO: 'ls'
ls
echo
echo DO: 'cd Fresh'
cd Fresh
echo
echo DO: 'ls'
ls
echo
echo DO: 'save trout'
save trout
echo
echo DO: 'lssave'
lssave
echo
echo DO: 'cd ..'
cd ..
echo
echo DO: 'drop'
drop
echo
echo DO: 'ls'
ls
echo
echo DO: 'lssave'
lssave
echo
echo DO: 'drop'
drop
echo
echo DO: 'ls'
ls
echo
echo DO: 'cd ..'
cd ..
echo
echo DO: 'drop'
drop
echo
echo DO: 'ls'
ls
echo
echo DO: 'cd Fish'
cd Fish
echo
echo DO: 'save salmon trout'
save salmon trout
echo
echo DO: 'cd ../Mammals/Lagomorphia'
cd ../Mammals/Lagomorphia
echo
echo DO: 'drop'
drop
echo
echo DO: 'save rabbit'
save rabbit
echo
echo DO: 'add pica'
add pica
echo
echo DO: 'cd ../..'
cd ../..
echo
echo DO: 'pwd'
pwd
echo
echo DO: 'mkdir -p Fuzzy'
mkdir -p Fuzzy
echo
echo DO: 'drop'
drop
echo
echo DO: 'cd Fuzzy'
cd Fuzzy
echo
echo DO: 'drop'
drop
echo
echo DO: 'ls | save'
ls | save
echo
echo DO: 'ls'
ls
echo
echo DO: 'cd ..'
cd ..
echo
echo DO: 'drop'
drop
echo
echo DO: 'cat saveData'
cat saveData
echo
echo DO: 'ls'
ls
echo
echo DO: 'grab snake turtle'
grab snake turtle
echo
echo DO: 'lssave'
lssave
echo
echo DO: 'mkdir -p Reptile'
mkdir -p Reptile
echo
echo DO: 'cd Reptile'
cd Reptile
echo
echo DO: 'drop'
drop
echo
echo DO: 'ls'
ls
echo
echo DO: 'cd ..'
cd ..
echo
echo DO: 'pwd'
pwd
echo
echo DO: 'echo toad > toad'
echo toad > toad
echo
echo DO: 'grab toad'
grab toad
echo
echo DO: 'echo crocodile | add'
echo crocodile | add
#if [ ! -s "~/.save/saveData" ]; then
#	>&2 echo "saveData not made"
#	exit 1
#fi
echo
echo DO: 'cd Reptile'
cd Reptile
echo
echo DO: 'drop'
drop
echo
echo DO: 'get'
get
echo
echo DO: 'mv saveData crocodile'
mv saveData crocodile
echo
echo DO: 'cd ..'
cd ..
echo
echo DO: 'echo "trombone" | grab'
echo "trombone" | grab
echo
echo DO: 'get'
get

echo PWD
pwd

echo
echo DO: 'cd Fish'
cd Fish
echo
echo DO: 'save Fresh salmon'
save Fresh salmon
echo
echo DO: 'cd Salt'
cd Salt
echo
echo DO: 'lssave'
lssave
echo
echo DO: 'drop'
drop
echo
echo DO: 'ls -R'
ls -R
echo
echo
echo DO: 'cd ../../..'
cd ../../..
echo
echo DO: 'find Animals -type f'
find Animals -type f

echo
echo "END OF TESTS"

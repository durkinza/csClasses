#!/bin/sh -x
# WARNING: this file destroys files: z, key

rm key
g++ key.cpp -o key
rm z
key z 
echo $?
key -n z
echo $?
rm z
key -k10 -v10 -p8 z 
echo $?
key -n z
echo snoopy | key -s z DOG
echo $?
echo garfield | key -s z CAT
echo $?
key -g z DOG
echo $?
key -g z CAT
echo $?
key -g z BAT
echo $?
key -g z
echo $?
# key -gq z BAT
# echo $?
echo katie | key -s z DOG
echo $?
echo elsie | key -s z COW
echo $?
echo p51 | key -s z AIRCRAFT
echo $?
key -g z DOG
echo $?
key -g z AIRCRAFT
echo $?
echo abbey | key -s z DOG
echo $?
echo ka | key -s z SNAKE
echo $?
key -g z CAT
echo $?
key -g z COW
echo $?
key -g z DOG
echo $?
key -g z SNAKE
echo $?
key -g z AIRCRAFT
echo $?
echo kaa | key -s z SNAKE
echo $?
key -g z SNAKE
echo $?
echo macromolecules | key -s z ALTERATIONS
echo $?
key -g z ALTERATIONS
echo $?
key -g z ALTERATIONABLE
echo $?
key -g z SNAKE
echo $?
key -k z 
echo $?
key -y
echo $?
key -h
echo $?
echo fido | key -so z DOG
echo $?
echo fido | key -soq z DOG
echo $?
key -g z DOG
echo $?

rm z
key z
echo $?
key -n z
echo $?
key -k100 z
echo $?
key -n z
echo $?

rm z
key -k20 -k30 -k40 -v120 -v130 -v140 -p220 -p230 -p240 z
echo $?
key -n z
echo $?
key -n z it is a wonderful day in the neighborhood
echo $?

rm z
echo zanzibar | key -k20 -v30 -p40 -sn z namibia
echo $?
key -n z
echo $?
key -g z namibia
echo $?
key -hhhhhhhhhhhhhhhhhnnnnnnnnnnnn z
echo $?

rm z
echo botswana | key -os z LION
echo $?
key -og z LION
echo $?
echo zambia | key -os z LION
echo $?
key -og z LION
echo $?

hi

date > z
key z
key -g z DOG
date | key -s z CAT
rm z
key -g z DOG
key -n
rm z
echo fido | key -s z DOG
key -g z DOG
key -n

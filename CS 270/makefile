BIN=main

SRCS=\
main.cpp\
other.cpp

OBJS= \
main.o\
other.o

HDRS=\
main.h\
other.h


CC=g++

dbug=gdb

LIBS=-lm

CXXFLAGS=-O3 -Wall
#CXXFLAGS=-g -Wall # use for debugging code

$(BIN): $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(BIN)

all : 
	touch $(SRCS)
	make

tar :
	tar -cvf $(BIN).tar $(SRCS) $(HDRS) makefile

debug : $(OBJS)
	$(CC) -g $(OBJS) -o $(BIN) 
	$(dbug) $(BIN)

clean :
	rm -f $(OBJS) a.out core
	
clobber :
	rm -f $(OBJS) a.out core $(BIN) $(BIN).tar

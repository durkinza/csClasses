BIN=main

SRCS=\
main.cpp\
table.cpp\
funcs.cpp

OBJS=\
main.o\
table.o\
funcs.o

HDRS=\
table.h\
funcs.h


CXX=g++

dbug=gdb

LIBS=#-lm

CXXFLAGS=-g -Wall
#CXXFLAGS=-g -Wall # use for debugging code

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $(BIN)

all : 
	touch $(HDRS) $(SRCS) 
	make

tar :
	tar -cvf $(BIN).tar $(SRCS) $(HDRS) makefile

debug : $(OBJS)
	$(CXX) -g $(OBJS) -o $(BIN) 
	$(dbug) $(BIN)

clean :
	rm -f *.o
	
clobber :
	rm -f $(OBJS) a.out core $(BIN) $(BIN).tar

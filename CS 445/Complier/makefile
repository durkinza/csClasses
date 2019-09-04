# Author: Zane Durkin <durk7832@vandals.uidaho.edu>
# Class: CS 445 
# 
#

# variables
cc = gcc
ccopts = -g -Wall -Wextra
lex = flex
lexopts = 
yacc = bison
yaccopts = -d

valgrindopts = --leak-check=full --track-origins=yes  --show-leak-kinds=all

zipname = durk7832.zip


vgo: _build/main.o _build/lex.yy.o src/yytab.h _build/tokenTree.o #y.tab.c y.tab.h
	$(cc) _build/main.o _build/lex.yy.o src/yytab.h _build/tokenTree.o $(ccopts) -o vgo

_build/main.o: _build src/main.c 
	$(cc) -c src/main.c $(ccopts) -o _build/main.o

clean: 
	rm -f vgo lex.yy.c  *.o  $(zipname) 2> /dev/null
	rm -f src/tokenTree.h.gch
	rm -f -r _build 2> /dev/null
		
zip: clean 
	rm -f $(zipname)
	zip -r $(zipname) .

_build: 
	mkdir _build

yacc.c yacc.h: _build yacc.y
	$(yacc) $(yaccopts) yacc.y

_build/lex.yy.c: _build flex/flex.l src/yytab.h 
	$(lex) -o _build/lex.yy.c $(lexopts) flex/flex.l

_build/lex.yy.o: _build/lex.yy.c
	$(cc) _build/lex.yy.c $(ccopts) -c -o _build/lex.yy.o

_build/tokenTree.o: _build src/tokenTree.c src/tokenTree.h
	$(cc) src/tokenTree.c $(ccopts) -c -o _build/tokenTree.o

test: vgo
	@echo;
	# test a basic file
	./vgo test/basic.vgo
	@echo;
	# Test without file ending
	./vgo test/basic
	@echo;
	# Test an empty file
	./vgo test/empty
	@echo;
	# Test a file that's not go
	./vgo test/notgo; true


val: vgo
	valgrind $(valgrindopts) ./vgo test/basic.go
	@echo;echo;
	valgrind $(valgrindopts) ./vgo test/basic.vgo
	@echo;echo;
	valgrind $(valgrindopts) ./vgo test/basic.go test/basic.vgo
	@echo;echo;
	valgrind $(valgrindopts) ./vgo test/basic test/basic.vgo
	@echo;echo;
	valgrind $(valgrindopts) ./vgo test/empty
	@echo;echo;
	valgrind $(valgrindopts) ./vgo test/notgo



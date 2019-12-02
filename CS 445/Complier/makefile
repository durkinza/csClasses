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
#yaccopts = -W
yaccopts =

valgrindopts = --leak-check=full --track-origins=yes
#valgrindopts =

zipname = durk7832.zip


vgo: _build/main.o _build/lex.yy.o _build/tokenTree.o _build/yacc.tab.o _build/nonDet.o _build/types.o _build/symtab.o _build/tac.o _build/codegen.o
	$(cc) _build/main.o _build/lex.yy.o _build/tokenTree.o _build/yacc.tab.o _build/nonDet.o _build/types.o _build/symtab.o _build/tac.o _build/codegen.o $(ccopts) -o vgo

_build: 
	mkdir _build

_build/main.o: _build src/main.c
	$(cc) -c src/main.c $(ccopts) -o _build/main.o

_build/nonDet.o: _build src/nonDet.c src/nonDet.h _build/yacc.tab.h
	$(cc) -c src/nonDet.c $(ccopts) -o _build/nonDet.o

_build/tokenTree.o: _build src/tokenTree.c src/tokenTree.h _build/yacc.tab.h
	$(cc) -c src/tokenTree.c $(ccopts) -o _build/tokenTree.o

_build/yacc.tab.o: _build/yacc.tab.c _build/yacc.tab.h
	$(cc) -c _build/yacc.tab.c $(ccopts) -o _build/yacc.tab.o

_build/lex.yy.o: _build/lex.yy.c src/tokenTree.h _build/yacc.tab.h
	$(cc) -c _build/lex.yy.c $(ccopts) -o _build/lex.yy.o

_build/types.o: _build src/types.h src/types.c src/types.h src/symtab.h src/tokenTree.h
	$(cc) -c src/types.c $(ccopts) -o _build/types.o

_build/symtab.o: _build src/symtab.h src/symtab.c _build/yacc.tab.h src/nonDet.h src/types.h
	$(cc) -c src/symtab.c $(ccopts) -o _build/symtab.o

_build/tac.o: _build src/tac.h src/tac.c src/nonDet.h
	$(cc) -c src/tac.c $(ccopts) -o _build/tac.o

_build/codegen.o: _build src/codegen.c src/codegen.h src/tac.h src/nonDet.h
	$(cc) -c src/codegen.c $(ccopts) -o _build/codegen.o


_build/yacc.tab.c _build/yacc.tab.h: _build src/yacc.y
	$(yacc) --output=_build/yacc.tab.c $(yaccopts) src/yacc.y

_build/lex.yy.c: _build src/flex.l _build/yacc.tab.h
	$(lex) -o _build/lex.yy.c $(lexopts) src/flex.l



clean: 
	rm -f vgo $(zipname) 2> /dev/null
	rm -f src/tokenTree.h.gch
	rm -f -r _build 2> /dev/null
	rm -f *.ic
		
zip: clean 
	zip -r $(zipname) .

test: vgo
	@echo;
	# Test basic files
	./vgo test/hello.go
	@echo;
	# Test without the file ending
	./vgo test/hello
	@echo;
	# Test an empty file
	./vgo test/empty.go; true
	@echo;
	# Test a file that's not go syntax
	./vgo test/notgo.go; true
	@echo;
	# Test two files
	./vgo test/hello.go test/hello.go
	@echo;
	# Test comments
	./vgo test/comments.go; true
	@echo;
	# Test float64
	./vgo test/float64.go
	@echo;
	# Test ascii art
	./vgo test/art.go


val: vgo
	valgrind $(valgrindopts) ./vgo test/hello.go
	@echo;echo;
	valgrind $(valgrindopts) ./vgo test/hello
	@echo;echo;
	valgrind $(valgrindopts) ./vgo test/empty.go; true;
	@echo;echo;
	valgrind $(valgrindopts) ./vgo test/notgo.go; true;
	@echo;echo;
	valgrind $(valgrindopts) ./vgo test/hello.go test/hello.go
	@echo;echo;
	valgrind $(valgrindopts) ./vgo test/comments.go; true;
	@echo;echo;
	valgrind $(valgrindopts) ./vgo test/float64.go
	@echo;echo;
	valgrind $(valgrindopts) ./vgo test/art.go



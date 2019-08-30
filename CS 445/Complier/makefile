# Author: Zane Durkin <durk7832@vandals.uidaho.edu>
# Class: CS 445 
# 
#

# variables
cc = gcc
ccopts = -Wall
lex = flex
lexopts = 
yacc = bison
yaccopts = -d

zipname = durk7832.zip


vgo: src/main.c lex.yy.c src/yytab.h tokenTree.o #y.tab.c y.tab.h
			$(cc) src/main.c lex.yy.c src/yytab.h token.o $(ccopts) -o vgo

clean: 
		rm -f vgo lex.yy.c  *.o  $(zipname) 2> /dev/null

zip: 
	rm $(zipname)
	zip -r $(zipname) .

yacc.c yacc.h: yacc.y
			$(yacc) $(yaccopts) yacc.y

lex.yy.c: flex/flex.l src/yytab.h 
	$(lex) $(lexopts) flex/flex.l

tokenTree.o: src/tokenTree.c src/tokenTree.h
	gcc -c src/tokenTree.c src/tokenTree.h -o tokenTree.o


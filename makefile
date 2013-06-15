analyzer: lex.yy.o y.tab.o semantico.o arbol.o
	gcc -o analyzer  lex.yy.o y.tab.o semantico.o arbol.o -ly -ll -I.
lex.yy.c: xhtmlscanner.l y.tab.c
	flex xhtmlscanner.l
y.tab.c: parser.y
	bison -vdty parser.y
arbol.o: arbol.c arbol.h
	gcc -c arbol.c
semantico.o: semantico.c semantico.h
	gcc -c semantico.c

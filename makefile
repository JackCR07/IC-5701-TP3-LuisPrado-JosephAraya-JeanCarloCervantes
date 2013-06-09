xhtml: lex.yy.o y.tab.o
	gcc -o xhtml lex.yy.o y.tab.o -ly -ll
lex.yy.c: xhtmlscanner.l y.tab.c
	flex xhtmlscanner.l
y.tab.c: parser.y
	bison -vdty parser.y

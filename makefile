all: trabalho entrada.xgrl
	./trabalho < entrada.xgrl

lex.yy.c: trabalho.lex
	lex trabalho.lex

y.tab.c: trabalho.y
	yacc trabalho.y



all: trabalho entrada.xgrl
	./trabalho < entrada.xgrl

lex.yy.c: trabalhoxgrl.lex
	lex trabalhoxgrl.lex

y.tab.c: trabalhoxgrl.y
	yacc trabalhoxgrl.y

trabalho: lex.yy.c y.tab.c
	g++ -o trabalhoxgrl y.tab.c -lfl

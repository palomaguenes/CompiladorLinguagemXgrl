%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define YYSTYPE string

int yylex();
int yyparse();
void yyerror(const char *);

%}

%token _ID _TUDAO _USANDOISSO _EXECUTEISSO _SE _EHVERDADE _EHMENTIRA 
%token _MOSTRE _ATRIB _FUNCTION _COM _FACA _ENQUANTO _REPITA _EXECUTE 
%token _NUMEROSEMPONTO _PALAVRA _NUMEROCOMPONTO _SIMBOLO
%token _ESCOLHA _SEFOR _OK _CASOCONTRARIO
%token _FUNCAO _RECEBE _RETORNA _NADA

%token _CTE_PALAVRA _CTE_NUMEROSEMPONTO _CTE_NUMEROCOMPONTO _CTE_SIMBOLO

%token _RESTO

%nonassoc '>' '<' '=' ">=" "<="
%left _RESTO
%left '+' '-'
%left '*' '/'

%start S

%%

S : TUDAO { cout << "Aceito" << endl; }
  ;
    
TUDAO  : _TUDAO '{' USANDOISSO FUNCTIONDECLS EXECUTEISSO'}'
       ;

USANDOISSO  : _USANDOISSO '{' DECLS '}'
	    |
	    ;

FUNCTIONDECLS : FUNCTIONDECL FUNCTIONDECLS
			  |
			  ;

FUNCTIONDECL : _FUNCAO _ID _RECEBE '(' PARAMETROS ')' _RETORNA '('_ID ':' TIPO ')' '{' USANDOISSO EXECUTEISSO'}'
			 | _FUNCAO _ID _RECEBE '(' PARAMETROS ')' '{' USANDOISSO EXECUTEISSO'}'
			 ;

PARAMETROS : DECL ',' PARAMETROS
		   | DECL
		   | 
           ;
     
EXECUTEISSO : _EXECUTEISSO '{' MIOLOS '}'
	    |
	    ;

DECLS : DECL ';' DECLS
      | DECL ';'
      ;
     
DECL : IDS ':' TIPO        
     ;      

IDS : _ID '&' IDS
    | _ID
    ; 

TIPO : _NUMEROSEMPONTO
     | _NUMEROCOMPONTO
     | _PALAVRA TAM_PALAVRA
	 | _SIMBOLO
     ;

TAM_PALAVRA: '[' _CTE_NUMEROSEMPONTO ']'
           |
           ; 

  
MIOLOS : MIOLO MIOLOS
       | 
       ;
       
MIOLO : FUNCTION
      | CMD
      ;                   

CMD : MOSTRE
    | CMD_SE
	| CMD_ATRIB
	| CMD_FOR
	| CMD_WHILE
	| CMD_DOWHILE
	| CMD_ESCOLHA
    ; 

CMD_ESCOLHA : _ESCOLHA F '{' ESCOLHAS '}'
			;

ESCOLHAS : CASOS CASOCONTRARIO
		 ;

CASOS : CASO CASOS
	  | 
	  ;

CASO : _SEFOR F ':' MIOLOS _OK
	 ;

CASOCONTRARIO : _CASOCONTRARIO ':' MIOLOS
			  ;

MOSTRE: _MOSTRE E ';'
      ; 

   
FUNCTION : _FUNCTION _ID '(' PARAMETROS ')' ':' TIPO ';' BLOCO ';'
         | _FUNCTION _ID ':' TIPO ';' BLOCO ';'
         ;    
         

CMD_ATRIB : _ID INDICE _ATRIB E ';'
          ;

CMD_ATRIB_SPV : _ID INDICE _ATRIB E
				 ;
          
INDICE : '[' EXPS ']' INDICE
       |
       ;         
       
EXPS : E ',' EXPS
     | E
     ;
    
BLOCO : _EXECUTEISSO '{' CMD '}'
      ;

CMD_SE : _SE '(' E ')' _EHVERDADE '{' CMD '}'
	   | _SE '(' E ')' _EHVERDADE '{' CMD '}' _EHMENTIRA '{' CMD '}'
	   ;

CMD_FOR : _COM '(' CMD_ATRIB_SPV ')' _FACA '(' CMD_ATRIB_SPV ')' _ENQUANTO '(' E ')' '{' CMD '}'
        ;

CMD_WHILE : _REPITA _SE '(' E ')' '{' CMD '}'
		  ;

CMD_DOWHILE : _EXECUTE '{' CMD '}' _REPITA _SE '(' E ')' ';'
			;

E : E '+' E
  | E '-' E
  | E '*' E
  | E '/' E
  | E '>' E
  | E '<' E
  | E "<=" E
  | E ">=" E
  | _RESTO '(' E '/' E ')' 
  | E '=' E 
  | F
  ;
  
F : _CTE_PALAVRA
  | _CTE_NUMEROSEMPONTO
  | _CTE_NUMEROCOMPONTO
  | _CTE_SIMBOLO
  | _ID
  ;
 
%%

#include "lex.yy.c"

void yyerror( const char* st )
{
   if( strlen( yytext ) == 0 )
     printf( "%s\nNo final do arquivo\n", st );
   else  
     printf( "%s\nProximo a: %s\nlinha: %d\n", st, yytext, yylineno );
}

int main( int argc, char* argv[] )
{
  yyparse();
}


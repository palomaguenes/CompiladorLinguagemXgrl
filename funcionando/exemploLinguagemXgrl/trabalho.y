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

%token _ID _TUDAO _USANDOISSO _PROGRAM _EXECUTEISSO _END _VAR _SE _EHVERDADE _EHMENTIRA
%token _MOSTRE _FOR _TO _DO _ATRIB _FUNCTION 
%token _NUMEROSEMPONTO _PALAVRA _NUMEROCOMPONTO

%token _CTE_PALAVRA _CTE_NUMEROSEMPONTO _CTE_NUMEROCOMPONTO

%nonassoc '>' '<' '='
%left '+' '-'
%left '*' '/'

%start S

%%

S : TUDAO { cout << "Aceito" << endl; }
  ;
    
TUDAO  : _TUDAO '{' USANDOISSO EXECUTEISSO'}'
       ;

USANDOISSO  : _USANDOISSO '{' DECLS '}'
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
    ; 

MOSTRE: _MOSTRE E ';'
      ; 

   
FUNCTION : _FUNCTION _ID '(' PARAMETROS ')' ':' TIPO ';' BLOCO ';'
         | _FUNCTION _ID ':' TIPO ';' BLOCO ';'
         ;    
         
PARAMETROS : DECL ';' PARAMETROS
           | DECL
           ;         
   
VARS : _VAR DECLS
     ;


CMD2 : 
    | CMD_FOR
    | BLOCO
    | CMD_ATRIB
    ;

CMD_ATRIB : _ID INDICE _ATRIB E
          ;    
          
INDICE : '[' EXPS ']' INDICE
       |
       ;         
       
EXPS : E ',' EXPS
     | E
     ;        
    
CMD_FOR : _FOR _ID _ATRIB E _TO E _DO CMD
        ;
    
BLOCO : _EXECUTEISSO CMD _END
      ;

CMD_SE : _SE '(' E ')' _EHVERDADE '{' CMD '}'
	   | _SE '(' E ')' _EHVERDADE '{' CMD '}' _EHMENTIRA '{' CMD '}'
	   ;


E : E '+' E
  | E '>' E
  | F
  ;
  
F : _CTE_PALAVRA
  | _CTE_NUMEROSEMPONTO
  | _CTE_NUMEROCOMPONTO
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


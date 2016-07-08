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

%token _ID _USANDOISSO _BEGIN _END _WRITELN _WRITE _VAR _IF _THEN _ELSE
%token _FOR _TO _DO _ATRIB _FUNCTION
%token _NUMEROSEMPONTO _PALAVRA _NUMEROCOMPONTO

%token _CTE_PALAVRA _CTE_NUMEROSEMPONTO _CTE_NUMEROCOMPONTO

%nonassoc '>' '<' '='
%left '+' '-'
%left '*' '/'

%start S

%%

S : MIOLOS { cout << "Aceito" << endl; }
  ;
   

MIOLOS : MIOLO MIOLOS
       | 
       ;
       
MIOLO : VARGLOBAL
      | FUNCTION
      ;          
   
VARGLOBAL : _USANDOISSO '{' DECLS
     ;
     
DECLS : DECL ';' DECLS
      | DECL ';' '}'
      ;   
     
DECL : IDS ':' TIPO        
     ;

IDS : _ID '&' IDS
    | _ID 
    ;      
   
     
TIPO : _NUMEROSEMPONTO
     | _PALAVRA TAM_PALAVRA
     | _NUMEROCOMPONTO
     ;
     
TAM_PALAVRA : '[' _CTE_NUMEROSEMPONTO ']'
           |
           ;     
     


PRINCIPAL : _BEGIN CMDS _END '.'
          ;

FUNCTION : _FUNCTION _ID '(' PARAMETROS ')' ':' TIPO ';' BLOCO ';'
         | _FUNCTION _ID ':' TIPO ';' BLOCO ';'
         ;    
         
PARAMETROS : DECL ';' PARAMETROS
           | DECL
           ;       

          
CMDS : CMD ';' CMDS
     | 
     ;                   
 
CMD : SAIDA
    | CMD_IF
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
    
BLOCO : _BEGIN CMDS _END
      ;    
    
CMD_IF : _IF E _THEN CMD
       | _IF E _THEN CMD _ELSE CMD
       ;    
    
SAIDA : _WRITE '(' E ')'
      | _WRITELN '(' E ')'
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


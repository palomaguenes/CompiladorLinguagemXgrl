%{
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct Atributo {
  string v, t, c;
  vector<string> lst;
}; 

#define YYSTYPE Atributo

int yylex();
int yyparse();
void yyerror(const char *);
void erro( string );

map<string,string> ts;

ostream& operator << ( ostream& o, const vector<string>& st ) {
  cout << "[ ";
  for( vector<string>::const_iterator itr = st.begin();
       itr != st.end(); ++itr )
    cout << *itr << " "; 
       
  cout << "]";
  return o;     
}

void declara_variavel( Atributo& ss, 
                       const Atributo& s1, const Atributo& s3 ) {
  ss.c = "";
  for( int i = 0; i < s1.lst.size(); i++ ) {
    if( ts.find( s1.lst[i] ) != ts.end() ) 
      erro( "Variável já declarada: " + s1.lst[i] );
    else {
      ts[ s1.lst[i] ] = s3.c; 
      ss.c += s3.c + " " + s1.lst[i] + ";\n"; 
    }  
  }
}

%}

%token _ID _TUDAO _USANDOISSO _EXECUTEISSO _SE _EHVERDADE _EHMENTIRA 
%token _MOSTRE _ATRIB _COM _FACA _ENQUANTO _REPITA _EXECUTE 
%token _NUMEROSEMPONTO _PALAVRA _NUMEROCOMPONTO _NUMEROGRANDECOMPONTO _SIMBOLO
%token _ESCOLHA _SEFOR _OK _CASOCONTRARIO
%token _FUNCAO _RECEBE _RETORNA _NADA

%token _CTE_PALAVRA _CTE_NUMEROSEMPONTO _CTE_NUMEROCOMPONTO _CTE_NUMEROGRANDECOMPONTO _CTE_SIMBOLO

%token _RESTO

%nonassoc '>' '<' '=' ">=" "<="
%left _RESTO
%left '+' '-'
%left '*' '/'

%start S

%%

S : TUDAO { cout << $1.c << endl; }
  ;
    
TUDAO  : _TUDAO '{' USANDOISSO FUNCTIONDECLS EXECUTEISSO '}'
		{ $$.c = "#include <stdlib.h>\n"
                 "#include <stdio.h>\n\n" + $3.c + "\n" + $4.c +
				 "int main() {\n" + $1.c + "}\n";	  
		 }
       ;

USANDOISSO  : _USANDOISSO '{' DECLS '}'  { $$.c = $3.c; }
	    	| { $$.c = ""; }
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

DECLS : DECL ';' DECLS { $$.c = $1.c + $3.c; }
      |	DECL ';'
      ;
     
DECL : IDS ':' TIPO  { declara_variavel( $$, $1, $3 ); }     
     ;      

IDS : IDS '&' _ID { $$.lst = $1.lst; $$.lst.push_back( $3.v ); }
    | _ID		  { $$.lst.push_back( $1.v ); }
    ; 

TIPO : _NUMEROSEMPONTO { $$.c = "int"; }
     | _NUMEROCOMPONTO { $$.c = "float"; }
	 | _NUMEROGRANDECOMPONTO { $$.c = "double"; }
     | _PALAVRA TAM_PALAVRA
	 | _SIMBOLO
     ;

TAM_PALAVRA: '[' _CTE_NUMEROSEMPONTO ']'
           |
           ; 

  
MIOLOS : MIOLO MIOLOS
       | 
       ;
       
MIOLO : CHAMADAFUNCAO
      | CMD
      ;              

CHAMADAFUNCAO: _ID '(' PARAM_CHAMADA ')' ';'
	     ;     

PARAM_CHAMADA: FS
	     ;

FS: F ',' FS
  | F
  |
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

MOSTRE: _MOSTRE E ';' { $$.c = "  printf( \"%"+ $3.t + "\\n\", " + $3.v + " );\n"; }
      ; 

CMD_ATRIB : _ID INDICE _ATRIB E ';'
	  | _ID INDICE _ATRIB CHAMADAFUNCAO
          ;

CMD_ATRIB_SPV : _ID INDICE _ATRIB E
				 ;
          
INDICE : '[' EXPS ']' INDICE
       |
       ;         
       
EXPS : E ',' EXPS
     | E
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
  
F : _CTE_PALAVRA 		{ $$ = $1; $$.t = "s"; }
  | _CTE_NUMEROSEMPONTO { $$ = $1; $$.t = "d"; }
  | _CTE_NUMEROCOMPONTO
  | _CTE_NUMEROGRANDECOMPONTO
  | _CTE_SIMBOLO
  | _ID
  ;
 
%%

#include "lex.yy.c"

void erro( string st ) {
  yyerror( st.c_str() );
  exit( 1 );
}

void yyerror( const char* st )
{
   if( strlen( yytext ) == 0 )
     fprintf( stderr, "%s\nNo final do arquivo\n", st );
   else  
     fprintf( stderr, "%s\nProximo a: %s\nlinha/coluna: %d/%d\n", st, 
              yytext, yylineno, yyrowno - (int) strlen( yytext ) );
}


int main( int argc, char* argv[] )
{
  yyparse();
}

